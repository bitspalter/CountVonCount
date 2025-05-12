//////////////////////////////////////////////////////////////////////////////////
// [ C_GUI_Class_Source ]
//////////////////////////////////////////////////////////////////////////////////

#include "C_GUI.hpp"

//////////////////////////////////////////////////////////////////////////////////
// [ Constructor ]
//////////////////////////////////////////////////////////////////////////////////
C_GUI::C_GUI(){

   this->set_default_size(1220, 600);

   create_button();
   create_label();
   create_entry();

   /////////////////////////////////////////////////
   // Button

   sbox.hbutton.pack_start(sbutton.start);
   sbox.hbutton.pack_start(sbutton.stop);

   auto context = sbox.hbutton.get_style_context();
   context->add_class(GTK_STYLE_CLASS_LINKED);
   
   //////////////////////////////////////////

   scombo.layer_type.set_size_request(126, 30);
   scombo.layer_type.append("conv");
   scombo.layer_type.append("pool");
   scombo.layer_type.append("fully");
   scombo.layer_type.set_active(0);

   sbox.cbutton.pack_start(sbutton.create);
   sbox.cbutton.pack_start(sbutton.destroy);

   context = sbox.cbutton.get_style_context();
   context->add_class(GTK_STYLE_CLASS_LINKED);

   sbox.lbutton.pack_start(scombo.layer_type);
   sbox.lbutton.pack_start(sbutton.add_layer);
   sbox.lbutton.pack_start(sbutton.del_layer);

   context = sbox.lbutton.get_style_context();
   context->add_class(GTK_STYLE_CLASS_LINKED);

   sbox.gbutton.pack_start(sbutton.save_config);
   sbox.gbutton.pack_start(sbutton.load_config);
   sbox.gbutton.pack_start(sbutton.save_param);
   sbox.gbutton.pack_start(sbutton.load_param);

   context = sbox.gbutton.get_style_context();
   context->add_class(GTK_STYLE_CLASS_LINKED);
   
   ///////////////////////////////////////////////////
   // Layout
   ///////////////////////////////////////////////////

   for(int n = 0, m = 10; n < 11; n++, m +=50){
      slayout.network.put(sbutton.cbuttons[n], m, 0);
   }

   //////////////////////////////////////////////////
   // Config

   sbox.input.pack_start(slabel.input);
   sbox.input.pack_start(slabel.input_x);
   sbox.input.pack_start(sentry.input_x);
   sbox.input.pack_start(slabel.input_y);
   sbox.input.pack_start(sentry.input_y);
   sbox.input.pack_start(slabel.input_z);
   sbox.input.pack_start(sentry.input_z);

   slayout.config.put(sbox.input, 10, 55);

   /////////////////////////////////////////////////
   // Dataset
   sbox.dataset.pack_start(slabel.dataset);

   sbox.dataset.pack_start(sbutton.dsbuttons[0]);
   sbox.dataset.pack_start(sbutton.dsbuttons[1]);

   sbox.dataset.pack_start(slabel.learning);
   sbox.dataset.pack_start(sentry.learning);
   sbox.dataset.pack_start(slabel.testing);
   sbox.dataset.pack_start(sentry.testing);
   sbox.dataset.pack_start(slabel.batch);
   sbox.dataset.pack_start(sentry.batch);

   slayout.config.put(sbox.dataset, 10, 10);

   ////////////////////////

   slayout.config.put(sbox.cbutton,  10, 100);
   slayout.config.put(sbox.lbutton, 245, 100);
   slayout.config.put(sbox.gbutton, 605, 100);
   
   ////////////////////////
   
   slayout.network. put(swindow.network,  0, 33);
   slayout.failed.  put(swindow.failed,   0, 33);
   slayout.gradient.put(swindow.gradient, 0, 33);

   // TEST
   swindow.config.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
   swindow.config.add(slayout.config);
   
   slayout.config.set_size(1000, pos_config_y);


   //////////////////////////////////////////////////////////////
   // Logging
   
   Gdk::RGBA colorB = Gdk::RGBA("#101010");
   Gdk::RGBA colorF = Gdk::RGBA("#777777");
   
   Pango::FontDescription fdsc("Courier New 11");

   sLogging.pbuffer = Gtk::TextBuffer::create();
   sLogging.view.set_buffer(sLogging.pbuffer);
   sLogging.view.override_font(fdsc);
   sLogging.view.override_color(colorF);
   sLogging.view.override_background_color(colorB);
   sLogging.window.add(sLogging.view);
   sLogging.window.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
   sLogging.window.set_size_request(200, 200);

   //////////////////////////////////////////////////////////////
   // Notebook
   
   notebook.append_page(slayout.network,  "Network");
   notebook.append_page(slayout.gradient, "Gradient");
   notebook.append_page(slayout.failed,   "Failed");
   notebook.append_page(sLogging.window,  "Logging");
   notebook.append_page(swindow.config,   "Config");

   //////////////////////////////////////////////////////////////
   // HeaderBar
   
   set_titlebar(header);
   
   header.set_title("NEU graf zahl");
   header.set_subtitle("Welcome");
   header.set_show_close_button();
   
   header.pack_start(sbox.hbutton);
   header.pack_start(sentry.eSleep);
   header.pack_start(sbutton.zoom_in);
   header.pack_start(sbutton.zoom_out);
   header.pack_start(sbutton.sleep);

   ///////////////////////////////////////////////////////

   box.set_orientation(Gtk::ORIENTATION_VERTICAL);
   box.set_spacing(0);
   box.set_homogeneous(false);

   add(box);

   box.pack_start(notebook);
}

