//////////////////////////////////////////////////////////////////////////////////
// [ APP_Class_Source ]
//////////////////////////////////////////////////////////////////////////////////

#include "C_App.hpp"

//////////////////////////////////////////////////////////////////////////////////
// [ Konstructor ]
//////////////////////////////////////////////////////////////////////////////////
C_App::C_App(){

   create_surface();
   create_signal();
   create_default();

   ////////////////////////
   // logging

   CNETManager.init(this);

   ////////////////////////

   show_all_children();

   ////////////////////////

   C_GUI::notebook.set_current_page(C_GUI_NB_CONFIG);
}

//////////////////////////////////////////////////////////////////////////////////
// [ create_surface ]
//////////////////////////////////////////////////////////////////////////////////
int C_App::create_surface(){

   CNNVisual.init(&CNETManager, C_APP_SURFACE_X, C_APP_SURFACE_Y);

   auto pSurfaces = CNNVisual.get_pSurfaces(); 

   // ??????? ///////
   signal_scroll_event().connect(sigc::mem_fun(*this, &C_App::on_scroll));
   // ??????? ///////

   pSurfaces->network.signal_scroll_event()
                     .connect(sigc::mem_fun(*this, &C_App::on_scroll));

   swindow.network.set_size_request(C_APP_SURFACE_X, C_APP_SURFACE_Y);
   swindow.network.add(pSurfaces->network);
   swindow.network.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

   /////////////////////////////////////////////

   pSurfaces->gradient.signal_scroll_event()
                      .connect(sigc::mem_fun(*this, &C_App::on_scroll));

   swindow.gradient.set_size_request(C_APP_SURFACE_X, C_APP_SURFACE_Y);
   swindow.gradient.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
   swindow.gradient.add(pSurfaces->gradient);

   /////////////////////////////////////////////

   pSurfaces->failed.signal_scroll_event()
                    .connect(sigc::mem_fun(*this, &C_App::on_scroll));

   swindow.failed.set_size_request(C_APP_SURFACE_X, C_APP_SURFACE_Y);
   swindow.failed.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
   swindow.failed.add(pSurfaces->failed);

   return(C_APP_READY);
}

//////////////////////////////////////////////////////////////////////////////////
// [ create_signal ]
//////////////////////////////////////////////////////////////////////////////////
int C_App::create_signal(){

   //////////////////////////////////////////////////////////////
   // Button

   for(int n = 0; n < 2; n++){
      sbutton.dsbuttons[n].signal_toggled().connect(sigc::bind(sigc::mem_fun(*this, &C_App::on_button_dataset), &sbutton.dsbuttons[n]));
   }

   for(int n = 0; n < 11; n++){
      sbutton.cbuttons[n].signal_toggled().connect(sigc::bind(sigc::mem_fun(*this, &C_App::on_button_class), &sbutton.cbuttons[n]));
   }

   sbutton.start.    signal_clicked().connect(sigc::mem_fun(*this, &C_App::on_button_start));
   sbutton.stop.     signal_clicked().connect(sigc::mem_fun(*this, &C_App::on_button_stop));

   sbutton.zoom_in.  signal_clicked().connect(sigc::mem_fun(*this, &C_App::on_button_zoom_in));
   sbutton.zoom_out. signal_clicked().connect(sigc::mem_fun(*this, &C_App::on_button_zoom_out));

   sbutton.create.   signal_clicked().connect(sigc::mem_fun(*this, &C_App::on_button_create));
   sbutton.destroy.  signal_clicked().connect(sigc::mem_fun(*this, &C_App::on_button_destroy));

   // TEST
   sbutton.add_layer.signal_clicked().connect(sigc::mem_fun(*this, &C_App::on_button_add_layer));
   sbutton.del_layer.signal_clicked().connect(sigc::mem_fun(*this, &C_App::on_button_del_layer));
   //sbutton.test.     signal_clicked().connect(sigc::mem_fun(*this, &C_App::on_button_test));

   sbutton.sleep.    signal_toggled().connect(sigc::mem_fun(*this, &C_App::on_button_sleep));

   sbutton.save_config.signal_clicked().connect(sigc::mem_fun(*this, &C_App::on_button_save_config));
   sbutton.load_config.signal_clicked().connect(sigc::mem_fun(*this, &C_App::on_button_load_config));

   sbutton.save_param.signal_clicked().connect(sigc::mem_fun(*this, &C_App::on_button_save_param));
   sbutton.load_param.signal_clicked().connect(sigc::mem_fun(*this, &C_App::on_button_load_param));

   //////////////////////////////////////////////////////////////
   // Config

   signal_configure_event()   .connect(sigc::mem_fun(*this, &C_App::on_configure_changed),  false);
   signal_window_state_event().connect(sigc::mem_fun(*this, &C_App::on_window_state_event), false);

   //////////////////////////////////////////////////////////////
   // Mouse

   signal_button_press_event()  .connect(sigc::mem_fun(*this, &C_App::on_mouse_click),   false);
   signal_button_release_event().connect(sigc::mem_fun(*this, &C_App::on_mouse_release), false);

   //////////////////////////////////////////////////////////////
   // Keyboard

   signal_key_press_event()  .connect(sigc::mem_fun(*this, &C_App::on_key_press),   false);
   signal_key_release_event().connect(sigc::mem_fun(*this, &C_App::on_key_release), false);

   //////////////////////////////////////////////////////////////
   // NoteBook

   notebook.signal_switch_page().connect(sigc::mem_fun(*this, &C_App::on_notebook_switch_page));

   //////////////////////////////////////////////////////////////

   signal_delete_event().connect(sigc::mem_fun(*this, &C_App::on_signal_delete));

   //////////////////////////////////////////////////////////////
   // Logging

   // Connect the handler to the dispatcher.
   hLogging = m_Dispatcher.connect(sigc::mem_fun(*this, &C_App::on_logging));
   bLogging = true;

   return(C_APP_READY);
}

