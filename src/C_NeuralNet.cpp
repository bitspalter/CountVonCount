//////////////////////////////////////////////////////////////////////////////////
// [ C_NeuralNet_Class_Source ]
//////////////////////////////////////////////////////////////////////////////////

#include "C_NeuralNet.hpp"

//////////////////////////////////////////////////////////////////////////////
// [ create ]
//////////////////////////////////////////////////////////////////////////////
int C_NeuralNet::create(vector<S_NN_Layer>* pVSLayer, S_NN_Tupel3D* pILayer){

   //cout << "C_NeuralNet::create" << endl;

   if(pVSLayer == nullptr || pILayer == nullptr)
      return(C_NEURALNET_ERROR);

   S_NN_Tupel3D DimInput = *pILayer;

   ////////////////////////////////////////////////////

   uint position = 0;

   for(S_NN_Layer slayer: *pVSLayer){

      add_layer(slayer.type);

      C_NN_Layer* player = VNLayer.back();

      switch(slayer.type){

         case 0:

            if(holds_alternative<S_Conv_Layer>(slayer.layer)){ 

               get<S_Conv_Layer>(slayer.layer).Pic = DimInput;

               if(create_conv_layer(&get<S_Conv_Layer>(slayer.layer), (C_Conv_Layer*)player) != C_NEURALNET_READY){
                  std::cout << "error: create_conv_layer" << std::endl;
               }

               DimInput.x = get<S_Conv_Layer>(slayer.layer).cSPic.x;
               DimInput.y = get<S_Conv_Layer>(slayer.layer).cSPic.y;
               DimInput.z = get<S_Conv_Layer>(slayer.layer).Kernel.z;
            }

            break;

         case 1:

            if(holds_alternative<S_Pool_Layer>(slayer.layer)){ 

               get<S_Pool_Layer>(slayer.layer).Pic = DimInput;

               if(create_pool_layer(&get<S_Pool_Layer>(slayer.layer), (C_Pool_Layer*)player) != C_NEURALNET_READY){
                  std::cout << "error: create_pool_layer" << std::endl;
               }

               DimInput.x = get<S_Pool_Layer>(slayer.layer).cSPic.x;
               DimInput.y = get<S_Pool_Layer>(slayer.layer).cSPic.y;
               DimInput.z = get<S_Pool_Layer>(slayer.layer).Pic.z;
            }

            /////////////////////////////////////////////
            // TEST
            if(slayer.bWarmup)
               create_warming_layer(position + 1, 
                                    DimInput.x * DimInput.y * DimInput.z);

            break;

         case 2:

            if(holds_alternative<S_FC_Layer>(slayer.layer)){ 

               get<S_FC_Layer>(slayer.layer).cInput  = DimInput.x * DimInput.y * DimInput.z;
               get<S_FC_Layer>(slayer.layer).bHidden = pVSLayer->size() - 1 == position ? false : true;

               if(create_fc_layer(&get<S_FC_Layer>(slayer.layer), (C_FC_Layer*)player) != C_NEURALNET_READY){
                  std::cout << "error: create_fc_layer" << std::endl;
               }

               DimInput.x = get<S_FC_Layer>(slayer.layer).cOutput;
               DimInput.y = 1;
               DimInput.z = 1;

               if(get<S_FC_Layer>(slayer.layer).bHidden && slayer.bWarmup)
                  create_warming_layer(position + 1, 
                                       DimInput.x * DimInput.y * DimInput.z);
            }

            break;

         default:
            break;
      }

      position++;
   }

   ////////////////////////////////////////////////

   this->VSLayer = *pVSLayer;

   ////////////////////////////////////////////////

   cout << "C_NeuralNet Ready" << endl;
   
   bReady = true;

   return(C_NEURALNET_READY);
}

//////////////////////////////////////////////////////////////////////////////
// [ destroy ]
//////////////////////////////////////////////////////////////////////////////
int C_NeuralNet::destroy(){

   //cout << "C_NeuralNet::destroy" << endl;

   C_NN_Layer** pLayer = VNLayer.data();

   for(size_t n = 0; n < VNLayer.size(); n++)
      delete pLayer[n];

   VNLayer.clear();

   /////////////////////////////////////////////

   C_NN_Layer** pWarmUpLayer = VTLayer.data();

   for(size_t n = 0; n < VTLayer.size(); n++)
      delete pWarmUpLayer[n];

   VTLayer.clear();
   VWarmUpLayer.clear();

   ///////////////////////////////////////

   bReady = false;

   treffer = 0;
   stage   = 1;

   //////////////////////////////////////////////////////

   return(C_NEURALNET_READY);
}