//////////////////////////////////////////////////////////////////////////////////
// [ create_button ]
//////////////////////////////////////////////////////////////////////////////////
void C_GUI::create_button(){

   sbutton.zoom_in.set_image_from_icon_name("zoom-in");
   sbutton.zoom_in.set_size_request(50, 30);
   sbutton.zoom_in.set_sensitive(true);

   sbutton.zoom_out.set_image_from_icon_name("zoom-out");
   sbutton.zoom_out.set_size_request(50, 30);
   sbutton.zoom_out.set_sensitive(true);

   sbutton.start.set_image_from_icon_name("media-playback-start");
   sbutton.start.set_size_request(50, 30);
   sbutton.start.set_sensitive(false);

   sbutton.stop.set_image_from_icon_name("media-playback-stop");
   sbutton.stop.set_size_request(50, 30);
   sbutton.stop.set_sensitive(false);

   //////////////////////////////////////////

   sbutton.save_config.set_label("save config");
   sbutton.save_config.set_size_request(100, 30);

   sbutton.load_config.set_label("load config");
   sbutton.load_config.set_size_request(100, 30);

   sbutton.save_param.set_label("save weights");
   sbutton.save_param.set_size_request(100, 30);

   sbutton.load_param.set_label("load weights");
   sbutton.load_param.set_size_request(100, 30);

   //////////////////////////////////////////

   sbutton.sleep.set_label("sleep");
   sbutton.sleep.set_size_request(100, 30);

   //////////////////////////////////////////

   sbutton.create.set_label("create");
   sbutton.create.set_size_request(100, 30);

   sbutton.destroy.set_label("destroy");
   sbutton.destroy.set_size_request(100, 30);
   sbutton.destroy.set_sensitive(false);

   //////////////////////////////////////////

   for(int n = 0; n < 11; n++){
      sbutton.cbuttons[n].set_size_request(50, 30);
      sbutton.cbuttons[n].join_group(sbutton.cbuttons[10]);
   }

   sbutton.cbuttons[0].set_label(" 0 ");
   sbutton.cbuttons[1].set_label(" 1 ");
   sbutton.cbuttons[2].set_label(" 2 ");
   sbutton.cbuttons[3].set_label(" 3 ");
   sbutton.cbuttons[4].set_label(" 4 ");
   sbutton.cbuttons[5].set_label(" 5 ");
   sbutton.cbuttons[6].set_label(" 6 ");
   sbutton.cbuttons[7].set_label(" 7 ");
   sbutton.cbuttons[8].set_label(" 8 ");
   sbutton.cbuttons[9].set_label(" 9 ");

   sbutton.cbuttons[10].set_label(" 0-9 ");
   sbutton.cbuttons[10].set_active();

   /////////////////////////////////////////////

   for(int n = 0; n < 2; n++){
      sbutton.dsbuttons[n].set_size_request(50, 30);
      sbutton.dsbuttons[n].join_group(sbutton.dsbuttons[1]);
   }

   sbutton.dsbuttons[0].set_label(" cifar-10 ");
   sbutton.dsbuttons[1].set_label(" mnist ");

   sbutton.dsbuttons[0].set_active();

   /////////////////////////////////////////////

   sbutton.add_layer.set_label("add layer");
   sbutton.add_layer.set_size_request(100, 30);

   sbutton.del_layer.set_label("del layer");
   sbutton.del_layer.set_size_request(100, 30);

   sbutton.test.set_label("test");
   sbutton.test.set_size_request(100, 30);
}