//////////////////////////////////////////////////////////////////////////////////
// [ create_default ] Fill Config with default values
//////////////////////////////////////////////////////////////////////////////////
int C_App::create_default(){

   ostringstream Str_1;

   Str_1 << (int) NET_IN_X;
   sentry.input_x.set_text(Str_1.str());
   Str_1.str("");

   Str_1 << (int) NET_IN_Y;
   sentry.input_y.set_text(Str_1.str());
   Str_1.str("");

   Str_1 << (int) NET_IN_Z;
   sentry.input_z.set_text(Str_1.str());
   Str_1.str("");

   //////////////////////////////////////////////
   // Kernel 1

   // Dataset
   Str_1 << (int) CCTRAIN;
   sentry.learning.set_text(Str_1.str());
   Str_1.str("");

   Str_1 << (int) CCTEST;
   sentry.testing.set_text(Str_1.str());
   Str_1.str("");

   Str_1 << (int) CBATCH;
   sentry.batch.set_text(Str_1.str());
   Str_1.str("");

   return(C_APP_READY);
}

//////////////////////////////////////////////////////////////////////////////////
// [ on_timeout ]
//////////////////////////////////////////////////////////////////////////////////
bool C_App::on_timeout(){

   switch(notebook.get_current_page()){
      case C_GUI_NB_NETWORK:  CNNVisual.show_network();  break;
      case C_GUI_NB_GRADIENT: CNNVisual.show_gradient(); break;  
      case C_GUI_NB_FAILED:   CNNVisual.show_failed();   break;
   }

   return(true);
}

//////////////////////////////////////////////////////////////////////////////////
// [ on_notebook_switch_page ]
//////////////////////////////////////////////////////////////////////////////////
void C_App::on_notebook_switch_page(Gtk::Widget* /* page */, guint page_num){
   
  //std::cout << "Switched to tab with index " << page_num << std::endl;
  //You can also use m_Notebook.get_current_page() to get this index.

  nTab = page_num;
   
   if(page_num == C_GUI_NB_LOGGING){

      if(!bLogging) return;

      sLogging.pbuffer->set_text(CNETManager.get_sLog()->c_str());
      
      // Wait for Gui to Redraw with added line to buffer
      while(gtk_events_pending())
         gtk_main_iteration_do(false);
   
      // Scoll to end of Buffer
      auto it = sLogging.pbuffer->get_iter_at_line(sLogging.pbuffer->get_line_count());
      sLogging.view.scroll_to(it);
   }
}

///////////////////////////////////////////////////////////////////////////
// lod_data() is called from C_NNetManager. It is executed in the worker
// thread. It triggers a call to on_logging(), which is
// executed in the GUI thread.
//////////////////////////////////////////////////////////////////////////////////
// [ log_data ]
//////////////////////////////////////////////////////////////////////////////////
void C_App::log_data(){

   if(bLogging) m_Dispatcher.emit();
}

//////////////////////////////////////////////////////////////////////////////////
// [ on_logging ] // called by a signal from m_Dispatcher.emit();
//////////////////////////////////////////////////////////////////////////////////
void C_App::on_logging(){

   if(!bLogging) return;

   sLogging.pbuffer->insert(sLogging.pbuffer->end(), CNETManager.get_sLogLast()->c_str());

   // Wait for Gui to Redraw with added line to buffer
   while(gtk_events_pending())
      gtk_main_iteration_do(false);

   // Scoll to end of Buffer
   auto it = sLogging.pbuffer->get_iter_at_line(sLogging.pbuffer->get_line_count());
   sLogging.view.scroll_to(it);
}

//////////////////////////////////////////////////////////////////////////////////
// [ on_configure_changed ]
//////////////////////////////////////////////////////////////////////////////////
bool C_App::on_configure_changed(GdkEventConfigure* configure_event){

   swindow.network. set_size_request(configure_event->width - Delta_X, configure_event->height - Delta_Y);
   swindow.failed.  set_size_request(configure_event->width - Delta_X, configure_event->height - Delta_Y);
   swindow.gradient.set_size_request(configure_event->width - Delta_X, configure_event->height - Delta_Y);

   return(false);
}

