//////////////////////////////////////////////////////////////////////////////////
// [ C_GUI_Class_Header ]
//////////////////////////////////////////////////////////////////////////////////
//
//
// [::Last modi: 06.05.20 L.ey (µ~)::]
//
//
#ifndef _C_GUI_H_
 #define _C_GUI_H_

 #include <iostream>
 #include <vector>
 #include <variant>

 using namespace std;

 #include <gtkmm.h>

 const int DX_MAX =  90;
 const int DY_MAX = 205;

 const int DX_NOT_MAX =  93;
 const int DY_NOT_MAX = 205;

//////////////////////////////////////////////////////////////////////////////////
// CONSTANT
//////////////////////////////////////////////////////////////////////////////////
 
 const int C_GUI_ERROR = 0x00;
 const int C_GUI_READY = 0x01;
 
 const int C_GUI_NB_NETWORK  = 0x00;
 const int C_GUI_NB_GRADIENT = 0x01;
 const int C_GUI_NB_FAILED   = 0x02;
 const int C_GUI_NB_LOGGING  = 0x03;
 const int C_GUI_NB_CONFIG   = 0x04;

 //const int C_GUI_LAYER_INPUT = 0x00;
 const int C_GUI_LAYER_CONV  = 0x00;
 const int C_GUI_LAYER_POOL  = 0x01;
 const int C_GUI_LAYER_FULLY = 0x02;
 
//////////////////////////////////////////////////////////////////////////////////
// STRUCT
//////////////////////////////////////////////////////////////////////////////////
 
 struct SButton {
    Gtk::Button open;

    Gtk::Button start;
    Gtk::Button stop;

    Gtk::Button save_config;
    Gtk::Button load_config;

    Gtk::Button save_param;
    Gtk::Button load_param;

    Gtk::Button zoom_in;
    Gtk::Button zoom_out;

    Gtk::ToggleButton sleep;
    
    Gtk::RadioButton cbuttons[11];
    
    Gtk::Button create;
    Gtk::Button destroy;

    Gtk::Button add_layer; // TEST
    Gtk::Button del_layer;
    Gtk::Button test;

    Gtk::RadioButton dsbuttons[2];
 };
 
 //////////////////////////////
 
 struct SLabel {

    Gtk::Label input, input_x, input_y, input_z;
    Gtk::Label dataset, learning, testing, batch;
 };
 
 //////////////////////////////
 
 struct SEntry {
    
    // Head
    Gtk::Entry eSleep;
    Gtk::Entry eFile;
    
    // Config
    Gtk::Entry input_x;
    Gtk::Entry input_y;
    Gtk::Entry input_z;
    
    // Hyper
    Gtk::Entry batch;
    
    // Dataset
    Gtk::Entry learning;
    Gtk::Entry testing;
 };
 
 //////////////////////////////
 
 struct SText {
    Gtk::ScrolledWindow window;
    Gtk::TextView view;   
    Glib::RefPtr<Gtk::TextBuffer> pbuffer; 
 };
 
 //////////////////////////////
 
 struct SWindow {
    Gtk::ScrolledWindow main;
    Gtk::ScrolledWindow network;
    Gtk::ScrolledWindow failed;
    Gtk::ScrolledWindow gradient;
    Gtk::ScrolledWindow config;
 };
 
 //////////////////////////////
 
 struct SLayout {
    Gtk::Layout data;
    Gtk::Layout network;
    Gtk::Layout failed;
    Gtk::Layout gradient;
    Gtk::Layout config;
 };
 
 ///////////////////////////////
 
 struct SBox {
    Gtk::Box input;
    Gtk::Box dataset;
    Gtk::Box cbutton;
    Gtk::Box lbutton;
    Gtk::Box gbutton;
    Gtk::Box hbutton;
 };
 
 ///////////////////////////////

 struct SCombo {

   Gtk::ComboBoxText layer_type;

 };

 ///////////////////////////////

 struct SCLayer {
    Gtk::Box box;
    Gtk::Label name;

    Gtk::Label kernel;
    Gtk::Label k_x;
    Gtk::Entry kernel_x;
    Gtk::Label k_y;
    Gtk::Entry kernel_y;
    Gtk::Label k_z;
    Gtk::Entry kernel_z;

    Gtk::Label slide;
    Gtk::Label s_x;
    Gtk::Entry slide_x;
    Gtk::Label s_y;
    Gtk::Entry slide_y;

    Gtk::ComboBoxText activation;

    Gtk::Label l_alpha;
    Gtk::Entry e_alpha;
 };

 struct SCL_Fill {
   uint kernel_x = {2};
   uint kernel_y = {2};
   uint kernel_z = {5};
   uint Slide_x  = {1};
   uint Slide_y  = {1};
   double alpha  = {0.1};
   uint8_t activation = {0};
 };

 ///////////////////////////////

 struct SPLayer {
    Gtk::Box box;
    Gtk::Label name;

    Gtk::Label pool;
    Gtk::Label po_x;
    Gtk::Entry pool_x;
    Gtk::Label po_y;
    Gtk::Entry pool_y;

    Gtk::Label padd;
    Gtk::Label pa_x;
    Gtk::Entry padd_x;
    Gtk::Label pa_y;
    Gtk::Entry padd_y;

    Gtk::CheckButton warmup;
 };

 struct SPL_Fill {
   uint pool_x = {2};
   uint pool_y = {2};
   uint padd_x = {0};
   uint padd_y = {0};
 };

 ///////////////////////////////

 struct SFLayer {
    Gtk::Box box;
    Gtk::Label name;

    Gtk::Entry neurons;

    Gtk::ComboBoxText activation;

    Gtk::Label l_alpha;
    Gtk::Entry e_alpha;

    Gtk::CheckButton warmup;
 };

 struct SFL_Fill {
   uint    neurons    = {10};
   uint8_t activation = {0};
   double  alpha      = {0.1};
 };

//////////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////////
 
 class C_GUI : public Gtk::Window {
 
    public:
      
       C_GUI();
       virtual ~C_GUI(){};

    private:

       void create_entry();
       void create_label();
       void create_button();

    protected:

       int remove_layer();

       int add_layer(int type); 
       int del_layer(int type);
       
       int del_CLayer();
       int del_PLayer();
       int del_FLayer();

       int add_layer(int type, std::variant<SCL_Fill, SPL_Fill, SFL_Fill>sFill);
       
       int add_CLayer(SCL_Fill* pSCL_Fill);
       int add_PLayer(SPL_Fill* pSPL_Fill);
       int add_FLayer(SFL_Fill* pSFL_Fill);

       ////////////////////////////////////////////

       vector <int> vLayer;
       
       vector<SCLayer*> vClayer;
       vector<SPLayer*> vPlayer;
       vector<SFLayer*> vFlayer;

       uint32_t pos_layer_y = {200};

       uint32_t pos_config_y = {300};

       bool entry_to_int(Gtk::Entry* pEntry, unsigned int* pInt);
       bool entry_to_dbl(Gtk::Entry* pEntry, double* pDouble);

       //////////////////////////////////
       
       Gtk::HeaderBar header;
       Gtk::Box       box;
       Gtk::Notebook  notebook;
       
       //////////////////////////////////

       SLabel   slabel;
       SButton  sbutton;
       SEntry   sentry;

       SWindow  swindow;
       SLayout  slayout;
       
       SBox     sbox;

       SCombo   scombo;
       
       //////////////////////////////////
       
       SText    sLogging;

       ///////////////////////////////////// TEST

       int Delta_X = {DX_NOT_MAX};
       int Delta_Y = {DY_NOT_MAX};
 };
 
#endif // _C_GUI_H_
