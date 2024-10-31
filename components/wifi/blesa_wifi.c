/**
* @file blesa_wifi.c
*
* @brief 
*
* COPYRIGHT NOTICE: (c) 2024 Byte Lab Grupa d.o.o.
* All rights reserved.
*/

//--------------------------------- INCLUDES ----------------------------------
#include "blesa_wifi.h"

//---------------------------------- MACROS -----------------------------------

//-------------------------------- DATA TYPES ---------------------------------
struct _wifi_manager_t
{
    wifi_connection_status_t    wifi_connection_status;
    wifi_on_status_changed_cb_t status_changed_callback;
};
//---------------------- PRIVATE FUNCTION PROTOTYPES --------------------------
/**
 * @brief It's a callback function that is called when an event occurs.
 *
 * @param [in] p_arg This is a pointer to the argument passed to the event handler.
 * @param [in] event_base The event base that the event is associated with.
 * @param [in] event_id The event ID.
 * @param [in] p_event_data This is the data that is passed to the event handler.
 */
static void _event_handler(void *p_arg, esp_event_base_t event_base, int32_t event_id, void *p_event_data);

/**
 * @brief If the NVS partition is not initialized, initialize it.
 *
 * @return The return value is the bitwise OR of the return values of nvs_flash_erase() and
 * nvs_flash_init().
 */
static esp_err_t _nvs_init(void);

/**
 * @brief Sets all wifi_manager_t member variables to initial values.
 * 
 */
void _wifi_manager_init_values();

//------------------------- STATIC DATA & CONSTANTS ---------------------------
static wifi_manager_t wifi_manager = {
    .wifi_connection_status =  WIFI_CONNECTION_STATUS_UNKNOWN,
    .status_changed_callback = NULL,
};

static const char *TAG = "WIFI";
//------------------------------- GLOBAL DATA ---------------------------------

//------------------------------ PUBLIC FUNCTIONS -----------------------------
wifi_err_t wifi_init(void)
{
    _wifi_manager_init_values();

    esp_err_t err = ESP_OK;
    err = _nvs_init();

    if(ESP_OK == err)
    {
        ESP_LOGI(TAG, " Initialize TCP/IP");
        err = esp_netif_init();
    }

    if(ESP_OK == err)
    {
        ESP_LOGI(TAG, " Initialize the event loop");
        err = esp_event_loop_create_default();
    }

    if(ESP_OK == err)
    {
        ESP_LOGI(TAG, " Register our event handler for Wi-Fi");
        err = esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &_event_handler, NULL);
    }

    if(ESP_OK == err)
    {
        ESP_LOGI(TAG, " Register our event handler for IP and related events");
        err = esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &_event_handler, NULL);
    }

    if(ESP_OK == err)
    {
        ESP_LOGI(TAG, " Default Wi-Fi configuration");
        wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
        err = esp_wifi_init(&cfg);
    }
    
    if(ESP_OK == err)
    {
        ESP_LOGI(TAG, "Set wifi credentials storage: RAM");
        err = esp_wifi_set_storage(WIFI_STORAGE_RAM);
    }

    if (ESP_OK != err)
    {
        ESP_LOGE(TAG, "Wi-Fi initialization failed: %s", esp_err_to_name(err));
        return WIFI_ERROR_INIT;
    }

    ESP_LOGI(TAG, "Wi-Fi initialization succesfull.");
    return WIFI_OK;
}

wifi_err_t wifi_connect(void)
{
    esp_err_t err = esp_wifi_set_mode(WIFI_MODE_STA);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to set Wi-Fi mode: %s", esp_err_to_name(err));
        return WIFI_ERROR_CONNECTION;
    }
    ESP_LOGI(TAG, "Set Wi-Fi mode to STA");

    static wifi_config_t wifi_config = {
        .sta = {
            .ssid = "Xiaomi 12X",
            .password = "lovorkosane", 
        },
    };

    err = esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to set Wi-Fi configuration: %s", esp_err_to_name(err));
        return WIFI_ERROR_CONNECTION;
    }
    ESP_LOGI(TAG, "Set Wi-Fi config");

    err = esp_wifi_start();
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to start Wi-Fi: %s", esp_err_to_name(err));
        return WIFI_ERROR_CONNECTION;
    }

    wifi_manager.wifi_connection_status = WIFI_CONNECTION_STATUS_CONNECTING;

    ESP_LOGI(TAG, "Wi-Fi connection initiated.");
    return WIFI_OK;
}

wifi_err_t wifi_disconnect(void)
{
    esp_err_t err = esp_wifi_disconnect();
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to disconnect from Wi-Fi: %s", esp_err_to_name(err));
        return WIFI_ERROR_CONNECTION;
    }

    wifi_manager.wifi_connection_status = WIFI_CONNECTION_STATUS_DISCONNECTED;

    if (wifi_manager.status_changed_callback)
    {
        wifi_manager.status_changed_callback(WIFI_CONNECTION_STATUS_DISCONNECTED);
    }

    ESP_LOGI(TAG, "Disconnected from Wi-Fi.");
    return WIFI_OK;
}

