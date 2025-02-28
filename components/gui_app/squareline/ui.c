// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.4.2
// LVGL version: 8.3.6
// Project name: SquareLine_Final_Project

#include "ui.h"
#include "ui_helpers.h"

///////////////////// VARIABLES ////////////////////


// SCREEN: ui_Welcome_screen
void ui_Welcome_screen_screen_init(void);
lv_obj_t * ui_Welcome_screen;
void ui_event_Function_generator_button(lv_event_t * e);
lv_obj_t * ui_Function_generator_button;
lv_obj_t * ui_Function_generator_label;
void ui_event_Osciloscope_button(lv_event_t * e);
lv_obj_t * ui_Osciloscope_button;
lv_obj_t * ui_Oscilocope_label;
void ui_event_More_options_button(lv_event_t * e);
lv_obj_t * ui_More_options_button;
lv_obj_t * ui_Split_screen_label;
lv_obj_t * ui_Label3;
lv_obj_t * ui_TemperatureLabel1;
lv_obj_t * ui_HumidityLabel1;


// SCREEN: ui_More_options_screen
void ui_More_options_screen_screen_init(void);
lv_obj_t * ui_More_options_screen;
lv_obj_t * ui_TemperatureLabel4;
lv_obj_t * ui_HumidityLabel4;
void ui_event_Button6(lv_event_t * e);
lv_obj_t * ui_Button6;
lv_obj_t * ui_Label15;
void ui_event_Look_at_screenshot_button(lv_event_t * e);
lv_obj_t * ui_Look_at_screenshot_button;
lv_obj_t * ui_See_last_screenshot_label;
void ui_event_Temp_and_humidity_button(lv_event_t * e);
lv_obj_t * ui_Temp_and_humidity_button;
lv_obj_t * ui_Temp_humidity_label;


// SCREEN: ui_Function_generator_choice1_screen
void ui_Function_generator_choice1_screen_screen_init(void);
lv_obj_t * ui_Function_generator_choice1_screen;
void ui_event_Sinus_button(lv_event_t * e);
lv_obj_t * ui_Sinus_button;
lv_obj_t * ui_Label11;
void ui_event_Squarewave_button(lv_event_t * e);
lv_obj_t * ui_Squarewave_button;
lv_obj_t * ui_Label12;
lv_obj_t * ui_Label8;
void ui_event_Sawtooth_button(lv_event_t * e);
lv_obj_t * ui_Sawtooth_button;
lv_obj_t * ui_Label9;
void ui_event_Triangle_button(lv_event_t * e);
lv_obj_t * ui_Triangle_button;
lv_obj_t * ui_Label10;
void ui_event_Button4(lv_event_t * e);
lv_obj_t * ui_Button4;
lv_obj_t * ui_Label13;
void ui_event_Button7(lv_event_t * e);
lv_obj_t * ui_Button7;
lv_obj_t * ui_Label16;


// SCREEN: ui_Preset_load
void ui_Preset_load_screen_init(void);
lv_obj_t * ui_Preset_load;
void ui_event_Button9(lv_event_t * e);
lv_obj_t * ui_Button9;
lv_obj_t * ui_Label23;
void ui_event_Preset_button_1(lv_event_t * e);
lv_obj_t * ui_Preset_button_1;
lv_obj_t * ui_Labeldc1;
lv_obj_t * ui_Labelf1;
lv_obj_t * ui_Labela1;
lv_obj_t * ui_Labelwf1;
lv_obj_t * ui_Label28;
void ui_event_Preset_button_2(lv_event_t * e);
lv_obj_t * ui_Preset_button_2;
lv_obj_t * ui_Labeldc3;
lv_obj_t * ui_Labelf3;
lv_obj_t * ui_Labela3;
lv_obj_t * ui_Labelwf3;
void ui_event_Preset_button_3(lv_event_t * e);
lv_obj_t * ui_Preset_button_3;
lv_obj_t * ui_Labeldc4;
lv_obj_t * ui_Labelf4;
lv_obj_t * ui_Labela4;
lv_obj_t * ui_Labelwf4;
void ui_event_Preset_button_4(lv_event_t * e);
lv_obj_t * ui_Preset_button_4;
lv_obj_t * ui_Labeldc5;
lv_obj_t * ui_Labelf5;
lv_obj_t * ui_Labela5;
lv_obj_t * ui_Labelwf5;


