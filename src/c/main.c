#include <pebble.h>

Window *my_window;
TextLayer *text_layer;
TextLayer *text_layer2;
TextLayer *text_layer3;
GFont s_text_font;
static char *s_info_str;
static char *s_info_str2;

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
 
  Tuple *temp_tuple3 = dict_find(iterator, MESSAGE_KEY_N);
  s_info_str = (char*)temp_tuple3->value->cstring;
 
  Tuple *temp_tuple4 = dict_find(iterator, MESSAGE_KEY_V);
  s_info_str2 = (char*)temp_tuple4->value->cstring;
  
  text_layer_set_text(text_layer3, "Crypto values:");
  text_layer_set_text(text_layer, s_info_str);
  text_layer_set_text(text_layer2, s_info_str2);
 
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
//  s_btc_up_to_date = false;
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
//  s_btc_up_to_date = false;
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  //APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}


void handle_init(void) {
  my_window = window_create();

  window_stack_push(my_window, true);
  
  s_text_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_DEFAULTFONT_18));
  
  Layer *window_layer = window_get_root_layer(my_window);
  
  window_set_background_color(my_window, GColorBlack);
  
  // text left
  text_layer = text_layer_create(GRect(4, 28, 136, 168));
  text_layer_set_font(text_layer, s_text_font);
  text_layer_set_background_color(text_layer, GColorClear);
  text_layer_set_text_color(text_layer, GColorGreen);
  text_layer_set_text_alignment(text_layer, GTextAlignmentLeft);
  text_layer_set_overflow_mode(text_layer, GTextOverflowModeWordWrap);
    
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
 
  // text right
  text_layer2 = text_layer_create(GRect(4, 28, 136, 168));
  text_layer_set_font(text_layer2, s_text_font);
  text_layer_set_background_color(text_layer2, GColorClear);
  text_layer_set_text_color(text_layer2, GColorOrange);
  text_layer_set_text_alignment(text_layer2, GTextAlignmentRight);
  text_layer_set_overflow_mode(text_layer2, GTextOverflowModeWordWrap);
    
  layer_add_child(window_layer, text_layer_get_layer(text_layer2));
  
   // text top
  text_layer3 = text_layer_create(GRect(0, 0, 144, 168));
  text_layer_set_font(text_layer3, s_text_font);
  text_layer_set_background_color(text_layer3, GColorClear);
  text_layer_set_text_color(text_layer3, GColorCyan);
  text_layer_set_text_alignment(text_layer3, GTextAlignmentCenter);
  text_layer_set_overflow_mode(text_layer3, GTextOverflowModeWordWrap);
    
  layer_add_child(window_layer, text_layer_get_layer(text_layer3));
  
  
  text_layer_set_text(text_layer3, "Resolving:");
  text_layer_set_text(text_layer, "");
  text_layer_set_text(text_layer2, "");
  
  
  // Register callbacks
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  
  const int inbox_size = 256;
  const int outbox_size = 256;
  app_message_open(inbox_size, outbox_size);
}

void handle_deinit(void) {
  app_message_deregister_callbacks();
  
  text_layer_destroy(text_layer2);  
  text_layer_destroy(text_layer3);
  text_layer_destroy(text_layer);
  
  fonts_unload_custom_font(s_text_font);
  

  window_destroy(my_window);
}

void request_values() {
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);

  //APP_LOG(APP_LOG_LEVEL_INFO, "Request update!");
  app_message_outbox_send();     
}
  

int main(void) {
  handle_init();
  request_values();
  app_event_loop();
  handle_deinit();
}