//////////////////////////////////////////////////////////////////////////////////
// [ on_signal_delete ]
//////////////////////////////////////////////////////////////////////////////////
bool C_App::on_signal_delete(GdkEventAny* event){

   cout << "on_signal_delete::event: " << event->type << endl;

   if(!CNETManager.get_bReady()) return(false);

   Gtk::MessageDialog dialog(
        "Would you like to close this window now?",
        false,
        Gtk::MESSAGE_QUESTION,
        Gtk::BUTTONS_YES_NO);

   dialog.set_title("CVC");
        
   int result = dialog.run();

   bool abortClose;
    
   switch(result){
      case Gtk::RESPONSE_YES: abortClose = false; break;
      case Gtk::RESPONSE_NO:  abortClose = true;  break;
      default:                abortClose = true;  break;
   }

   if(!abortClose){

      bLogging = false;
      hLogging.disconnect();

      if(CNETManager.get_bRun()){
         on_button_stop();
         sleep(2);
      }

      CNETManager.destroy();
   }

   return(abortClose);
}

//////////////////////////////////////////////////////////////////////////////////
// [ on_window_state_event ]
//////////////////////////////////////////////////////////////////////////////////
bool C_App::on_window_state_event(GdkEventWindowState* window_state_event){

   if((window_state_event->new_window_state & 4) == 4){

      // cout << "MAXIMIZED" << endl;

      Delta_X = DX_MAX;
      Delta_Y = DY_MAX;

   }else{

      Delta_X = DX_NOT_MAX;
      Delta_Y = DY_NOT_MAX;

      // cout << "NOT MAXIMIZED" << endl;
   }

   return(false);
}

//////////////////////////////////////////////////////////////////////////////////
// [ on_scroll ]
//////////////////////////////////////////////////////////////////////////////////
bool C_App::on_scroll(GdkEventScroll* scroll_event){

   // cout << scroll_event->window << endl;

   auto pSurfaces = CNNVisual.get_pSurfaces();

   if(scroll_event->direction == GDK_SCROLL_UP){

      // cout << "GDK_SCROLL_UP" << endl;

      if(bKeyControl == true){

         switch(nTab){
            case 0: pSurfaces->network.zoom_in();  break;
            case 1: pSurfaces->gradient.zoom_in(); break;
            case 2: pSurfaces->failed.zoom_in();   break;
         }
         // sSurface.network.zoom_in();
         return(true);
      }
   }

   if(scroll_event->direction == GDK_SCROLL_DOWN){

      // cout << "GDK_SCROLL_DOWN" << endl;

      if(bKeyControl == true){
         switch(nTab){
            case 0: pSurfaces->network.zoom_out();  break;
            case 1: pSurfaces->gradient.zoom_out(); break;
            case 2: pSurfaces->failed.zoom_out();   break;
         }
         // sSurface.network.zoom_out();
         return(true);
      }
   }

   return(false);
}

//////////////////////////////////////////////////////////////////////////////////
// [ on_key_press ]
//////////////////////////////////////////////////////////////////////////////////
bool C_App::on_key_press(GdkEventKey* key_event){

   // cout << "on_key_press: " << key_event->keyval << endl;

   if(key_event->keyval == 65507){ // CONTROL

      // cout << "CONTROL_PRESS" << endl;

      bKeyControl = true;
   }

   return(false);
}

//////////////////////////////////////////////////////////////////////////////////
// [ on_key_release ]
//////////////////////////////////////////////////////////////////////////////////
bool C_App::on_key_release(GdkEventKey* key_event){

   // cout << "on_key_release" << key_event->keyval << endl;

   if((key_event->state & 4) == 4){ // CONTROL

      // cout << "CONTROL_RELEASE" << endl;

      bKeyControl = false;
   }


   return(false);
}

//////////////////////////////////////////////////////////////////////////////////
// [ on_key_release ]
//////////////////////////////////////////////////////////////////////////////////
bool C_App::on_mouse_click(GdkEventButton* button_event){

   // cout << "on_mouse_click" << endl;

   if(button_event == nullptr) return(false);

   return(false);
}

//////////////////////////////////////////////////////////////////////////////////
// [ on_key_release ]
//////////////////////////////////////////////////////////////////////////////////
bool C_App::on_mouse_release(GdkEventButton* button_event){

   // cout << "on_mouse_release" << endl;
   if(button_event == nullptr) return(false);
   
   return(false);
}

