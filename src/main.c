#include <pebble.h>

static Window *s_main_window;
static Layer *s_canvas;
static TimeUnits lowest_dial_unit = MINUTE_UNIT;
static GFont intervals_font = FONT_KEY_GOTHIC_14;

static void canvas_update_proc(Layer *this_layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(this_layer);

  GPoint center = GPoint(bounds.size.w / 2, (bounds.size.h / 2));
  GPoint dial1_center = GPoint(bounds.size.w / 4, bounds.size.h / 4);
  GPoint dial2_center = GPoint(bounds.size.w / 4 * 3, bounds.size.h / 4);
  GPoint dial3_center = GPoint(bounds.size.w / 4, bounds.size.h / 4 * 3);
  GPoint dial4_center = GPoint(bounds.size.w / 4 * 3, bounds.size.h / 4 * 3);

  GRect dial1_interval_top_rect = GRect(dial1_center.x, dial1_center.y - 15, 30, 14);

  // Draw the 'loop' of the 'P'
  graphics_context_set_stroke_color(ctx, GColorWhite);
  graphics_draw_circle(ctx, dial1_center, 30);

  //graphics_draw_text(ctx, "60\0", intervals_font, dial1_interval_top_rect,
  //GTextOverflowModeFill, GTextAlignmentCenter, NULL);


  graphics_draw_circle(ctx, dial2_center, 30);
  graphics_draw_circle(ctx, dial3_center, 30);
  graphics_draw_circle(ctx, dial4_center, 30);

}

static void main_window_load(Window *window) {

  window_set_background_color(s_main_window, GColorBlack);

  Layer *window_layer = window_get_root_layer(s_main_window);
  GRect window_bounds = layer_get_bounds(window_layer);

  s_canvas = layer_create(GRect(0, 0, window_bounds.size.w, window_bounds.size.h));
  layer_add_child(window_layer, s_canvas);

  layer_set_update_proc(s_canvas, canvas_update_proc);
}

static void main_window_unload(Window *window) {

}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {


}

static void init() {
  s_main_window = window_create();

  window_set_window_handlers(s_main_window, (WindowHandlers){
    .load = main_window_load,
    .unload = main_window_unload
  });

  window_stack_push(s_main_window, true);

  tick_timer_service_subscribe(lowest_dial_unit, tick_handler);
}

static void deinit() {

  window_destroy(s_main_window);

}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
