//////////////////////////////////////////////////////////////////////////////////
// [ Surface_Class_Source ]
//////////////////////////////////////////////////////////////////////////////////

#include "C_Surface.hpp"


//////////////////////////////////////////////////////////////////////////////////
// [ init ]
//////////////////////////////////////////////////////////////////////////////////
int C_Surface::init(unsigned int x, unsigned int y, double Sx, double Sy){

   vSurface.resize(x * y * 4);
   
   iSurface = 
   Cairo::ImageSurface::create(vSurface.data(), 
                               Cairo::FORMAT_RGB24, 
                               x, y,
                               Cairo::ImageSurface::format_stride_for_width(Cairo::FORMAT_RGB24, x));

   if(!iSurface) return(C_SURFACE_ERROR);

   PicX = x;
   PicY = y;
   
   icontext = Cairo::Context::create(iSurface); // test

   if(!icontext) return(C_SURFACE_ERROR);
   
   ScaleX = Sx;
   ScaleY = Sy;

   set_size_request(PicX * ScaleX - 20, PicY * ScaleY - 20); // TEST

   bReady = true;

   return(C_SURFACE_READY);
}

//////////////////////////////////////////////////////////////////////////////////
// [ destroy ]
//////////////////////////////////////////////////////////////////////////////////
int C_Surface::destroy(){

   //if(iSurface) iSurface->surface_destroy();

   //cairo_surface_destroy(iSurface);

   //Cairo::Surface::cobject::destroy(iSurface);

   //Cairo::Surface::destroy(iSurface);

   //cairo_surface_destroy(iSurface->cobj());

   //Cairo::Surface::cobject::destroy(iSurface);

   return(C_SURFACE_READY);
}

//////////////////////////////////////////////////////////////////////////////////
// [ on_draw ]
//////////////////////////////////////////////////////////////////////////////////
bool C_Surface::on_draw(const Cairo::RefPtr<Cairo::Context>& cr){

   cr->scale(ScaleX, ScaleY);
   
   cr->set_source(iSurface, 0, 0);
   cr->paint();
   
   /////////////////////////////////////////////

   cr->stroke();

   return(true);
}

//////////////////////////////////////////////////////////////////////////////////
// [ on_timeout ]
//////////////////////////////////////////////////////////////////////////////////
bool C_Surface::on_timeout(){

   // force our program to redraw
   auto win = get_window();
   
   if(win){
      Gdk::Rectangle r(0, 0, get_allocation().get_width(), get_allocation().get_height());
      win->invalidate_rect(r, false);
   }
   
   return(true);
}

//////////////////////////////////////////////////////////////////////////////////
// [ zoom_in ]
//////////////////////////////////////////////////////////////////////////////////
void C_Surface::zoom_in(){

   ScaleX++; ScaleY++;

   set_size_request(PicX * ScaleX - 20, PicY * ScaleY - 20);
};

//////////////////////////////////////////////////////////////////////////////////
// [ zoom_out ]
//////////////////////////////////////////////////////////////////////////////////
void C_Surface::zoom_out(){

   if(ScaleX == 1.0) return;
      
   ScaleX--; ScaleY--;

   set_size_request(PicX * ScaleX - 20, PicY * ScaleY - 20);
};

//////////////////////////////////////////////////////////////////////////////////
// [ writeInt ]  
//////////////////////////////////////////////////////////////////////////////////
void C_Surface::writeInt(int width, int height, int val){

   if(!bReady) return;

   Pango::FontDescription font;

   font.set_family("Monospace");
   font.set_weight(Pango::WEIGHT_BOLD);

   std::string spic = std::to_string(val);
  
   auto layout = create_pango_layout(spic);

   layout->set_font_description(font);

   int text_width;
   int text_height;

   layout->get_pixel_size(text_width, text_height);

   icontext->set_source_rgb(0, 1, 0);
  
   icontext->move_to((width - text_width) / 2, (height - text_height) / 2);

   layout->show_in_cairo_context(icontext);
}