//////////////////////////////////////////////////////////////////////////////////
// [ create_label ]
//////////////////////////////////////////////////////////////////////////////////
void C_GUI::create_label(){

   slabel.input.set_label(" Input Layer: ");
   slabel.input_x.set_label(" [ x ] ");
   slabel.input_y.set_label(" [ y ] ");
   slabel.input_z.set_label(" [ z ] ");

   /////////////////////

   slabel.batch.set_label(" Batch Size: ");

   /////////////////////

   slabel.dataset.set_label(" Dataset: ");
   slabel.learning.set_label(" Training Samples: ");
   slabel.testing.set_label(" Test Samples: ");
}

//////////////////////////////////////////////////////////////////////////////////
// [ create_entry ]
//////////////////////////////////////////////////////////////////////////////////
void C_GUI::create_entry(){

   sentry.eSleep.set_max_length(5);
   sentry.eSleep.set_alignment(0.5);
   sentry.eSleep.set_width_chars(7);
   sentry.eSleep.set_size_request(-1, 30);
   sentry.eSleep.set_text("100");

   sentry.eFile.set_alignment(0.5);
   sentry.eFile.set_width_chars(12);
   sentry.eFile.set_size_request(-1, 30);

   ////////////////////////////////////////////
   // Input

   sentry.input_x.set_max_length(5);
   sentry.input_x.set_alignment(0.5);
   sentry.input_x.set_width_chars(7);
   sentry.input_x.set_size_request(-1, 30);

   sentry.input_y.set_max_length(5);
   sentry.input_y.set_alignment(0.5);
   sentry.input_y.set_width_chars(7);
   sentry.input_y.set_size_request(-1, 30);

   sentry.input_z.set_max_length(5);
   sentry.input_z.set_alignment(0.5);
   sentry.input_z.set_width_chars(7);
   sentry.input_z.set_size_request(-1, 30);

   ////////////////////////////////////////////
   // Hyper
   sentry.batch.set_max_length(5);
   sentry.batch.set_alignment(0.5);
   sentry.batch.set_width_chars(7);
   sentry.batch.set_size_request(-1, 30);

   ////////////////////////////////////////////

   sentry.learning.set_max_length(5);
   sentry.learning.set_alignment(0.5);
   sentry.learning.set_width_chars(7);
   sentry.learning.set_size_request(-1, 30);

   sentry.testing.set_max_length(5);
   sentry.testing.set_alignment(0.5);
   sentry.testing.set_width_chars(7);
   sentry.testing.set_size_request(-1, 30);
}

