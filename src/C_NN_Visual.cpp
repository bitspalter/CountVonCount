//////////////////////////////////////////////////////////////////////////////////
// [ C_NN_Visual_Class_Source ]
//////////////////////////////////////////////////////////////////////////////////

#include "C_NN_Visual.hpp"

//////////////////////////////////////////////////////////////////////////////////
// [init]
//////////////////////////////////////////////////////////////////////////////////
uint C_NN_Visual::init(C_NNetManager* pNM, uint x, uint y){

   if(pNM == nullptr || bReady) return(C_NN_VISUAL_ERROR);

   pNManager = pNM;

   ///////////////////////////

   sSurface.network.init(x, y, 1.0, 1.0);
   sSurface.network.add_events(sSurface.network.get_events() | Gdk::SCROLL_MASK);

   sSurface.gradient.init(x, y, 1.0, 1.0);
   sSurface.gradient.add_events(sSurface.gradient.get_events() | Gdk::SCROLL_MASK);

   sSurface.failed.init(x, y, 1.0, 1.0);
   sSurface.failed.add_events(sSurface.failed.get_events() | Gdk::SCROLL_MASK);

   ///////////////////////////

   bReady = true;

   return(C_NN_VISUAL_READY);
}

//////////////////////////////////////////////////////////////////////////////////
// [show_head]
//////////////////////////////////////////////////////////////////////////////////
uint C_NN_Visual::show_head(C_Surface* pSurface){

   S_Sf_Buffer sfbuffer;
   S_Sf_Color  sfcolor;
   
   ///////////////////////////////////////////////////////
   
   if(pSurface == nullptr) return(0);
   
   /////////////////

   sfbuffer.px = 0;
   sfbuffer.py = 0;


   
   sfbuffer.bx = 2000;
   sfbuffer.by = 2000;
   
   sfcolor.red   = 1;
   sfcolor.green = 1;
   sfcolor.blue  = 1;
   
   /////////////////

   pSurface->writeColor2Surface(&sfcolor, &sfbuffer);
         
   pSurface->writeString( 35, 15, "pic:");
   pSurface->writeInt(   110, 15, pNManager->get_pic());

   pSurface->writeString(200, 15, "hit:");
   pSurface->writeInt(   275, 15, pNManager->CNeuralNet.get_hits());

   pSurface->writeString(355, 15, "%:");
   pSurface->writeDouble(410, 15, (double)(pNManager->CNeuralNet.get_hits() * 100) / (pNManager->get_pic() + 1) );

   pSurface->writeString(520, 15, "epoche:");
   pSurface->writeInt(   590, 15, pNManager->get_epoch() + 1);
   
   //////////////////////////////////////////////////////////

   C_NNetM_Param* pParam = pNManager->get_NParam();

   //////////////////////////////////////////////////////////
   // Progress Bar
         
   if(pNManager->get_bTest()){

      pSurface->writeDouble(750, 15, (double)((pNManager->get_pic() + 1) * 100) / (pNManager->get_ctest()));
      pSurface->writeString(850, 15, "test");

      //////////////////////////
      
      sfbuffer.px = 317;
      sfbuffer.py = 2;
      
      sfbuffer.bx = 31;
      sfbuffer.by = 13;
   
      sfcolor.red   = 100;
      sfcolor.green = 100;
      sfcolor.blue  = 0;
   
      pSurface->writeColor2Surface(&sfcolor, &sfbuffer);
      
      //////////////////////////
      
      sfbuffer.px = 320;
      sfbuffer.py = 5;
      
      sfbuffer.bx = ((pNManager->get_pic() * 100) / pNManager->get_ctest()) / 4;
      sfbuffer.by = 7;
   
      sfcolor.red   = 200;
      sfcolor.green = 0;
      sfcolor.blue  = 0;
   
      pSurface->writeColor2Surface(&sfcolor, &sfbuffer);
      
   }else{

      pSurface->writeString(850, 15, "train");
      
      pSurface->writeDouble(750, 15, (double)((pNManager->get_pic() + 1) * 100) / (pNManager->get_ctrain()));
      
      /////////////////////////

      sfbuffer.px = 317;
      sfbuffer.py = 2;
      
      sfbuffer.bx = 31;
      sfbuffer.by = 13;
   
      sfcolor.red   = 0;
      sfcolor.green = 100;
      sfcolor.blue  = 100;
   
      pSurface->writeColor2Surface(&sfcolor, &sfbuffer);
      
      //////////////////////////
      
      sfbuffer.px = 320;
      sfbuffer.py = 5;
      
      sfbuffer.bx = ((pNManager->get_pic() * 100) / pNManager->get_ctrain()) / 4;
      sfbuffer.by = 7;
   
      sfcolor.red   = 0;
      sfcolor.green = 0;
      sfcolor.blue  = 200;
   
      pSurface->writeColor2Surface(&sfcolor, &sfbuffer);
   }
   
   //////////////////////////////////////////////////////////
   // Current Picture

   int off_y = 20; // TEST
         
   sfbuffer.px = 0;
   sfbuffer.py = off_y;
      
   sfbuffer.bx = pParam->ILayer.x;
   sfbuffer.by = pParam->ILayer.y;

   switch(pParam->dataset){
      case C_DATASET_CIFAR: pSurface->writeCifar2RGBSurface(pNManager->get_pBufferP(), &sfbuffer); break;
      case C_DATASET_MNIST: pSurface->write2Surface(pNManager->get_pBufferP(), &sfbuffer); break;
      default: break;
   }

   sfbuffer.px = 40;

   ////////////////////////////////////////////////////////////////////////////////

   sfbuffer.px = 40;
   sfbuffer.py = off_y;

   sfbuffer.bx = 10;
   sfbuffer.by = 1;

   sfbuffer.py = off_y;
   pSurface->write2Surface(pNManager->get_pBufferK(), &sfbuffer);
   sfbuffer.py = off_y + 1;
   pSurface->write2Surface(pNManager->get_pBufferK(), &sfbuffer);

   C_NN_Layer* pLast = pNManager->CNeuralNet.get_VNLayer()->back();

   const double* const pData  = pLast->get_pOutput();
   const double* const pDelta = pLast->get_pDelta();

   sfbuffer.py = off_y + 5;
   pSurface->write2Surface(pData, &sfbuffer);
   sfbuffer.py = off_y + 6;
   pSurface->write2Surface(pData, &sfbuffer);

   sfbuffer.py = off_y + 10;
   pSurface->write2Surface(pDelta, &sfbuffer);
   sfbuffer.py = off_y + 11;
   pSurface->write2Surface(pDelta, &sfbuffer);   

   //////////////////////////////////////////////////////////////////////////////////

   off_y += 50; 

   C_NN_Layer** pLayer = pNManager->CNeuralNet.get_VNLayer()->data();

   for(size_t n = 0; n < pNManager->CNeuralNet.get_VNLayer()->size(); n++){
      switch(pLayer[n]->get_type()){
         case C_NN_LAYER_CONV: off_y = print_conv_layer(pSurface, (C_Conv_Layer*)pLayer[n], off_y); break;
         case C_NN_LAYER_POOL: off_y = print_pool_layer(pSurface, (C_Pool_Layer*)pLayer[n], off_y); break;
         case C_NN_LAYER_FC:   off_y = print_fc_layer(  pSurface, (C_FC_Layer*)  pLayer[n], 0, off_y); break;
         default: break;
      }
   }
      
   /////////////////////////////////////////////////////////////////////

   sfbuffer.px = 0;
   sfbuffer.py = off_y;
      
   sfbuffer.bx = pParam->ILayer.x;
   sfbuffer.by = pParam->ILayer.y;

   switch(pParam->dataset){
      case C_DATASET_CIFAR: pSurface->writeCifar2RGBSurface(pNManager->get_pBufferP(), &sfbuffer); break;
      case C_DATASET_MNIST: pSurface->write2Surface(pNManager->get_pBufferP(), &sfbuffer); break;
      default: break;
   }

   sfbuffer.px = 40;

   off_y += pParam->ILayer.y + 5;

   return(off_y + 10);
}