//////////////////////////////////////////////////////////////////////////////////
// [ writeDouble ]  
//////////////////////////////////////////////////////////////////////////////////
void C_Surface::writeDouble(int width, int height, double val){

   if(!bReady) return;

   Pango::FontDescription font;

   font.set_family("Monospace");
   font.set_weight(Pango::WEIGHT_BOLD);

   std::string spic = std::to_string(val);
  
   spic.erase(spic.begin() + 5, spic.end());
  
   auto layout = create_pango_layout(spic);

   layout->set_font_description(font);

   int text_width;
   int text_height;

   layout->get_pixel_size(text_width, text_height);

   icontext->set_source_rgb(0, 1, 0);
  
   icontext->move_to((width - text_width) / 2, (height - text_height) / 2);

   layout->show_in_cairo_context(icontext);
}

//////////////////////////////////////////////////////////////////////////////////
// [ writeString ]
//////////////////////////////////////////////////////////////////////////////////
void C_Surface::writeString(int width, int height, const char* const val){

   if(!bReady) return;

   Pango::FontDescription font;

   font.set_family("Monospace");
   font.set_weight(Pango::WEIGHT_BOLD);
   
   std::string spic = val; 
   
   auto layout = create_pango_layout(spic);

   layout->set_font_description(font);

   int text_width;
   int text_height;

   layout->get_pixel_size(text_width, text_height);

   icontext->set_source_rgb(0, 1, 0);
  
   icontext->move_to((width - text_width) / 2, (height - text_height) / 2);

   layout->show_in_cairo_context(icontext);
}

//////////////////////////////////////////////////////////////////////////////////
// [ writeCifar2RGBSurface ]
//////////////////////////////////////////////////////////////////////////////////
int C_Surface::writeCifar2RGBSurface(double* pBuffer, S_Sf_Buffer* pSFBuffer){

   if(!bReady) return(C_SURFACE_ERROR);

   if(pBuffer == nullptr || pSFBuffer == nullptr) return(C_SURFACE_ERROR);

   if(pSFBuffer->px >= PicX || pSFBuffer->py >= PicY) return(C_SURFACE_ERROR);

   if(pSFBuffer->px + pSFBuffer->bx >= PicX) pSFBuffer->bx = PicX - pSFBuffer->px;

   if(pSFBuffer->py + pSFBuffer->by >= PicY) pSFBuffer->by = PicY - pSFBuffer->py;

   ///////////////////////////////////////////////////////

   unsigned char* piBuffer = vSurface.data();

   for(unsigned int y = pSFBuffer->py, n = 0; y < pSFBuffer->py + pSFBuffer->by; y++){

      for(unsigned int x = pSFBuffer->px; x < pSFBuffer->px + pSFBuffer->bx; x++, n++){

         int sx = (y * PicY + x) * 4;

         piBuffer[sx + 0] = pBuffer[n + pSFBuffer->bx * pSFBuffer->by * 2] * 255;
         piBuffer[sx + 1] = pBuffer[n + pSFBuffer->bx * pSFBuffer->by] * 255;
         piBuffer[sx + 2] = pBuffer[n] * 255;
      }
   }

   return(C_SURFACE_READY);
}

//////////////////////////////////////////////////////////////////////////////////
// [ writeR2Surface ]
//////////////////////////////////////////////////////////////////////////////////
int C_Surface::writeR2Surface(double* pBuffer, S_Sf_Buffer* pSFBuffer){

   if(!bReady) return(C_SURFACE_ERROR);

   if(pBuffer == nullptr || pSFBuffer == nullptr) return(C_SURFACE_ERROR);

   if(pSFBuffer->px >= PicX || pSFBuffer->py >= PicY) return(C_SURFACE_ERROR);

   if(pSFBuffer->px + pSFBuffer->bx >= PicX) pSFBuffer->bx = PicX - pSFBuffer->px;
   
   if(pSFBuffer->py + pSFBuffer->by >= PicY) pSFBuffer->by = PicY - pSFBuffer->py;
   
   /////////////////////////////////////////
   
   unsigned char * piBuffer = vSurface.data();

   for(unsigned int y = pSFBuffer->py, n = 0; y < pSFBuffer->py + pSFBuffer->by; y++){
      for(unsigned int x = pSFBuffer->px; x < pSFBuffer->px + pSFBuffer->bx; x++, n++){
   
         int sx = (y * PicY + x) * 4;  
         
         piBuffer[sx + 0] = 0;
         piBuffer[sx + 1] = 0;
         piBuffer[sx + 2] = pBuffer[n] * 255;
      }
   }
   
   return(C_SURFACE_READY);
}

