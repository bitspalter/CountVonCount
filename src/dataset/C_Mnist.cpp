//////////////////////////////////////////////////////////////////////////////////
// [ C_DataSet_Class_Source ]
//////////////////////////////////////////////////////////////////////////////////

#include "C_Mnist.hpp"

////////////////////////////////////////////////////////////////////////////////////////////
// [ read_train_cases ]
////////////////////////////////////////////////////////////////////////////////////////////
int C_Mnist::read_train_cases(uint Sx, uint Sy){

   u_char* train_image  = read_file(C_MNIST_FILEPATH_TRAIN_IMAGE);
   u_char* train_labels = read_file(C_MNIST_FILEPATH_TRAIN_LABEL);

   if(train_image == nullptr || train_labels == nullptr) return(C_DATASET_ERROR);

   uint32_t case_count = __builtin_bswap32(*(uint32_t*)(train_image + 4));

   uint dx = (Sx - C_MNIST_X) / 2;
   uint dy = (Sy - C_MNIST_Y) / 2;
   
   vTrainImg.resize(case_count);
   vTrainLab.resize(case_count);
   
   for(uint32_t i = 0; i < case_count; i++){

      u_char* img   = train_image  + 16 + i * (C_MNIST_X * C_MNIST_Y);
      u_char* label = train_labels +  8 + i;

      vTrainImg[i].resize(Sx * Sy);
      vTrainLab[i].resize(C_MNIST_CLASSES);

      for(uint x = 0; x < C_MNIST_X; x++)
         for(uint y = 0; y < C_MNIST_Y; y++)
            vTrainImg[i][x + dx + (y + dy) * Sx] = img[x + y * C_MNIST_X] / 255.f;

      for(uint b = 0; b < C_MNIST_CLASSES; b++) vTrainLab[i][b] = *label == b ? 1.0f : 0.0f; //1.0f - 2.0f;
   }

   delete [] train_image;
   delete [] train_labels;

   return(C_DATASET_READY);
}
////////////////////////////////////////////////////////////////////////////////////////////
// [ read_test_cases ]
////////////////////////////////////////////////////////////////////////////////////////////
int C_Mnist::read_test_cases(uint Sx, uint Sy){

   u_char* train_image  = read_file(C_MNIST_FILEPATH_TEST_IMAGE);
   u_char* train_labels = read_file(C_MNIST_FILEPATH_TEST_LABEL);

   if(train_image == nullptr || train_labels == nullptr) return(C_DATASET_ERROR);

   uint32_t case_count = __builtin_bswap32(*(uint32_t*)(train_image + 4));

   uint dx = (Sx - C_MNIST_X) / 2;
   uint dy = (Sy - C_MNIST_Y) / 2;
   
   vTestImg.resize(case_count);
   vTestLab.resize(case_count);
   
   for(uint32_t i = 0; i < case_count; i++){

      u_char* img   = train_image  + 16 + i * (C_MNIST_X * C_MNIST_Y);
      u_char* label = train_labels +  8 + i;

      vTestImg[i].resize(Sx * Sy);
      vTestLab[i].resize(C_MNIST_CLASSES);

      for(uint x = 0; x < C_MNIST_X; x++)
         for(uint y = 0; y < C_MNIST_Y; y++)
            vTestImg[i][x + dx + (y + dy) * Sx] = img[x + y * C_MNIST_X] / 255.f;

      for(uint b = 0; b < C_MNIST_CLASSES; b++) vTestLab[i][b] = *label == b ? 1.0f : 0.0f; //1.0f - 2.0f;
   }
   
   delete [] train_image;
   delete [] train_labels;

   return(C_DATASET_READY);
}
////////////////////////////////////////////////////////////////////////////////////////////
// [ create ]
////////////////////////////////////////////////////////////////////////////////////////////
int C_Mnist::create(uint Sx, uint Sy){

   std::cout << "C_Mnist::create\n";

   if(read_train_cases(Sx, Sy) != C_DATASET_READY){
      std::cout << "C_Mnist::error::read_train_cases\n";
      return(C_DATASET_ERROR);
   }

   if(read_test_cases(Sx, Sy) != C_DATASET_READY){
      std::cout << "C_Mnist::error::read_test_cases\n";
      return(C_DATASET_ERROR);
   }

   //std::cout << "C_Mnist::create 2\n";
   
   ///////////////////////////////////////
   
   vTrainNumbers.resize(C_MNIST_CLASSES);
   vTestNumbers.resize(C_MNIST_CLASSES);

   vTrainNumbers[0].resize(5923);
   vTrainNumbers[1].resize(6742);
   vTrainNumbers[2].resize(5958);
   vTrainNumbers[3].resize(6131);
   vTrainNumbers[4].resize(5842);
   vTrainNumbers[5].resize(5421);
   vTrainNumbers[6].resize(5918);
   vTrainNumbers[7].resize(6265);
   vTrainNumbers[8].resize(5851);
   vTrainNumbers[9].resize(5949);

   vTestNumbers[0].resize(980);
   vTestNumbers[1].resize(1135);
   vTestNumbers[2].resize(1032);
   vTestNumbers[3].resize(1010);
   vTestNumbers[4].resize(982);
   vTestNumbers[5].resize(892);
   vTestNumbers[6].resize(958);
   vTestNumbers[7].resize(1028);
   vTestNumbers[8].resize(974);
   vTestNumbers[9].resize(1009);

   ///////////////////////////////////////

   //std::cout << "C_Mnist::create 3\n";

   uint Numbers[10] = {0,0,0,0,0,0,0,0,0,0};
   uint Offset[10]  = {0,0,0,0,0,0,0,0,0,0};

   for(uint pic = 0; pic < 60000; pic++){
      
      double* pBufferK = &vTrainLab[pic][0];
      
      for(uint n = 0; n < 10; n++){
         if(pBufferK[n] == 1){
            Numbers[n]++;
            vTrainNumbers[n][Offset[n]] = pic;
            Offset[n]++;
         }
      }
   }
   
   uint R = 0;
   
   for(uint n = 0; n < 10; n++){
      R += Numbers[n];
   }
   
   ///////////////////////////
   
   for(uint n = 0; n < 10; n++){
      Numbers[n] = 0; 
      Offset[n]  = 0;
   }
   
   ///////////////////////////

   //std::cout << "C_Mnist::create 4\n";
   
   for(uint pic = 0; pic < 10000; pic++){

      double* pBufferK = &vTestLab[pic][0];
      
      for(uint n = 0; n < 10; n++){
         if(pBufferK[n] == 1){
            Numbers[n]++;
            vTestNumbers[n][Offset[n]] = pic;
            Offset[n]++;
         }
      }
   }
   
   R = 0;
   
   for(uint n = 0; n < 10; n++){
      R += Numbers[n];
   }

   return(C_DATASET_READY);
}
////////////////////////////////////////////////////////////////////////////////////////////
// [ destroy ]
////////////////////////////////////////////////////////////////////////////////////////////
int C_Mnist::destroy(){
   
   vTrainImg.clear();
   vTrainLab.clear();
   
   vTestImg.clear();
   vTestLab.clear();
   
   for(int n = 0; n < 10; n++){
      vTrainNumbers[n].clear();
      vTestNumbers[n].clear();
   }
   
   return(C_DATASET_READY);
}
