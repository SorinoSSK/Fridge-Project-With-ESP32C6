/*
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <sdkconfig.h>
#include <freertos/FreeRTOS.h>
#include <freertos/timers.h>

#include <iot_button.h>
#include <esp_rmaker_standard_types.h>

#include <app_reset.h>
#include <ws2812_led.h>
#include "app_priv.h"

#include <iot_servo.h>

#define BUTTON_ACTIVE_LEVEL  0

#define WIFI_RESET_BUTTON_TIMEOUT       3
#define FACTORY_RESET_BUTTON_TIMEOUT    10

// ===== Config for Door Sensor ===== //
#define FRIDGE_DOOR_SENSOR      2
static bool fridgeDoorState = DEFAULT_DOOR_STATE;

// ===== Config for Door Lock ===== //
#define SERVO_CH0_PIN       3
#define FRIDGE_LOCK_BTN     11
static bool fridgeLockState = DEFAULT_LOCK_STATE;
servo_config_t servo_cfg = {
    .max_angle = 180,
    .min_width_us = 500,
    .max_width_us = 2500,
    .freq = 50,
    .timer_number = LEDC_TIMER_0,
    .channels = {
        .servo_pin = {
            SERVO_CH0_PIN
        },
        .ch = {        },
    },
    .channel_number = 1,
} ;

// ===== Log Tag ===== //
// static const char *TAG = "app_main";

void app_driver_init()
{
    initFridgeLockBtn();
    initDoorSensor();
    initFridgeLock();
}

// ===== Fridge Lock Servo Function ===== //
void initFridgeLock(void)
{
    iot_servo_init(LEDC_LOW_SPEED_MODE, &servo_cfg);
    fridgeLockFunc(DEFAULT_LOCK_STATE);
}
// Function to lock or unlock the door.
void fridgeLockFunc(bool state)
{
    if (state)
    {
        iot_servo_write_angle(LEDC_LOW_SPEED_MODE, 0, 180.0f);
    }
    else
    {
        iot_servo_write_angle(LEDC_LOW_SPEED_MODE, 0, 0.0f);
    }
}

// ===== Fridge Lock Button Function ===== //
void initFridgeLockBtn(void)
{
    button_handle_t fridgeLockBtn = iot_button_create(FRIDGE_LOCK_BTN, BUTTON_ACTIVE_LEVEL);
    if (fridgeLockBtn) 
    {
        iot_button_set_evt_cb(fridgeLockBtn, BUTTON_CB_TAP, fridgeLockPushEvent, NULL);
        // /* Register Wi-Fi reset and factory reset functionality on same button */
        // app_reset_button_register(btn_handle, WIFI_RESET_BUTTON_TIMEOUT, FACTORY_RESET_BUTTON_TIMEOUT);
    }
}
// Button event to lock and unlock the fridge. Send to rainmaker app after locking or unlocking.
static void fridgeLockPushEvent(void *arg)
{
    fridgeLockState = !fridgeLockState;
    fridgeLockFunc(fridgeLockState);
    esp_rmaker_param_update_and_report(
            esp_rmaker_device_get_param_by_type(fridgeLock, ESP_RMAKER_PARAM_POWER),
            esp_rmaker_bool(fridgeLockState));
}

// ===== Fridge Sensor Limit Switch Function ===== //
void initDoorSensor(void)
{
    button_handle_t fridgeDoorSensor = iot_button_create(FRIDGE_DOOR_SENSOR, BUTTON_ACTIVE_LEVEL);
    if (fridgeDoorSensor) 
    {
        iot_button_set_evt_cb(fridgeDoorSensor, BUTTON_CB_RELEASE, fridgeDoorOpenEvent, NULL);
        iot_button_set_evt_cb(fridgeDoorSensor, BUTTON_CB_PUSH, fridgeDoorCloseEvent, NULL);
    }
}
// Event to detect if door is open
static void fridgeDoorOpenEvent(void *arg)
{
    fridgeDoorState = false;
    esp_rmaker_param_update_and_report(
            esp_rmaker_device_get_param_by_type(fridgeState, ESP_RMAKER_PARAM_TEMPERATURE),
            esp_rmaker_float(fridgeDoorState));
}
// Event to detect if door is close
static void fridgeDoorCloseEvent(void *arg)
{
    fridgeDoorState = true;
    esp_rmaker_param_update_and_report(
            esp_rmaker_device_get_param_by_type(fridgeState, ESP_RMAKER_PARAM_TEMPERATURE),
            esp_rmaker_float(fridgeDoorState));
}
// Function to get door status
bool app_get_door_state(void)
{
    return fridgeDoorState;
}