//////////////////////////////////////////////////////////////////////////////
// [ forward ]
//////////////////////////////////////////////////////////////////////////////
int C_NeuralNet::forward(double* pPicture, double* pResult){

   //cout << "C_NeuralNet::forward" << endl;

   double maxV = -1;
   int    maxN = 0;

   const double* pNetOut = nullptr;

   const double* pLastOutput = nullptr;

   ///////////////////////////////////////////////////////////

   if(stage <= VWarmUpLayer.size()){

      vector <C_NN_Layer*> TLayer; 

      TLayer = VWarmUpLayer[stage - 1];

      C_NN_Layer** pLayer = TLayer.data();

      pLastOutput = pPicture;

      for(size_t n = 0; n < TLayer.size(); n++){

         pLayer[n]->forward(pLastOutput);

         pNetOut = pLastOutput = pLayer[n]->get_pOutput();
      }

   }else{

      C_NN_Layer** pLayer = VNLayer.data();

      pLastOutput = pPicture;

      for(size_t n = 0; n < VNLayer.size(); n++){

         pLayer[n]->forward(pLastOutput);

         pNetOut = pLastOutput = pLayer[n]->get_pOutput();
      }
   }

   ///////////////////////////////////////////////////////////

   for(int n = 0; n < 10; n++){
      if(pNetOut[n] > maxV){
         maxV = pNetOut[n];
         maxN = n;
      }
   }

   //////////////////////////////////////////

   for(int n = 0; n < 10; n++){
      double f = pResult[n];

      if(f == 1) AResN[n]++;
   }

   if(pResult[maxN] > 0){
      AResR[maxN]++;
      treffer++; // TEST
      return(C_NEURALNET_READY);
   }

   return(C_NEURALNET_ERROR);
}

//////////////////////////////////////////////////////////////////////////////
// [ backward ]
//////////////////////////////////////////////////////////////////////////////
void C_NeuralNet::backward(double* pResult){

   //cout << "C_NeuralNet::backward" << endl;
   
   ////////////////////////////////////////////////////

   const double* pLastDelta = nullptr;

   if(stage <= VWarmUpLayer.size()){

      vector <C_NN_Layer*> TLayer; 

      TLayer = VWarmUpLayer[stage - 1];

      C_NN_Layer** pLayer = TLayer.data();

      pLastDelta = pResult;

      for(size_t n = TLayer.size() - 1, p = 0; n < TLayer.size(); n--, p++){

         pLayer[n]->backward(pLastDelta);

         pLastDelta = pLayer[n]->get_pDeltaOut();
      }

   }else{

      C_NN_Layer** pLayer = VNLayer.data();

      pLastDelta = pResult;

      for(size_t n = VNLayer.size() - 1; n < VNLayer.size(); n--){

         pLayer[n]->backward(pLastDelta);

         pLastDelta = pLayer[n]->get_pDeltaOut();
      }
   }
}

//////////////////////////////////////////////////////////////////////////////
// [ update ]
//////////////////////////////////////////////////////////////////////////////
void C_NeuralNet::update(){

   //cout << "C_NeuralNet::update" << endl;

   if(stage <= VWarmUpLayer.size()){

      vector <C_NN_Layer*> TLayer; 

      TLayer = VWarmUpLayer[stage - 1];

      C_NN_Layer** pLayer = TLayer.data();

      for(size_t n = 0, p = 0; n < TLayer.size(); n++, p++)
         pLayer[n]->update();

   }else{

      C_NN_Layer** pLayer = VNLayer.data();

      for(size_t n = 0; n < VNLayer.size(); n++)
         pLayer[n]->update();
   }
}

//////////////////////////////////////////////////////////////////////////////
// [ add_layer ]
//////////////////////////////////////////////////////////////////////////////
int C_NeuralNet::add_layer(int type){

   C_NN_Layer* pCLayer = {nullptr};

   switch(type){
      case C_NN_LAYER_CONV: pCLayer = new C_Conv_Layer(); break;
      case C_NN_LAYER_POOL: pCLayer = new C_Pool_Layer(); break;
      case C_NN_LAYER_FC:   pCLayer = new C_FC_Layer();   break;
   }

   if(pCLayer == nullptr) return(C_NEURALNET_ERROR);

   VNLayer.push_back(pCLayer);

   return(C_NEURALNET_READY);
}