wifi_err_t wifi_scan(void)
{
    esp_err_t err;
    uint16_t num_ap = 0;
    wifi_ap_record_t ap_info[20];

    err = esp_wifi_set_mode(WIFI_MODE_STA);
    if(ESP_OK != err)
    {
        ESP_LOGE(TAG, "Failed to set Wi-Fi mode: %s", esp_err_to_name(err));
        return WIFI_ERROR_CONNECTION;
    }

    ESP_LOGI(TAG, "Scanning for Wi-Fi networks...");
    err = esp_wifi_scan_start(NULL, true);
    if(ESP_OK != err)
    {
        ESP_LOGE(TAG, "Failed to start Wi-Fi scan: %s", esp_err_to_name(err));
        return WIFI_ERROR_SCAN;
    }

    err = esp_wifi_scan_get_ap_num(&num_ap);
    if(ESP_OK != err)
    {
        ESP_LOGE(TAG, "Failed to get number of access points: %s", esp_err_to_name(err));
        return WIFI_ERROR_SCAN;
    }

    if (num_ap > 0)
    {
        err = esp_wifi_scan_get_ap_records(&num_ap, ap_info);
        if (err != ESP_OK)
        {
            ESP_LOGE(TAG, "Failed to get access point records: %s", esp_err_to_name(err));
            return WIFI_ERROR_SCAN;
        }

        for (int i = 0; i < num_ap; i++)
        {
            ESP_LOGI(TAG, "Found AP %d: SSID: %s, RSSI: %d", 
                      i + 1, 
                      ap_info[i].ssid, 
                      ap_info[i].rssi);
        }
    }
    else
    {
        ESP_LOGI(TAG, "No access points found.");
    }

    return WIFI_OK;
}

void wifi_register_on_status_changed(wifi_on_status_changed_cb_t cbk)
{
    wifi_manager.status_changed_callback = cbk;
}
//---------------------------- PRIVATE FUNCTIONS ------------------------------
static esp_err_t _nvs_init(void)
{
    esp_err_t ret = nvs_flash_init();
    if((ESP_ERR_NVS_NO_FREE_PAGES == ret) || (ESP_ERR_NVS_NEW_VERSION_FOUND == ret))
    {
        /* NVS partition was truncated
         * and needs to be erased */
        ret = nvs_flash_erase();

        /* Retry nvs_flash_init */
        ret |= nvs_flash_init();
    }
    return ret;
}

void _wifi_manager_init_values()
{
    wifi_manager.wifi_connection_status = WIFI_CONNECTION_STATUS_DISCONNECTED;
    wifi_manager.status_changed_callback = NULL;
}
//---------------------------- INTERRUPT HANDLERS -----------------------------
static void _event_handler(void *p_arg, esp_event_base_t event_base, int32_t event_id, void *p_event_data)
{
    ESP_LOGI(TAG, "Event handler triggered: base=%s, event_id=%d", event_base, (int)event_id);

    if ((WIFI_EVENT == event_base) && (WIFI_EVENT_STA_START == event_id))
    {
        ESP_LOGI(TAG, "Wi-Fi started, trying to connect...");
        wifi_manager.wifi_connection_status = WIFI_CONNECTION_STATUS_CONNECTING;
        esp_wifi_connect();
    }
    else if ((WIFI_EVENT == event_base) && (WIFI_EVENT_STA_CONNECTED == event_id))
    {
        ESP_LOGI(TAG, "Successfully connected to Wi-Fi.");
        wifi_manager.wifi_connection_status = WIFI_CONNECTION_STATUS_CONNECTED;
        
        if (wifi_manager.status_changed_callback)
        {
            wifi_manager.status_changed_callback(WIFI_CONNECTION_STATUS_CONNECTED);
        }
    }
    else if ((WIFI_EVENT == event_base) && (WIFI_EVENT_STA_DISCONNECTED == event_id))
    {
        ESP_LOGI(TAG, "DISCONNECTED from Wi-Fi. Attempting to reconnect...");
        wifi_manager.wifi_connection_status = WIFI_CONNECTION_STATUS_DISCONNECTED;

        if (wifi_manager.status_changed_callback)
        {
            wifi_manager.status_changed_callback(WIFI_CONNECTION_STATUS_DISCONNECTED);
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
        esp_wifi_connect();
    }
    else if ((WIFI_EVENT == event_base) && (WIFI_EVENT_AP_STACONNECTED == event_id))
    {
        wifi_event_ap_staconnected_t *event = (wifi_event_ap_staconnected_t *)p_event_data;
        // You can use event->mac directly for your logic
        ESP_LOGI(TAG, "A station connected to the SoftAP.");
    }
    else if ((WIFI_EVENT == event_base) && (WIFI_EVENT_AP_STADISCONNECTED == event_id))
    {
        wifi_event_ap_stadisconnected_t *event = (wifi_event_ap_stadisconnected_t *)p_event_data;
        // You can use event->mac directly for your logic
        ESP_LOGI(TAG, "A station disconnected from the SoftAP.");
    }
}
