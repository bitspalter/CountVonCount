//////////////////////////////////////////////////////////////////////////////////
// [ C_NN_Visual_Header ]
//////////////////////////////////////////////////////////////////////////////////
//
//
// [::Last modi: 05.08.24 L.ey (µ~)::]
//
//
#ifndef _C_NN_VISUAL_H_
 #define _C_NN_VISUAL_H_

 #include "C_Surface.hpp"
 #include "C_NNetManager.hpp"

//////////////////////////////////////////////////////////////////////////////////
// CONSTANT
//////////////////////////////////////////////////////////////////////////////////
 
 const uint C_NN_VISUAL_ERROR = 0x00;
 const uint C_NN_VISUAL_READY = 0x01;

 const uint C_NN_VISUAL_NETWORK  = 0x00;
 const uint C_NN_VISUAL_GRADIENT = 0x01;

//////////////////////////////////////////////////////////////////////////////////
// STRUCT
//////////////////////////////////////////////////////////////////////////////////

 struct SSurface {
    C_Surface network;
    C_Surface gradient;
    C_Surface failed;
 };

//////////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////////

 class C_NN_Visual {

    public:

       C_NN_Visual(){};
      ~C_NN_Visual(){};

       uint init(C_NNetManager* pNManager, uint x, uint y);

       void show_network();
       void show_gradient();
       void show_failed();

       ////////////////////////////////

       auto const get_pSurfaces(){return(&sSurface);}

    private:

       uint show_head(C_Surface* pSurface);

       uint print_conv_layer(C_Surface* pSurface, C_Conv_Layer* pConv_Layer, uint posy);
       uint print_pool_layer(C_Surface* pSurface, C_Pool_Layer* pPool_Layer, uint posy);
       uint print_fc_layer  (C_Surface* pSurface, C_FC_Layer*     pFC_Layer, uint posx, uint posy);

       uint print_conv_kernel(C_Surface* pSurface, C_Conv_Layer* pConv_Layer, uint type, uint posy);
       uint print_fc_kernel  (C_Surface* pSurface, C_FC_Layer*     pFC_Layer, uint type, uint posy);

       /////////////////////////////////

       C_NNetManager* pNManager = {nullptr};

       /////////////////////////////////
       
       SSurface sSurface;

       /////////////////////////////////

       bool bReady = {false};

       uint x = {0};
       uint y = {0};
 };


#endif // _C_NN_VISUAL_H_


