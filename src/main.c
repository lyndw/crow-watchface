#include <pebble.h>
static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_day_layer;
static GFont s_time_font;
static BitmapLayer *s_background_layer;
static GBitmap *s_crow_bitmap;

static void main_window_load(Window *window){
    
  // Create GBitmap, then set to created BitmapLayer
  s_crow_bitmap = gbitmap_create_with_resource(RESOURCE_ID_CROW);
  s_background_layer = bitmap_layer_create(GRect(0, 56, 144, 112));
  bitmap_layer_set_bitmap(s_background_layer, s_crow_bitmap);
  bitmap_layer_set_background_color(s_background_layer, GColorClear);

  // Create time TextLayer
  s_time_layer = text_layer_create(GRect(0, 0, 144, 37));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
 
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_DOS_FONT_36));
  text_layer_set_font(s_time_layer, s_time_font);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

   // Create day TextLayer
   s_day_layer = text_layer_create(GRect(0, 30, 144, 62));
   text_layer_set_background_color(s_day_layer, GColorClear);
   text_layer_set_text_color(s_day_layer, GColorBlack);
 
   text_layer_set_font(s_day_layer, s_time_font);
   text_layer_set_text_alignment(s_day_layer, GTextAlignmentCenter);


  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_background_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
   layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_day_layer));
}
static void main_window_unload(Window *window){
  fonts_unload_custom_font(s_time_font);
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_day_layer);
  // Destroy GBitmap
  gbitmap_destroy(s_crow_bitmap);

  // Destroy BitmapLayer
  bitmap_layer_destroy(s_background_layer);

}

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Create two long-lived buffers
  static char buffer[] = "00:00";
  static char dayBuffer[] = "Mon 11";
  

  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) {
    // Use 24 hour format
    strftime(buffer, sizeof(buffer), "%H:%M", tick_time);
  } else {
    // Use 12 hour format
    strftime(buffer, sizeof(buffer), "%I:%M", tick_time);
  }
  // write the day
  strftime(dayBuffer, sizeof(dayBuffer), "%a %d", tick_time);
  

  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, buffer);
  text_layer_set_text(s_day_layer, dayBuffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}
static void init(){
  s_main_window = window_create();
  
  window_set_window_handlers(s_main_window, (WindowHandlers){
    .load = main_window_load,
    .unload = main_window_unload
  });
  
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  
  // display with animation true
  window_stack_push(s_main_window, true);
  update_time();
}
static void deinit(){
  window_destroy(s_main_window);
}
int main(void){
  init();
  app_event_loop();
  deinit();
}