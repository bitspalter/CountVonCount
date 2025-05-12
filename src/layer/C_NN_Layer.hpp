//////////////////////////////////////////////////////////////////////////////////
// [ NN_Layer_Class_Header ]
//////////////////////////////////////////////////////////////////////////////////
//
// Base Class for Neural Network Layer
//
// [::Last modi: 27.06.24 L.ey (µ~)::]
//
//
#ifndef _C_NN_LAYER_H_
 #define _C_NN_LAYER_H_
 
 #include <iostream>
 using namespace std;

 #include <math.h>
 #include <string.h>

 #include "C_Activation.hpp"

 #include "C_Tensor.hpp"
 
//////////////////////////////////////////////////////////////////////////////////
// CONSTANT
//////////////////////////////////////////////////////////////////////////////////
 
 const int C_NN_LAYER_ERROR = 0x00;
 const int C_NN_LAYER_READY = 0x01;

 const uint C_NN_LAYER_CONV  = 0x00;
 const uint C_NN_LAYER_POOL  = 0x01;
 const uint C_NN_LAYER_FC    = 0x02;

//////////////////////////////////////////////////////////////////////////////////
// STRUCT
//////////////////////////////////////////////////////////////////////////////////
 
 struct S_NN_Tupel2D {
    uint32_t x = {0};
    uint32_t y = {0};
 };

 struct S_NN_Tupel3D {
    uint32_t x = {0};
    uint32_t y = {0};
    uint32_t z = {0};
 };

//////////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////////
 
 class C_NN_Layer {
  
    public:
    
       C_NN_Layer(){};
       C_NN_Layer(int layer_typ) : type(layer_typ){};

       // [Destructor] //////////////////////////////////
       virtual ~C_NN_Layer(){
          if(pActivation != nullptr) 
             delete pActivation; // TEST
       }
      
       virtual int create(){return(C_NN_LAYER_READY);};

       virtual void forward(const double* const pInput) = 0;
       virtual void backward(const double* const pDeltaIn) = 0;
       virtual void update() = 0;

       // TEST
       const double* get_pOutput()  {return(this->pOutput);}
       const double* get_pDelta()   {return(this->pDelta);}
       const double* get_pDeltaOut(){return(this->pDeltaOut);}

       const double* get_pBias(){return(this->pBias);}
       uint get_sBias(){return(TpBias.get_size());}

       uint get_type(){return(this->type);}

       void fill_Bias(double* pBiasEx){
          if(pBiasEx == nullptr || this->pBias == nullptr)
            return;
          memcpy(this->pBias, pBiasEx, TpBias.get_size());
       }

    protected:

       const double* pInput = {nullptr}; // extern
       
       double* pOutput   = {nullptr}; // global
       double* pDelta    = {nullptr}; // global
       double* pDeltaOut = {nullptr}; // global

       double* nBatch    = {nullptr}; // local
       double* pBias     = {nullptr}; // local
       double* pBiasGrad = {nullptr}; // local

       //////////////////////////////////

       C_Tensor<double> TOutput;
       C_Tensor<double> TDelta;
       C_Tensor<double> TDeltaOut;

       C_Tensor<double> TnBatch; //??? <uint>
       C_Tensor<double> TpBias;
       C_Tensor<double> TpBiasGrad;

       //////////////////////////////////

       C_NN_Activation* pActivation = {nullptr};

       ///////////////////////////////////

       uint type;

    private:
       
       bool dropout = {false};
 };

#endif // _C_NN_LAYER_H_
