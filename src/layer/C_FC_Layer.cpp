//////////////////////////////////////////////////////////////////////////////////
// [ FC_Layer_Class_Source ]
//////////////////////////////////////////////////////////////////////////////////

#include "C_FC_Layer.hpp"

//////////////////////////////////////////////////////////////////////////////////
// [Destructor]
//////////////////////////////////////////////////////////////////////////////////
C_FC_Layer::~C_FC_Layer(){

   //cout << "dectructor C_FC_Layer" << endl;

   //if(pWeight == nullptr) return;

   for(unsigned int n = 0; n < SFC_Layer.cOutput; n++){
      //delete [] this->pGradient[n];
      //delete [] this->pMomentum[n];
      //delete [] this->pWeight[n];
   }
   
   //delete [] this->pGradient;
   //delete [] this->pMomentum;
   //delete [] this->pWeight;
}
//////////////////////////////////////////////////////////////////////////////////
// [create]
//////////////////////////////////////////////////////////////////////////////////
int C_FC_Layer::create(S_FC_Layer* pS_FC_Layer){
   
   if(!pS_FC_Layer) return(C_FC_LAYER_ERROR);

   C_NN_Layer::type = C_NN_LAYER_FC;
   
   this->TOutput.   create(pS_FC_Layer->cOutput, 1, 1);
   this->TnBatch.   create(pS_FC_Layer->cOutput, 1, 1);
   this->TDelta.    create(pS_FC_Layer->cOutput, 1, 1);
   this->TpBias.    create(pS_FC_Layer->cOutput, 1, 1);
   this->TpBiasGrad.create(pS_FC_Layer->cOutput, 1, 1);

   this->pOutput   = this->TOutput.   get_data();
   this->nBatch    = this->TnBatch.   get_data();
   this->pDelta    = this->TDelta.    get_data();
   this->pBias     = this->TpBias.    get_data();
   this->pBiasGrad = this->TpBiasGrad.get_data();

   this->TDeltaOut.create(pS_FC_Layer->cInput, 1, 1);
   this->pDeltaOut = this->TDeltaOut.get_data();

   ////////////////////////////////

   this->TpGradient.create(pS_FC_Layer->cOutput, pS_FC_Layer->cInput, 1);
   this->TpMomentum.create(pS_FC_Layer->cOutput, pS_FC_Layer->cInput, 1);
   this->TpKernel.  create(pS_FC_Layer->cOutput, pS_FC_Layer->cInput, 1);

   this->pKernel   = this->TpKernel.  get_data();
   this->pGradient = this->TpGradient.get_data();
   this->pMomentum = this->TpMomentum.get_data();

   //////////////////////////////////
   // Init Kernel

   for(uint n = 0; n < pS_FC_Layer->cOutput; n++){

      double sum = 0;

      do{
         sum = 0;

         for(unsigned int w = 0; w < pS_FC_Layer->cInput; w++)
            sum += pKernel[n * pS_FC_Layer->cInput + w] = 
            (-0.50 + (double)rand() / RAND_MAX) / pS_FC_Layer->Divisor;
      
      }while(sum < -0.4 || sum > 0.4);

      this->pBias[n]     = sum - 0.05; // TEST
      this->pDelta[n]    = 0;
      this->pBiasGrad[n] = 0;
   }
   
   ///////////////////////////////////////////////////
   
   SFC_Layer = *pS_FC_Layer;

   //////////////////////////////////////////////////////
   // TEST

   switch(pS_FC_Layer->activation){

      case 0: C_NN_Layer::pActivation = new C_NN_Relu(); break;
      case 1: C_NN_Layer::pActivation = new C_NN_Sigm(); break;
      case 2: C_NN_Layer::pActivation = new C_NN_Soft(); break;
      default: 
         cout << "FC_Layer->activation error" << endl; break;
   }

   return(C_FC_LAYER_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [forward]
//////////////////////////////////////////////////////////////////////////////////
void C_FC_Layer::forward(const double* const pInput){
   
   this->pInput = pInput;

   for(uint nOutput = 0; nOutput < this->SFC_Layer.cOutput; nOutput++){
      
      this->pOutput[nOutput] = 0;

      uint nOut = nOutput * this->SFC_Layer.cInput;

      for(uint nInput = 0; nInput < this->SFC_Layer.cInput; nInput++)
         this->pOutput[nOutput] += this->pInput[nInput] * this->pKernel[nOut + nInput];

      this->pOutput[nOutput] -= this->pBias[nOutput];
  }

   //////////////////////////////////////////////////////////

   this->pActivation->forward(this->pOutput, this->SFC_Layer.cOutput);
}

//////////////////////////////////////////////////////////////////////////////////
// [backward]
//////////////////////////////////////////////////////////////////////////////////
void C_FC_Layer::backward(const double* const pDeltaIn){
   
   this->pActivation->backward(this->pOutput, this->SFC_Layer.cOutput, pDeltaIn, 
                               this->pDelta, this->SFC_Layer.bHidden);

   ///////////////////////////////////////////////////////

   for(uint nOutput = 0; nOutput < this->SFC_Layer.cOutput; nOutput++){
         
      if(pDelta[nOutput]){

         uint nOut = nOutput * this->SFC_Layer.cInput;

         this->mse += (this->pDelta[nOutput] * this->pDelta[nOutput]) / 2;

         double DLR = this->pDelta[nOutput] * this->SFC_Layer.Alpha;

         for(uint m = 0; m < this->SFC_Layer.cInput; m++){
            this->pGradient[nOut + m] += DLR * this->pInput[m];
         }

         this->pBiasGrad[nOutput] += DLR;
         this->nBatch[nOutput]++;
      }
   }

   ///////////////////////////////////////////////////////

   for(uint nInput = 0; nInput < SFC_Layer.cInput; nInput++){
      pDeltaOut[nInput] = 0;
      for(uint nOutput = 0; nOutput < SFC_Layer.cOutput; nOutput++){
         pDeltaOut[nInput] += this->pDelta[nOutput] * pKernel[nOutput * SFC_Layer.cInput + nInput];
      }
   }
}
//////////////////////////////////////////////////////////////////////////////////
// [update]
//////////////////////////////////////////////////////////////////////////////////
void C_FC_Layer::update(){
   
   for(uint nOutput = 0; nOutput < this->SFC_Layer.cOutput; nOutput++){
      
      if(!this->nBatch[nOutput]) continue;
      
      for(uint nInput = 0; nInput < this->SFC_Layer.cInput; nInput++){

         uint offset = nOutput * this->SFC_Layer.cInput + nInput;

         double sum = this->pGradient[offset] / this->nBatch[nOutput] + this->pMomentum[offset] * this->SFC_Layer.Beta;

         this->pKernel[offset] += sum;

         this->pMomentum[offset] = this->pGradient[offset] / this->nBatch[nOutput];

         this->pGradient[offset] = 0;
      }

      double sum = this->pBiasGrad[nOutput] / this->nBatch[nOutput];

      this->pBias[nOutput] -= sum;
      this->pBiasGrad[nOutput] = 0;
      
      this->nBatch[nOutput] = 0;
   }
   
   this->mse_last = this->mse;
   this->mse = 0;

   ///////////////////////////////////////////////////////////////
   // Test Weight Decay

/*    double qsum    = 0;
   double qsumAll = 0; 

   for(uint nOutput = 0; nOutput < SFC_Layer.cOutput; nOutput++){

      for(uint nInput = 0; nInput < SFC_Layer.cInput; nInput++){
         uint offset = nOutput * SFC_Layer.cInput + nInput;
         qsum += pKernel[offset] * pKernel[offset];
      }

      //cout << "nOutput:" << nOutput << " qsum:" << qsum << endl;

      qsumAll += qsum;
      qsum = 0;
   }

   //cout << "qsumAll:" << qsumAll << " " << (qsumAll / (SFC_Layer.cOutput * SFC_Layer.cInput)) / 100 << endl;

   ///////////////////////////////////////////////////////////////

   double wd = (qsumAll / (SFC_Layer.cOutput * SFC_Layer.cInput)) / 100;

   for(uint nOutput = 0; nOutput < SFC_Layer.cOutput; nOutput++){

      for(uint nInput = 0; nInput < SFC_Layer.cInput; nInput++){
         uint offset = nOutput * SFC_Layer.cInput + nInput;
         pKernel[offset] < 0 ? pKernel[offset] += wd : pKernel[offset] -= wd;
      }
   } */
}