//////////////////////////////////////////////////////////////////////////////////
// [show_network]
//////////////////////////////////////////////////////////////////////////////////
void C_NN_Visual::show_network(){

   if(!bReady) return;

   int offset = show_head(&sSurface.network);

   //////////////

   uint off_y = offset;

   //////////////

   C_NN_Layer** pLayer = pNManager->CNeuralNet.get_VNLayer()->data();

   for(size_t n = 0; n < pNManager->CNeuralNet.get_VNLayer()->size(); n++){

      switch(pLayer[n]->get_type()){
         case C_NN_LAYER_CONV: 
            off_y = print_conv_kernel(&sSurface.network, (C_Conv_Layer*)pLayer[n], C_NN_VISUAL_NETWORK, off_y); 
            break;
         case C_NN_LAYER_POOL:
            break;
         case C_NN_LAYER_FC:{
            // TEST
            //C_NN_Layer* pPrevLayer = nullptr;
            //if(n) pPrevLayer = pLayer[n - 1];

            off_y = print_fc_kernel(&sSurface.network, (C_FC_Layer*)pLayer[n], C_NN_VISUAL_NETWORK, off_y); 
            break;
         }
         default: break;
      }
   }

   //////////////

   sSurface.network.on_timeout();
}

//////////////////////////////////////////////////////////////////////////////////
// [show_gradient]
//////////////////////////////////////////////////////////////////////////////////
void C_NN_Visual::show_gradient(){

   if(!bReady) return;

   //sSurface.gradient.on_timeout(); // TEST
   //return; // TEST

   int offset = show_head(&sSurface.gradient);

   //////////////

   uint off_y = offset;

   //////////////
   
   C_NN_Layer** pLayer = pNManager->CNeuralNet.get_VNLayer()->data();

   for(size_t n = 0; n < pNManager->CNeuralNet.get_VNLayer()->size(); n++){
      switch(pLayer[n]->get_type()){
         case C_NN_LAYER_CONV: 
            off_y = print_conv_kernel(&sSurface.gradient, (C_Conv_Layer*)pLayer[n], C_NN_VISUAL_GRADIENT, off_y); 
            break;
         case C_NN_LAYER_POOL: 
            break;
         case C_NN_LAYER_FC:{
            // TEST
            //C_NN_Layer* pPrevLayer = nullptr;
            //if(n) pPrevLayer = pLayer[n - 1];

            off_y = print_fc_kernel(&sSurface.gradient, (C_FC_Layer*)pLayer[n], C_NN_VISUAL_GRADIENT, off_y); 
            break;
         }
         default: break;
      }
   }

   //////////////

   sSurface.gradient.on_timeout(); 
}

