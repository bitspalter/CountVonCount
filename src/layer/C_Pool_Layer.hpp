//////////////////////////////////////////////////////////////////////////////////
// [ Pool_Layer Class_Header ]
//////////////////////////////////////////////////////////////////////////////////
//
// Pooling
//
// [::Last modi: 27.06.24 L.ey (µ~)::]
//
//
#ifndef _C_POOL_LAYER_H_
 #define _C_POOL_LAYER_H_
 
 #include <iostream>
 using namespace std;

 #include "C_NN_Layer.hpp"
 
//////////////////////////////////////////////////////////////////////////////////
// CONSTANT
//////////////////////////////////////////////////////////////////////////////////
 
 const int C_POOL_LAYER_ERROR = 0x00;
 const int C_POOL_LAYER_READY = 0x01;

//////////////////////////////////////////////////////////////////////////////////
// STRUCT
//////////////////////////////////////////////////////////////////////////////////

 struct S_Pool_Layer {
    S_NN_Tupel3D Pic;
    S_NN_Tupel2D Kernel;
    S_NN_Tupel2D Slide;
    S_NN_Tupel2D Pad;
    S_NN_Tupel2D cSPic;
 };

//////////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////////
 
 class C_Pool_Layer : public C_NN_Layer {
  
    public:
       
       C_Pool_Layer(){};
       C_Pool_Layer(int layer_typ) : C_NN_Layer(layer_typ){};
       virtual ~C_Pool_Layer(){};
      
       int  create(S_Pool_Layer* pSPool);
       
       void forward(const double* const pInput);
       void backward(const double* const pDeltaIn);
       void update(){};

       ////////////////////////////////

       S_Pool_Layer get_slayer(){return(this->SPoolLayer);};
       
    private:
       
       S_Pool_Layer SPoolLayer;

       ////////////////////////////////

       uint cOutput = {0};
       uint cInput  = {0};

       uint cOutXY  = {0};
       uint cInXY   = {0};

       /////////////////////////////////

       uint* pBack = {nullptr};

       C_Tensor<uint> TpBack;
 };

#endif // _C_POOL_LAYER_H_
