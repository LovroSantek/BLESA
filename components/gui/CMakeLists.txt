set(COMPONENT_SRCS "gui.c")
set(COMPONENT_ADD_INCLUDEDIRS ".")
set(COMPONENT_REQUIRES esp_timer lvgl lvgl_esp32_drivers gui_app)

register_component()