//////////////////////////////////////////////////////////////////////////////////
// [show_failed]
//////////////////////////////////////////////////////////////////////////////////
void C_NN_Visual::show_failed(){

   if(!bReady) return;
   //std::cout << "show_failed\n";
   
   S_Sf_Buffer sfbuffer;
   S_Sf_Color  sfcolor;
   
   ////////////////

   sfbuffer.px = 0;
   sfbuffer.py = 0;
   
   sfbuffer.bx = 2000;
   sfbuffer.by = 1000;
   
   sfcolor.red   = 0;
   sfcolor.green = 0;
   sfcolor.blue  = 0;
         
   sSurface.failed.writeColor2Surface(&sfcolor, &sfbuffer);

   ///////////////////////////////////////////////////////
         
   sSurface.failed.writeInt(120, 30, pNManager->get_pic());
   sSurface.failed.writeInt(210, 30, pNManager->CNeuralNet.get_hits());
   
   sSurface.failed.writeDouble(320, 30, (double)(pNManager->CNeuralNet.get_hits() * 100) / (pNManager->get_pic() + 1) );
         
   sSurface.failed.writeInt(450, 30, pNManager->get_epoch() + 1);

   //////////////////////////////////////////////////////////

   C_NNetM_Param* pParam = pNManager->get_NParam();

   ///////////////////////////////////////////////////////
   
   if(pNManager->get_bTest()){

      sfbuffer.px = 300;
      sfbuffer.py = 2;
      
      sfbuffer.bx = (pNManager->get_pic() * 100) / pNManager->get_ctest();
      sfbuffer.by = 5;
   
      sfcolor.red   = 200;
      sfcolor.green = 0;
      sfcolor.blue  = 0;
   
      sSurface.failed.writeColor2Surface(&sfcolor, &sfbuffer);
      
      
      sfbuffer.px = 300;
      sfbuffer.py = 10;
      
      sfbuffer.bx = 100;
      sfbuffer.by = 5;
   
      sfcolor.red   = 100;
      sfcolor.green = 100;
      sfcolor.blue  = 0;
   
      sSurface.failed.writeColor2Surface(&sfcolor, &sfbuffer);
      
   }else{

      sfbuffer.px = 300;
      sfbuffer.py = 2;
      
      sfbuffer.bx = (pNManager->get_pic() * 100) / pNManager->get_ctrain();
      sfbuffer.by = 5;
   
      sfcolor.red   = 0;
      sfcolor.green = 0;
      sfcolor.blue  = 200;
   
      sSurface.failed.writeColor2Surface(&sfcolor, &sfbuffer);
      
      
      sfbuffer.px = 300;
      sfbuffer.py = 10;
      
      sfbuffer.bx = 100;
      sfbuffer.by = 5;
   
      sfcolor.red   = 0;
      sfcolor.green = 100;
      sfcolor.blue  = 100;
   
      sSurface.failed.writeColor2Surface(&sfcolor, &sfbuffer);
   }
   
   sfbuffer.px = 0;
   sfbuffer.py = 0;
      
   sfbuffer.bx = pParam->ILayer.x;
   sfbuffer.by = pParam->ILayer.y;

   switch(pParam->dataset){
      case C_DATASET_CIFAR: sSurface.failed.writeCifar2RGBSurface(pNManager->get_pBufferP(), &sfbuffer); break;
      case C_DATASET_MNIST: sSurface.failed.write2Surface(pNManager->get_pBufferP(), &sfbuffer); break;
      default: break;
   }

   double* pBufP = {nullptr};

   sfbuffer.py = pParam->ILayer.y;

   vector<int>* pVError = pNManager->get_vError();

   int terror = pVError->size();
   int* pData = pVError->data();

   if(pNManager->get_bTest()){

      auto pImage = pNManager->get_pVTestImage();

      for(int n = 0, m = 0; n < terror; n++, m++){

         pBufP = &pImage[0][ pData[n] ][0];

         if(!(n % 50) && n > 1){
            sfbuffer.py += pParam->ILayer.y;
            sfbuffer.px = 0;
            m = 0;
         }
      
         sfbuffer.px = pParam->ILayer.x * m;
      
         switch(pParam->dataset){
            case C_DATASET_CIFAR: sSurface.failed.writeCifar2RGBSurface(pBufP, &sfbuffer); break;
            case C_DATASET_MNIST: sSurface.failed.write2Surface(pBufP, &sfbuffer); break;
            default: break;
         }
      }

   }else{

      auto pImage = pNManager->get_pVTrainImage();

      for(int n = 0, m = 0; n < terror; n++, m++){

         pBufP = &pImage[0][ pData[n] ][0];

         if(!(n % 50) && n > 1){
            sfbuffer.py += pParam->ILayer.y;
            sfbuffer.px = 0;
            m = 0;
         }
      
         sfbuffer.px = pParam->ILayer.x * m;
      
         switch(pParam->dataset){
            case C_DATASET_CIFAR: sSurface.failed.writeCifar2RGBSurface(pBufP, &sfbuffer); break;
            case C_DATASET_MNIST: sSurface.failed.write2Surface(pBufP, &sfbuffer); break;
            default: break;
         }
      }
   }

   ////////////////
   
   sSurface.failed.on_timeout();
}

