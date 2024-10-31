/**
* @file my_mqtt.c
*
* @brief Implementation of MQTT initialization and data publishing functions for
*        temperature and humidity readings.
*
* COPYRIGHT NOTICE: (c) 2024 Byte Lab Grupa d.o.o.
* All rights reserved.
*/

//--------------------------------- INCLUDES ----------------------------------
#include "my_mqtt.h"
#include "esp_log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//---------------------------------- MACROS -----------------------------------
#define TAG "MQTT"

//-------------------------------- DATA TYPES ---------------------------------

//---------------------- PRIVATE FUNCTION PROTOTYPES --------------------------
/**
 * @brief MQTT event handler that handles connection and disconnection events.
 */
static void _mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);

//------------------------- STATIC DATA & CONSTANTS ---------------------------
static esp_mqtt_client_handle_t mqtt_client = NULL;

//------------------------------- GLOBAL DATA ---------------------------------

//------------------------------ PUBLIC FUNCTIONS -----------------------------

/**
 * @brief Initializes the MQTT client and connects to the broker.
 */
void mqtt_init(void)
{
    static esp_mqtt_client_config_t mqtt5_cfg = {
    .broker.address.hostname  = "192.168.92.218",
    .broker.address.transport = MQTT_TRANSPORT_OVER_TCP,
    .broker.address.path      = "/blesa/final_task",
    .broker.address.port      = 1883,
};

    mqtt_client = esp_mqtt_client_init(&mqtt5_cfg);
    if (mqtt_client == NULL) {
        ESP_LOGE(TAG, "Failed to initialize MQTT client");
        return;
    }

    esp_mqtt_client_register_event(mqtt_client, ESP_EVENT_ANY_ID, _mqtt_event_handler, NULL);

    esp_mqtt_client_start(mqtt_client);
}


void send_data(uint32_t temperature, uint32_t humidity)
{
    char msg[64];

    // Format the message as a string
    int len = snprintf(msg, sizeof(msg), "{\"temperature\": %d, \"humidity\": %d}", (int)temperature, (int)humidity);
    if (len < 0 || len >= sizeof(msg)) {
        ESP_LOGE(TAG, "Failed to create message string");
        return;
    }

    int msg_id = esp_mqtt_client_publish(mqtt_client, "sensor/data", msg, 0, 1, 0);
    ESP_LOGI(TAG, "Sent publish message, msg_id=%d", msg_id);
}

//---------------------------- PRIVATE FUNCTIONS ------------------------------
static void _mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    esp_mqtt_event_handle_t event = event_data;
    switch (event->event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
            break;
        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "MQTT_EVENT_DATA received");
            break;
        case MQTT_EVENT_ERROR:
            ESP_LOGE(TAG, "MQTT_EVENT_ERROR");
            break;
        default:
            ESP_LOGI(TAG, "Other MQTT event: %d", event->event_id);
            break;
    }
}

//---------------------------- INTERRUPT HANDLERS -----------------------------
