/**
 * @file ui_app.c
 *
 * @brief See the source file.
 *
 * COPYRIGHT NOTICE: (c) 2023 Byte Lab Grupa d.o.o.
 * All rights reserved.
 */

#ifndef __UI_APP_C__
#define __UI_APP_C__

#ifdef __cplusplus
extern "C" {
#endif

//--------------------------------- INCLUDES ----------------------------------
#include <stdio.h>

//---------------------------------- MACROS -----------------------------------

//-------------------------------- DATA TYPES ---------------------------------

//---------------------- PUBLIC FUNCTION PROTOTYPES --------------------------
/**
 * @brief Initialize the user interface (UI) application.
 *
 * This function initializes the UI by calling `ui_init()` and creates
 * object groups for managing inputs. It also sets up an input device
 * of type keypad and registers the callback for reading keypad input.
 * The input device is associated with the welcome screen group for
 * navigation using external buttons.
 */
void ui_app_init(void);

/**
 * @brief Set the last pressed button.
 *
 * This function stores the last button that was pressed by updating
 * the `last_pressed` variable. This can be used for tracking button 
 * presses in the user interface for navigation or other purposes.
 *
 * @param button The ID of the button that was pressed.
 */
void set_last_pressed_button(uint8_t button);

#ifdef __cplusplus
}
#endif

#endif // __UI_APP_C__
