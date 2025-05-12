//////////////////////////////////////////////////////////////////////////////////
// [ C_Activation_Header ]
//////////////////////////////////////////////////////////////////////////////////
//
// Base Class for Neural Network Layer Activation
//
// [::Last modi: 06.06.24 L.ey (µ~)::]
//
//
#ifndef _C_NN_ACTIVATION_H_
 #define _C_NN_ACTIVATION_H_

 #include <iostream>
 using namespace std;

 #include <math.h>

//////////////////////////////////////////////////////////////////////////////////
// CONSTANT
//////////////////////////////////////////////////////////////////////////////////
 //(exp(2 * this->pOutput[n]) - 1) / (exp(2 * this->pOutput[n]) + 1); // tanh
 
 const int C_NN_ACTIVATION_ERROR = 0x00;
 const int C_NN_ACTIVATION_READY = 0x01;

 const int C_NN_ACTIVATION_RELU  = 0x00;
 const int C_NN_ACTIVATION_SIGM  = 0x01;
 const int C_NN_ACTIVATION_SOFT  = 0x02;
 const int C_NN_ACTIVATION_IDENT = 0x03;

//////////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////////
 
 class C_NN_Activation {

    public:

       C_NN_Activation(){};
       virtual ~C_NN_Activation(){};

       virtual int forward(double* pData, unsigned int cData) = 0;

       virtual int backward(double* pOutput, unsigned int cOutput, 
                            const double* pDeltaIn, double* pDelta, 
                            bool bhidden) = 0;
 };

/////////////////////////////////////////////////////////////////////

 class C_NN_Ident : public C_NN_Activation {

    public:

       C_NN_Ident(){};
       virtual ~C_NN_Ident(){};

       //////////////////////////////////////////
       int forward(double* pData, unsigned int cData){

          if(pData == nullptr || cData == 0) 
            return(C_NN_ACTIVATION_ERROR);

          return(C_NN_ACTIVATION_READY);
       }

       //////////////////////////////////////////
       int backward(double* pOutput, unsigned int cOutput,
                    const double* pDeltaIn, double* pDelta, bool bhidden){

          if(pOutput  == nullptr || 
             pDeltaIn == nullptr || 
             pDelta   == nullptr || 
             cOutput  == 0) return(C_NN_ACTIVATION_ERROR);

          if(bhidden){
            for(unsigned int n = 0; n < cOutput; n++)
               pDelta[n] = pDeltaIn[n];
          }else{
            for(unsigned int n = 0; n < cOutput; n++)
               pDelta[n] = pDeltaIn[n] - pOutput[n];
          }

          return(C_NN_ACTIVATION_READY);
       }
 };

/////////////////////////////////////////////////////////////////////

 class C_NN_Relu : public C_NN_Activation {

    public:

       C_NN_Relu(){};
       virtual ~C_NN_Relu(){};

       //////////////////////////////////////////
       int forward(double* pData, unsigned int cData){

          if(pData == nullptr || cData == 0) return(C_NN_ACTIVATION_ERROR);

          for(unsigned int n = 0; n < cData; n++)
             pData[n] = (pData[n] < 0) ? 0 : pData[n];

          return(C_NN_ACTIVATION_READY);
       }

       //////////////////////////////////////////
       int backward(double* pOutput, unsigned int cOutput,
                    const double* pDeltaIn, double* pDelta, bool bhidden){

          if(pOutput  == nullptr || 
             pDeltaIn == nullptr || 
             pDelta   == nullptr || 
             cOutput  == 0) return(C_NN_ACTIVATION_ERROR);

          if(bhidden){
            for(unsigned int n = 0; n < cOutput; n++)
               (pOutput[n] > 0) ? pDelta[n] = pDeltaIn[n] : pDelta[n] = 0;
          }else{
            for(unsigned int n = 0; n < cOutput; n++)
               (pOutput[n] > 0) ? pDelta[n] = (pDeltaIn[n] - pOutput[n]) : pDelta[n] = 0;
          }

          return(C_NN_ACTIVATION_READY);
       }
 };

/////////////////////////////////////////////////////////////////////

 class C_NN_Sigm : public C_NN_Activation {

    public:

       C_NN_Sigm(){};
       virtual ~C_NN_Sigm(){};

       //////////////////////////////////////////
       int forward(double* pData, unsigned int cData){

          if(pData == nullptr || cData == 0) return(C_NN_ACTIVATION_ERROR);

          for(unsigned int n = 0; n < cData; n++)
             pData[n] = 1.0 - (2.0 / (1.0 + exp(2.0 * pData[n]))); // Sigmoid

             //this->pData[n] = (this->pData[n] < 0) ? 0 : this->pData[n];

          return(C_NN_ACTIVATION_READY);
       }

       //////////////////////////////////////////
       int backward(double* pOutput, unsigned int cOutput,
                const double* pDeltaIn, double* pDelta, bool bhidden){

          if(pOutput  == nullptr || 
             pDeltaIn == nullptr || 
             pDelta   == nullptr || 
             cOutput  == 0) return(C_NN_ACTIVATION_ERROR);

          if(bhidden){
            for(unsigned int n = 0; n < cOutput; n++)
               pDelta[n] = pDeltaIn[n] * (1 + pOutput[n]) * (1 - pOutput[n]); // Sigmoid
          }else{
            for(unsigned int n = 0; n < cOutput; n++)
               pDelta[n] = (pDeltaIn[n] - pOutput[n]) * (1 + pOutput[n]) * (1 - pOutput[n]); // Sigmoid
          }

          return(C_NN_ACTIVATION_READY);
       }
 };

/////////////////////////////////////////////////////////////////////

 class C_NN_Soft : public C_NN_Activation {

    public:

       C_NN_Soft(){};
       virtual ~C_NN_Soft(){};

       //////////////////////////////////////////
       int forward(double* pData, unsigned int cData){

          if(pData == nullptr || cData == 0) return(C_NN_ACTIVATION_ERROR);

          double sum = 0;

          for(unsigned int m = 0; m < cData; m++){
             pData[m] = exp(pData[m]);
             sum += pData[m];
          }

          double C = 0.00001;
 
          sum += C;

          for(unsigned int m = 0; m < cData; m++){
             pData[m] = (pData[m] + C) / sum;
          }

          return(C_NN_ACTIVATION_READY);
       }

       //////////////////////////////////////////
       int backward(double* pOutput, unsigned int cOutput,
                    const double* pDeltaIn, double* pDelta, bool bhidden){

          if(pOutput  == nullptr || 
             pDeltaIn == nullptr || 
             pDelta   == nullptr || 
             bhidden  == true    ||
             cOutput  == 0) return(C_NN_ACTIVATION_ERROR);

          for(unsigned int n = 0; n < cOutput; n++){
             pDelta[n] = pDeltaIn[n] - pOutput[n];
          }

          return(C_NN_ACTIVATION_READY);
       }
 };

#endif // _C_NN_ACTIVATION_H_