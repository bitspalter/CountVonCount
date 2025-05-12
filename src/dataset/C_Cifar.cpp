//////////////////////////////////////////////////////////////////////////////////
// [ C_DataSet_Class_Source ]
//////////////////////////////////////////////////////////////////////////////////

#include "C_Cifar.hpp"

////////////////////////////////////////////////////////////////////////////////////////////
// [ create ]
////////////////////////////////////////////////////////////////////////////////////////////
int C_Cifar::create(uint Sx, uint Sy){

   this->Sx = Sx;
   this->Sy = Sy;

   if(read_train_cases(Sx, Sy) != C_DATASET_READY){
      std::cout << "C_Cifar::error::read_train_cases\n";
      return(C_DATASET_ERROR);
   }

   if(read_test_cases(Sx, Sy) != C_DATASET_READY){
      std::cout << "C_Cifar::error::read_test_cases\n";
      return(C_DATASET_ERROR);
   }

   //normalize(); // TEST

   //create_luminanz(Sx, Sy);

   //create_filter(Sx, Sy);

   create_Numbers();

   return(C_DATASET_READY);
}

////////////////////////////////////////////////////////////////////////////////////////////
// [ destroy ]
////////////////////////////////////////////////////////////////////////////////////////////
int C_Cifar::destroy(){
   
   vTrainImg.clear();
   vTrainLab.clear();
   //vTrainImgLum.clear();
   
   vTestImg.clear();
   vTestLab.clear();
   //vTestImgLum.clear();
   
   vTrainNumbers.clear();
   vTestNumbers.clear();

   for(int n = 0; n < 10; n++){
      vTrainNumbers[n].clear();
      vTestNumbers[n].clear();
   }
   
   return(C_DATASET_READY);
}

////////////////////////////////////////////////////////////////////////////////////////////
// [ read_train_cases ]
////////////////////////////////////////////////////////////////////////////////////////////
int C_Cifar::read_train_cases(uint Sx, uint Sy){

   u_char* train_image = read_file(C_CIFAR_FILEPATH_TRAIN);

   if(train_image == nullptr) return(C_DATASET_ERROR);

   vTrainImg.resize(C_CIFAR_TRAIN);
   vTrainLab.resize(C_CIFAR_TRAIN);

   uint dx = (Sx - C_CIFAR_X) / 2;
   uint dy = (Sy - C_CIFAR_Y) / 2;

   uint offset = 0;
   uint cPic = C_CIFAR_X * C_CIFAR_Y * C_CIFAR_Z;

   for(uint i = 0; i < C_CIFAR_TRAIN; i++){

      u_char* label = train_image + offset++;
      u_char* img   = train_image + offset;

      vTrainImg[i].resize(Sx * Sy * C_CIFAR_Z);
      vTrainLab[i].resize(C_CIFAR_CLASSES);

      for(uint n = 0; n < C_CIFAR_Z; n++){

         uint off1 = n * C_CIFAR_X * C_CIFAR_Y;
         uint off2 = n * Sx * Sy;

         for(uint x = 0; x < C_CIFAR_X; x++)
            for(uint y = 0; y < C_CIFAR_Y; y++)
               vTrainImg[i][x + dx + (y + dy) * Sx + off2] = img[x + y * C_CIFAR_X + off1] / 255.f;
      }

      for(uint b = 0; b < C_CIFAR_CLASSES; b++) vTrainLab[i][b] = *label == b ? 1.0f : 0.0f; //1.0f - 2.0f;

      //////////////

      offset += cPic;
   }

   delete [] train_image;

   return(C_DATASET_READY);
}
////////////////////////////////////////////////////////////////////////////////////////////
// [ read_test_cases ]
////////////////////////////////////////////////////////////////////////////////////////////
int C_Cifar::read_test_cases(uint Sx, uint Sy){

   u_char* test_image = read_file(C_CIFAR_FILEPATH_TEST);

   if(test_image == nullptr) return(C_DATASET_ERROR);

   vTestImg.resize(C_CIFAR_TEST);
   vTestLab.resize(C_CIFAR_TEST);

   uint dx = (Sx - C_CIFAR_X) / 2;
   uint dy = (Sy - C_CIFAR_Y) / 2;

   uint offset = 0;
   uint cPic = C_CIFAR_X * C_CIFAR_Y * C_CIFAR_Z;

   for(uint i = 0; i < C_CIFAR_TEST; i++){

      u_char* label = test_image + offset++;
      u_char* img   = test_image + offset;

      vTestImg[i].resize(Sx * Sy * C_CIFAR_Z);
      vTestLab[i].resize(C_CIFAR_CLASSES);

      for(uint n = 0; n < C_CIFAR_Z; n++){

         uint off1 = n * C_CIFAR_X * C_CIFAR_Y;
         uint off2 = n * Sx * Sy;

         for(uint x = 0; x < C_CIFAR_X; x++)
            for(uint y = 0; y < C_CIFAR_Y; y++)
               vTestImg[i][x + dx + (y + dy) * Sx + off2] = img[x + y * C_CIFAR_X + off1] / 255.f;
      }

      for(uint b = 0; b < C_CIFAR_CLASSES; b++) vTestLab[i][b] = *label == b ? 1.0f : 0.0f; //1.0f - 2.0f;

      //////////////

      offset += cPic;
   }

   delete [] test_image;

   return(C_DATASET_READY);
}