//////////////////////////////////////////////////////////////////////////////
// [ del_layer ]
//////////////////////////////////////////////////////////////////////////////
int C_NeuralNet::del_layer(){

   if(!VNLayer.size()) return(C_NEURALNET_ERROR);

   C_NN_Layer* pCLayer = VNLayer.back();

   delete pCLayer;

   VNLayer.pop_back();

   return(C_NEURALNET_READY);
}

//////////////////////////////////////////////////////////////////////////////
// [ create_conv_layer ]
//////////////////////////////////////////////////////////////////////////////
int C_NeuralNet::create_conv_layer(S_Conv_Layer* pSConv_Layer, 
                                   C_Conv_Layer* pConv_Layer){

   if(pSConv_Layer == nullptr || pConv_Layer  == nullptr) 
      return(C_NEURALNET_ERROR);

   if(pConv_Layer->create(pSConv_Layer) != C_CONV_LAYER_READY)
      return(C_NEURALNET_ERROR);

   return(C_NEURALNET_READY);
}

//////////////////////////////////////////////////////////////////////////////
// [ create_pool_layer ]
//////////////////////////////////////////////////////////////////////////////
int C_NeuralNet::create_pool_layer(S_Pool_Layer* pSPool_Layer, 
                                   C_Pool_Layer* pPool_Layer){

   if(pSPool_Layer == nullptr || pPool_Layer  == nullptr) 
      return(C_NEURALNET_ERROR);

   if(pPool_Layer->create(pSPool_Layer) != C_POOL_LAYER_READY)
      return(C_NEURALNET_ERROR);

   return(C_NEURALNET_READY);
}

//////////////////////////////////////////////////////////////////////////////
// [ create_fc_layer ]
//////////////////////////////////////////////////////////////////////////////
int C_NeuralNet::create_fc_layer(S_FC_Layer* pSFC_Layer, 
                                 C_FC_Layer* pFC_Layer){

   if(pSFC_Layer == nullptr || pFC_Layer  == nullptr) 
      return(C_NEURALNET_ERROR);

   if(pFC_Layer->create(pSFC_Layer) != C_FC_LAYER_READY)
      return(C_NEURALNET_ERROR);

   return(C_NEURALNET_READY);
}

//////////////////////////////////////////////////////////////////////////////
// [ create_warming_layer ]
//////////////////////////////////////////////////////////////////////////////
int C_NeuralNet::create_warming_layer(unsigned int position,
                                      unsigned int cInput){

   if(!position || !cInput) return(C_NEURALNET_ERROR);

   C_NN_Layer** pLayer = VNLayer.data();

   vector <C_NN_Layer*> TWULayer;

   for(size_t m = 0; m < position; m++)
      TWULayer.push_back(pLayer[m]);

   C_NN_Layer* pTLayer = new C_FC_Layer();

   S_FC_Layer SFC_Layer;

   SFC_Layer.Alpha = 0.1;
   SFC_Layer.Beta  = 0.9;
   SFC_Layer.Bias  = -0.1 / 50;
   SFC_Layer.Divisor  = 100;
   SFC_Layer.cInput   = cInput;
   SFC_Layer.cOutput  = 10;
   SFC_Layer.bHidden  = false;
   SFC_Layer.activation = 2; // Softmax
         
   ((C_FC_Layer*)pTLayer)->create(&SFC_Layer);

   //////////////////////////////////////////////////

   VTLayer.push_back(pTLayer);

   //////////////////////////////////////////////////

   TWULayer.push_back(pTLayer);

   VWarmUpLayer.push_back(TWULayer);

   return(C_NEURALNET_READY);
}