// SCREEN: ui_Function_generator_choice2_screen
void ui_Function_generator_choice2_screen_screen_init(void);
void ui_event_Function_generator_choice2_screen(lv_event_t * e);
lv_obj_t * ui_Function_generator_choice2_screen;
void ui_event_Slider_frequency(lv_event_t * e);
lv_obj_t * ui_Slider_frequency;
void ui_event_Slider_amplitude(lv_event_t * e);
lv_obj_t * ui_Slider_amplitude;
void ui_event_Slider_duty_cycle(lv_event_t * e);
lv_obj_t * ui_Slider_duty_cycle;
lv_obj_t * ui_Frequency_bar_label;
lv_obj_t * ui_Amplitude_bar_label;
lv_obj_t * ui_Duty_cycle_bar_label;
lv_obj_t * ui_Label17;
lv_obj_t * ui_Label18;
lv_obj_t * ui_Label19;
void ui_event_Generate_button(lv_event_t * e);
lv_obj_t * ui_Generate_button;
lv_obj_t * ui_Label20;
void ui_event_Button3(lv_event_t * e);
lv_obj_t * ui_Button3;
lv_obj_t * ui_Label4;


// SCREEN: ui_Function_generator_display
void ui_Function_generator_display_screen_init(void);
void ui_event_Function_generator_display(lv_event_t * e);
lv_obj_t * ui_Function_generator_display;
lv_obj_t * ui_Chart1;
lv_obj_t * ui_TemperatureLabel2;
lv_obj_t * ui_HumidityLabel2;
void ui_event_Switch2(lv_event_t * e);
lv_obj_t * ui_Switch2;
lv_obj_t * ui_Label21;
void ui_event_Button10(lv_event_t * e);
lv_obj_t * ui_Button10;
lv_obj_t * ui_Label24;
void ui_event_Button2(lv_event_t * e);
lv_obj_t * ui_Button2;
lv_obj_t * ui_Label2;
lv_obj_t * ui_Label25;
lv_obj_t * ui_Label26;
void ui_event_Button8(lv_event_t * e);
lv_obj_t * ui_Button8;
lv_obj_t * ui_Label22;
lv_obj_t * ui_Label51;
lv_obj_t * ui_Label52;
lv_obj_t * ui_Label53;
lv_obj_t * ui_Label54;
lv_obj_t * ui_Label55;


// SCREEN: ui_Oscilloscope_display
void ui_Oscilloscope_display_screen_init(void);
lv_obj_t * ui_Oscilloscope_display;
lv_obj_t * ui_OscilloscopeChart;
lv_obj_t * ui_TemperatureLabel3;
lv_obj_t * ui_HumidityLabel3;
void ui_event_Button5(lv_event_t * e);
lv_obj_t * ui_Button5;
lv_obj_t * ui_Label14;
void ui_event_Button13(lv_event_t * e);
lv_obj_t * ui_Button13;
lv_obj_t * ui_Label31;
lv_obj_t * ui_Label41;
lv_obj_t * ui_Label42;
lv_obj_t * ui_Label43;
lv_obj_t * ui_Label44;
lv_obj_t * ui_Label45;
void ui_event_CH1minus_button(lv_event_t * e);
lv_obj_t * ui_CH1minus_button;
lv_obj_t * ui_Label47;
void ui_event_CH1plus_button(lv_event_t * e);
lv_obj_t * ui_CH1plus_button;
lv_obj_t * ui_Label48;
lv_obj_t * ui_CH1msdiv_label;
lv_obj_t * ui_CH2msdiv_label;
void ui_event_CH2minus_button(lv_event_t * e);
lv_obj_t * ui_CH2minus_button;
lv_obj_t * ui_Label46;
void ui_event_CH2plus_button(lv_event_t * e);
lv_obj_t * ui_CH2plus_button;
lv_obj_t * ui_Label49;
lv_obj_t * ui_Ch1Vpp;
lv_obj_t * ui_Ch2Vpp;