//////////////////////////////////////////////////////////////////////////////////
// [ print_conv_layer ]
//////////////////////////////////////////////////////////////////////////////////
uint C_NN_Visual::print_conv_layer(C_Surface* pSurface, C_Conv_Layer* pConv_Layer, 
                             uint posy){

   if(pSurface == nullptr || pConv_Layer == nullptr) return(0);

   S_Sf_Buffer sfbuffer;

   S_Conv_Layer SSplitter = pConv_Layer->get_slayer();

   sfbuffer.bx = SSplitter.cSPic.x;
   sfbuffer.by = SSplitter.cSPic.y;

   uint cOut = sfbuffer.bx * sfbuffer.by;

   const double* const pData  = pConv_Layer->get_pOutput();
   const double* const pDelta = pConv_Layer->get_pDelta();

   for(uint output = 0, xOff = 0; output < SSplitter.Kernel.z; output++, xOff++){
      // TEST
      if(output > 1 && !(output % 20)){
         xOff = 0;
         posy += sfbuffer.by * 2;
      }

      sfbuffer.px = xOff * (SSplitter.cSPic.x + 2);
      sfbuffer.py = posy;

      pSurface->write2Surface(&pData[output * cOut], &sfbuffer); // TEST
      
      sfbuffer.py += SSplitter.cSPic.y;

      pSurface->write2Surface(&pDelta[output * cOut], &sfbuffer); // TEST
   }

   posy += sfbuffer.by * 2 + 10;

   return(posy);
}