//////////////////////////////////////////////////////////////////////////////////
// [ on_button_create ]
//////////////////////////////////////////////////////////////////////////////////
void C_App::on_button_create(){

   if(!C_GUI::vLayer.size()){
      Gtk::MessageDialog dialog(*this, "add some layer", false, Gtk::MESSAGE_ERROR);
      dialog.run();
      return;
   }

   C_NNetM_Param snparam;

   S_Conv_Layer SConv_Layer;
   S_Pool_Layer SPool_Layer;
   S_FC_Layer   SFC_Layer;

   int nclayer = 0;
   int nplayer = 0;
   int nflayer = 0;

   SCLayer** pclayer = vClayer.data();
   SPLayer** pplayer = vPlayer.data();
   SFLayer** pflayer = vFlayer.data();

   /////////////////////////////////////////////////////

   S_NN_Tupel3D DimInput;

   if(read_input(&snparam) != C_APP_READY){
      Gtk::MessageDialog dialog(*this, "please specify all parameters", false, Gtk::MESSAGE_ERROR);
      dialog.run();
      return;
   }

   //////////////////////////////////////////////////////////////////////////////

   vector <S_NN_Layer> VSLayer;

   S_NN_Layer SLayer;

   int* pGLayer = C_GUI::vLayer.data();

   for(size_t n = 0; n < C_GUI::vLayer.size(); n++){

      switch(pGLayer[n]){

         case C_GUI_LAYER_CONV: {

            SCLayer* pTemp = pclayer[nclayer];
            
            if(read_conv_data(&SConv_Layer, pTemp) != C_APP_READY){
               Gtk::MessageDialog dialog(*this, "please specify all conv parameters", false, Gtk::MESSAGE_ERROR);
               dialog.run();
               return;
            }

            SLayer.type  = C_GUI_LAYER_CONV;
            SLayer.layer = SConv_Layer;

            VSLayer.push_back(SLayer);

            nclayer++;

            break;
         }
         case C_GUI_LAYER_POOL: {

            SPLayer* pTemp = pplayer[nplayer];

            if(read_pool_data(&SPool_Layer, pTemp) != C_APP_READY){
               Gtk::MessageDialog dialog(*this, "please specify all pool parameters", false, Gtk::MESSAGE_ERROR);
               dialog.run();
               return;
            }

            SLayer.type  = C_GUI_LAYER_POOL;
            SLayer.layer = SPool_Layer;
            
            // TEST
            SLayer.bWarmup = pTemp->warmup.get_active();

            VSLayer.push_back(SLayer);

            nplayer++;

            break;
         }
         case C_GUI_LAYER_FULLY: {

            SFLayer* pTemp = pflayer[nflayer];

            if(read_fc_data(&SFC_Layer, pTemp) != C_APP_READY){
               Gtk::MessageDialog dialog(*this, "please specify all fc parameters", false, Gtk::MESSAGE_ERROR);
               dialog.run();
               return;
            }

            SLayer.type  = C_GUI_LAYER_FULLY;
            SLayer.layer = SFC_Layer;

            // TEST
            SLayer.bWarmup = pTemp->warmup.get_active();

            VSLayer.push_back(SLayer);

            nflayer++;

            break;
         }
      }
   }

   /////////////////////////////////////////////////////

   if(CNETManager.create(&VSLayer, &snparam) != C_NNETMANAGER_READY){
      Gtk::MessageDialog dialog(*this, 
                                "Error: CNETManager.create", 
                                 false, Gtk::MESSAGE_ERROR);
      dialog.run();
      return;
   }

   /////////////////////////////////////////////////////

   sbutton.start.set_sensitive(true);
   sbutton.destroy.set_sensitive(true);

   sbutton.add_layer.set_sensitive(false);
   sbutton.del_layer.set_sensitive(false);

   sbutton.create.set_sensitive(false);
   sbutton.test.set_sensitive(false);
}

//////////////////////////////////////////////////////////////////////////////////
// [ on_button_destroy ]
//////////////////////////////////////////////////////////////////////////////////
void C_App::on_button_destroy(){
   
   CNETManager.destroy();

   sbutton.start.set_sensitive(false);
   sbutton.destroy.set_sensitive(false);

   sbutton.add_layer.set_sensitive(true);
   sbutton.del_layer.set_sensitive(true);

   sbutton.create.set_sensitive(true);

   sbutton.test.set_sensitive(true);
}

//////////////////////////////////////////////////////////////////////////////////
// [ on_button_zoom_in ]
//////////////////////////////////////////////////////////////////////////////////
void C_App::on_button_zoom_in(){

   auto pSurfaces = CNNVisual.get_pSurfaces();

   switch(notebook.get_current_page()){
      case C_GUI_NB_NETWORK:  pSurfaces->network. zoom_in(); break;
      case C_GUI_NB_GRADIENT: pSurfaces->gradient.zoom_in(); break;
      case C_GUI_NB_FAILED:   pSurfaces->failed.  zoom_in(); break;
   }
}