//////////////////////////////////////////////////////////////////////////////////
// [ readFile2Config ]
//////////////////////////////////////////////////////////////////////////////////
int C_NeuralNet::readFile2Config(string sFile, vector <S_NN_Layer>* pVSLayer){

   if(pVSLayer == nullptr || !sFile.length()) return(C_NEURALNET_ERROR);

   ifstream myfile(sFile, std::ios::binary);

   if(myfile.is_open()){

      myfile.seekg(0, myfile.end);
      int cFile = myfile.tellg();
      myfile.seekg(0, myfile.beg);

      if(cFile < 16) return(C_NEURALNET_ERROR);

      C_Tensor<uint8_t> TFile(cFile, 1, 1);

      uint8_t* pFile = TFile.get_data();

      myfile.read((char*)pFile, cFile);

      cout << "FileSize:" << cFile << endl;

      if((*(uint64_t*)pFile) != NET_FILE_CONFIG){
         return(C_NEURALNET_ERROR);
      }

      cout << "magic:" << hex << *((uint64_t*)pFile) << dec << endl;

      uint64_t size = *((uint64_t*)pFile + 1);

      cout << "size:" << size << endl;

      S_Conv_Layer* pConvLayer = nullptr;
      S_Pool_Layer* pPoolLayer = nullptr;
      S_FC_Layer*   pFcLayer   = nullptr;

      S_NN_Layer SLayer;

      int offset = 16;

      for(uint n = 0; n < size; n++){

         uint32_t type = *((uint32_t*)((uint8_t*)pFile + offset));

         if((offset += 4) > cFile) return(C_NEURALNET_ERROR);

         cout << "type:" << type << " offset:" << offset << endl;

         switch(type){
            case C_NN_LAYER_CONV:

               pConvLayer = (S_Conv_Layer*)((uint8_t*)pFile + offset);

               if((offset += sizeof(S_Conv_Layer)) > cFile) return(C_NEURALNET_ERROR);

               cout << pConvLayer->Pic.x << ":" << pConvLayer->Pic.y << ":" << pConvLayer->Pic.z << " offset:" << offset << endl;

               SLayer.type  = C_NN_LAYER_CONV;
               SLayer.layer = *pConvLayer;

               pVSLayer->push_back(SLayer);

               break;

            case C_NN_LAYER_POOL:

               pPoolLayer = (S_Pool_Layer*)((uint8_t*)pFile + offset);

               if((offset += sizeof(S_Pool_Layer)) > cFile) return(C_NEURALNET_ERROR);

               cout << pPoolLayer->Pic.x << ":" << pPoolLayer->Pic.y << ":" << pPoolLayer->Pic.z << " offset:" << offset << endl;

               SLayer.type  = C_NN_LAYER_POOL;
               SLayer.layer = *pPoolLayer;

               pVSLayer->push_back(SLayer);

               break;

            case C_NN_LAYER_FC:

               pFcLayer = (S_FC_Layer*)((uint8_t*)pFile + offset);

               if((offset += sizeof(S_FC_Layer)) > cFile) return(C_NEURALNET_ERROR);

               cout << pFcLayer->cInput << ":" << pFcLayer->cOutput << " offset:" << offset << endl;

               SLayer.type  = C_NN_LAYER_FC;
               SLayer.layer = *pFcLayer;

               pVSLayer->push_back(SLayer);

               break;

            default: break;
         }
      }

      myfile.close();

   }else{
      cout << "Unable to open file" << endl;
      return(C_NEURALNET_ERROR);  
   }

   return(C_NEURALNET_READY);
}

//////////////////////////////////////////////////////////////////////////////////
// [ saveNet2File ]  
//////////////////////////////////////////////////////////////////////////////////
int C_NeuralNet::saveConfig2File(string sName){

   if(!sName.length()) return(C_NEURALNET_ERROR);

   ofstream myfile(sName, std::ios::binary);

   if(myfile.is_open()){

      myfile.write((char*)&NET_FILE_CONFIG, 8);

      uint64_t size = this->VNLayer.size();
      myfile.write((char*)&size, 8);

      uint32_t type = 0;

      S_Conv_Layer convlayer;
      S_Pool_Layer poollayer;
      S_FC_Layer fclayer;

      C_Conv_Layer* pCLayer = nullptr;
      C_Pool_Layer* pPLayer = nullptr;
      C_FC_Layer*   pFLayer = nullptr;

      C_NN_Layer** pLayer = VNLayer.data();

      for(uint n = 0; n < this->VNLayer.size(); n++){

         switch(VNLayer[n]->get_type()){

            case C_NN_LAYER_CONV:

               pCLayer = ((C_Conv_Layer*)pLayer[n]);
            
               type = C_NN_LAYER_CONV;

               myfile.write((char*)&type, 4);

               convlayer = pCLayer->get_slayer();

               myfile.write((char*)&convlayer, sizeof(S_Conv_Layer));

               break;

            case C_NN_LAYER_POOL: 
            
               pPLayer = ((C_Pool_Layer*)pLayer[n]);

               type = C_NN_LAYER_POOL;

               myfile.write((char*)&type, 4);

               poollayer = pPLayer->get_slayer();

               myfile.write((char*)&poollayer, sizeof(S_Pool_Layer));

               break;

            case C_NN_LAYER_FC: 

               pFLayer = ((C_FC_Layer*)pLayer[n]);
              
               type = C_NN_LAYER_FC;

               myfile.write((char*)&type, 4);

               fclayer = pFLayer->get_slayer();
              
               myfile.write((char*)&fclayer, sizeof(S_FC_Layer));

               break;
         }
      }

      myfile.close();

   }else{
      cout << "Unable to open file" << endl;
      return(C_NEURALNET_ERROR);  
   }

   return(C_NEURALNET_READY);
}