////////////////////////////////////////////////////////////////////////////////////////////
// [ create_Numbers ]
////////////////////////////////////////////////////////////////////////////////////////////
void C_Cifar::create_Numbers(){

   vTrainNumbers.resize(C_CIFAR_CLASSES);
   vTestNumbers.resize(C_CIFAR_CLASSES);

   for(uint n = 0; n < 10; n++){
      vTrainNumbers[n].resize(5000);
      vTestNumbers[n].resize(1000);
   }

   uint Numbers[C_CIFAR_CLASSES] = {0,0,0,0,0,0,0,0,0,0};
   uint Offset[C_CIFAR_CLASSES]  = {0,0,0,0,0,0,0,0,0,0};

   for(uint pic = 0; pic < C_CIFAR_TRAIN; pic++){

      double* pBufferK = &vTrainLab[pic][0];

      for(uint n = 0; n < C_CIFAR_CLASSES; n++){
         if(pBufferK[n] == 1){
            Numbers[n]++;
            vTrainNumbers[n][Offset[n]] = pic;
            Offset[n]++;
         }
      }
   }

   ///////////////////////////

   for(uint n = 0; n < C_CIFAR_CLASSES; n++){
      Numbers[n] = 0;
      Offset[n]  = 0;
   }

   ///////////////////////////

   for(uint pic = 0; pic < C_CIFAR_TEST; pic++){

      double* pBufferK = &vTestLab[pic][0];

      for(uint n = 0; n < C_CIFAR_CLASSES; n++){
         if(pBufferK[n] == 1){
            Numbers[n]++;
            vTestNumbers[n][Offset[n]] = pic;
            Offset[n]++;
         }
      }
   }
}

////////////////////////////////////////////////////////////////////////////////////////////
// [ read_test_cases ]
////////////////////////////////////////////////////////////////////////////////////////////
/* int C_Cifar::create_luminanz(uint Sx, uint Sy){
   
   vTrainImgLum.resize(C_DATASET_CIFAR_TRAIN);
   vTestImgLum.resize(C_DATASET_CIFAR_TEST);

   for(uint i = 0; i < C_DATASET_CIFAR_TRAIN; i++){

      vTrainImgLum[i].resize(Sx * Sy);

      for(uint n = 0; n < 1024; n++){

         vTrainImgLum[i][n] = vTrainImg[i][n]        * 0.299 +  // RED
                              vTrainImg[i][n + 1024] * 0.587 +  // GREEN
                              vTrainImg[i][n + 2048] * 0.114;   // BLUE
      }
   }

   ///////////////////////

   for(uint i = 0; i < C_DATASET_CIFAR_TEST; i++){

      vTestImgLum[i].resize(Sx * Sy);

      for(uint n = 0; n < 1024; n++){

         vTestImgLum[i][n] = vTestImg[i][n]        * 0.299 +
                             vTestImg[i][n + 1024] * 0.587 +
                             vTestImg[i][n + 2048] * 0.114;
      }
   }

   return(C_DATASET_READY);
} */

////////////////////////////////////////////////////////////////////////////////////////////
// [ create_filter ]
////////////////////////////////////////////////////////////////////////////////////////////
/* void C_Cifar::create_filter(uint Sx, uint Sy){


   return;
}
 */
