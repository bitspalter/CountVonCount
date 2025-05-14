//////////////////////////////////////////////////////////////////////////////////
// [ FC_Layer_Class_Header ]
//////////////////////////////////////////////////////////////////////////////////
//
// Fully Connect Neural Network Layer
//
// [::Last modi: 27.06.24 L.ey (µ~)::]
//
//
#ifndef _C_FC_LAYER_H_
 #define _C_FC_LAYER_H_
 
 #include <iostream>
 using namespace std;

 #include "C_NN_Layer.hpp"
 
//////////////////////////////////////////////////////////////////////////////////
// CONSTANT
//////////////////////////////////////////////////////////////////////////////////
 
 const int C_FC_LAYER_ERROR = 0x00;
 const int C_FC_LAYER_READY = 0x01;

//////////////////////////////////////////////////////////////////////////////////
// STRUCT
//////////////////////////////////////////////////////////////////////////////////

 struct S_FC_Layer {
    uint cInput;
    uint cOutput;
    bool bHidden;
    double Alpha;
    double Beta;
    double Bias;
    double Divisor;
    uint8_t activation;
 };

//////////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////////
 
 class C_FC_Layer : public C_NN_Layer {
  
    public:
       
       C_FC_Layer(){};
       C_FC_Layer(int layer_typ) : C_NN_Layer(layer_typ){};
       virtual ~C_FC_Layer();
      
       int create(S_FC_Layer* pS_FC_Layer);
       
       void forward(const double* const pInput);
       void backward(const double* const pDeltaIn);
       void update();
       
       //////////////////////////////////////////////

       S_FC_Layer get_slayer(){return(this->SFC_Layer);};

       //////////////////////////////////////////////

       const double* get_pKernel()  {return(this->pKernel);}
       const double* get_pGradient(){return(this->pGradient);}

       uint get_sKernel(){return(TpKernel.get_size());}

       // TEST
       void fill_Kernel(double* pKernelEx){
          if(pKernelEx == nullptr || this->pKernel == nullptr)
            return;
            
          memcpy(this->pKernel, pKernelEx, TpKernel.get_size());
       }

       // TEST
       double mse_last = {0};

    private:
       
       S_FC_Layer SFC_Layer;

       //////////////////////////////////////

       double* pKernel   = {nullptr};
       double* pGradient = {nullptr};
       double* pMomentum = {nullptr};

       //////////////////////////////////////

       C_Tensor<double> TpKernel;
       C_Tensor<double> TpGradient;
       C_Tensor<double> TpMomentum;

       //////////////////////////////////////

       double mse = {0};
       
 };

#endif // _C_FC_LAYER_H_