//////////////////////////////////////////////////////////////////////////////////
// [ add_layer ]
//////////////////////////////////////////////////////////////////////////////////
int C_GUI::add_layer(int type){

   SCL_Fill scl;
   SPL_Fill spl;
   SFL_Fill sfl;

   switch(type){
      case C_GUI_LAYER_CONV:  
         add_CLayer(&scl); 
         break;
      case C_GUI_LAYER_POOL:  
         add_PLayer(&spl); 
         break;
      case C_GUI_LAYER_FULLY: 
         add_FLayer(&sfl); 
         break;

      default: return(C_GUI_ERROR); break;
   }

   vLayer.push_back(type); 

   // TEST
   pos_config_y += 50;
   slayout.config.set_size(1200, pos_config_y);
   //cout << "pos_config_y:" << pos_config_y << endl;

   return(C_GUI_READY);
}

//////////////////////////////////////////////////////////////////////////////////
// [ add_layer ]
//////////////////////////////////////////////////////////////////////////////////
int C_GUI::add_layer(int type, std::variant<SCL_Fill, SPL_Fill, SFL_Fill> sFill){

   switch(type){
      case C_GUI_LAYER_CONV:

         if(holds_alternative<SCL_Fill>(sFill))
            add_CLayer(&get<SCL_Fill>(sFill));

         break;

      case C_GUI_LAYER_POOL: 

         if(holds_alternative<SPL_Fill>(sFill))
            add_PLayer(&get<SPL_Fill>(sFill));

         break;

      case C_GUI_LAYER_FULLY:

         if(holds_alternative<SFL_Fill>(sFill))
            add_FLayer(&get<SFL_Fill>(sFill));

         break;

      default: return(C_GUI_ERROR); break;
   }

   vLayer.push_back(type); 

   // TEST
   pos_config_y += 50;
   slayout.config.set_size(1200, pos_config_y);

   return(C_GUI_READY);
}

//////////////////////////////////////////////////////////////////////////////////
// [ del_layer ] // delete Last Layer
//////////////////////////////////////////////////////////////////////////////////
int C_GUI::del_layer(int type){

   switch(type){
      case C_GUI_LAYER_CONV:  del_CLayer(); break;
      case C_GUI_LAYER_POOL:  del_PLayer(); break;
      case C_GUI_LAYER_FULLY: del_FLayer(); break;

      default: return(C_GUI_ERROR); break;
   }

   vLayer.pop_back();

   // TEST
   pos_config_y -= 50;
   slayout.config.set_size(1200, pos_config_y);

   return(C_GUI_READY);
}