////////////////////////////////////////////////////////////////////////////////////////////
// [ normalize ]
////////////////////////////////////////////////////////////////////////////////////////////
/* int C_Cifar::normalize(){

   double meanR = 0, meanG = 0, meanB = 0; // meanR: 0.4914  meanG: 0.482158  meanB: 0.446531
   double varR = 0, varG = 0, varB = 0;    // varR: 0.0610249  varG: 0.059285  varB: 0.0684282
   double stdR = 0, stdG = 0, stdB = 0;    // stdR: 0.247032  stdG: 0.243485  stdB: 0.261588

   double minR = 1, maxR = 0; 
   double minG = 1, maxG = 0;
   double minB = 1, maxB = 0;
   
   uint sizeChannel = this->Sx * this->Sy;

   for(uint i = 0; i < C_DATASET_CIFAR_TRAIN; i++){
      for(uint n = 0; n < this->Sx * this->Sy; n++){
         meanR += vTrainImg[i][n];
         meanG += vTrainImg[i][n + sizeChannel];
         meanB += vTrainImg[i][n + sizeChannel * 2];

         if(vTrainImg[i][n]                   < minR) minR = vTrainImg[i][n];
         if(vTrainImg[i][n]                   > maxR) maxR = vTrainImg[i][n];

         if(vTrainImg[i][n + sizeChannel]     < minG) minG = vTrainImg[i][n + sizeChannel];
         if(vTrainImg[i][n + sizeChannel]     > maxG) maxG = vTrainImg[i][n + sizeChannel];

         if(vTrainImg[i][n + sizeChannel * 2] < minB) minB = vTrainImg[i][n + sizeChannel * 2];
         if(vTrainImg[i][n + sizeChannel * 2] > maxB) maxB = vTrainImg[i][n + sizeChannel * 2];
      }
   }

   uint divisor = C_DATASET_CIFAR_TRAIN * this->Sx * this->Sy;

   meanR /= divisor;
   meanG /= divisor;
   meanB /= divisor;

   cout << "meanR: " << meanR << "  meanG: " << meanG << "  meanB: " << meanB << endl;

   //////////////////////////////////////////

   for(uint i = 0; i < C_DATASET_CIFAR_TRAIN; i++){
      for(uint n = 0; n < this->Sx * this->Sy; n++){
         varR += (vTrainImg[i][n] - meanR) * (vTrainImg[i][n] - meanR);
         varG += (vTrainImg[i][n + sizeChannel] - meanG) * (vTrainImg[i][n + sizeChannel] - meanG);
         varB += (vTrainImg[i][n + sizeChannel * 2] - meanB) * (vTrainImg[i][n + sizeChannel * 2] - meanB);
      }
   }

   varR /= divisor;
   varG /= divisor;
   varB /= divisor;

   cout << "varR: " << varR << "  varG: " << varG << "  varB: " << varB << endl;

   ///////////////////////////////////////////

   stdR = sqrt(varR);
   stdG = sqrt(varG);
   stdB = sqrt(varB);

   cout << "stdR: " << stdR << "  stdG: " << stdG << "  stdB: " << stdB << endl;

   ///////////////////////////////////////////

   for(uint i = 0; i < C_DATASET_CIFAR_TRAIN; i++){
      for(uint n = 0; n < this->Sx * this->Sy; n++){

         vTrainImg[i][n]                   = (vTrainImg[i][n]                   - meanR) / stdR;
         vTrainImg[i][n + sizeChannel]     = (vTrainImg[i][n + sizeChannel]     - meanG) / stdG;
         vTrainImg[i][n + sizeChannel * 2] = (vTrainImg[i][n + sizeChannel * 2] - meanB) / stdB;
      }
   }

   cout << "minR: " << minR << "  maxR: " << maxR 
      << "  minG: " << minG << "  maxG: " << maxG 
      << "  minB: " << minB << "  maxB: " << maxB << endl;

   double scaleR = maxR - minR - 0.5, scaleG = maxG - minG - 0.5, scaleB = maxB - minB - 0.5;

   for(uint i = 0; i < C_DATASET_CIFAR_TRAIN; i++){
      for(uint n = 0; n < this->Sx * this->Sy; n++){

         vTrainImg[i][n]                   = (vTrainImg[i][n] - minR) / (maxR - minR);
         vTrainImg[i][n + sizeChannel]     = (vTrainImg[i][n + sizeChannel] - minG) / (maxG - minG);
         vTrainImg[i][n + sizeChannel * 2] = (vTrainImg[i][n + sizeChannel * 2] - minB) / (maxB - minB);

         vTrainImg[i][n]                   = (vTrainImg[i][n]                  ) * 1 + 0;
         vTrainImg[i][n + sizeChannel]     = (vTrainImg[i][n + sizeChannel]    ) * 1 + 0;
         vTrainImg[i][n + sizeChannel * 2] = (vTrainImg[i][n + sizeChannel * 2]) * 1 + 0;
      }
   }

   return(C_DATASET_READY);
} */

////////////////////////////////////////////////////////////////////////////////////////////
// [ flip_images ]
////////////////////////////////////////////////////////////////////////////////////////////
int C_Cifar::flip_images(){

   //std::cout << "flip_images" << std::endl;

   try{

      for(uint pic = 0; pic < C_CIFAR_TRAIN; pic++){

         //std::cout << "pic:" << pic << std::endl;

         double* pBufferK = &vTrainImg[pic][0];

         //std::cout << "pic:" << pic << " : " << pBufferK << std::endl;

         for(uint c = 0; c < C_CIFAR_Z; c++){

            for(uint raw = 0; raw < this->Sx; raw++){

               double* pStart = pBufferK;
               double* pEnd   = pBufferK;

               pEnd += this->Sx - 1;

               double temp = 0;

               for(uint m = 0; m < this->Sx / 2; m++){

                  temp    = *pStart;
                  *pStart = *pEnd;
                  *pEnd   = temp;

                  pStart++;
                  pEnd--;
               }

               pBufferK += this->Sx;
            }
         }
      }

   }catch(std::exception& e){

      std::cout << "exception:" << e.what() << std::endl;
 
   }

   //std::cout << "flip_images finish" << std::endl;

   return(C_DATASET_READY);
}