//////////////////////////////////////////////////////////////////////////////////
// [ writeG2Surface ]
//////////////////////////////////////////////////////////////////////////////////
int C_Surface::writeG2Surface(double* pBuffer, S_Sf_Buffer* pSFBuffer){

   if(!bReady) return(C_SURFACE_ERROR);

   if(pBuffer == nullptr || pSFBuffer == nullptr) return(C_SURFACE_ERROR);

   if(pSFBuffer->px >= PicX || pSFBuffer->py >= PicY) return(C_SURFACE_ERROR);

   if(pSFBuffer->px + pSFBuffer->bx >= PicX) pSFBuffer->bx = PicX - pSFBuffer->px;

   if(pSFBuffer->py + pSFBuffer->by >= PicY) pSFBuffer->by = PicY - pSFBuffer->py;

   /////////////////////////////////////////

   unsigned char * piBuffer = vSurface.data();

   for(unsigned int y = pSFBuffer->py, n = 0; y < pSFBuffer->py + pSFBuffer->by; y++){
      for(unsigned int x = pSFBuffer->px; x < pSFBuffer->px + pSFBuffer->bx; x++, n++){

         int sx = (y * PicY + x) * 4;

         piBuffer[sx + 0] = 0;
         piBuffer[sx + 1] = pBuffer[n] * 255;
         piBuffer[sx + 2] = 0;
      }
   }

   return(C_SURFACE_READY);
}

//////////////////////////////////////////////////////////////////////////////////
// [ writeB2Surface ]
//////////////////////////////////////////////////////////////////////////////////
int C_Surface::writeB2Surface(double* pBuffer, S_Sf_Buffer* pSFBuffer){

   if(!bReady) return(C_SURFACE_ERROR);

   if(pBuffer == nullptr || pSFBuffer == nullptr) return(C_SURFACE_ERROR);

   if(pSFBuffer->px >= PicX || pSFBuffer->py >= PicY) return(C_SURFACE_ERROR);

   if(pSFBuffer->px + pSFBuffer->bx >= PicX) pSFBuffer->bx = PicX - pSFBuffer->px;

   if(pSFBuffer->py + pSFBuffer->by >= PicY) pSFBuffer->by = PicY - pSFBuffer->py;

   /////////////////////////////////////////

   unsigned char * piBuffer = vSurface.data();

   for(unsigned int y = pSFBuffer->py, n = 0; y < pSFBuffer->py + pSFBuffer->by; y++){
      for(unsigned int x = pSFBuffer->px; x < pSFBuffer->px + pSFBuffer->bx; x++, n++){

         int sx = (y * PicY + x) * 4;

         piBuffer[sx + 0] = pBuffer[n] * 255;
         piBuffer[sx + 1] = 0;
         piBuffer[sx + 2] = 0;
      }
   }

   return(C_SURFACE_READY);
}