//////////////////////////////////////////////////////////////////////////////////
// [ add_CLayer ]
//////////////////////////////////////////////////////////////////////////////////
int C_GUI::add_CLayer(SCL_Fill* pSCL_Fill){

   if(pSCL_Fill == nullptr) return(C_GUI_ERROR);
   
   std::string sData;

   SCLayer* player = new SCLayer;

   player->name.set_label(" conv ");
   player->name.set_alignment(0.5);
   player->name.set_width_chars(7);
   player->name.set_size_request(-1, 30);

   ////////////////////////////////////////////////

   player->kernel.set_label(" kernel ");
   player->kernel.set_alignment(0.5);
   player->kernel.set_width_chars(7);
   player->kernel.set_size_request(-1, 30);

   player->k_x.set_label("x");
   player->k_x.set_alignment(0.5);
   player->k_x.set_size_request(-1, 30);

   player->kernel_x.set_max_length(5);
   player->kernel_x.set_alignment(0.5);
   player->kernel_x.set_width_chars(6);
   player->kernel_x.set_size_request(-1, 30);

   sData = std::to_string(pSCL_Fill->kernel_x);
   player->kernel_x.set_text(sData);

   player->k_y.set_label("y");
   player->k_y.set_alignment(0.5);
   player->k_y.set_size_request(-1, 30);

   player->kernel_y.set_max_length(5);
   player->kernel_y.set_alignment(0.5);
   player->kernel_y.set_width_chars(6);
   player->kernel_y.set_size_request(-1, 30);

   sData = std::to_string(pSCL_Fill->kernel_y);
   player->kernel_y.set_text(sData);

   player->k_z.set_label("z");
   player->k_z.set_alignment(0.5);
   player->k_z.set_size_request(-1, 30);

   player->kernel_z.set_max_length(5);
   player->kernel_z.set_alignment(0.5);
   player->kernel_z.set_width_chars(6);
   player->kernel_z.set_size_request(-1, 30);

   sData = std::to_string(pSCL_Fill->kernel_z);
   player->kernel_z.set_text(sData);

   ////////////////////////////////////////////////

   player->slide.set_label(" slide ");
   player->slide.set_alignment(0.5);
   player->slide.set_width_chars(7);
   player->slide.set_size_request(-1, 30);

   player->s_x.set_label("x");
   player->s_x.set_alignment(0.5);
   player->s_x.set_size_request(-1, 30);

   player->slide_x.set_max_length(5);
   player->slide_x.set_alignment(0.5);
   player->slide_x.set_width_chars(6);
   player->slide_x.set_size_request(-1, 30);

   sData = std::to_string(pSCL_Fill->Slide_x);
   player->slide_x.set_text(sData);

   player->s_y.set_label("y");
   player->s_y.set_alignment(0.5);
   player->s_y.set_size_request(-1, 30);

   player->slide_y.set_max_length(5);
   player->slide_y.set_alignment(0.5);
   player->slide_y.set_width_chars(6);
   player->slide_y.set_size_request(-1, 30);

   sData = std::to_string(pSCL_Fill->Slide_y);
   player->slide_y.set_text(sData);

   ////////////////////////////////////////////////

   player->activation.set_size_request(100, 30); 

   player->activation.append("ReLu");
   player->activation.append("Sigmoid");
   //player->activation.append("Softmax");

   player->activation.set_active(pSCL_Fill->activation);

   ////////////////////////////////////////////////

   player->l_alpha.set_label("alpha");

   player->e_alpha.set_max_length(5);
   player->e_alpha.set_alignment(0.5);
   player->e_alpha.set_width_chars(6);
   player->e_alpha.set_size_request(-1, 30);
   player->e_alpha.set_text("0.1");

   sData = std::to_string(pSCL_Fill->alpha);
   player->e_alpha.set_text(sData);

   ////////////////////////////////////////////////

   player->box.set_spacing(5);
   player->box.set_homogeneous(false);

   player->box.pack_start(player->name);

   player->box.pack_start(player->kernel);
   player->box.pack_start(player->k_x);
   player->box.pack_start(player->kernel_x);
   player->box.pack_start(player->k_y);
   player->box.pack_start(player->kernel_y);
   player->box.pack_start(player->k_z);
   player->box.pack_start(player->kernel_z);

   player->box.pack_start(player->slide);
   player->box.pack_start(player->s_x);
   player->box.pack_start(player->slide_x);
   player->box.pack_start(player->s_y);
   player->box.pack_start(player->slide_y);

   player->box.pack_start(player->activation);

   player->box.pack_start(player->l_alpha);
   player->box.pack_start(player->e_alpha);

   //////////////////////////////////////////////////

   slayout.config.put(player->box, 10, pos_layer_y);

   pos_layer_y += 50;

   vClayer.push_back(player);

   show_all_children();

   return(C_GUI_READY);
}

//////////////////////////////////////////////////////////////////////////////////
// [ del_CLayer ]
//////////////////////////////////////////////////////////////////////////////////
int C_GUI::del_CLayer(){

   if(!vClayer.size()) return(C_GUI_READY);

   SCLayer* player = vClayer.back();

   delete player;

   vClayer.pop_back();

   pos_layer_y -= 50;

   return(C_GUI_READY);
}

