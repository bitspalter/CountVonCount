//////////////////////////////////////////////////////////////////////////////////
// [ C_DataSet_Class_Source ]
//////////////////////////////////////////////////////////////////////////////////

#include "C_NNetManager.hpp"
#include "C_App.hpp"

//////////////////////////////////////////////////////////////////////////////////
// [ init ]  
//////////////////////////////////////////////////////////////////////////////////
int C_NNetManager::init(C_App* pC_App){
   
   if(pC_App == nullptr) return(C_NNETMANAGER_ERROR);

   pCApp = pC_App;

   bLog = true;

   return(C_NNETMANAGER_READY);
}

//////////////////////////////////////////////////////////////////////////////////
// [ print_network ]
//////////////////////////////////////////////////////////////////////////////////
int C_NNetManager::print_network(){

   ostringstream sData;
   
   sData << endl << "////////////////////////////[ NEW NETWORK ]//////////////////////////////////////" << endl << endl;
   
   C_NN_Layer** pLayer = CNeuralNet.get_VNLayer()->data();

   int param = 0;
   int paramAll = 0;

   for(size_t n = 0; n < CNeuralNet.get_VNLayer()->size(); n++){

      if(typeid(*pLayer[n]).hash_code() == typeid(C_Conv_Layer).hash_code()){

         C_Conv_Layer* pCLayer = ((C_Conv_Layer*)pLayer[n]);

         S_Conv_Layer slayer = pCLayer->get_slayer();

         sData << endl << "// [ C_Conv_Layer ] " << endl;

         sData << " Input: "  << slayer.Pic.x
               << "x"         << slayer.Pic.y
               << "x"         << slayer.Pic.z;

         sData << " Kernel: " << slayer.Kernel.x
               << "x"         << slayer.Kernel.y
               << "x"         << slayer.Kernel.z;

         sData << " Output: " << slayer.cSPic.x
               << "x"         << slayer.cSPic.y
               << "x"         << slayer.Kernel.z << endl;

         param = (slayer.Kernel.x * slayer.Kernel.y * slayer.Kernel.z + slayer.Kernel.z) * 
                  slayer.Pic.z;
   
         sData << " Param: " << param << endl;
   
         paramAll += param;

      }else
      if(typeid(*pLayer[n]).hash_code() == typeid(C_Pool_Layer).hash_code()){

         C_Pool_Layer* pPLayer = ((C_Pool_Layer*)pLayer[n]);
         
         S_Pool_Layer slayer = pPLayer->get_slayer();

         sData << endl << "// [ C_Pool_Layer ] " << endl;

         sData << " Pic: "    << slayer.Pic.x
               << "x"         << slayer.Pic.y
               << "x"         << slayer.Pic.z;

         sData << " Slide: "  << slayer.Slide.x
               << "x"         << slayer.Slide.y
               << " Pad: "    << slayer.Pad.x
               << "x"         << slayer.Pad.y;

         sData << " Output: " << slayer.cSPic.x
               << "x"         << slayer.cSPic.y
               << "x"         << slayer.Pic.z << endl;

      }else
      if(typeid(*pLayer[n]).hash_code() == typeid(C_FC_Layer).hash_code()){

         C_FC_Layer* pFLayer = ((C_FC_Layer*)pLayer[n]);

         S_FC_Layer slayer = pFLayer->get_slayer();

         sData << endl << "// [ C_FC_Layer ] " << endl;

         sData << " Input:"  << slayer.cInput
               << " Output:" << slayer.cOutput << endl;

         param = slayer.cInput * slayer.cOutput + slayer.cOutput;
   
         sData << " Param: " << param << endl;
   
         paramAll += param;

      }
   }

   sData << endl << " ParamAll: " << paramAll << endl;

   writeLog(sData.str().c_str());
      
   sData.str("");

   return(C_NNETMANAGER_READY);
}

