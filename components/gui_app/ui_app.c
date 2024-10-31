/**
 * @file ui_app.c
 *
 * @brief This file runs an UI application depending on a Kconfig parameter
 *
 * COPYRIGHT NOTICE: (c) 2023 Byte Lab Grupa d.o.o.
 * All rights reserved.
 */

//--------------------------------- INCLUDES ----------------------------------
#include "ui_app.h"
#include "squareline/ui.h"
#include "ui.h"

#include <stdio.h>
//---------------------------------- MACROS -----------------------------------

//-------------------------------- DATA TYPES ---------------------------------

//---------------------- PRIVATE FUNCTION PROTOTYPES --------------------------
/**
 * @brief This function returns a pressed key with enum.
 *
 * @return A key which was pressed.
 */
static uint8_t _keypad_get_key(void);

/**
 * @brief The function reads the state of the keypad and returns the key pressed.
 *
 * @param [in] p_indev_drv The driver descriptor.
 * @param [in] p_data This is the data structure that will be passed to the callback function.
 */
static void _keypad_read(lv_indev_drv_t *p_indev_drv, lv_indev_data_t *p_data);

/**
 * @brief Switch the active input device group based on the current active screen.
 * 
 * This function sets the input device group according to which screen is currently active.
 * It allows navigation between different UI elements on different screens.
 */
static void _switch_to_active_screen_group(void);

/**
 * @brief Creates groups for all screens and adds buttons to corresponding group.
 * 
 */
static void _create_groups_and_add_objects(void);

//------------------------- STATIC DATA & CONSTANTS ---------------------------
static lv_indev_t *p_kb_indev = NULL;

static lv_group_t *p_group_welcome_screen = NULL;
static lv_group_t *p_group_fg1_screen = NULL;
static lv_group_t *p_group_fg2_screen = NULL;
static lv_group_t *p_group_display_screen = NULL;
static lv_group_t *p_group_Oscilloscope_display = NULL;
static lv_group_t *p_group_more_options_screen = NULL;
static lv_group_t *p_group_load_presets_screen = NULL;
static lv_group_t *p_group_save_presets_screen = NULL;
static lv_group_t *p_group_temp_hum_history_screen = NULL;

//------------------------------- GLOBAL DATA ---------------------------------
uint8_t last_pressed = 0;
//------------------------------ PUBLIC FUNCTIONS -----------------------------
void ui_app_init(void)
{
    ui_init();

    _create_groups_and_add_objects();
     
    /* Initialize input device */
    static lv_indev_drv_t kb_drv;
    lv_indev_drv_init(&kb_drv);
    kb_drv.type            = LV_INDEV_TYPE_KEYPAD;
    kb_drv.read_cb         = _keypad_read;
    p_kb_indev = lv_indev_drv_register(&kb_drv);
    lv_indev_set_group(p_kb_indev, p_group_welcome_screen);
}

void set_last_pressed_button(uint8_t button)
{
    last_pressed = button;
}

//---------------------------- PRIVATE FUNCTIONS ------------------------------
static uint8_t _keypad_get_key(void)
{
    uint8_t return_value = last_pressed;
    last_pressed = 0;
    return return_value;
}

static void _keypad_read(lv_indev_drv_t *p_indev_drv, lv_indev_data_t *p_data)
{
    _switch_to_active_screen_group();
    static uint32_t last_key = 0;

    /* Get whether a key is pressed and save the pressed key*/
    uint32_t act_key = _keypad_get_key();
    if(act_key != 0)
    {
        p_data->state = LV_INDEV_STATE_PR;

        /*Translate the keys to LVGL control characters according to your key definitions*/
        switch(act_key)
        {
            case 1:
                act_key = LV_KEY_PREV;
                break;
            case 2:
                act_key = LV_KEY_ENTER;
                break;
            case 3:
                act_key = LV_KEY_NEXT;
                break;
        }
        last_key = act_key;
    }
    else
    {
        p_data->state = LV_INDEV_STATE_REL;
    }

    p_data->key = last_key;
}