//////////////////////////////////////////////////////////////////////////////////
// [ write2Surface ]
//////////////////////////////////////////////////////////////////////////////////
int C_Surface::write2Surface(const double* const pBuffer, S_Sf_Buffer* pSFBuffer){

   if(!bReady) return(C_SURFACE_ERROR);

   if(pBuffer == nullptr || pSFBuffer == nullptr) return(C_SURFACE_ERROR);

   if(pSFBuffer->px >= PicX || pSFBuffer->py >= PicY) return(C_SURFACE_ERROR);

   if(pSFBuffer->px + pSFBuffer->bx >= PicX) pSFBuffer->bx = PicX - pSFBuffer->px;

   if(pSFBuffer->py + pSFBuffer->by >= PicY) pSFBuffer->by = PicY - pSFBuffer->py;

   /////////////////////////////////////////

   unsigned char * piBuffer = vSurface.data();

   for(unsigned int y = pSFBuffer->py, n = 0; y < pSFBuffer->py + pSFBuffer->by; y++){
      for(unsigned int x = pSFBuffer->px; x < pSFBuffer->px + pSFBuffer->bx; x++, n++){

         int sx = (y * PicY + x) * 4;

         if(pBuffer[n] < 0){
            piBuffer[sx + 0] = 0;
            piBuffer[sx + 1] = abs(pBuffer[n] < -1 ? -1 : pBuffer[n]) * 255;
            piBuffer[sx + 2] = 0;
         }else{
            piBuffer[sx + 0] = 0;
            piBuffer[sx + 1] = 0;
            piBuffer[sx + 2] = pBuffer[n] > 1 ? 1 : pBuffer[n] * 255;
         }
      }
   }

   return(C_SURFACE_READY);
}

//////////////////////////////////////////////////////////////////////////////////
// [ writeLum2Surface ]
//////////////////////////////////////////////////////////////////////////////////
int C_Surface::writeLum2Surface(double* pBuffer, S_Sf_Buffer* pSFBuffer){

   if(!bReady) return(C_SURFACE_ERROR);

   if(pBuffer == nullptr || pSFBuffer == nullptr) return(C_SURFACE_ERROR);

   if(pSFBuffer->px >= PicX || pSFBuffer->py >= PicY) return(C_SURFACE_ERROR);

   if(pSFBuffer->px + pSFBuffer->bx >= PicX) pSFBuffer->bx = PicX - pSFBuffer->px;

   if(pSFBuffer->py + pSFBuffer->by >= PicY) pSFBuffer->by = PicY - pSFBuffer->py;

   /////////////////////////////////////////

   unsigned char * piBuffer = vSurface.data();

   for(unsigned int y = pSFBuffer->py, n = 0; y < pSFBuffer->py + pSFBuffer->by; y++){
      for(unsigned int x = pSFBuffer->px; x < pSFBuffer->px + pSFBuffer->bx; x++, n++){

         int sx = (y * PicY + x) * 4;

         piBuffer[sx + 0] = pBuffer[n] * 255;
         piBuffer[sx + 1] = pBuffer[n] * 255;
         piBuffer[sx + 2] = pBuffer[n] * 255;
      }
   }

   return(C_SURFACE_READY);
}

//////////////////////////////////////////////////////////////////////////////////
// [ writeColor2Surface ]
//////////////////////////////////////////////////////////////////////////////////
int C_Surface::writeColor2Surface(S_Sf_Color* pSFColor, S_Sf_Buffer* pSFBuffer){

   if(!bReady) return(C_SURFACE_ERROR);

   if(pSFColor == nullptr || pSFBuffer == nullptr) return(C_SURFACE_ERROR);

   if(pSFBuffer->px >= PicX || pSFBuffer->py >= PicY) return(C_SURFACE_ERROR);

   if(pSFBuffer->px + pSFBuffer->bx >= PicX) pSFBuffer->bx = PicX - pSFBuffer->px;
   
   if(pSFBuffer->py + pSFBuffer->by >= PicY) pSFBuffer->by = PicY - pSFBuffer->py;
   
   /////////////////////////////////////////
   
   unsigned char * piBuffer = vSurface.data();

   for(unsigned int y = pSFBuffer->py, n = 0; y < pSFBuffer->py + pSFBuffer->by; y++){
      for(unsigned int x = pSFBuffer->px; x < pSFBuffer->px + pSFBuffer->bx; x++, n++){

         int sx = (y * PicY + x) * 4;

         piBuffer[sx + 0] = pSFColor->blue;
         piBuffer[sx + 1] = pSFColor->green;
         piBuffer[sx + 2] = pSFColor->red;
      }
   }
   
   return(C_SURFACE_READY);
}
