#pragma once
#include "pet.h"

// Builds and refreshes the LVGL screen for the virtual pet.
namespace ui {

void build(Pet *pet);   // call once after lv_init() + display/indev registration
void refresh();          // call every loop to sync widgets with pet state

} // namespace ui