//////////////////////////////////////////////////////////////////////////////////
// [ print_pool_layer ]
//////////////////////////////////////////////////////////////////////////////////
uint C_NN_Visual::print_pool_layer(C_Surface* pSurface, C_Pool_Layer* pPool_Layer, 
                             uint posy){

   if(pSurface == nullptr || pPool_Layer == nullptr) return(0);

   S_Sf_Buffer sfbuffer;

   S_Pool_Layer slayer = pPool_Layer->get_slayer();

   sfbuffer.bx = slayer.cSPic.x;
   sfbuffer.by = slayer.cSPic.y;

   uint cOut = sfbuffer.bx * sfbuffer.by;

   const double* const pData  = pPool_Layer->get_pOutput();

   for(uint output = 0, xOff = 0; output < slayer.Pic.z; output++, xOff++){

      // TEST
      if(output > 1 && !(output % 20)){
         xOff = 0;
         posy += sfbuffer.by;
      }

      sfbuffer.px = xOff * (slayer.cSPic.x + 2);
      sfbuffer.py = posy;

      pSurface->write2Surface(&pData[output * cOut], &sfbuffer);
   }

   posy += sfbuffer.by + 10;

   return(posy);
}

//////////////////////////////////////////////////////////////////////////////////
// [ print_fc_layer ]
//////////////////////////////////////////////////////////////////////////////////
uint C_NN_Visual::print_fc_layer(C_Surface* pSurface, C_FC_Layer* pFC_Layer, 
                                 uint posx, uint posy){

   if(pSurface == nullptr || pFC_Layer == nullptr) return(0);

   const double* const pData  = pFC_Layer->get_pOutput();
   const double* const pDelta = pFC_Layer->get_pDelta();

   S_Sf_Buffer sfbuffer;

   S_FC_Layer slayer = pFC_Layer->get_slayer();

   sfbuffer.bx = slayer.cOutput;
   sfbuffer.by = 1;
   sfbuffer.px = posx;
   sfbuffer.py = posy;

   pSurface->write2Surface(pData, &sfbuffer); 

   sfbuffer.py += 2;

   pSurface->write2Surface(pDelta, &sfbuffer); 

   posy += 10;

   return(posy);
}

//////////////////////////////////////////////////////////////////////////////////
// [ print_conv_kernel ]
//////////////////////////////////////////////////////////////////////////////////
uint C_NN_Visual::print_conv_kernel(C_Surface* pSurface, C_Conv_Layer* pConv_Layer, 
                              uint type, uint posy){

   if(pSurface == nullptr || pConv_Layer == nullptr) return(0);

   S_Sf_Buffer sfbuffer;

   S_Conv_Layer SConv = pConv_Layer->get_slayer();

   sfbuffer.bx = SConv.Kernel.x;
   sfbuffer.by = SConv.Kernel.y;

   const double* pData = nullptr;

   type == C_NN_VISUAL_NETWORK ? pData = pConv_Layer->get_pKernel() : pData = pConv_Layer->get_pGradient();

   for(uint nPic = 0; nPic < SConv.Pic.z; nPic++){

      sfbuffer.py = posy + nPic * (sfbuffer.by + 2);

      for(uint nKernel = 0; nKernel < SConv.Kernel.z; nKernel++){

         sfbuffer.px = nKernel * (sfbuffer.bx + 2);

         pSurface->write2Surface(&pData[nPic * pConv_Layer->get_vKernel() + nKernel * pConv_Layer->get_cKernel()], &sfbuffer);
      }
   }

   posy += SConv.Pic.z * (sfbuffer.by + 2) + 5;

   return(posy);
}

//////////////////////////////////////////////////////////////////////////////////
// [ print_fc_kernel ]
//////////////////////////////////////////////////////////////////////////////////
uint C_NN_Visual::print_fc_kernel(C_Surface* pSurface, C_FC_Layer* pFC_Layer, uint type, uint posy){

   if(pSurface == nullptr || pFC_Layer == nullptr) return(0);

   S_Sf_Buffer sfbuffer;

   S_FC_Layer sFC = pFC_Layer->get_slayer();

   sfbuffer.bx = sFC.cInput;
   sfbuffer.by = 1;

   const double* pData = nullptr;

   type == C_NN_VISUAL_NETWORK ? pData = pFC_Layer->get_pKernel() : pData = pFC_Layer->get_pGradient();

   for(uint nNeuron = 0; nNeuron < sFC.cOutput; nNeuron++){

      sfbuffer.px = 0;
      sfbuffer.py = posy + nNeuron;

      pSurface->write2Surface(&pData[nNeuron * sFC.cInput], &sfbuffer);
   }

   posy += sFC.cOutput + 10;

   return(posy);
}
