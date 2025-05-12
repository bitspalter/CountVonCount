// // //////////////////////////////////////////////////////////////////////////////////
// [ Surface_Class_Header ]
//////////////////////////////////////////////////////////////////////////////////
//
// SURFACE
//
// [::Last modi: 12.05.21 L.ey (µ~)::]
//
//
#ifndef _C_SURFACE_H_
#define _C_SURFACE_H_

 #include <iostream>
 using namespace std;

 #include <gtkmm/drawingarea.h>
 #include <glibmm/main.h>
 
 #include <vector>
 #include <exception>
 
//////////////////////////////////////////////////////////////////////////////////
// CONSTANT
//////////////////////////////////////////////////////////////////////////////////
 
 const int C_SURFACE_ERROR = 0x00;
 const int C_SURFACE_READY = 0x01;
 
//////////////////////////////////////////////////////////////////////////////////
// STRUCT
//////////////////////////////////////////////////////////////////////////////////
 
 struct S_Sf_Size {
    unsigned int x, y; 
 };

 struct S_Sf_Buffer {
    unsigned int px, py, bx, by; 
 };
 
  struct S_Sf_Color {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
 };
 
//////////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////////
 
  class C_Surface : public Gtk::DrawingArea {
 
    public:
      
       C_Surface(){};
       virtual ~C_Surface(){};
       
       int init(unsigned int x, unsigned int y, double Sx, double Sy);

       int destroy();
       
       bool on_timeout();

       ////////////////////////////////////
       
       int write2Surface(const double* const pBuffer, S_Sf_Buffer* pSFBuffer);
       
       int writeColor2Surface(S_Sf_Color* pSFColor, S_Sf_Buffer* pSFBuffer);

       int writeLum2Surface(double* pBuffer, S_Sf_Buffer* pSFBuffer);

       int writeCifar2RGBSurface(double* pBuffer, S_Sf_Buffer* pSFBuffer);

       int writeR2Surface(double* pBuffer, S_Sf_Buffer* pSFBuffer);
       int writeG2Surface(double* pBuffer, S_Sf_Buffer* pSFBuffer);
       int writeB2Surface(double* pBuffer, S_Sf_Buffer* pSFBuffer);

       void writeInt(int width, int height, int val);
       void writeDouble(int width, int height, double val);
       void writeString(int width, int height, const char* const val);
       
       ////////////////////////////////////

       unsigned char* getpBuffer(){return(vSurface.data());}; // TEST

       ////////////////////////////////////

       void zoom_in();
       void zoom_out();

    protected:
      
       //Override default signal handler:
       bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;

       /////////////////////////////////////
       
       Cairo::RefPtr<Cairo::ImageSurface> iSurface;

       Cairo::RefPtr<Cairo::Context> icontext;
       
       /////////////////////////////////////
       
       unsigned int PicX = {0}, PicY = {0};
       
       double ScaleX = {1.0}, ScaleY = {1.0};

       bool bReady = {false};
       
       /////////////////////////////////////
       
       vector <unsigned char> vSurface;

    private:

 };
 
#endif // _C_SURFACE_H_