//////////////////////////////////////////////////////////////////////////////////
// [ on_button_zoom_out ]
//////////////////////////////////////////////////////////////////////////////////
void C_App::on_button_zoom_out(){

   auto pSurfaces = CNNVisual.get_pSurfaces();

   switch(notebook.get_current_page()){
      case C_GUI_NB_NETWORK:  pSurfaces->network. zoom_out(); break;
      case C_GUI_NB_GRADIENT: pSurfaces->gradient.zoom_out(); break;
      case C_GUI_NB_FAILED:   pSurfaces->failed.  zoom_out(); break;
   }
}

//////////////////////////////////////////////////////////////////////////////////
// [ on_button_start ]
//////////////////////////////////////////////////////////////////////////////////
void C_App::on_button_start(){

   Glib::ustring SEdit;
    
   SEdit = sentry.eSleep.get_text();

   hTimer = Glib::signal_timeout().connect(sigc::mem_fun(this, &C_App::on_timeout), atoi(SEdit.c_str()));
   
   if(CNETManager.get_bRun() != true) CNETManager.start();
   
   sbutton.stop.set_sensitive(true);
   sbutton.start.set_sensitive(false);

   sbutton.destroy.set_sensitive(false);
}

//////////////////////////////////////////////////////////////////////////////////
// [ on_button_stop ]
//////////////////////////////////////////////////////////////////////////////////
void C_App::on_button_stop(){

   hTimer.disconnect();
    
   CNETManager.stop();
   
   sbutton.stop.set_sensitive(false);
   sbutton.start.set_sensitive(true);

   sbutton.destroy.set_sensitive(true);
}

//////////////////////////////////////////////////////////////////////////////////
// [ on_button_save_config ]
//////////////////////////////////////////////////////////////////////////////////
void C_App::on_button_save_config(){

   if(!CNETManager.get_bReady()) return;

   Gtk::FileChooserDialog dialog("Please choose a file", Gtk::FILE_CHOOSER_ACTION_SAVE);
   dialog.set_transient_for(*this);

   dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
   dialog.add_button("_Open",   Gtk::RESPONSE_OK);

   int result = dialog.run();

   switch(result){
      case(Gtk::RESPONSE_OK): {
         
         string sFile = dialog.get_filename();

         CNETManager.CNeuralNet.saveConfig2File(sFile);

         string slog = "\nSave Config: ";

         slog.append(sFile);
         slog.append("\n");

         //CNETManager.writeLogEx("Save Config:");
         CNETManager.writeLogEx(slog.c_str());

         break;
      }
      case(Gtk::RESPONSE_CANCEL): break;
      default: break;
   }
}

//////////////////////////////////////////////////////////////////////////////////
// [ on_button_load_config ]
//////////////////////////////////////////////////////////////////////////////////
void C_App::on_button_load_config(){
    
   vector <S_NN_Layer> VSLayer;

   /////////////////////////////////////////////////

   Gtk::FileChooserDialog dialog("Please choose a file", Gtk::FILE_CHOOSER_ACTION_OPEN);
   dialog.set_transient_for(*this);

   dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
   dialog.add_button("_Open",   Gtk::RESPONSE_OK);

   int result = dialog.run();

   switch(result){
      case(Gtk::RESPONSE_OK):{

         string sFile = dialog.get_filename();

         cout << "opem File:" << dialog.get_filename() << endl;

         if(CNETManager.CNeuralNet.readFile2Config(sFile, &VSLayer) !=C_NEURALNET_READY){
            // MsgBox !!!
            cout << "Error: readFile2Net" << endl;
            return;
         }

         string slog = "\nLoad Config: ";

         slog.append(sFile);
         slog.append("\n");

         //CNETManager.writeLogEx("Load Config:");
         CNETManager.writeLogEx(slog.c_str());
            
         break;
      }
      case(Gtk::RESPONSE_CANCEL): return; break;
      default:                    return; break;
   }

   /////////////////////////////////////////////////

   std::variant<SCL_Fill, SPL_Fill, SFL_Fill> sFill;

   SCL_Fill scl_fill;
   SPL_Fill spl_fill;
   SFL_Fill sfl_fill;

   for(S_NN_Layer slayer: VSLayer){

      switch(slayer.type){

         case C_GUI_LAYER_CONV:  

            if(holds_alternative<S_Conv_Layer>(slayer.layer)){ 
               scl_fill.kernel_x   = get<S_Conv_Layer>(slayer.layer).Kernel.x;
               scl_fill.kernel_y   = get<S_Conv_Layer>(slayer.layer).Kernel.y;
               scl_fill.kernel_z   = get<S_Conv_Layer>(slayer.layer).Kernel.z;
               scl_fill.Slide_x    = get<S_Conv_Layer>(slayer.layer).Slide.x;
               scl_fill.Slide_y    = get<S_Conv_Layer>(slayer.layer).Slide.y;
               scl_fill.alpha      = get<S_Conv_Layer>(slayer.layer).Alpha;
               scl_fill.activation = get<S_Conv_Layer>(slayer.layer).activation;

               sFill = scl_fill;
               C_GUI::add_layer(slayer.type, sFill);
            }
         
            break;

         case C_GUI_LAYER_POOL:

            if(holds_alternative<S_Pool_Layer>(slayer.layer)){ 
               spl_fill.pool_x = get<S_Pool_Layer>(slayer.layer).Kernel.x;
               spl_fill.pool_y = get<S_Pool_Layer>(slayer.layer).Kernel.y;

               spl_fill.padd_x = get<S_Pool_Layer>(slayer.layer).Pad.x;
               spl_fill.padd_y = get<S_Pool_Layer>(slayer.layer).Pad.y;

               sFill = spl_fill;
               C_GUI::add_layer(slayer.type, sFill);
            }
         
            break;

         case C_GUI_LAYER_FULLY: 

            if(holds_alternative<S_FC_Layer>(slayer.layer)){

               sfl_fill.neurons    = get<S_FC_Layer>(slayer.layer).cOutput;
               sfl_fill.alpha      = get<S_FC_Layer>(slayer.layer).Alpha;
               sfl_fill.activation = get<S_FC_Layer>(slayer.layer).activation;

               sFill = sfl_fill;
               C_GUI::add_layer(slayer.type, sFill);
            }

            break;
      }
   }
}

