set(COMPONENT_SRCS "oscilloscope.c")
set(COMPONENT_ADD_INCLUDEDIRS ".")
set(COMPONENT_REQUIRES esp_timer adc gui_app led lvgl)

register_component()