//////////////////////////////////////////////////////////////////////////////////
// [ C_DataSet_Class_Header ]
//////////////////////////////////////////////////////////////////////////////////
//
// DATASET MNIST
//
// [::Last modi: 07.11.19 L.ey (µ~)::]
//
//
#ifndef _C_MNIST_H_
#define _C_MNIST_H_

 #include "C_Dataset.hpp"

//////////////////////////////////////////////////////////////////////////////////
// CONSTANT
//////////////////////////////////////////////////////////////////////////////////

 #define C_MNIST_FILEPATH_TRAIN_IMAGE "./src/dataset/mnist/train-images.idx3-ubyte"
 #define C_MNIST_FILEPATH_TRAIN_LABEL "./src/dataset/mnist/train-labels.idx1-ubyte"

 #define C_MNIST_FILEPATH_TEST_IMAGE  "./src/dataset/mnist/t10k-images.idx3-ubyte"
 #define C_MNIST_FILEPATH_TEST_LABEL  "./src/dataset/mnist/t10k-labels.idx1-ubyte"

 const uint C_MNIST_X = 28;
 const uint C_MNIST_Y = 28;
 const uint C_MNIST_Z =  1;

 const uint C_MNIST_TRAIN = 60000;
 const uint C_MNIST_TEST  = 10000;

 const uint C_MNIST_CLASSES = 10;

//////////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////////
 
 class C_Mnist : public C_Dataset {
 
    public:
      
       C_Mnist(){};
       virtual ~C_Mnist(){destroy();};

       int create(uint x, uint y);
       int destroy();

       //////////////////////////////

       int flip_images(){return(C_DATASET_READY);}

    private:

       int read_train_cases(uint x, uint y);
       int read_test_cases(uint x, uint y);
 };
 
#endif // _C_MNIST_H_
