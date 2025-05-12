//////////////////////////////////////////////////////////////////////////////////
// [ APP_Class_Header ]
//////////////////////////////////////////////////////////////////////////////////
//
// graf zahl ver:0.1
//
// [::Last modi: 25.06.24 L.ey (µ~)::]
//
//
#ifndef _C_APP_H_
 #define _C_APP_H_

 #include <iostream>
 #include <vector>
 #include <string>
 
 using namespace std;

 #include "C_GUI.hpp"
 #include "C_NNetManager.hpp"
 #include "C_NN_Visual.hpp"
 
//////////////////////////////////////////////////////////////////////////////////
// CONSTANT
//////////////////////////////////////////////////////////////////////////////////
 
 const int C_APP_ERROR = 0x00;
 const int C_APP_READY = 0x01;

 const int C_APP_SURFACE_X = 2000;
 const int C_APP_SURFACE_Y = 2000;

 const int C_APP_NETWORK  = 0x00;
 const int C_APP_GRADIENT = 0x01;

//////////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////////

 class C_App : public C_GUI { 
    
    public:
        
       C_App();
       virtual ~C_App(){};

       // Called from the thread in CNETManager
       void log_data();
       
    private:

       int create_surface();
       int create_signal();
       int create_default();

       /////////////////////////////////

       C_NNetManager CNETManager;

       /////////////////////////////////

       C_NN_Visual CNNVisual;

       /////////////////////////////////

       int read_input(C_NNetM_Param* pParam);
       int read_conv_data(S_Conv_Layer* pSConv_Layer, SCLayer* pSCLayer);
       int read_pool_data(S_Pool_Layer* pSPool_Layer, SPLayer* pSPLayer);
       int read_fc_data(S_FC_Layer* pS_FC_Layer, SFLayer* pSFLayer);  

       //////////////////////////////////
       // Signal handler

       void on_button_create();
       void on_button_destroy();

       void on_button_start();
       void on_button_stop();

       void on_button_save_config();
       void on_button_load_config();

       void on_button_save_param();
       void on_button_load_param();
       
       void on_button_zoom_in();
       void on_button_zoom_out();

       void on_button_add_layer();
       void on_button_del_layer();
       void on_button_test();

       void on_button_sleep();

       void on_button_class(Gtk::RadioButton* radio);
       void on_button_dataset(Gtk::RadioButton* radio);

       //////
       bool on_signal_delete(GdkEventAny* event);
       //////
       bool on_configure_changed(GdkEventConfigure* configure_event);
       bool on_window_state_event(GdkEventWindowState* window_state_event);
       //////
       void on_notebook_switch_page(Gtk::Widget* page, guint page_num);
       //////
       bool on_timeout();
       //////

       bool on_scroll(GdkEventScroll* scroll_event);
      
       //////

       bool on_key_press(GdkEventKey* key_event);
       bool on_key_release(GdkEventKey* key_event);

       bool bKeyControl = {false};
       int  nTab = {0};

       //////

       bool on_mouse_click(GdkEventButton* button_event);
       bool on_mouse_release(GdkEventButton* button_event);

       /////////////////////////////////
       // Logging

       void on_logging();
  
       Glib::Dispatcher m_Dispatcher;

       sigc::connection hLogging;

       bool bLogging = {false};

       /////////////////////////////////
       
       sigc::connection hTimer;     
};

#endif // _C_APP_H_
