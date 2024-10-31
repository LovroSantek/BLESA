/**
* @file blesa_wifi.h
*
* @brief See the source file.
* 
* COPYRIGHT NOTICE: (c) 2024 Byte Lab Grupa d.o.o.
* All rights reserved.
*/

#ifndef __BLESA_WIFI_H__
#define __BLESA_WIFI_H__

#ifdef __cplusplus
extern "C" {
#endif

//--------------------------------- INCLUDES ----------------------------------
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_log.h"

//---------------------------------- MACROS -----------------------------------

//-------------------------------- DATA TYPES ---------------------------------
struct _wifi_manager_t;
typedef struct _wifi_manager_t wifi_manager_t;

typedef enum
{
    WIFI_CONNECTION_STATUS_UNKNOWN      = -1,
    WIFI_CONNECTION_STATUS_CONNECTED    =  0, 
    WIFI_CONNECTION_STATUS_DISCONNECTED =  1,
    WIFI_CONNECTION_STATUS_CONNECTING   =  2, 
} wifi_connection_status_t;

typedef void (*wifi_on_status_changed_cb_t)(wifi_connection_status_t new_status);

typedef enum
{
    WIFI_OK = 0,

    // Warnings:
    WIFI_ALREADY_PROVISIONED = 10,

    // Errors:
    WIFI_ERROR_CONNECTION = -10,
    WIFI_ERROR_INIT = -30,
    WIFI_ERROR_SCAN = -40,
  
   WIFI_COUNT
} wifi_err_t;

//---------------------- PUBLIC FUNCTION PROTOTYPES --------------------------
/**
 * @brief Initializes everything Wi-Fi related (nvs, driver, event handler).
 * 
 * @return wifi_err_t Returns WIFI_OK is everything is good.
 */
wifi_err_t wifi_init(void);

/**
 * @brief Connects to an AP if possible.
 * 
 * @return wifi_err_t Returns WIFI_OK is everything is good.
 */
wifi_err_t wifi_connect(void);

/**
 * @brief Disconnect if connected.
 * 
 * @return wifi_err_t Returns WIFI_OK is everything is good.
 */
wifi_err_t wifi_disconnect(void);

/**
 * @brief Scans and prints all available acess points.
 * 
 * @return wifi_err_t Returns WIFI_OK is everything is good.
 */
wifi_err_t wifi_scan(void);

/**
 * @brief Saves function callback to a global Wi-Fi object. 
 * 
 * @param cbk Functions that needs to be called on status change.
 */
void wifi_register_on_status_changed(wifi_on_status_changed_cb_t cbk);


#ifdef __cplusplus
}
#endif

#endif // __BLESA_WIFI_H__
