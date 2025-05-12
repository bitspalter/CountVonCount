//////////////////////////////////////////////////////////////////////////////////
// [ C_DataSet_Class_Header ]
//////////////////////////////////////////////////////////////////////////////////
//
// DATASET
//
// [::Last modi: 08.10.23 L.ey (µ~)::]
//
//
#ifndef _C_DATASET_CIFAR_H_
#define _C_DATASET_CIFAR_H_

 #include <map>
 #include <algorithm>
 #include <cmath>

 #include <iomanip>
 #include <iostream>
 #include <chrono>
 #include <thread>

 #include <unistd.h> // TEST

 #include "C_Dataset.hpp"

//////////////////////////////////////////////////////////////////////////////////
// CONSTANT
//////////////////////////////////////////////////////////////////////////////////
 
 #define C_CIFAR_FILEPATH_TRAIN "./src/dataset/cifar_10/data_batch.bin" 
 #define C_CIFAR_FILEPATH_TEST  "./src/dataset/cifar_10/test_batch.bin" 

 const uint C_CIFAR_X = 32;
 const uint C_CIFAR_Y = 32;
 const uint C_CIFAR_Z =  3;

 const uint C_CIFAR_TRAIN = 50000;
 const uint C_CIFAR_TEST  = 10000;

 const uint C_CIFAR_CLASSES = 10;

//////////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////////
 
 class C_Cifar : public C_Dataset {
 
    public:
      
       C_Cifar(){}
       virtual ~C_Cifar(){destroy();}

       int create(uint x, uint y);
       int destroy();

       int flip_images();

    private:

       int read_train_cases(uint x, uint y);
       int read_test_cases(uint x, uint y);

       void create_Numbers();

       //int  create_luminanz(uint Sx, uint Sy);
       //void create_filter(uint Sx, uint Sy);
       //int  normalize();
 };
 
#endif // _C_DATASET_CIFAR_H_