//////////////////////////////////////////////////////////////////////////////////
// [ create ]
//////////////////////////////////////////////////////////////////////////////////
int C_NNetManager::create(vector<S_NN_Layer>* pVSLayer, C_NNetM_Param* pParam){

   //cout << "C_NNetManager::create" << endl;

   if(pVSLayer == nullptr || pParam == nullptr) 
      return(C_NNETMANAGER_ERROR);
   
   this->ctrain = pParam->ctrain;
   this->ctest  = pParam->ctest;
   this->batch  = pParam->batch;

   S_NN_Tupel3D DimInput = pParam->ILayer;

   this->SNParam = *pParam;

   ////////////////////////////////////////////////////////////

   switch(this->SNParam.dataset){
      case C_DATASET_CIFAR: this->pDataset = new C_Cifar(); break; 
      case C_DATASET_MNIST: this->pDataset = new C_Mnist(); break; 
      default: return(C_NNETMANAGER_ERROR); break;
   }

   if(this->pDataset->create(DimInput.x, DimInput.y) != C_DATASET_READY){
      cout << "error::pDataset->create" << endl;
      return(C_NNETMANAGER_ERROR);
   }

   pVTrainImg     = this->pDataset->get_pTrainImage();
   pVTrainLab     = this->pDataset->get_pTrainLabel();
   pVTestImg      = this->pDataset->get_pTestImage();
   pVTestLab      = this->pDataset->get_pTestLabel();
   pVTrainNumbers = this->pDataset->get_pTrainNumbers();
   pVTestNumbers  = this->pDataset->get_pTestNumbers();

   // TEST
   //TPic.create(DimInput.x, DimInput.y, DimInput.z);

   /////////////////////////////////////////////////////

   if(CNeuralNet.create(pVSLayer, &DimInput) != C_NEURALNET_READY)
      return(C_NNETMANAGER_ERROR);

   /////////////////////////////////////////////////////

   print_network();
   
   /////////////////////////////////////////////////////

   this->bReady = true;

   return(C_NNETMANAGER_READY);
}

//////////////////////////////////////////////////////////////////////////////////
// [ destroy ]
//////////////////////////////////////////////////////////////////////////////////
int C_NNetManager::destroy(){

   std::ostringstream sData;
   
   sData << std::endl << "C_NeuralNet destroy" << std::endl << std::endl;
   
   this->writeLog(sData.str().c_str());
      
   sData.str("");
   
   if(this->pDataset != nullptr){
      //this->pDataset.reset(); 
      delete this->pDataset;
      this->pDataset = nullptr;
   }
   
   this->CNeuralNet.destroy();

   /////////////////////////////

   pBufferP = nullptr;
   pBufferK = nullptr;

   pBufferP_RGB = nullptr;
   pBufferP_Lum = nullptr;

   pVTrainImg = nullptr;
   pVTrainLab = nullptr;

   pVTrainLab = nullptr;
   pVTestLab = nullptr;

   pVTrainNumbers = nullptr;
   pVTestNumbers = nullptr;

   /////////////////////////////

   pic = 0;
   epoche = 0;
   
   vError.clear();
   ErrorLast = 0;
   
   bWarmUp = false;
   btest   = true;
   bReady  = false;
       
   return(C_NNETMANAGER_READY);
}

//////////////////////////////////////////////////////////////////////////////////
// [ start ]
//////////////////////////////////////////////////////////////////////////////////
void C_NNetManager::start(){
   
   if(bRun != true){

      bRun = true;

      pThread = new std::thread(&C_NNetManager::run, this);

      if(pThread) 
         pThread->detach();
      else{
         std::ostringstream sData;
         sData << "ERROR: Start Thread" << std::endl;
         this->writeLog(sData.str().c_str());
         bRun = false;
      }
   }
}

//////////////////////////////////////////////////////////////////////////////////
// [ stop ]
//////////////////////////////////////////////////////////////////////////////////
void C_NNetManager::stop(){
   
   if(bRun == true){
      bRun = false;
      delete pThread;
      pThread = nullptr;
   }
}

