/**
* @file app_main.c

* @brief 

* @par
*
* COPYRIGHT NOTICE: (c) 2022 Byte Lab Grupa d.o.o.
* All rights reserved.
*/

//--------------------------------- INCLUDES ----------------------------------
#include <stdio.h>
#include "user_interface.h"
#include "waveform_generator.h"
#include "temp_humidity.h"
#include "gui.h"

#include "blesa_wifi.h"  // temproary for testing, delete afterwards

//---------------------------------- MACROS -----------------------------------
#define TEST_FREQUENCY  (10000U)
#define TEST_AMPLITUDE  (2000U)
#define TEST_DUTY_CYCLE (50U)

//-------------------------------- DATA TYPES ---------------------------------

//---------------------- PRIVATE FUNCTION PROTOTYPES --------------------------

//------------------------- STATIC DATA & CONSTANTS ---------------------------

//------------------------------- GLOBAL DATA ---------------------------------

//------------------------------ PUBLIC FUNCTIONS -----------------------------
void app_main(void)
{
    if(TEMP_HUMIDITY_STATUS_OK != temp_humidity_initialize())
    {
        ESP_LOGE("MAIN: ", "Tempearature and humidity sensors initialization failed!");
    }
    ESP_LOGI("MAIN: ", "I2C initialized succesfully!");
    ESP_LOGI("MAIN: ", "Tempearature and humidity sensors initialized succesfully!");

    if(ESP_OK != waveform_generator_init(DAC_CHANNEL_TO_USE))
    {
        ESP_LOGE("MAIN: ", "Waveform generator on DAC_CHANNEL_2 initialization failed!");
    }
    ESP_LOGI("MAIN: ", "Waveform generator on DAC_CHANNEL_2 initialized succesfully!");

    if(WIFI_OK != wifi_init())
    {
        ESP_LOGE("MAIN: ", "Wi-Fi initialization failed!");
    }
    ESP_LOGI("MAIN: ", "Wi-Fi initialized successfully.");

    ESP_LOGI("MAIN: ", "Start user interface initialization...");
    if(USER_INTERFACE_OK != user_interface_init())
    {
        ESP_LOGE("MAIN: ", "User interface initialization failed!");
    }

    ESP_LOGI("MAIN: ", "User interface initialization success.");

    gui_init();
    ESP_LOGI("MAIN: ", "Started GUI.");
}
//---------------------------- PRIVATE FUNCTIONS ------------------------------

//---------------------------- INTERRUPT HANDLERS ------------------------------