//////////////////////////////////////////////////////////////////////////////////
// [ on_button_save_param ]
//////////////////////////////////////////////////////////////////////////////////
void C_App::on_button_save_param(){

   if(!CNETManager.get_bReady()) return;

   Gtk::FileChooserDialog dialog("Please choose a file", Gtk::FILE_CHOOSER_ACTION_SAVE);
   dialog.set_transient_for(*this);

   dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
   dialog.add_button("_Open",   Gtk::RESPONSE_OK);

   int result = dialog.run();

   switch(result){
      case(Gtk::RESPONSE_OK): {
         
         string sFile = dialog.get_filename();

         CNETManager.CNeuralNet.saveParam2File(sFile);

         string slog = "\nSave Param: ";

         slog.append(sFile);
         slog.append("\n");

         //CNETManager.writeLogEx("Save Param:");
         CNETManager.writeLogEx(slog.c_str());

         break;
      }
      case(Gtk::RESPONSE_CANCEL): break;
      default: break;
   }
}

//////////////////////////////////////////////////////////////////////////////////
// [ on_button_load_param ]
//////////////////////////////////////////////////////////////////////////////////
void C_App::on_button_load_param(){

   if(!CNETManager.get_bReady()) return;

   Gtk::FileChooserDialog dialog("Please choose a file", Gtk::FILE_CHOOSER_ACTION_OPEN);
   dialog.set_transient_for(*this);

   dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
   dialog.add_button("_Open",   Gtk::RESPONSE_OK);

   int result = dialog.run();

   switch(result){
      case(Gtk::RESPONSE_OK):{

         string sFile = dialog.get_filename();

         cout << "opem File:" << dialog.get_filename() << endl;

         if(CNETManager.CNeuralNet.readFile2Param(sFile) !=C_NEURALNET_READY){
            // MsgBox !!!
            cout << "Error: readFile2Net" << endl;
            return;
         }

         string slog = "\nLoad Param: ";

         slog.append(sFile);
         slog.append("\n");

         CNETManager.writeLogEx(slog.c_str());
         //CNETManager.writeLogEx(sFile.c_str());
            
         break;
      }
      case(Gtk::RESPONSE_CANCEL): return; break;
      default:                    return; break;
   }
}

//////////////////////////////////////////////////////////////////////////////////
// [ on_button_sleep ]
//////////////////////////////////////////////////////////////////////////////////
void C_App::on_button_sleep(){
   CNETManager.get_bSleep() == true ? 
   CNETManager.set_bSleep(false) : 
   CNETManager.set_bSleep(true);
}

//////////////////////////////////////////////////////////////////////////////////
// [ on_button_add_layer ]
//////////////////////////////////////////////////////////////////////////////////
void C_App::on_button_add_layer(){

   int type = C_GUI::scombo.layer_type.get_active_row_number();

   C_GUI::add_layer(type);
}

//////////////////////////////////////////////////////////////////////////////////
// [ on_button_del_layer ]
//////////////////////////////////////////////////////////////////////////////////
void C_App::on_button_del_layer(){

   if(C_GUI::vLayer.size())
      C_GUI::del_layer(C_GUI::vLayer.back());
}

