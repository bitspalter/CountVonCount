//////////////////////////////////////////////////////////////////////////////////
// [ C_NNetManager_Class_Header ]
//////////////////////////////////////////////////////////////////////////////////
//
//
//
// [::Last modi: 24.04.2020 L.ey (µ~)::]
//
//
#ifndef _C_NNETMANAGER_H_
#define _C_NNETMANAGER_H_

 #include <array>
 #include <vector>

 #include <iomanip>
 #include <iostream>
 #include <fstream>
 #include <chrono>
 #include <thread>
 #include <string>
 #include <exception>
 
 using namespace std;
 
 class C_App;
 
 #include <gtkmm.h>
 #include "./dataset/C_Mnist.hpp"
 #include "./dataset/C_Cifar.hpp"
 #include "C_NeuralNet.hpp"
 
//////////////////////////////////////////////////////////////////////////////////
// CONSTANT
//////////////////////////////////////////////////////////////////////////////////
 
 const int C_NNETMANAGER_ERROR = 0x00;
 const int C_NNETMANAGER_READY = 0x01;
 
 const int VALL = 0x0A;
 
//////////////////////////////////////////////////////////////////////////////////
// STRUCT
//////////////////////////////////////////////////////////////////////////////////
 
 struct C_NNetM_Result {
   int pic; 
   double result[10];
 };

 struct C_NNetM_Param {
    S_NN_Tupel3D ILayer;

    uint ctrain;    //  = {0}; // {CCTRAIN};
    uint ctest;     //  = {0}; // {CCTEST};
    uint batch;     //  = {0}; // {CBATCH};
    
    uint ctrainwup; //  = {CCTRAINWUP};
    uint ctestwup;  //  = {CCTESTWUP};

    // TEST
    uint dataset;
 };

//////////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////////
 
 class C_NNetManager {
 
    public:
      
       C_NNetManager(){};
       virtual ~C_NNetManager(){};
       
       int init(C_App* pCApp);
       
       int create(vector<S_NN_Layer>* pVSLayer, C_NNetM_Param* pParam);
       int destroy();

       void start();
       void stop();
       
       int print_network();

       /////////////////////////////
       
       void set_value(int new_value);
       
       /////////////////////////////

       uint get_pic()   {return(this->pic);}
       uint get_epoch() {return(this->epoche);}
       uint get_ctrain(){return(this->ctrain);}
       uint get_ctest() {return(this->ctest);}

       /////////////////////////////

       string* get_sLog()    {return(&this->SLog);}
       string* get_sLogLast(){return(&this->SLogLast);}

       /////////////////////////////

       vector<int>* get_vError(){return(&this->vError);}

       /////////////////////////////

       bool get_bRun() {return(this->bRun);}
       bool get_bTest(){return(this->btest);}

       bool get_bSleep(){return(this->bsleep);}
       void set_bSleep(bool b){this->bsleep = b;}

       bool get_bReady(){return(this->bReady);}
       
       /////////////////////////////

       auto get_pVTrainImage(){return(pVTrainImg);}
       auto get_pVTestImage() {return(pVTestImg);}

       /////////////////////////////

       double* get_pBufferP(){return(this->pBufferP);}
       double* get_pBufferK(){return(this->pBufferK);}

       double* get_pBufferP_RGB(){return(this->pBufferP_RGB);}

       //////////////////////////////

       C_NNetM_Param* get_NParam(){return(&this->SNParam);}

       //////////////////////////////
       
       C_NeuralNet CNeuralNet;

       //////////////////////////////

       void writeLogEx(const char* pData){
          writeLog(pData);
       }

    private:

       void last_train();
       void last_test();

      /////////////////////////////

       C_NNetM_Param SNParam;

       /////////////////////////////
       // Dataset

       C_Dataset* pDataset = {nullptr};

       std::vector <std::vector <double>>* pVTrainImg = {nullptr};
       std::vector <std::vector <double>>* pVTrainLab = {nullptr};
       
       std::vector <std::vector <double>>* pVTestImg = {nullptr};
       std::vector <std::vector <double>>* pVTestLab = {nullptr};

       std::vector <std::vector <uint>>* pVTrainNumbers = {nullptr};
       std::vector <std::vector <uint>>* pVTestNumbers  = {nullptr};

       /////////////////////////////

       double* pBufferP = {nullptr};
       double* pBufferK = {nullptr};

       double* pBufferP_RGB = {nullptr};
       double* pBufferP_Lum = {nullptr};

       /////////////////////////////

       bool bWarmUp = {false};
       bool btest   = {true};
       bool bReady  = {false};
       bool bsleep  = {false};

       /////////////////////////////

       uint pic    = {0};
       uint epoche = {0};

       uint batch  = {0}; //{CBATCH};

       uint ctrain = {0}; //{CCTRAIN};
       uint ctest  = {0}; //{CCTEST};
       
       uint ctrainwup = {0};//{CCTRAINWUP};
       uint ctestwup  = {0};//{CCTESTWUP};

       ////////////////////////////
       // logging

       string SLog;
       string SLogLast;
       
       C_App* pCApp = {nullptr};

       bool bLog = {false};

       void writeLog(const char* pData);

       ////////////////////////////

       vector<C_NNetM_Result> vRError;

       vector<int> vError;

       int ErrorLast = {0};

       ////////////////////////////

       std::thread* pThread = {nullptr};

       bool bRun = {false};
       void run();
       
       //////////////////////////////
       
       double mse1    = {0};
       uint mse1batch = {0};
       
       uint Value  = {VALL};
       bool bValue = {false};
 };
 
#endif // _C_NNETMANAGER_H_
