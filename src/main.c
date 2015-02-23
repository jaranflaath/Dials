#include <pebble.h>

static Window *s_main_window;
static Layer *window_layer;
static Layer *s_canvas;
static TimeUnits lowest_dial_unit = SECOND_UNIT;
static GFont intervals_font;
static GColor background_color = GColorWhite;
static GColor foreground_color = GColorBlack;
static int current_seconds;
static int current_minutes;
static int current_hours;
static int current_days;

GPoint getDialHandEndPoint(GPoint *center, int unit_amount, int radius, int unit_max);

static void draw_intervals_from_center(GContext *ctx, GPoint center, const char *intervals[]) {

    GRect top_rect = GRect(center.x - 14, center.y - 34, 30, 14);
    GRect left_rect = GRect(center.x - 28, center.y - 12, 30, 14);
    GRect right_rect = GRect(center.x, center.y - 12, 30, 14);
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

    int radius = 28;

    GPoint p_seconds = getDialHandEndPoint(&dial1_center, current_seconds, radius, 60);
    graphics_draw_line(ctx, dial1_center, p_seconds);

    GPoint p_minutes = getDialHandEndPoint(&dial2_center, current_minutes, radius, 60);
    graphics_draw_line(ctx, dial2_center, p_minutes);

    GPoint p_hours = getDialHandEndPoint(&dial3_center, current_hours, radius, 24);
    graphics_draw_line(ctx, dial3_center, p_hours);

    GPoint p_days = getDialHandEndPoint(&dial4_center, current_days, radius, 31);
    graphics_draw_line(ctx, dial4_center, p_days);
}

GPoint getDialHandEndPoint(GPoint *center, int unit_amount, int radius, int unit_max) {
    int32_t angle = TRIG_MAX_ANGLE * unit_amount / unit_max;
    int x = (sin_lookup(angle) * radius / TRIG_MAX_RATIO) + (*center).x;
    int y = (-cos_lookup(angle) * radius / TRIG_MAX_RATIO) + (*center).y;
    GPoint point = GPoint(x, y);
    return point;
}

static void main_window_load(Window *window) {

    window_set_background_color(s_main_window, background_color);

    window_layer = window_get_root_layer(s_main_window);
    GRect window_bounds = layer_get_bounds(window_layer);

    s_canvas = layer_create(GRect(0, 0, window_bounds.size.w, window_bounds.size.h));
    layer_add_child(window_layer, s_canvas);

    layer_set_update_proc(s_canvas, canvas_update_proc);
}

static void main_window_unload(Window *window) {

}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {

    current_seconds = tick_time->tm_sec;
    current_minutes = tick_time->tm_min;
    current_hours = tick_time->tm_hour;
    current_days = tick_time->tm_mday;

    APP_LOG(APP_LOG_LEVEL_INFO, "Marking layer as dirty");
    layer_mark_dirty(window_layer);
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
