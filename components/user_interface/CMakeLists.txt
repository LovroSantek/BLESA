set(COMPONENT_SRCS "user_interface.c")
set(COMPONENT_ADD_INCLUDEDIRS ".")
set(COMPONENT_REQUIRES driver led button waveform_generator i2c temp_humidity wifi gui_app lvgl adc oscilloscope my_mqtt)

register_component()