// SCREEN: ui_Preset_save
void ui_Preset_save_screen_init(void);
lv_obj_t * ui_Preset_save;
void ui_event_Button11(lv_event_t * e);
lv_obj_t * ui_Button11;
lv_obj_t * ui_Label27;
void ui_event_Preset_button_6(lv_event_t * e);
lv_obj_t * ui_Preset_button_6;
lv_obj_t * ui_Labeldc6;
lv_obj_t * ui_Labelf6;
lv_obj_t * ui_Labela6;
lv_obj_t * ui_Labelwf6;
void ui_event_Preset_button_7(lv_event_t * e);
lv_obj_t * ui_Preset_button_7;
lv_obj_t * ui_Labeldc7;
lv_obj_t * ui_Labelf7;
lv_obj_t * ui_Labela7;
lv_obj_t * ui_Labelwf7;
void ui_event_Preset_button_8(lv_event_t * e);
lv_obj_t * ui_Preset_button_8;
lv_obj_t * ui_Labeldc8;
lv_obj_t * ui_Labelf8;
lv_obj_t * ui_Labela8;
lv_obj_t * ui_Labelwf8;
void ui_event_Preset_button_9(lv_event_t * e);
lv_obj_t * ui_Preset_button_9;
lv_obj_t * ui_Labeldc9;
lv_obj_t * ui_Labelf9;
lv_obj_t * ui_Labela9;
lv_obj_t * ui_Labelwf9;
lv_obj_t * ui_Label29;


// SCREEN: ui_Overheat_screen
void ui_Overheat_screen_screen_init(void);
lv_obj_t * ui_Overheat_screen;
lv_obj_t * ui_Overheat_fire_picture;
lv_obj_t * ui_Label5;
lv_obj_t * ui_Label6;
lv_obj_t * ui_Label7;


// SCREEN: ui_Screenshot_screen
void ui_Screenshot_screen_screen_init(void);
void ui_event_Screenshot_screen(lv_event_t * e);
lv_obj_t * ui_Screenshot_screen;
void ui_event_Button12(lv_event_t * e);
lv_obj_t * ui_Button12;
void ui_event_Label30(lv_event_t * e);
lv_obj_t * ui_Label30;
lv_obj_t * ui_OscilloscopeChart2;


// SCREEN: ui_Temp_and_humididty_history_screen
void ui_Temp_and_humididty_history_screen_screen_init(void);
void ui_event_Temp_and_humididty_history_screen(lv_event_t * e);
lv_obj_t * ui_Temp_and_humididty_history_screen;
void ui_event_Button14(lv_event_t * e);
lv_obj_t * ui_Button14;
lv_obj_t * ui_Label32;
lv_obj_t * ui_Temperature_chart;
lv_obj_t * ui_Label34;
lv_obj_t * ui_Label1;
lv_obj_t * ui_Label33;
lv_obj_t * ui_Humidity_chart;
lv_obj_t * ui_Label35;
lv_obj_t * ui_Label36;
lv_obj_t * ui_Label37;
lv_obj_t * ui_Label38;
lv_obj_t * ui_Label39;
lv_obj_t * ui_Label40;
lv_obj_t * ui____initial_actions0;

///////////////////// TEST LVGL SETTINGS ////////////////////
#if LV_COLOR_DEPTH != 16
    #error "LV_COLOR_DEPTH should be 16bit to match SquareLine Studio's settings"
#endif
#if LV_COLOR_16_SWAP !=1
    #error "LV_COLOR_16_SWAP should be 1 to match SquareLine Studio's settings"
#endif

///////////////////// ANIMATIONS ////////////////////

