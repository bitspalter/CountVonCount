//////////////////////////////////////////////////////////////////////////////////
// [ Comv_Layer_Class_Source ]
//////////////////////////////////////////////////////////////////////////////////

#include "C_Conv_Layer.hpp"

// Changed: dpu 07.10.23 - update bias
// Changed: dpu 06.06.24 - add activation class

//////////////////////////////////////////////////////////////////////////////////
// [Destructor]
//////////////////////////////////////////////////////////////////////////////////
C_Conv_Layer::~C_Conv_Layer(){

}
//////////////////////////////////////////////////////////////////////////////////
// [create]
//////////////////////////////////////////////////////////////////////////////////
int C_Conv_Layer::create(S_Conv_Layer* pSCNNLayer){
   
   if(!pSCNNLayer) return(C_CONV_LAYER_ERROR);

   C_NN_Layer::type = C_NN_LAYER_CONV;
   
   pSCNNLayer->cSPic.x = (pSCNNLayer->Pic.x - pSCNNLayer->Kernel.x) / pSCNNLayer->Slide.x + 1;
   pSCNNLayer->cSPic.y = (pSCNNLayer->Pic.y - pSCNNLayer->Kernel.y) / pSCNNLayer->Slide.y + 1;

   SConvLayer = *pSCNNLayer;

   this->cKernel = SConvLayer.Kernel.x * SConvLayer.Kernel.y;
   this->vKernel = pSCNNLayer->Kernel.z * this->cKernel;

   ////////////////////////////////////////////////////
   // create
   //
   // pKernel[ pSCNNLayer->PicZ ][ pSCNNLayer->KernelZ ][ pSCNNLayer->KernelX * pSCNNLayer->KernelY ];
   
   TpKernel.  create(pSCNNLayer->Pic.z, pSCNNLayer->Kernel.z, this->cKernel);
   TpGradient.create(pSCNNLayer->Pic.z, pSCNNLayer->Kernel.z, this->cKernel);
   TpMomentum.create(pSCNNLayer->Pic.z, pSCNNLayer->Kernel.z, this->cKernel);

   this->pKernel   = TpKernel.  get_data();
   this->pGradient = TpGradient.get_data();
   this->pMomentum = TpMomentum.get_data();

   this->TnBatch.   create(pSCNNLayer->Kernel.z, 1, 1);
   this->TpBias.    create(pSCNNLayer->Kernel.z, 1, 1);
   this->TpBiasGrad.create(pSCNNLayer->Kernel.z, 1, 1);
   
   this->nBatch    = this->TnBatch.   get_data();
   this->pBias     = this->TpBias.    get_data();
   this->pBiasGrad = this->TpBiasGrad.get_data();
   
   /////////////////////////////////////////////////////

   for(uint nPic = 0; nPic < pSCNNLayer->Pic.z; nPic++){
      
      for(uint nKernel = 0; nKernel < pSCNNLayer->Kernel.z; nKernel++){

         double sum = 0;

         do{
            sum = 0;

            for(uint k = 0; k < pSCNNLayer->Kernel.x * pSCNNLayer->Kernel.y; k++){

               sum += 
               pKernel[nPic * this->vKernel + nKernel * this->cKernel + k] = 
               (-0.5 + (double)rand() / RAND_MAX) / pSCNNLayer->Divisor;
            }
         }while(sum < 0.0 || sum > 0.1);

         this->pBias[nKernel] = sum - 0.05;
      }
   }

   /////////////////////////////////////////////////////

   this->TDelta. create(pSCNNLayer->cSPic.x, pSCNNLayer->cSPic.y, pSCNNLayer->Kernel.z);
   this->TOutput.create(pSCNNLayer->cSPic.x, pSCNNLayer->cSPic.y, pSCNNLayer->Kernel.z);

   this->pDelta  = this->TDelta. get_data();
   this->pOutput = this->TOutput.get_data();

   /////////////////////////////////////////////////////

   this->TDeltaOut.create(pSCNNLayer->Pic.x, pSCNNLayer->Pic.y, pSCNNLayer->Pic.z);
   this->pDeltaOut = this->TDeltaOut.get_data();

   /////////////////////////////////////////////////////
   
   cPicXY  = SConvLayer.Pic.x * SConvLayer.Pic.y;
   cDelta  = sizeof(double) * cPicXY * SConvLayer.Pic.z;
   cSPicXY = SConvLayer.cSPic.x * SConvLayer.cSPic.y;

   /////////////////////////////////////////////////////

   TpDendrite.create(cSPicXY, this->cKernel, 1);

   this->pDendrite = TpDendrite.get_data();

   for(uint dy = 0, n = 0; dy < SConvLayer.cSPic.y; dy++){
      for(uint dx = 0; dx < SConvLayer.cSPic.x; dx++, n++){
         for(uint y = dy * SConvLayer.Slide.y, m = 0; y < SConvLayer.Kernel.y + dy * SConvLayer.Slide.y; y++){
            for(uint x = dx * SConvLayer.Slide.x; x < SConvLayer.Kernel.x + dx * SConvLayer.Slide.x; x++, m++){
               this->pDendrite[n * this->cKernel + m] = y * SConvLayer.Pic.x + x;
            }
         } 
      }
   }

   //////////////////////////////////////////////////////

   switch(pSCNNLayer->activation){

      case C_NN_ACTIVATION_RELU:
         C_NN_Layer::pActivation = new C_NN_Relu(); break;

      case C_NN_ACTIVATION_SIGM:
         C_NN_Layer::pActivation = new C_NN_Sigm(); break;
      
      case C_NN_ACTIVATION_SOFT:
         C_NN_Layer::pActivation = new C_NN_Soft(); break;

      case C_NN_ACTIVATION_IDENT:
         C_NN_Layer::pActivation = new C_NN_Ident(); break; 

      default: cout << "SCNNLayer->activation error" << endl; break;
   }

   return(C_CONV_LAYER_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [forward]
//////////////////////////////////////////////////////////////////////////////////
void C_Conv_Layer::forward(const double* const pInput){

   //cout << "C_Conv_Layer::forward" << endl;

   this->pInput = pInput;

   for(uint nKernel = 0; nKernel < SConvLayer.Kernel.z; nKernel++){

      double sum = 0; // TEST

      for(uint nInput = 0; nInput < this->cSPicXY; nInput++){

         double t = 0;
            
         for(uint nPic = 0; nPic < SConvLayer.Pic.z; nPic++){

            int ck = nPic * this->cPicXY;
               
            for(uint m = 0; m < this->cKernel; m++){

               t += this->pKernel[nPic * this->vKernel + nKernel * this->cKernel + m] * 
                    this->pInput[ck + this->pDendrite[nInput * this->cKernel + m]];
            }
         }
            
         if(t > 0) t -= this->pBias[nKernel];
            
         sum += this->pOutput[nKernel * this->cSPicXY + nInput] = t;
      }
   }

   ////////////////

   pActivation->forward(this->pOutput, this->SConvLayer.Kernel.z * this->cSPicXY);
}
//////////////////////////////////////////////////////////////////////////////////
// [backward]
//////////////////////////////////////////////////////////////////////////////////
void C_Conv_Layer::backward(const double* const pDeltaIn){

   memset(this->pDeltaOut, 0, cDelta);
   
   /////////////////////////////////////////////

   pActivation->backward(this->pOutput, this->SConvLayer.Kernel.z * this->cSPicXY, 
                         pDeltaIn, this->pDelta, this->SConvLayer.bHidden);

   /////////////////////////////////////////////
   
   for(uint nInput = 0; nInput < cSPicXY; nInput++){

      for(uint nKernel = 0; nKernel < SConvLayer.Kernel.z; nKernel++){
            
         if(!pDeltaIn[nKernel * cSPicXY + nInput] || !pOutput[nKernel * cSPicXY + nInput]){
            continue;
         }

         int ck2 = nKernel * cSPicXY;

         double DLR = pDelta[ck2 + nInput] * SConvLayer.Alpha;

         for(uint nPic = 0; nPic < SConvLayer.Pic.z; nPic++){
         
            int ck = nPic * cPicXY;
               
            for(uint m = 0; m < cKernel; m++){
               this->pGradient[nPic * this->vKernel + nKernel * this->cKernel + m] += 
               DLR * this->pInput[ck + this->pDendrite[nInput * this->cKernel + m]];

               this->pDeltaOut[ck + this->pDendrite[nInput * this->cKernel + m]] += 
               this->pDelta[ck2 + nInput] * this->pKernel[nPic * this->vKernel + nKernel * this->cKernel + m];
            }
         }
            
         this->pBiasGrad[nKernel] += DLR;
               
         this->nBatch[nKernel]++;
      }
   }
}
//////////////////////////////////////////////////////////////////////////////////
// [update]
//////////////////////////////////////////////////////////////////////////////////
void C_Conv_Layer::update(){

   for(uint nKernel = 0; nKernel < SConvLayer.Kernel.z; nKernel++){

      if(nBatch[nKernel]){
            
         for(uint nPic = 0; nPic < SConvLayer.Pic.z; nPic++){ 

            for(uint m = 0; m < this->cKernel; m++){

               uint offset = nPic * this->vKernel + nKernel * this->cKernel + m;
          
               double sum = pGradient[offset] / nBatch[nKernel] + pMomentum[offset] * SConvLayer.Beta;

               pKernel[offset] += sum;

               pMomentum[offset] = pGradient[offset] / nBatch[nKernel];
               pGradient[offset] = 0;
            } 
         }  
      
         double sum = pBiasGrad[nKernel] / nBatch[nKernel];

         // TEST
         //pBias[k2] -= sum / (SConvLayer.KernelX * SConvLayer.KernelY);
         pBias[nKernel] -= sum;

         pBiasGrad[nKernel] = 0;
         nBatch[nKernel]    = 0;
      }
   }
}