//////////////////////////////////////////////////////////////////////////////////
// [ on_button_test ]
//////////////////////////////////////////////////////////////////////////////////
void C_App::on_button_test(){

   //cout << "test" << endl;

 /*   if(!C_GUI::vLayer.size()){
      Gtk::MessageDialog dialog(*this, "add some layer", false, Gtk::MESSAGE_ERROR);
      dialog.run();
      return;
   }

   C_NNetM_Param snparam;

   S_Conv_Layer SConv_Layer;
   S_Pool_Layer SPool_Layer;
   S_FC_Layer   SFC_Layer;

   int nclayer = 0;
   int nplayer = 0;
   int nflayer = 0;

   SCLayer** pclayer = vClayer.data();
   SPLayer** pplayer = vPlayer.data();
   SFLayer** pflayer = vFlayer.data();

   /////////////////////////////////////////////////////

   S_NN_Tupel DimInput;

   if(read_input(&snparam) != C_APP_READY){
      Gtk::MessageDialog dialog(*this, "please specify all parameters", false, Gtk::MESSAGE_ERROR);
      dialog.run();
      return;
   }

   //////////////////////////////////////////////////////////////////////////////

   vector <S_NN_Layer> VSLayer;

   S_NN_Layer SLayer;

   int* pGLayer = C_GUI::vLayer.data();

   cout << "C_GUI::vLayer.size():" << C_GUI::vLayer.size() << endl;

   for(size_t n = 0; n < C_GUI::vLayer.size(); n++){

      cout << "pGLayer[n]:" << pGLayer[n] << endl;

      switch(pGLayer[n]){

         case C_GUI_LAYER_CONV: {

            cout << "C_GUI_LAYER_CONV" << endl;

            SCLayer* pTemp = pclayer[nclayer];
            
            if(read_conv_data(&SConv_Layer, pTemp) != C_APP_READY){
               Gtk::MessageDialog dialog(*this, "please specify all conv parameters", false, Gtk::MESSAGE_ERROR);
               dialog.run();
               return;
            }

            SLayer.type = C_GUI_LAYER_CONV;
            SLayer.layer.CLayer = SConv_Layer;

            VSLayer.push_back(SLayer);

            nclayer++;

            break;
         }
         case C_GUI_LAYER_POOL: {

            cout << "C_GUI_LAYER_POOL" << endl;

            SPLayer* pTemp = pplayer[nplayer];

            if(read_pool_data(&SPool_Layer, pTemp) != C_APP_READY){
               Gtk::MessageDialog dialog(*this, "please specify all pool parameters", false, Gtk::MESSAGE_ERROR);
               dialog.run();
               return;
            }

            SLayer.type = C_GUI_LAYER_POOL;
            SLayer.layer.PLayer = SPool_Layer;

            VSLayer.push_back(SLayer);

            nplayer++;

            break;
         }
         case C_GUI_LAYER_FULLY: {

            cout << "C_GUI_LAYER_FULLY" << endl;

            SFLayer* pTemp = pflayer[nflayer];

            if(read_fc_data(&SFC_Layer, pTemp) != C_APP_READY){
               Gtk::MessageDialog dialog(*this, "please specify all fc parameters", false, Gtk::MESSAGE_ERROR);
               dialog.run();
               return;
            }

            SLayer.type = C_GUI_LAYER_FULLY;
            SLayer.layer.FLayer = SFC_Layer;

            VSLayer.push_back(SLayer);

            nflayer++;

            break;
         }
      }
   }

   /////////////////////////////////////////////////////

   CNETManager.create(&VSLayer, &snparam);

   /////////////////////////////////////////////////////

   sbutton.start.set_sensitive(true);
   
   sbutton.destroy.set_sensitive(true);
   sbutton.test.set_sensitive(false); */
}

//////////////////////////////////////////////////////////////////////////////////
// [ read_input ]
//////////////////////////////////////////////////////////////////////////////////
int C_App::read_input(C_NNetM_Param* pParam){

   if(pParam == nullptr) return(C_APP_ERROR);

   if(!C_GUI::entry_to_int(&sentry.input_x, &pParam->ILayer.x)) return(C_APP_ERROR);
   if(!C_GUI::entry_to_int(&sentry.input_y, &pParam->ILayer.y)) return(C_APP_ERROR);
   if(!C_GUI::entry_to_int(&sentry.input_z, &pParam->ILayer.z)) return(C_APP_ERROR);

   if(!C_GUI::entry_to_int(&sentry.learning, &pParam->ctrain)) return(C_APP_ERROR);
   if(!C_GUI::entry_to_int(&sentry.testing,  &pParam->ctest))  return(C_APP_ERROR);
   if(!C_GUI::entry_to_int(&sentry.batch,    &pParam->batch))  return(C_APP_ERROR);

   // TEST
   if(sbutton.dsbuttons[C_DATASET_CIFAR].get_active()){
      pParam->dataset = C_DATASET_CIFAR;
   }else{
      pParam->dataset = C_DATASET_MNIST;
   }

   return(C_APP_READY);
}

