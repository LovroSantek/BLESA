set(COMPONENT_SRCS "app_main.c")
set(COMPONENT_ADD_INCLUDEDIRS "")
set(COMPONENT_REQUIRES driver esp_event button led user_interface i2c temp_humidity wifi gui adc)

register_component()