//////////////////////////////////////////////////////////////////////////////////
// [ last_train ]
//////////////////////////////////////////////////////////////////////////////////
void C_NNetManager::last_train(){

   CNeuralNet.update();
   
   epoche++;
   pic = 0;
   
   ErrorLast = 0;
   vError.clear();
   
   time_t Zeitstempel;
   tm *nun;
   Zeitstempel = time(0);
   nun = localtime(&Zeitstempel);

   /////////

   ostringstream sData;

   sData << "train epoche:" << setw(3) << epoche << "  treffer:" 
                            << setw(5) << CNeuralNet.get_hits() << "  " 
                            << setw(3) << (CNeuralNet.get_hits() * 100) / ctrain << "% ";


   sData << nun->tm_mday << '.' << nun->tm_mon + 1 << '.' 
         << nun->tm_year + 1900 << " - " << setw(2) 
         << nun->tm_hour << ':' << setw(2) 
         << nun->tm_min  << ':' << setw(2) 
         << nun->tm_sec;

   sData << "  mse:" << mse1 / ctrain << endl;

   /////////////////////////////////////////////////////////
   
   if(epoche > 2 && !(epoche % 3)) btest = true;

   ////////////////////////////////////////////////////////

   if(epoche <= 500){

      if(epoche && !(epoche % 24)){
         CNeuralNet.inc_stage();
         sData << "bstage" << CNeuralNet.get_stage() << " = true" << " " << CNeuralNet.get_sWarmup() <<  endl;

         if(CNeuralNet.get_stage() > CNeuralNet.get_sWarmup() + 1){
            batch *= 2;
         }
      }



/*       if(epoche == 6 * 2){
         CNeuralNet.inc_stage();
         sData << "bstage" << CNeuralNet.get_stage() << " = true" << endl;
      }

      if(epoche == 12 * 2){
         CNeuralNet.inc_stage();
         sData << "bstage" << CNeuralNet.get_stage() << " = true" << endl;
      }

      if(epoche == 18 * 2){
         CNeuralNet.inc_stage();
         sData << "bstage" << CNeuralNet.get_stage() << " = true" << endl;
      }

      if(epoche == 24 * 2){
         CNeuralNet.inc_stage();
         sData << "bstage" << CNeuralNet.get_stage() << " = true" << endl;
      }

      if(epoche == 30 * 2){
         CNeuralNet.inc_stage();
         sData << "bstage" << CNeuralNet.get_stage() << " = true" << endl;
         batch = 50;
      }

      if(epoche == 42 * 2){
         CNeuralNet.inc_stage();
         sData << "bstage" << CNeuralNet.get_stage() << " = true" << endl;
         batch = 100;
      } */

/*                if(epoche == 66){
         CNeuralNet.inc_stage();
         sData << "bstage" << CNeuralNet.get_stage() << " = true" << endl;
         batch = 250;
      }

      if(epoche == 78){
         CNeuralNet.inc_stage();
         sData << "bstage" << CNeuralNet.get_stage() << " = true" << endl;
         batch = 500;
      } */
   }
   
   ////////////////////////////////////////////////////////

   writeLog(sData.str().c_str());
   
   ////////////////////////////////////////////////////////

   mse1 = 0;
   CNeuralNet.clr_hits();
   
   //////////////////////////
   
   //memset(CNeuralNet.AResL, 0, sizeof(CNeuralNet.AResL));
}

//////////////////////////////////////////////////////////////////////////////////
// [ last_test ]
//////////////////////////////////////////////////////////////////////////////////
void C_NNetManager::last_test(){

   //cout << "pic >= ctest" << endl;
   
   epoche++;
   pic   = 0;
   btest = false;
   
   ErrorLast = 0;
   vError.clear();
   
   time_t Zeitstempel;
   tm *nun;
   Zeitstempel = time(0);
   nun = localtime(&Zeitstempel);

   ///////////////////////////////////////

   ostringstream sData;

   sData << "test  epoche:" << setw(3) << epoche << "  treffer:" 
                            << setw(5) << CNeuralNet.get_hits() << "  " 
                            << setw(3) << (CNeuralNet.get_hits() * 100) / ctest << "% ";

   sData << nun->tm_mday << '.' 
         << nun->tm_mon + 1 << '.' 
         << nun->tm_year + 1900 << " - " 
         << setw(2) << nun->tm_hour << ':' 
         << setw(2) << nun->tm_min << ':' 
         << setw(2) << nun->tm_sec;

   sData << " batch:" << batch << endl; 

   writeLog(sData.str().c_str());
   
   /////////

   CNeuralNet.clr_hits();
   
   //memset(CNeuralNet.AResL, 0, sizeof(CNeuralNet.AResL));
   
   //////////////////////////// test : autosave
   
/*    sData << "log/" << nun->tm_mday << '.' 
                   << nun->tm_mon + 1 << '.' 
                   << nun->tm_year + 1900 << " - " 
                   << setw(2) << nun->tm_hour << ':' 
                   << setw(2) << nun->tm_min << ':' 
                   << setw(2) << nun->tm_sec;

   sData << ".hpp";
   
   string s = sData.str();
   
   CNeuralNet.saveNet2File((char*)s.c_str()); // TEST
   
   sData.str(""); */

   if(epoche > 1){
      if(this->pDataset != nullptr){
         this->pDataset->flip_images();
      }
   }
}