//////////////////////////////////////////////////////////////////////////////
// [ saveParam2File ]
//////////////////////////////////////////////////////////////////////////////
int C_NeuralNet::saveParam2File(string sName){

   cout << "saveParam2File:" << sName << endl;

   if(!this->bReady || !sName.length()) 
      return(C_NEURALNET_ERROR);

   ofstream myfile(sName, std::ios::binary);

   if(myfile.is_open()){

      myfile.write((char*)&NET_FILE_PARAM, 8);

      uint64_t size = this->VNLayer.size();
      myfile.write((char*)&size, 8);

      uint32_t type       = 0;
      uint32_t SizeKernel = 0;
      uint32_t SizeBias   = 0;

      uint32_t SizeInput  = 0;
      uint32_t SizeOutput = 0;

      S_Conv_Layer convlayer;
      S_Pool_Layer poollayer;
      S_FC_Layer fclayer;

      C_Conv_Layer* pCLayer = nullptr;
      //C_Pool_Layer* pPLayer = nullptr;
      C_FC_Layer*   pFLayer = nullptr;

      C_NN_Layer** pLayer = VNLayer.data();

      for(uint n = 0; n < this->VNLayer.size(); n++){

         switch(VNLayer[n]->get_type()){

            case C_NN_LAYER_CONV:

               pCLayer = ((C_Conv_Layer*)pLayer[n]);

               type = C_NN_LAYER_CONV;
               myfile.write((char*)&type, 4);

               convlayer = pCLayer->get_slayer();

               myfile.write((char*)&convlayer.Kernel, sizeof(S_NN_Tupel3D));

               SizeKernel = pCLayer->get_sKernel();
               myfile.write((char*)&SizeKernel, 4);

               myfile.write((char*)pCLayer->get_pKernel(), SizeKernel);

               SizeBias = pCLayer->get_sBias();
               myfile.write((char*)&SizeBias, 4);

               myfile.write((char*)pCLayer->get_pBias(), SizeBias);

               break;

            case C_NN_LAYER_POOL: 
               
               type = C_NN_LAYER_POOL;
               myfile.write((char*)&type, 4);

               break;

            case C_NN_LAYER_FC: 

               pFLayer = ((C_FC_Layer*)pLayer[n]);
              
               type = C_NN_LAYER_FC;
               myfile.write((char*)&type, 4);

               fclayer = pFLayer->get_slayer();

               SizeInput  = fclayer.cInput;
               SizeOutput = fclayer.cOutput;

               myfile.write((char*)&SizeInput,  sizeof(SizeInput));
               myfile.write((char*)&SizeOutput, sizeof(SizeOutput));

               ////////////////////////////////////////

               SizeKernel = pFLayer->get_sKernel();
               myfile.write((char*)&SizeKernel, 4);

               myfile.write((char*)pFLayer->get_pKernel(), SizeKernel);
               
               ////////////////////////////////////////

               SizeBias = pFLayer->get_sBias();
               myfile.write((char*)&SizeBias, 4);

               myfile.write((char*)pFLayer->get_pBias(), SizeBias);

               break;
         }
      }

      myfile.close();

   }else{
      cout << "Unable to open file" << endl;
      return(C_NEURALNET_ERROR);  
   }

   return(C_NEURALNET_READY);
}