///////////////////// FUNCTIONS ////////////////////
void ui_event_Function_generator_button(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        _ui_screen_change(&ui_Function_generator_choice1_screen, LV_SCR_LOAD_ANIM_MOVE_LEFT, 500, 0,
                          &ui_Function_generator_choice1_screen_screen_init);
    }
}
void ui_event_Osciloscope_button(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        _ui_screen_change(&ui_Oscilloscope_display, LV_SCR_LOAD_ANIM_MOVE_LEFT, 500, 0, &ui_Oscilloscope_display_screen_init);
        lv_start_oscilloscope(e);
    }
}
void ui_event_More_options_button(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        _ui_screen_change(&ui_More_options_screen, LV_SCR_LOAD_ANIM_MOVE_TOP, 500, 0, &ui_More_options_screen_screen_init);
    }
}
void ui_event_Button6(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        _ui_screen_change(&ui_Welcome_screen, LV_SCR_LOAD_ANIM_MOVE_BOTTOM, 500, 0, &ui_Welcome_screen_screen_init);
    }
}
void ui_event_Look_at_screenshot_button(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        _ui_screen_change(&ui_Screenshot_screen, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_Screenshot_screen_screen_init);
    }
}
void ui_event_Temp_and_humidity_button(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        _ui_screen_change(&ui_Temp_and_humididty_history_screen, LV_SCR_LOAD_ANIM_MOVE_LEFT, 500, 0,
                          &ui_Temp_and_humididty_history_screen_screen_init);
    }
}
void ui_event_Sinus_button(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        _ui_screen_change(&ui_Function_generator_choice2_screen, LV_SCR_LOAD_ANIM_MOVE_LEFT, 500, 0,
                          &ui_Function_generator_choice2_screen_screen_init);
        setWaveformSine(e);
    }
}
void ui_event_Squarewave_button(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        _ui_screen_change(&ui_Function_generator_choice2_screen, LV_SCR_LOAD_ANIM_MOVE_LEFT, 500, 0,
                          &ui_Function_generator_choice2_screen_screen_init);
        setWaveformSquarewave(e);
    }
}
void ui_event_Sawtooth_button(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        _ui_screen_change(&ui_Function_generator_choice2_screen, LV_SCR_LOAD_ANIM_MOVE_LEFT, 500, 0,
                          &ui_Function_generator_choice2_screen_screen_init);
        setWaveformSawtooth(e);
    }
}
void ui_event_Triangle_button(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        _ui_screen_change(&ui_Function_generator_choice2_screen, LV_SCR_LOAD_ANIM_MOVE_LEFT, 500, 0,
                          &ui_Function_generator_choice2_screen_screen_init);
        setWaveformTriangle(e);
    }
}
void ui_event_Button4(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        _ui_screen_change(&ui_Welcome_screen, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 500, 0, &ui_Welcome_screen_screen_init);
    }
}
void ui_event_Button7(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        _ui_screen_change(&ui_Preset_load, LV_SCR_LOAD_ANIM_MOVE_BOTTOM, 500, 0, &ui_Preset_load_screen_init);
        displayPresets1(e);
    }
}
void ui_event_Button9(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        _ui_screen_change(&ui_Function_generator_choice1_screen, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 500, 0,
                          &ui_Function_generator_choice1_screen_screen_init);
    }
}
void ui_event_Preset_button_1(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        loadPreset1(e);
        _ui_screen_change(&ui_Function_generator_choice2_screen, LV_SCR_LOAD_ANIM_MOVE_TOP, 500, 0,
                          &ui_Function_generator_choice2_screen_screen_init);
    }
}
void ui_event_Preset_button_2(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        loadPreset2(e);
        _ui_screen_change(&ui_Function_generator_choice2_screen, LV_SCR_LOAD_ANIM_MOVE_TOP, 500, 0,
                          &ui_Function_generator_choice2_screen_screen_init);
    }
}
void ui_event_Preset_button_3(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        loadPreset3(e);
        _ui_screen_change(&ui_Function_generator_choice2_screen, LV_SCR_LOAD_ANIM_MOVE_TOP, 500, 0,
                          &ui_Function_generator_choice2_screen_screen_init);
    }
}
void ui_event_Preset_button_4(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        loadPreset4(e);
        _ui_screen_change(&ui_Function_generator_choice2_screen, LV_SCR_LOAD_ANIM_MOVE_TOP, 500, 0,
                          &ui_Function_generator_choice2_screen_screen_init);
    }
}
void ui_event_Function_generator_choice2_screen(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_SCREEN_LOADED) {
        setSliders(e);
    }
}
void ui_event_Slider_frequency(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_VALUE_CHANGED) {
        _ui_slider_set_text_value(ui_Frequency_bar_label, target, "", " Hz");
    }
    if(event_code == LV_EVENT_SCREEN_LOADED) {
        _ui_slider_set_text_value(ui_Frequency_bar_label, target, "", " Hz");
    }
}
void ui_event_Slider_amplitude(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_VALUE_CHANGED) {
        _ui_slider_set_text_value(ui_Amplitude_bar_label, target, "", " mV");
    }
}
void ui_event_Slider_duty_cycle(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_VALUE_CHANGED) {
        _ui_slider_set_text_value(ui_Duty_cycle_bar_label, target, "", " %");
    }
}
void ui_event_Generate_button(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        _ui_screen_change(&ui_Function_generator_display, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0,
                          &ui_Function_generator_display_screen_init);
        startGenerating(e);
    }
}
void ui_event_Button3(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        _ui_screen_change(&ui_Function_generator_choice1_screen, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 500, 0,
                          &ui_Function_generator_choice1_screen_screen_init);
    }
}
void ui_event_Function_generator_display(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_SCREEN_LOADED) {
        drawWaveform(e);
    }
}
void ui_event_Switch2(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_VALUE_CHANGED &&  lv_obj_has_state(target, LV_STATE_CHECKED)) {
        pauseWavegen(e);
    }
    if(event_code == LV_EVENT_VALUE_CHANGED &&  !lv_obj_has_state(target, LV_STATE_CHECKED)) {
        restartWavegen(e);
    }
}
void ui_event_Button10(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        pauseWavegen(e);
        _ui_screen_change(&ui_Function_generator_choice2_screen, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 500, 0,
                          &ui_Function_generator_choice2_screen_screen_init);
    }
}
void ui_event_Button2(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        pauseWavegen(e);
        _ui_screen_change(&ui_Welcome_screen, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 500, 0, &ui_Welcome_screen_screen_init);
    }
}
void ui_event_Button8(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        _ui_screen_change(&ui_Preset_save, LV_SCR_LOAD_ANIM_MOVE_LEFT, 500, 0, &ui_Preset_save_screen_init);
        DisplayPresets2(e);
    }
}
void ui_event_Button5(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        _ui_screen_change(&ui_Welcome_screen, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 500, 0, &ui_Welcome_screen_screen_init);
        lv_stop_oscilloscope(e);
    }
}
void ui_event_Button13(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        takeScreenshot(e);
    }
}
void ui_event_CH1minus_button(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        CH1ZoomOut(e);
    }
}
void ui_event_CH1plus_button(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        CH1ZoomIn(e);
    }
}
void ui_event_CH2minus_button(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        CH2ZoomOut(e);
    }
}
void ui_event_CH2plus_button(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        CH2ZoomIn(e);
    }
}
void ui_event_Button11(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        _ui_screen_change(&ui_Function_generator_display, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 500, 0,
                          &ui_Function_generator_display_screen_init);
        startGenerating(e);
    }
}
void ui_event_Preset_button_6(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        _ui_screen_change(&ui_Function_generator_display, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 500, 0,
                          &ui_Function_generator_display_screen_init);
        savePreset1(e);
    }
}
void ui_event_Preset_button_7(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        _ui_screen_change(&ui_Function_generator_display, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 500, 0,
                          &ui_Function_generator_display_screen_init);
        savePreset2(e);
    }
}
void ui_event_Preset_button_8(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        _ui_screen_change(&ui_Function_generator_display, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 500, 0,
                          &ui_Function_generator_display_screen_init);
        savePreset3(e);
    }
}
void ui_event_Preset_button_9(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        _ui_screen_change(&ui_Function_generator_display, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 500, 0,
                          &ui_Function_generator_display_screen_init);
        savePreset4(e);
    }
}
void ui_event_Screenshot_screen(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_SCREEN_LOADED) {
        drawScreenshot(e);
    }
}
void ui_event_Button12(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        _ui_screen_change(&ui_Welcome_screen, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_Welcome_screen_screen_init);
    }
}
void ui_event_Label30(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        _ui_screen_change(&ui_Welcome_screen, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_Welcome_screen_screen_init);
    }
}
void ui_event_Temp_and_humididty_history_screen(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_SCREEN_LOADED) {
        showTempAndHumidityHistory(e);
    }
}
void ui_event_Button14(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        _ui_screen_change(&ui_More_options_screen, LV_SCR_LOAD_ANIM_MOVE_BOTTOM, 500, 0, &ui_More_options_screen_screen_init);
    }
}

///////////////////// SCREENS ////////////////////

void ui_init(void)
{
    lv_disp_t * dispp = lv_disp_get_default();
    lv_theme_t * theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
                                               true, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    ui_Welcome_screen_screen_init();
    ui_More_options_screen_screen_init();
    ui_Function_generator_choice1_screen_screen_init();
    ui_Preset_load_screen_init();
    ui_Function_generator_choice2_screen_screen_init();
    ui_Function_generator_display_screen_init();
    ui_Oscilloscope_display_screen_init();
    ui_Preset_save_screen_init();
    ui_Overheat_screen_screen_init();
    ui_Screenshot_screen_screen_init();
    ui_Temp_and_humididty_history_screen_screen_init();
    ui____initial_actions0 = lv_obj_create(NULL);
    lv_disp_load_scr(ui_Welcome_screen);
}