//////////////////////////////////////////////////////////////////////////////////
// [ run ]
//////////////////////////////////////////////////////////////////////////////////
void C_NNetManager::run(){

   //cout << "C_NNetManager::run" << endl;
   
   time_t Zeitstempel;
   tm *nun;
   Zeitstempel = time(0);
   nun = localtime(&Zeitstempel);
   
   /////////
   
   ostringstream sData;

   sData << endl << "C_NeuralNet Start" << endl << endl;

   sData << "start epoche:" << setw(3) << epoche << "  treffer:" 
                            << setw(5) << CNeuralNet.get_hits() << "  " 
                            << setw(3) << (CNeuralNet.get_hits() * 100) / ctrain << "% ";

   sData << nun->tm_mday << '.' 
         << nun->tm_mon + 1 << '.' 
         << nun->tm_year + 1900 << " - " 
         << setw(2) << nun->tm_hour << ':' 
         << setw(2) << nun->tm_min << ':' 
         << setw(2) << nun->tm_sec;

   sData << "  mse:" << mse1 << endl; 

   writeLog(sData.str().c_str());
      
   sData.str("");

   ////////////////////////////////////////

   while(bRun){
      if(btest){
         ////////////////////////////////////////////////////////////////////////////
         // [ TESTPHASE ]
         ////////////////////////////////////////////////////////////////////////////
         if(pic >= ctest){
            this->last_test();
            continue;
         }
         //////////////////////////////////////////////////////////
         switch(Value){
            case VALL:
               // TEST
               //pBufferP_Lum = &CDataSetCifar.vTestImgLum[pic][0];
               pBufferP_RGB = &pVTestImg[0][pic][0];
               pBufferP = &pVTestImg[0][pic][0];
               pBufferK = &pVTestLab[0][pic][0];
               break;
            default:
               int tpic = pVTestNumbers[0][Value][pic];
               // TEST
               pBufferP_RGB = &pVTestImg[0][tpic][0];
               //pBufferP_Lum = &CDataSetCifar.vTestImgLum[tpic][0];
               pBufferP = &pVTestImg[0][tpic][0];
               pBufferK = &pVTestLab[0][tpic][0];
               break;
         }

      }else{
         ////////////////////////////////////////////////////////////////////////////
         // [ LERNPHASE ]
         ////////////////////////////////////////////////////////////////////////////
            if(pic >= ctrain){
               this->last_train();
               continue;
            }
         //////////////////////////////////////////////////////////////
         switch(Value){
            case VALL:
               // TEST
               //pBufferP_Lum = &CDataSetCifar.vTrainImgLum[pic][0];
               pBufferP_RGB = &pVTrainImg[0][pic][0];
               pBufferP = &pVTrainImg[0][pic][0];
               pBufferK = &pVTrainLab[0][pic][0];
               break;
            default:
               int tpic = pVTrainNumbers[0][Value][pic];
               // TEST
               //pBufferP_Lum = &CDataSetCifar.vTrainImgLum[tpic][0];
               pBufferP_RGB = &pVTrainImg[0][tpic][0];

               pBufferP = &pVTrainImg[0][tpic][0];
               pBufferK = &pVTrainLab[0][tpic][0];
               break;
         }
      }

      /////////////////////////////////////////////////////////////////////////////

      //auto pDiff = CNeuralNet.diff_forward(pBufferP);

      if(CNeuralNet.forward(pBufferP, pBufferK) != C_NEURALNET_READY){

         if(bValue){

            uint t = 0;

            if(btest) t = pVTestNumbers[0][Value][pic];
            else      t = pVTrainNumbers[0][Value][pic];

            vError.push_back(t);

         }else{
            vError.push_back(pic);
         }
      }

      if(!btest){ 

         CNeuralNet.backward(pBufferK);

         if(!((pic + 1) % batch)){

            CNeuralNet.update();

            // TEST
            C_FC_Layer* pLast = (C_FC_Layer*)CNeuralNet.get_VNLayer()->back();
            //cout << "MSE:" << pLast->mse_last << endl;

            mse1 += pLast->mse_last;
            //cout << mse1 << " :  " << mse2 << endl; //////////

            mse1batch++;
         } 
      }

      /////////////////////////////////////////////////////////////////////////////
      if(bsleep)
         sleep(1);

      pic++;
   }
}

//////////////////////////////////////////////////////////////////////////////////
// [ set_value ]
//////////////////////////////////////////////////////////////////////////////////
void C_NNetManager::set_value(int new_value){
          
   Value   = new_value;
   pic     = 0;
   CNeuralNet.clr_hits();
          
   vError.clear();
          
   if(Value == VALL){
      ctrain = ctrain; //CCTRAIN;
      ctest  = ctest;  //CCTEST;
      bValue = false;
   }else{
      ctrain = pVTrainNumbers[0][Value].size();
      ctest  = pVTestNumbers[0][Value].size();
      bValue = true;
   }
};

//////////////////////////////////////////////////////////////////////////////////
// [ writeLog ]
//////////////////////////////////////////////////////////////////////////////////
void C_NNetManager::writeLog(const char* pData){
   
   if(!bLog || !pData) return;
   
   SLogLast.clear();
   
   SLogLast.append(pData);
   SLog.append(pData);
   
   cout << pData;

   //cout << " pCApp:" << pCApp << "pCApp->log_data()" << endl;
   
   pCApp->log_data(); // Call the MainThread to log new data   
}
