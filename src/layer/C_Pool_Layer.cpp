//////////////////////////////////////////////////////////////////////////////////
// [ Pool_Layer_Class_Source ]
//////////////////////////////////////////////////////////////////////////////////

#include "C_Pool_Layer.hpp"

//////////////////////////////////////////////////////////////////////////////////
// [ create ]
//////////////////////////////////////////////////////////////////////////////////
int C_Pool_Layer::create(S_Pool_Layer* pSPool){
   
   if(!pSPool) return(C_POOL_LAYER_ERROR);

   C_NN_Layer::type = C_NN_LAYER_POOL;
   
   pSPool->cSPic.x = (pSPool->Pic.x - pSPool->Kernel.x) / pSPool->Slide.x + 1;
   pSPool->cSPic.y = (pSPool->Pic.y - pSPool->Kernel.y) / pSPool->Slide.y + 1;
   
   pSPool->cSPic.x += pSPool->Pad.x * 2;
   pSPool->cSPic.y += pSPool->Pad.y * 2;

   cOutput = pSPool->Pic.z * pSPool->cSPic.x * pSPool->cSPic.y;
   cInput  = pSPool->Pic.z * pSPool->Pic.x  * pSPool->Pic.y;
   
   // TEST
   this->TOutput.create(pSPool->cSPic.x, pSPool->cSPic.y, pSPool->Pic.z); // TEST
   this->pOutput = this->TOutput.get_data();

   // TEST
   this->TDeltaOut.create(pSPool->Pic.x, pSPool->Pic.y, pSPool->Pic.z); // TEST
   this->pDeltaOut = this->TDeltaOut.get_data();
   
   // TEST
   this->TpBack.create(cOutput, 1, 1); // TEST
   this->pBack = this->TpBack.get_data();

   //////////////////////////

   memcpy(&SPoolLayer, pSPool, sizeof(S_Pool_Layer));

   //////////////////////////
   
   cOutXY = SPoolLayer.cSPic.x * SPoolLayer.cSPic.y;
   cInXY  = SPoolLayer.Pic.x  * pSPool->Pic.y;

   return(C_POOL_LAYER_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [ forward ]
//////////////////////////////////////////////////////////////////////////////////
void C_Pool_Layer::forward(const double* const pInput){

   for(unsigned int input_n = 0; input_n < SPoolLayer.Pic.z; input_n++){

      unsigned int off_input  = input_n * cInXY;
      unsigned int off_output = input_n * cOutXY;

      for(unsigned int output_y = 0, n = 0; output_y < SPoolLayer.cSPic.y - 2 * SPoolLayer.Pad.y; output_y++){
         for(unsigned int output_x = 0; output_x < SPoolLayer.cSPic.x - 2 * SPoolLayer.Pad.x; output_x++, n++){
             
            double dmax = 0;
            unsigned int nMax = 0; 
            
            for(unsigned int y = output_y * SPoolLayer.Slide.y, m = 0; y < SPoolLayer.Kernel.y + output_y * SPoolLayer.Slide.y; y++){
               for(unsigned int x = output_x * SPoolLayer.Slide.x; x < SPoolLayer.Kernel.x + output_x * SPoolLayer.Slide.x; x++, m++){
                  int v = y * SPoolLayer.Pic.x + x; 

                  if(pInput[off_input + v] > dmax){
                  //if(abs(pInput[off_input + v]) > abs(dmax)){
                     dmax = pInput[off_input + v];
                     nMax = v;
                  }
               }
            }

            unsigned int out = off_output + 
                              (output_y + SPoolLayer.Pad.y) * SPoolLayer.cSPic.x + 
                               output_x + SPoolLayer.Pad.x;

            //cout << "POOLTEST: " << "OUT: " << out << endl;

            pOutput[out] = dmax;

            pBack[out] = nMax + off_input;

            //if(input_n < 1)
            //   cout << out << " pBack[out]: " << pBack[out] << " nMax:" << nMax << " input_n * cInXY = " << input_n * cInXY << endl;

            //cout << "POOLTEST: " << "OUT: " << out << endl;
         }
      }
   }
}
//////////////////////////////////////////////////////////////////////////////////
// [backward]
//////////////////////////////////////////////////////////////////////////////////
void C_Pool_Layer::backward(const double* const pDeltaIn){

   //cout << "C_Pool_Layer::backward: " << endl;

   memset(pDeltaOut, 0, cInput * sizeof(double));
   
   for(unsigned int n = 0; n < cOutput; n++){
      pDeltaOut[pBack[n]] = pDeltaIn[n];
   }
}
