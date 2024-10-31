set(COMPONENT_SRCS "ui_app.c"
                   "squareline/screens/ui_Welcome_screen.c"
                   "squareline/screens/ui_More_options_screen.c"
                   "squareline/screens/ui_Function_generator_choice1_screen.c"
                   "squareline/screens/ui_Function_generator_choice2_screen.c"
                   "squareline/screens/ui_Function_generator_display.c"
                   "squareline/screens/ui_Oscilloscope_display.c"
                   "squareline/screens/ui_Preset_load.c"
                   "squareline/screens/ui_Preset_save.c"
                   "squareline/screens/ui_Overheat_screen.c"
                   "squareline/screens/ui_Temp_and_humididty_history_screen.c"
                   "squareline/images/ui_img_450846187.c"
                   "squareline/screens/ui_Screenshot_screen.c"
                   "squareline/ui.c"
                   "squareline/ui_helpers.c"
                   "squareline/ui_events.c"
)

set(COMPONENT_ADD_INCLUDEDIRS "" "." "squareline")
set(COMPONENT_PRIV_REQUIRES lvgl lvgl_esp32_drivers waveform_generator led oscilloscope user_interface)

register_component()