//////////////////////////////////////////////////////////////////////////////////
// [ add_PLayer ]
//////////////////////////////////////////////////////////////////////////////////
int C_GUI::add_PLayer(SPL_Fill* pSPL_Fill){

   if(pSPL_Fill == nullptr) return(C_GUI_ERROR);

   std::string sData;

   SPLayer* player = new SPLayer;

   player->name.set_label(" pool ");
   player->name.set_alignment(0.5);
   player->name.set_width_chars(7);
   player->name.set_size_request(-1, 30);

   player->pool.set_label(" pool ");
   player->pool.set_alignment(0.5);
   player->pool.set_width_chars(7);
   player->pool.set_size_request(-1, 30);

   player->po_x.set_label("x");
   player->po_x.set_alignment(0.5);
   player->po_x.set_size_request(-1, 30);

   player->pool_x.set_max_length(5);
   player->pool_x.set_alignment(0.5);
   player->pool_x.set_width_chars(6);
   player->pool_x.set_size_request(-1, 30);

   sData = std::to_string(pSPL_Fill->pool_x);
   player->pool_x.set_text(sData);

   player->po_y.set_label("y");
   player->po_y.set_alignment(0.5);
   player->po_y.set_size_request(-1, 30);

   player->pool_y.set_max_length(5);
   player->pool_y.set_alignment(0.5);
   player->pool_y.set_width_chars(6);
   player->pool_y.set_size_request(-1, 30);

   sData = std::to_string(pSPL_Fill->pool_y);
   player->pool_y.set_text(sData);

   ////////////////////////////////////////////////

   player->padd.set_label(" padd ");
   player->padd.set_alignment(0.5);
   player->padd.set_width_chars(7);
   player->padd.set_size_request(-1, 30);

   player->pa_x.set_label("x");
   player->pa_x.set_alignment(0.5);
   player->pa_x.set_size_request(-1, 30);

   player->padd_x.set_max_length(5);
   player->padd_x.set_alignment(0.5);
   player->padd_x.set_width_chars(6);
   player->padd_x.set_size_request(-1, 30);

   sData = std::to_string(pSPL_Fill->padd_x);
   player->padd_x.set_text(sData);

   player->pa_y.set_label("y");
   player->pa_y.set_alignment(0.5);
   player->pa_y.set_size_request(-1, 30);

   player->padd_y.set_max_length(5);
   player->padd_y.set_alignment(0.5);
   player->padd_y.set_width_chars(6);
   player->padd_y.set_size_request(-1, 30);

   sData = std::to_string(pSPL_Fill->padd_y);
   player->padd_y.set_text(sData);

   // TEST
   player->warmup.set_label("warmup");
   player->warmup.set_active(true);

   ////////////////////////////////////////////////

   player->box.set_spacing(5);
   player->box.set_homogeneous(false);

   player->box.pack_start(player->name);

   player->box.pack_start(player->pool);
   player->box.pack_start(player->po_x);
   player->box.pack_start(player->pool_x);
   player->box.pack_start(player->po_y);
   player->box.pack_start(player->pool_y);

   player->box.pack_start(player->padd);
   player->box.pack_start(player->pa_x);
   player->box.pack_start(player->padd_x);
   player->box.pack_start(player->pa_y);
   player->box.pack_start(player->padd_y);

   player->box.pack_start(player->warmup);
   
   //////////////////////////////////////////////////

   slayout.config.put(player->box, 10, pos_layer_y);

   pos_layer_y += 50;

   vPlayer.push_back(player);

   show_all_children();

   return(C_GUI_READY);
}

//////////////////////////////////////////////////////////////////////////////////
// [ del_PLayer ]
//////////////////////////////////////////////////////////////////////////////////
int C_GUI::del_PLayer(){

   if(!vPlayer.size()) return(C_GUI_READY);

   SPLayer* player = vPlayer.back();

   delete player;

   vPlayer.pop_back();

   pos_layer_y -= 50;

   return(C_GUI_READY);
}

