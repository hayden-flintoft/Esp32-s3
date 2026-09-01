#include "ui.h"
#include <lvgl.h>

namespace ui {

static Pet *s_pet = nullptr;

// Face
static lv_obj_t *face_body;
static lv_obj_t *eye_l, *eye_r;
static lv_obj_t *mouth;

// Text
static lv_obj_t *label_stage;
static lv_obj_t *label_mood;
static lv_obj_t *label_age;

// Bars
static lv_obj_t *bar_hunger, *bar_happy, *bar_energy, *bar_hygiene;

// Buttons
static lv_obj_t *btn_sleep_label;

static lv_color_t mood_color(PetMood m) {
    switch (m) {
        case PetMood::Great: return lv_palette_main(LV_PALETTE_GREEN);
        case PetMood::Okay:  return lv_palette_main(LV_PALETTE_YELLOW);
        case PetMood::Sad:   return lv_palette_main(LV_PALETTE_ORANGE);
        case PetMood::Sick:  return lv_palette_main(LV_PALETTE_RED);
    }
    return lv_color_black();
}

static void feed_cb(lv_event_t *e)     { (void)e; s_pet->feed(); }
static void play_cb(lv_event_t *e)     { (void)e; s_pet->play(); }
static void clean_cb(lv_event_t *e)    { (void)e; s_pet->clean(); }
static void sleep_cb(lv_event_t *e)    { (void)e; s_pet->toggleSleep(); }
static void medicine_cb(lv_event_t *e) { (void)e; s_pet->giveMedicine(); }

static lv_obj_t *make_button(lv_obj_t *parent, const char *text, lv_event_cb_t cb) {
    lv_obj_t *btn = lv_btn_create(parent);
    lv_obj_set_size(btn, 42, 42);
    lv_obj_add_event_cb(btn, cb, LV_EVENT_CLICKED, NULL);
    lv_obj_t *lbl = lv_label_create(btn);
    lv_label_set_text(lbl, text);
    lv_obj_center(lbl);
    return btn;
}

static lv_obj_t *make_stat_bar(lv_obj_t *parent, const char *name, lv_color_t color, int y) {
    lv_obj_t *lbl = lv_label_create(parent);
    lv_label_set_text(lbl, name);
    lv_obj_set_pos(lbl, 8, y);

    lv_obj_t *bar = lv_bar_create(parent);
    lv_obj_set_size(bar, 150, 12);
    lv_obj_set_pos(bar, 60, y + 2);
    lv_bar_set_range(bar, 0, 100);
    lv_obj_set_style_bg_color(bar, color, LV_PART_INDICATOR);
    return bar;
}

void build(Pet *pet) {
    s_pet = pet;

    lv_obj_t *scr = lv_scr_act();
    lv_obj_set_style_bg_color(scr, lv_color_hex(0xEFF6FF), 0);

    // --- Header ---
    label_stage = lv_label_create(scr);
    lv_obj_set_style_text_font(scr, &lv_font_montserrat_20, 0);
    lv_obj_align(label_stage, LV_ALIGN_TOP_MID, 0, 6);

    label_mood = lv_label_create(scr);
    lv_obj_align(label_mood, LV_ALIGN_TOP_MID, 0, 30);

    label_age = lv_label_create(scr);
    lv_obj_align(label_age, LV_ALIGN_TOP_RIGHT, -8, 6);

    // --- Face (circle body + eyes + mouth arc) ---
    face_body = lv_obj_create(scr);
    lv_obj_set_size(face_body, 130, 130);
    lv_obj_align(face_body, LV_ALIGN_TOP_MID, 0, 60);
    lv_obj_set_style_radius(face_body, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_border_width(face_body, 0, 0);
    lv_obj_clear_flag(face_body, LV_OBJ_FLAG_SCROLLABLE);

    eye_l = lv_obj_create(face_body);
    lv_obj_set_size(eye_l, 14, 14);
    lv_obj_set_style_radius(eye_l, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_color(eye_l, lv_color_black(), 0);
    lv_obj_align(eye_l, LV_ALIGN_CENTER, -28, -15);

    eye_r = lv_obj_create(face_body);
    lv_obj_set_size(eye_r, 14, 14);
    lv_obj_set_style_radius(eye_r, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_color(eye_r, lv_color_black(), 0);
    lv_obj_align(eye_r, LV_ALIGN_CENTER, 28, -15);

    mouth = lv_arc_create(face_body);
    lv_obj_set_size(mouth, 60, 60);
    lv_arc_set_bg_angles(mouth, 20, 160); // smile arc by default
    lv_arc_set_value(mouth, 0);
    lv_obj_remove_style(mouth, NULL, LV_PART_KNOB);
    lv_obj_set_style_arc_width(mouth, 5, LV_PART_MAIN);
    lv_obj_set_style_arc_color(mouth, lv_color_black(), LV_PART_MAIN);
    lv_obj_align(mouth, LV_ALIGN_CENTER, 0, 20);
    lv_obj_clear_flag(mouth, LV_OBJ_FLAG_CLICKABLE);

    // --- Stat bars ---
    int y = 205;
    bar_hunger  = make_stat_bar(scr, "Food",  lv_palette_main(LV_PALETTE_ORANGE), y);
    bar_happy   = make_stat_bar(scr, "Mood",  lv_palette_main(LV_PALETTE_PINK), y + 20);
    bar_energy  = make_stat_bar(scr, "Rest",  lv_palette_main(LV_PALETTE_BLUE), y + 40);
    bar_hygiene = make_stat_bar(scr, "Clean", lv_palette_main(LV_PALETTE_CYAN), y + 60);

    // --- Buttons ---
    lv_obj_t *row = lv_obj_create(scr);
    lv_obj_remove_style(row, NULL, LV_PART_SCROLLBAR);
    lv_obj_set_size(row, 240, 56);
    lv_obj_align(row, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_flex_flow(row, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(row, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_bg_color(row, lv_color_hex(0xDCE9FA), 0);
    lv_obj_clear_flag(row, LV_OBJ_FLAG_SCROLLABLE);

    make_button(row, "Feed", feed_cb); // no built-in LVGL "food" symbol; plain text instead
    make_button(row, LV_SYMBOL_PLAY, play_cb);
    make_button(row, LV_SYMBOL_TINT, clean_cb);
    lv_obj_t *sleepBtn = make_button(row, LV_SYMBOL_EYE_CLOSE, sleep_cb);
    btn_sleep_label = lv_obj_get_child(sleepBtn, 0);
    make_button(row, LV_SYMBOL_CHARGE, medicine_cb);
}

void refresh() {
    if (!s_pet) return;
    const PetState &s = s_pet->state();
    PetMood mood = s_pet->mood();

    lv_label_set_text_fmt(label_stage, "%s", s_pet->stageName());
    lv_label_set_text_fmt(label_mood, "Feeling: %s%s", s_pet->moodName(), s.asleep ? " (asleep)" : "");

    uint32_t mins = s.ageSeconds / 60;
    lv_label_set_text_fmt(label_age, "%lu min", (unsigned long)mins);

    lv_bar_set_value(bar_hunger, s.hunger, LV_ANIM_ON);
    lv_bar_set_value(bar_happy, s.happiness, LV_ANIM_ON);
    lv_bar_set_value(bar_energy, s.energy, LV_ANIM_ON);
    lv_bar_set_value(bar_hygiene, s.hygiene, LV_ANIM_ON);

    lv_obj_set_style_bg_color(face_body, mood_color(mood), 0);

    // Eyes: closed (thin) while asleep, open otherwise.
    lv_obj_set_size(eye_l, 14, s.asleep ? 3 : 14);
    lv_obj_set_size(eye_r, 14, s.asleep ? 3 : 14);

    // Mouth: smile when happy, flat/frown otherwise.
    if (mood == PetMood::Great) {
        lv_arc_set_bg_angles(mouth, 20, 160);
    } else if (mood == PetMood::Okay) {
        lv_arc_set_bg_angles(mouth, 0, 180);
    } else {
        lv_arc_set_bg_angles(mouth, 200, 340); // frown
    }

    lv_label_set_text(btn_sleep_label, s.asleep ? LV_SYMBOL_EYE_OPEN : LV_SYMBOL_EYE_CLOSE);

    if (s.stage == PetStage::Egg) {
        lv_obj_set_style_bg_color(face_body, lv_color_hex(0xF5F5DC), 0);
    }
}

} // namespace ui
