#include <pebble.h>

static Window *s_main_window;
static Layer *s_canvas;
static TimeUnits lowest_dial_unit = MINUTE_UNIT;
static GFont intervals_font;
static GColor background_color = GColorWhite;
static GColor foreground_color = GColorBlack;

static void draw_intervals_from_center(GContext *ctx, GPoint center, const char *intervals[]) {

    GRect top_rect = GRect(center.x - 14, center.y - 34, 30, 14);
    GRect left_rect = GRect(center.x - 28, center.y - 10, 30, 14);
    GRect right_rect = GRect(center.x, center.y - 10, 30, 14);
    GRect bottom_rect = GRect(center.x - 14, center.y + 10, 30, 14);

    graphics_draw_text(ctx,
            intervals[0], intervals_font, top_rect,
            GTextOverflowModeFill, GTextAlignmentCenter, NULL);

    graphics_draw_text(ctx,
            intervals[1], intervals_font, right_rect,
            GTextOverflowModeFill, GTextAlignmentRight, NULL);

    graphics_draw_text(ctx,
            intervals[2], intervals_font, bottom_rect,
            GTextOverflowModeFill, GTextAlignmentCenter, NULL);

    graphics_draw_text(ctx,
            intervals[3], intervals_font, left_rect,
            GTextOverflowModeFill, GTextAlignmentLeft, NULL);
}

static void canvas_update_proc(Layer *this_layer, GContext *ctx) {

    graphics_context_set_stroke_color(ctx, foreground_color);
    graphics_context_set_text_color(ctx, foreground_color);

    GRect bounds = layer_get_bounds(this_layer);

    GPoint dial1_center = GPoint(bounds.size.w / 4, bounds.size.h / 4);
    GPoint dial2_center = GPoint(bounds.size.w / 4 * 3, bounds.size.h / 4);
    GPoint dial3_center = GPoint(bounds.size.w / 4, bounds.size.h / 4 * 3);
    GPoint dial4_center = GPoint(bounds.size.w / 4 * 3, bounds.size.h / 4 * 3);


    graphics_context_set_fill_color(ctx, GColorBlack);
    graphics_fill_circle(ctx, dial1_center, 32);
    graphics_fill_circle(ctx, dial1_center, 32);
    graphics_fill_circle(ctx, dial2_center, 32);
    graphics_fill_circle(ctx, dial3_center, 32);
    graphics_fill_circle(ctx, dial4_center, 32);

    graphics_context_set_fill_color(ctx, GColorWhite);
    graphics_fill_circle(ctx, dial1_center, 30);
    graphics_fill_circle(ctx, dial1_center, 30);
    graphics_fill_circle(ctx, dial2_center, 30);
    graphics_fill_circle(ctx, dial3_center, 30);
    graphics_fill_circle(ctx, dial4_center, 30);


    const char *interval_seconds_minutes[4];
    interval_seconds_minutes[0] = "60";
    interval_seconds_minutes[1] = "15";
    interval_seconds_minutes[2] = "30";
    interval_seconds_minutes[3] = "45";
    draw_intervals_from_center(ctx, dial1_center, interval_seconds_minutes);

    draw_intervals_from_center(ctx, dial2_center, interval_seconds_minutes);

    const char *interval_hours[4];
    interval_hours[0] = "24";
    interval_hours[1] = "6";
    interval_hours[2] = "12";
    interval_hours[3] = "18";
    draw_intervals_from_center(ctx, dial3_center, interval_hours);

    const char *interval_days[4];
    interval_days[0] = "31";
    interval_days[1] = "8";
    interval_days[2] = "15";
    interval_days[3] = "23";
    draw_intervals_from_center(ctx, dial4_center, interval_days);

}

static void main_window_load(Window *window) {

    window_set_background_color(s_main_window, background_color);

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

    intervals_font = fonts_get_system_font(FONT_KEY_GOTHIC_18);

    s_main_window = window_create();

    window_set_window_handlers(s_main_window, (WindowHandlers) {
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
