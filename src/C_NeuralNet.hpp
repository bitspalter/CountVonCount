//////////////////////////////////////////////////////////////////////////////////
// [ C_NeuralNet_Class_Header ]
//////////////////////////////////////////////////////////////////////////////////
//
// NEURALNET
//
// [::Last modi: 25.11.19 L.ey (µ~)::]
//
//
#ifndef _C_NEURALNET_H_
#define _C_NEURALNET_H_

 #include <array>
 #include <vector>

 #include <iomanip>
 #include <iostream>
 #include <fstream>
 #include <chrono>
 #include <thread>
 #include <variant>

 using namespace std;

 #include "./layer/C_NN_Layer.hpp"
 #include "./layer/C_FC_Layer.hpp"
 #include "./layer/C_Pool_Layer.hpp"
 #include "./layer/C_Conv_Layer.hpp"

//////////////////////////////////////////////////////////////////////////////////
// CONSTANT
//////////////////////////////////////////////////////////////////////////////////
 
 const int C_NEURALNET_ERROR = 0x00;
 const int C_NEURALNET_READY = 0x01;

 const int CCTRAIN = 50000;
 const int CCTEST  = 10000;
 
 const int CCTRAINWUP = 50000;
 const int CCTESTWUP  = 10000;
 
 const int CBATCH = 25;

 const int NET_IN_X = 32;
 const int NET_IN_Y = 32;
 const int NET_IN_Z = 3;

 const uint64_t NET_FILE_CONFIG = 0x01010DAC1D; // Version 0x01
 const uint64_t NET_FILE_PARAM  = 0x01020DAC1D; // Version 0x01

//////////////////////////////////////////////////////////////////////////////////
// STRUCT
//////////////////////////////////////////////////////////////////////////////////

 struct S_NN_Layer {
    int type;
    variant<S_Conv_Layer, 
            S_Pool_Layer, 
            S_FC_Layer> layer;
    bool bWarmup = {false};
 };

//////////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////////
 
 class C_NeuralNet {
 
    public:
      
       C_NeuralNet(){};
      ~C_NeuralNet(){this->destroy();};
       
       int create(vector<S_NN_Layer>* pVSLayer, S_NN_Tupel3D* pDimInput);
       int destroy();
       
       int  forward(double* pPicture, double* pResult);
       void backward(double* pResult);
       void update();

       ///////////////////////////////////
       
       int saveConfig2File(string sName);
       int readFile2Config(string sName, vector <S_NN_Layer>* pVSLayer);
       
       int saveParam2File(string sName);
       int readFile2Param(string sName);

       ///////////////////////////////////

       vector <C_NN_Layer*>* get_VNLayer(){return(&this->VNLayer);};

       ////////////////////////////////////////////

       uint get_hits(){return(this->treffer);};
       void clr_hits(){this->treffer = 0;};

       uint get_stage(){return(this->stage);}
       void inc_stage(){this->stage++;};

       uint get_sWarmup(){return(VWarmUpLayer.size());}

    private:

       bool bReady = {false};

       uint treffer = {0};
       uint stage   = {1};
       
       int AResN[10] = {0,0,0,0,0,0,0,0,0,0};
       int AResR[10] = {0,0,0,0,0,0,0,0,0,0};
       
       //int AResL[CCTRAIN];
       
       ////////////////////////////

       vector <C_NN_Layer*> VNLayer;
       vector <S_NN_Layer>  VSLayer;
       
       vector <C_NN_Layer*> VTLayer;
       vector <vector <C_NN_Layer*>> VWarmUpLayer;

       ////////////////////////////

       int add_layer(int type);
       int del_layer();

       ////////////////////////////

       int create_conv_layer(S_Conv_Layer* pSConv_Layer, C_Conv_Layer* pConv_Layer);
       int create_pool_layer(S_Pool_Layer* pSPool_Layer, C_Pool_Layer* pPool_Layer);
       int create_fc_layer(S_FC_Layer* pSFC_Layer, C_FC_Layer* pFC_Layer);

       ////////////////////////////
       
       int create_warming_layer(unsigned int position, unsigned int cInput);
 };
 
#endif // _C_NEURALNET_H_