//////////////////////////////////////////////////////////////////////////////
// [ readFile2Param ]
//////////////////////////////////////////////////////////////////////////////
int C_NeuralNet::readFile2Param(string sFile){

   if(!sFile.length()) return(C_NEURALNET_ERROR);

   ifstream myfile(sFile, std::ios::binary);

   if(myfile.is_open()){

      myfile.seekg(0, myfile.end);
      int cFile = myfile.tellg();
      myfile.seekg(0, myfile.beg);

      if(cFile < 16) return(C_NEURALNET_ERROR);

      C_Tensor<uint8_t> TFile(cFile, 1, 1);

      uint8_t* pFile = TFile.get_data();

      myfile.read((char*)pFile, cFile);

      cout << "FileSize:" << cFile << endl;

      if((*(uint64_t*)pFile) != NET_FILE_PARAM){
         return(C_NEURALNET_ERROR);
      }

      cout << "magic:" << hex << *((uint64_t*)pFile) << dec << endl;

      uint64_t size = *((uint64_t*)pFile + 1);

      cout << "size:" << size << " this->VNLayer.size():" << this->VNLayer.size() << endl;

      if(size != this->VNLayer.size()){
         cout << "Error: Config incorect" << endl;
         return(C_NEURALNET_ERROR);
      }

      C_Conv_Layer* pCLayer = nullptr;
      C_FC_Layer*   pFLayer = nullptr;

      S_NN_Layer SLayer;

      int offset = 16;

      uint32_t SizeKernel = 0;
      uint32_t SizeBias   = 0;

      uint32_t SizeInput  = 0;
      uint32_t SizeOutput = 0;

      S_NN_Tupel3D kernel;

      C_NN_Layer** pLayer = VNLayer.data();

      for(uint n = 0; n < size; n++){

         uint32_t type = *((uint32_t*)((uint8_t*)pFile + offset));

         if((offset += 4) > cFile) return(C_NEURALNET_ERROR);

         cout << "type:" << type << " offset:" << offset << endl;

         switch(type){
            case C_NN_LAYER_CONV:

               kernel = *((S_NN_Tupel3D*)((uint8_t*)pFile + offset));

               if((offset += sizeof(S_NN_Tupel3D)) > cFile) return(C_NEURALNET_ERROR);

               cout << "kernel:" << kernel.x << ":" << kernel.y << ":" << kernel.z << endl;

               SizeKernel = *((uint32_t*)((uint8_t*)pFile + offset));
               if((offset += 4) > cFile) return(C_NEURALNET_ERROR);

               pCLayer = ((C_Conv_Layer*)pLayer[n]);

               pCLayer->fill_Kernel(((double*)((uint8_t*)pFile + offset)));

               if((offset += SizeKernel) > cFile) return(C_NEURALNET_ERROR);

               SizeBias = *((uint32_t*)((uint8_t*)pFile + offset));
               if((offset += 4) > cFile) return(C_NEURALNET_ERROR);

               pCLayer->fill_Bias(((double*)((uint8_t*)pFile + offset)));

               if((offset += SizeBias) > cFile) return(C_NEURALNET_ERROR);

               break;

            case C_NN_LAYER_POOL:


               break;

            case C_NN_LAYER_FC:

               pFLayer = ((C_FC_Layer*)pLayer[n]);

               SizeInput = *((uint32_t*)((uint8_t*)pFile + offset));
               if((offset += 4) > cFile) return(C_NEURALNET_ERROR);

               SizeOutput = *((uint32_t*)((uint8_t*)pFile + offset));
               if((offset += 4) > cFile) return(C_NEURALNET_ERROR);

               SizeKernel = *((uint32_t*)((uint8_t*)pFile + offset));
               if((offset += 4) > cFile) return(C_NEURALNET_ERROR);

               pFLayer->fill_Kernel(((double*)((uint8_t*)pFile + offset)));

               if((offset += SizeKernel) > cFile) return(C_NEURALNET_ERROR);

               SizeBias = *((uint32_t*)((uint8_t*)pFile + offset));
               if((offset += 4) > cFile) return(C_NEURALNET_ERROR);

               pFLayer->fill_Bias(((double*)((uint8_t*)pFile + offset)));

               if((offset += SizeBias) > cFile) return(C_NEURALNET_ERROR);

               cout << "SizeInput:" << SizeInput 
                    << " SizeOutput:" << SizeOutput 
                    << " SizeKernel:" << SizeKernel 
                    << " SizeBias:" << SizeBias
                    << endl;

               break;

            default: break;
         }
      }

      myfile.close();

   }else{
      cout << "Unable to open file" << endl;
      return(C_NEURALNET_ERROR);  
   }

   return(C_NEURALNET_READY);
}
