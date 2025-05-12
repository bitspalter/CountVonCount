//////////////////////////////////////////////////////////////////////////////////
// [ Conv_Layer_Class_Header ]
//////////////////////////////////////////////////////////////////////////////////
//
// Convolutional Neural Net Layer
//
// [::Last modi: 27.06.24 L.ey (µ~)::]
//
//
#ifndef _C_CONV_LAYER_H_
 #define _C_CONV_LAYER_H_

 #include <iostream>
 using namespace std;

 #include "C_NN_Layer.hpp"

//////////////////////////////////////////////////////////////////////////////////
// CONSTANT
//////////////////////////////////////////////////////////////////////////////////

 const int C_CONV_LAYER_READY = 0x01;
 const int C_CONV_LAYER_ERROR = 0x00;

//////////////////////////////////////////////////////////////////////////////////
// STRUCT
//////////////////////////////////////////////////////////////////////////////////
 
 struct S_Conv_Layer {
    S_NN_Tupel3D Pic;     //12
    S_NN_Tupel3D Kernel;  //12
    S_NN_Tupel2D Slide;   // 8
    double Alpha;         // 8
    double Beta;          // 8
    double Bias;          // 8
    double Divisor;       // 8

    uint8_t activation;   // 1
    bool bHidden;         // 1

    // Out
    S_NN_Tupel2D cSPic;   // 8

    uint8_t aligned[6]; // TEST
 };
 
//////////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////////
 
 class C_Conv_Layer : public C_NN_Layer {
   
    public:

       C_Conv_Layer(){}
       C_Conv_Layer(int layer_typ) : C_NN_Layer(layer_typ){}
       virtual ~C_Conv_Layer();

       int create(S_Conv_Layer* pSCNNLayer);
      
       void forward(const double* const pInput);
       void backward(const double* const pDelta);
       void update();

       //////////////////////////////////////////////////////////

       S_Conv_Layer  get_slayer()   {return(this->SConvLayer);}

       const double* get_pKernel()  {return(this->pKernel);}
       const double* get_pGradient(){return(this->pGradient);}
       const uint*   get_pDendrite(){return(this->pDendrite);}

       uint get_vKernel(){return(this->vKernel);}  // X * Y * Z
       uint get_cKernel(){return(this->cKernel);}  // X * Y
       uint get_cSPicXY(){return(this->cSPicXY);}  // Output X * Y
       uint get_cPicXY() {return(this->cPicXY);}   // Input  X * Y

       uint get_sKernel(){return(TpKernel.get_size());}

       // TEST
       void fill_Kernel(const double* const pKernelEx){
          if(pKernelEx == nullptr || this->pKernel == nullptr)
            return;
          //std::copy(pKernelEx, pKernelEx + TpKernel.get_size(), this->pKernel);
          memcpy(this->pKernel, pKernelEx, TpKernel.get_size());
       }

     private:

       S_Conv_Layer SConvLayer;
       
       //////////////////////////////////

       double* pKernel   = {nullptr};
       double* pGradient = {nullptr};
       double* pMomentum = {nullptr};

       C_Tensor<double> TpKernel;
       C_Tensor<double> TpGradient;
       C_Tensor<double> TpMomentum;

       ///////////////////////////////////

       uint* pDendrite = {nullptr};

       C_Tensor<uint> TpDendrite;
       
       ///////////////////////////////////
       
       uint cPicXY  = {0};
       uint cDelta  = {0};
       uint cSPicXY = {0};

       uint cKernel = {0};
       uint vKernel = {0};
 };

#endif // _C_CONV_LAYER_H_