//////////////////////////////////////////////////////////////////////////////////
// [ read_fc_data ]
//////////////////////////////////////////////////////////////////////////////////
int C_App::read_fc_data(S_FC_Layer* pS_FC_Layer, 
                        SFLayer*    pSFLayer){

   if(pS_FC_Layer == nullptr ||
      pSFLayer    == nullptr) return(C_APP_ERROR);

   if(!C_GUI::entry_to_int(&pSFLayer->neurons, &pS_FC_Layer->cOutput)) return(C_APP_ERROR);
   if(!C_GUI::entry_to_dbl(&pSFLayer->e_alpha, &pS_FC_Layer->Alpha))   return(C_APP_ERROR);

   pS_FC_Layer->activation = pSFLayer->activation.get_active_row_number();

   pS_FC_Layer->Beta    = 0.9;
   pS_FC_Layer->Bias    = -0.1 / pS_FC_Layer->cOutput;
   pS_FC_Layer->Divisor = pS_FC_Layer->cOutput; //pS_FC_Layer->cInput / 2;

   return(C_APP_READY);
}

//////////////////////////////////////////////////////////////////////////////////
// [ read_pool_data ]
//////////////////////////////////////////////////////////////////////////////////
int C_App::read_pool_data(S_Pool_Layer* pSPool_Layer, 
                          SPLayer*      pSPLayer){

   if(pSPool_Layer == nullptr ||
      pSPLayer     == nullptr) return(C_APP_ERROR);

   if(!C_GUI::entry_to_int(&pSPLayer->pool_x, &pSPool_Layer->Kernel.x)) return(C_APP_ERROR);
   if(!C_GUI::entry_to_int(&pSPLayer->pool_y, &pSPool_Layer->Kernel.y)) return(C_APP_ERROR);

   if(!C_GUI::entry_to_int(&pSPLayer->padd_x, &pSPool_Layer->Pad.x))    return(C_APP_ERROR);
   if(!C_GUI::entry_to_int(&pSPLayer->padd_y, &pSPool_Layer->Pad.y))    return(C_APP_ERROR);

   pSPool_Layer->Slide.x = pSPool_Layer->Kernel.x; // ???
   pSPool_Layer->Slide.y = pSPool_Layer->Kernel.y; // ???

   return(C_APP_READY);
}

//////////////////////////////////////////////////////////////////////////////////
// [ read_conv_data ]
//////////////////////////////////////////////////////////////////////////////////
int C_App::read_conv_data(S_Conv_Layer* pSConv_Layer, 
                          SCLayer*      pSCLayer){

   if(pSConv_Layer == nullptr ||
      pSCLayer     == nullptr) return(C_APP_ERROR);

   if(!C_GUI::entry_to_int(&pSCLayer->kernel_x, &pSConv_Layer->Kernel.x)) return(C_APP_ERROR);
   if(!C_GUI::entry_to_int(&pSCLayer->kernel_y, &pSConv_Layer->Kernel.y)) return(C_APP_ERROR);
   if(!C_GUI::entry_to_int(&pSCLayer->kernel_z, &pSConv_Layer->Kernel.z)) return(C_APP_ERROR);

   if(!C_GUI::entry_to_int(&pSCLayer->slide_x,  &pSConv_Layer->Slide.x))  return(C_APP_ERROR);
   if(!C_GUI::entry_to_int(&pSCLayer->slide_y,  &pSConv_Layer->Slide.y))  return(C_APP_ERROR);

   if(!C_GUI::entry_to_dbl(&pSCLayer->e_alpha,  &pSConv_Layer->Alpha))   return(C_APP_ERROR);

   pSConv_Layer->activation = pSCLayer->activation.get_active_row_number();

   pSConv_Layer->Beta    = 0.9;
   pSConv_Layer->Bias    = -0.2;
   pSConv_Layer->Divisor = (pSConv_Layer->Kernel.x + pSConv_Layer->Kernel.y + pSConv_Layer->Kernel.z) / 3;
   pSConv_Layer->bHidden = true;

   return(C_APP_READY);
}

//////////////////////////////////////////////////////////////////////////////////
// [ on_button_class ]
//////////////////////////////////////////////////////////////////////////////////
void C_App::on_button_class(Gtk::RadioButton* radio){

   if(radio->get_active()){
      for(int n = 0; n < 11; n++){
         if(sbutton.cbuttons[n].get_active()){
            CNETManager.set_value(n);
            break;
         }
      }
   }
}

//////////////////////////////////////////////////////////////////////////////////
// [ on_button_dataset ]
//////////////////////////////////////////////////////////////////////////////////
void C_App::on_button_dataset(Gtk::RadioButton* radio){

   if(!radio->get_active()) return;

   if(sbutton.dsbuttons[0].get_active()){ // cifar-10

      cout << "cifar" << endl;

      C_GUI::sentry.input_x.set_text("32");
      C_GUI::sentry.input_y.set_text("32");
      C_GUI::sentry.input_z.set_text("3");

      C_GUI::sentry.learning.set_text("50000");
      C_GUI::sentry.testing. set_text("10000");

   }else{ // mnist

      cout << "mnist" << endl;

      C_GUI::sentry.input_x.set_text("28");
      C_GUI::sentry.input_y.set_text("28");
      C_GUI::sentry.input_z.set_text("1");

      C_GUI::sentry.learning.set_text("60000");
      C_GUI::sentry.testing. set_text("10000");
   }
}