//////////////////////////////////////////////////////////////////////////////////
// [ Dataset_Class_Header ]
//////////////////////////////////////////////////////////////////////////////////
//
// Base Class for all Datasets
//
// [::Last modi: 12.07.24 L.ey (µ~)::]
//
//
#ifndef _C_DATASET_H_
 #define _C_DATASET_H_
 
 #include <iostream>
 #include <vector>
 #include <fstream>

//////////////////////////////////////////////////////////////////////////////////
// CONSTANT
//////////////////////////////////////////////////////////////////////////////////
 
 const int C_DATASET_ERROR = 0x00;
 const int C_DATASET_READY = 0x01;

 const uint C_DATASET_EMPTY = 0xFF;
 const uint C_DATASET_CIFAR = 0x00;
 const uint C_DATASET_MNIST = 0x01;

//////////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////////
 
 class C_Dataset {

    public:

       C_Dataset(){};
       virtual ~C_Dataset(){};

       virtual int create(uint x, uint y) = 0;
       virtual int destroy() = 0;

       //////////////////////////

       auto const get_pTrainImage(){return(&vTrainImg);}
       auto const get_pTrainLabel(){return(&vTrainLab);}

       auto const get_pTestImage(){return(&vTestImg);}
       auto const get_pTestLabel(){return(&vTestLab);}

       auto const get_pTrainNumbers(){return(&vTrainNumbers);}
       auto const get_pTestNumbers(){return(&vTestNumbers);}

       ///////////////////////////

       virtual int flip_images() = 0;

    protected:
       
       u_char* read_file(const char* szFile);

       virtual int read_train_cases(uint x, uint y) = 0;
       virtual int read_test_cases(uint x, uint y) = 0;
       
       //////////////////////////

       std::vector <std::vector <double>> vTrainImg;
       std::vector <std::vector <double>> vTrainLab;
       
       std::vector <std::vector <double>> vTestImg; 
       std::vector <std::vector <double>> vTestLab;

       std::vector <std::vector <uint>> vTrainNumbers;
       std::vector <std::vector <uint>> vTestNumbers;

       //////////////////////////

       uint Sx = {0};
       uint Sy = {0};

    private:

 };

#endif // _C_DATASET_H_



