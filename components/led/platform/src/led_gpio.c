/**
* @file led_gpio.c

* @brief 

* @par 
* 
* COPYRIGHT NOTICE: (c) 2022 Byte Lab Grupa d.o.o.
* All rights reserved.
*/

//--------------------------------- INCLUDES ----------------------------------
#include "led_gpio.h"
#include "driver/gpio.h"
//---------------------------------- MACROS -----------------------------------
#define GPIO_BIT_MASK(X) ((1ULL << (X)))

#define LEVEL_HIGH (1)
#define LEVEL_LOW (0)
//-------------------------------- DATA TYPES ---------------------------------
struct _led_gpio_t
{
    uint8_t pin;
};
//---------------------- PRIVATE FUNCTION PROTOTYPES --------------------------
/**
 * @brief Allocate memory for a led_gpio_t structure and return a pointer to it.
 *
 * @return A pointer to a button_gpio_t struct.
 */
static led_gpio_t *_led_alloc(void);

/**
 * @brief This function frees the memory allocated for the led_gpio_t structure.
 *
 * @param [in] p_led A pointer to the led_gpio_t structure that was created by the _led_create()
 * function.
 */
static void _led_free(led_gpio_t *p_led);

/**
 * @brief It configures the GPIO pin as an output.
 *
 * @param [in] p_btn a pointer to the led_gpio_t structure.
 *
 * @return Status of creation.
 */
static esp_err_t _led_create(led_gpio_t *p_led);
//------------------------- STATIC DATA & CONSTANTS ---------------------------

//------------------------------- GLOBAL DATA ---------------------------------

//------------------------------ PUBLIC FUNCTIONS -----------------------------

led_gpio_t *led_gpio_create(uint8_t pin)
{
    led_gpio_t *p_led = _led_alloc();

    if (NULL == p_led)
    {
        return NULL;
    }

    p_led->pin = pin;

    if(ESP_OK != _led_create(p_led))
    {
        _led_free(p_led);
        return NULL;
    }

    return p_led;
}

void led_gpio_delete(led_gpio_t *p_led)
{
    if(NULL != p_led)
    {
        _led_free(p_led);
    }
}

void led_gpio_on(led_gpio_t *p_led)
{
    gpio_set_level(p_led->pin, LEVEL_HIGH);
}

void led_gpio_off(led_gpio_t *p_led)
{
    gpio_set_level(p_led->pin, LEVEL_LOW);
}

//---------------------------- PRIVATE FUNCTIONS ------------------------------

static led_gpio_t *_led_alloc(void)
{
    return (led_gpio_t *)malloc(sizeof(led_gpio_t));
}

static void _led_free(led_gpio_t *p_led)
{
    free(p_led);
}

static esp_err_t _led_create(led_gpio_t *p_led)
{
    gpio_config_t io_conf = {
        .pin_bit_mask = GPIO_BIT_MASK(p_led->pin),
        .intr_type    = GPIO_INTR_DISABLE,
        .mode         = GPIO_MODE_OUTPUT,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en   = GPIO_PULLUP_DISABLE,        
    };

    esp_err_t esp_err = gpio_config(&io_conf);

    return esp_err;
}

//---------------------------- INTERRUPT HANDLERS -----------------------------


