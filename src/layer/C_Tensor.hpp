//////////////////////////////////////////////////////////////////////////////////
// [ Tensor_Class_Header ]
//////////////////////////////////////////////////////////////////////////////////
//
//
// [::Last modi: 25.06.24 L.ey (µ~)::]
//
//

#ifndef _C_TENSOR_H_
 #define _C_TENSOR_H_

  #include <iostream>
  #include <algorithm>
  #include <cstring>

//////////////////////////////////////////////////////////////////////////////////
// CONSTANT
//////////////////////////////////////////////////////////////////////////////////
 
 const uint C_TENSOR_ERROR = 0x00;
 const uint C_TENSOR_READY = 0x01;

//////////////////////////////////////////////////////////////////////////////////
// STRUCT
//////////////////////////////////////////////////////////////////////////////////

 struct S_Tensor {
    uint32_t x = {0};
    uint32_t y = {0};
    uint32_t z = {0};
 };

//////////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////////
 
 template<typename T> class C_Tensor {

    public:

        C_Tensor(){};

        ////////////////////////////////////////

        C_Tensor(uint x, uint y, uint z){
           S_Tensor STensor;
           STensor.x = x; STensor.y = y; STensor.z = z;
           this->create(&STensor);
        };
        
        /////////////////////////////////////////

       ~C_Tensor(){destroy();};

        ////////////////////////////////////////////////////////////////////////
        // [create]
        ///////////////////////////////////////////////////////////////////////
        uint create(S_Tensor* pSTensor){

            if(pSTensor == nullptr || this->bReady == true)
                return(C_TENSOR_ERROR);

            this->size = pSTensor->x * pSTensor->y * pSTensor->z;

            try {
               this->pData = new T[size]; 
            }catch(int e){
               std::cout << "An exception occurred.\n Exception Nr. " << e << " C_Tensor::create()\n";
               return(C_TENSOR_ERROR);
            }

            this->size *= sizeof(T); 
            
            memset(this->pData, 0, this->size);

            STensor = *pSTensor;

            this->bReady = true;

            return(C_TENSOR_READY);
        }

        ////////////////////////////////////////////////////////////////////////
        // [create]
        ///////////////////////////////////////////////////////////////////////
        uint create(uint x, uint y, uint z){

           S_Tensor STensor;

           STensor.x = x;
           STensor.y = y;
           STensor.z = z;

           return(this->create(&STensor));
        }

        ////////////////////////////////////////////////////////////////////////
        // [destroy]
        ///////////////////////////////////////////////////////////////////////
        uint destroy(){

           if(this->bReady != true)
              return(C_TENSOR_ERROR);

           try {
              delete [] this->pData; 
           }catch(int e){
              std::cout << "An exception occurred. Exception Nr. " << e << '\n';
              return(C_TENSOR_ERROR);
           }

           this->pData  = nullptr;
           this->size   = 0;
           this->bReady = false;

           return(C_TENSOR_READY);
        }

        ////////////////////////////////////////////////////////////////////////
        // [get_size]
        ///////////////////////////////////////////////////////////////////////
        size_t get_size(){ 
           return(this->size); 
        }

        ////////////////////////////////////////////////////////////////////////
        // [get_data]
        ///////////////////////////////////////////////////////////////////////
        T* get_data(){
           return(this->pData);
        }

    private:
        
        T* pData = {nullptr};

        bool bReady = {false};

        S_Tensor STensor; 

        size_t size = {0}; // in bytes
 };

#endif // _C_TENSOR_H_