static void _switch_to_active_screen_group(void)
{
    lv_obj_t *current_screen = lv_scr_act();

    if (current_screen == ui_Welcome_screen)
    {
        lv_indev_set_group(p_kb_indev, p_group_welcome_screen);
    } 
    else if (current_screen == ui_Function_generator_choice1_screen)
    {
        lv_indev_set_group(p_kb_indev, p_group_fg1_screen);
    }
    else if (current_screen == ui_Function_generator_choice2_screen)
    {
        lv_indev_set_group(p_kb_indev, p_group_fg2_screen);
    }
    else if (current_screen == ui_Function_generator_display)
    {
        lv_indev_set_group(p_kb_indev, p_group_display_screen);
    }
    else if (current_screen == ui_Oscilloscope_display)
    {
        lv_indev_set_group(p_kb_indev, p_group_Oscilloscope_display);
    }
    else if (current_screen == ui_More_options_screen)
    {
        lv_indev_set_group(p_kb_indev, p_group_more_options_screen);
    }
    else if (current_screen == ui_Preset_load)
    {
        lv_indev_set_group(p_kb_indev, p_group_load_presets_screen);
    }
    else if (current_screen == ui_Preset_save)
    {
        lv_indev_set_group(p_kb_indev, p_group_save_presets_screen);
    }
    else if (current_screen == ui_Temp_and_humididty_history_screen)
    {
        lv_indev_set_group(p_kb_indev, p_group_temp_hum_history_screen);
    }
    else
    {
        lv_indev_set_group(p_kb_indev, p_group_welcome_screen);
    }
}

static void _create_groups_and_add_objects(void)
{
    p_group_welcome_screen = lv_group_create();
    p_group_fg1_screen = lv_group_create();
    p_group_fg2_screen = lv_group_create();
    p_group_display_screen = lv_group_create();
    p_group_Oscilloscope_display = lv_group_create();
    p_group_more_options_screen = lv_group_create();
    p_group_load_presets_screen = lv_group_create();
    p_group_save_presets_screen = lv_group_create();
    p_group_temp_hum_history_screen = lv_group_create();


    lv_group_add_obj(p_group_welcome_screen, ui_Function_generator_button);
    lv_group_add_obj(p_group_welcome_screen, ui_Osciloscope_button);
    lv_group_add_obj(p_group_welcome_screen, ui_More_options_button);

    lv_group_add_obj(p_group_fg1_screen, ui_Sinus_button);
    lv_group_add_obj(p_group_fg1_screen, ui_Button7);
    lv_group_add_obj(p_group_fg1_screen, ui_Button4);
    lv_group_add_obj(p_group_fg1_screen, ui_Sawtooth_button);
    lv_group_add_obj(p_group_fg1_screen, ui_Triangle_button);
    lv_group_add_obj(p_group_fg1_screen, ui_Squarewave_button);

    lv_group_add_obj(p_group_fg2_screen, ui_Generate_button);
    lv_group_add_obj(p_group_fg2_screen, ui_Button3);

    lv_group_add_obj(p_group_display_screen, ui_Button10);
    lv_group_add_obj(p_group_display_screen, ui_Switch2);
    lv_group_add_obj(p_group_display_screen, ui_Button8);
    lv_group_add_obj(p_group_display_screen, ui_Button2);

    lv_group_add_obj(p_group_Oscilloscope_display, ui_Button5);
    lv_group_add_obj(p_group_Oscilloscope_display,ui_Button13);
    lv_group_add_obj(p_group_Oscilloscope_display,ui_CH2plus_button);
    lv_group_add_obj(p_group_Oscilloscope_display,ui_CH2minus_button);
    lv_group_add_obj(p_group_Oscilloscope_display,ui_CH1plus_button);
    lv_group_add_obj(p_group_Oscilloscope_display,ui_CH1minus_button);

    lv_group_add_obj(p_group_more_options_screen, ui_Temp_and_humidity_button);
    lv_group_add_obj(p_group_more_options_screen, ui_Look_at_screenshot_button);
    lv_group_add_obj(p_group_more_options_screen, ui_Button6);

    lv_group_add_obj(p_group_load_presets_screen, ui_Preset_button_1);
    lv_group_add_obj(p_group_load_presets_screen, ui_Button9);
    lv_group_add_obj(p_group_load_presets_screen, ui_Preset_button_4);
    lv_group_add_obj(p_group_load_presets_screen, ui_Preset_button_3);
    lv_group_add_obj(p_group_load_presets_screen, ui_Preset_button_2);

    lv_group_add_obj(p_group_save_presets_screen, ui_Preset_button_6);
    lv_group_add_obj(p_group_save_presets_screen, ui_Button11);
    lv_group_add_obj(p_group_save_presets_screen, ui_Preset_button_9);
    lv_group_add_obj(p_group_save_presets_screen, ui_Preset_button_8);
    lv_group_add_obj(p_group_save_presets_screen, ui_Preset_button_7);

    lv_group_add_obj(p_group_temp_hum_history_screen, ui_Button14);
}

//---------------------------- INTERRUPT HANDLERS -----------------------------