//////////////////////////////////////////////////////////////////////////////////
// [ add_FLayer ]
//////////////////////////////////////////////////////////////////////////////////
int C_GUI::add_FLayer(SFL_Fill* pSFL_Fill){

   if(pSFL_Fill == nullptr) return(C_GUI_ERROR);

   std::string sData;

   SFLayer* player = new SFLayer;

   player->name.set_label(" fully ");
   player->name.set_alignment(0.5);
   player->name.set_width_chars(7);
   player->name.set_size_request(-1, 30);

   player->neurons.set_max_length(5);
   player->neurons.set_alignment(0.5);
   player->neurons.set_width_chars(6);
   player->neurons.set_size_request(-1, 30);

   sData = std::to_string(pSFL_Fill->neurons);
   player->neurons.set_text(sData);
   
   //////////////////////////////////////////////

   player->activation.set_size_request(100, 30); 

   player->activation.append("ReLu");
   player->activation.append("Sigmoid");
   player->activation.append("Softmax");

   player->activation.set_active(pSFL_Fill->activation);

   ////////////////////////////////////////////////

   player->l_alpha.set_label("alpha");

   player->e_alpha.set_max_length(5);
   player->e_alpha.set_alignment(0.5);
   player->e_alpha.set_width_chars(6);
   player->e_alpha.set_size_request(-1, 30);

   sData = std::to_string(pSFL_Fill->alpha);
   player->e_alpha.set_text(sData);

   // TEST
   player->warmup.set_label("warmup");
   player->warmup.set_active(true);
   
   ////////////////////////////////////////////////

   player->box.set_spacing(5);
   player->box.set_homogeneous(false);

   player->box.pack_start(player->name);
   player->box.pack_start(player->neurons);

   player->box.pack_start(player->activation);

   player->box.pack_start(player->l_alpha);
   player->box.pack_start(player->e_alpha);

   // TEST
   player->box.pack_start(player->warmup);

   //////////////////////////////////////////////

   slayout.config.put(player->box, 10, pos_layer_y);

   pos_layer_y += 50;

   vFlayer.push_back(player);

   show_all_children();

   return(C_GUI_READY);
}

//////////////////////////////////////////////////////////////////////////////////
// [ del_FLayer ]
//////////////////////////////////////////////////////////////////////////////////
int C_GUI::del_FLayer(){

   if(!vFlayer.size()) return(C_GUI_READY);

   SFLayer* player = vFlayer.back();

   delete player;

   vFlayer.pop_back();

   pos_layer_y -= 50;

   return(C_GUI_READY);
}

//////////////////////////////////////////////////////////////////////////////////
// [ remove_layer ]
//////////////////////////////////////////////////////////////////////////////////
int C_GUI::remove_layer(){

   if(vFlayer.size()){
      for(SFLayer* x : vFlayer) 
         delete x;
      vFlayer.clear(); 
   }

   for(SPLayer* x : vPlayer) 
      delete x;
   vPlayer.clear(); 

   for(SCLayer* x : vClayer) 
      delete x;
   vClayer.clear(); 

   vLayer.clear(); // TEST

   pos_layer_y = 200;

   return(C_GUI_READY);
}

//////////////////////////////////////////////////////////////////////////////////
// [ entry_to_int ]
//////////////////////////////////////////////////////////////////////////////////
bool C_GUI::entry_to_int(Gtk::Entry* pEntry, unsigned int* pInt){

   if(pEntry == nullptr || pInt == nullptr) return(false);

   Glib::ustring SEdit;

   SEdit = pEntry->get_text();

   if(SEdit.empty()) return(false);

   *pInt = stoul(SEdit, nullptr, 0);

   return(true);
}

//////////////////////////////////////////////////////////////////////////////////
// [ entry_to_dbl ]
//////////////////////////////////////////////////////////////////////////////////
bool C_GUI::entry_to_dbl(Gtk::Entry* pEntry, double* pDouble){

   if(pEntry == nullptr || pDouble == nullptr) return(false);

   Glib::ustring SEdit;

   SEdit = pEntry->get_text();
   if(SEdit.empty()){
      cout << "alle Felder ausfüllen !!!" << endl;
      return(false);
   }
   
   int nfound = SEdit.find(".", 0);  // stod() does not work with "."
   if(nfound != -1) SEdit.replace(nfound, 1, ",");

   *pDouble = stod(SEdit, nullptr);

   return(true);
}