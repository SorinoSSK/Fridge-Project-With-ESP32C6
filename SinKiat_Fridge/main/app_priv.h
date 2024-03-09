/*
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <esp_log.h>
#include <esp_rmaker_core.h>
#include <esp_rmaker_standard_params.h>

#define DEFAULT_LOCK_STATE          true
#define DEFAULT_DOOR_STATE          true
#define REPORTING_PERIOD            60 /* Seconds */

extern esp_rmaker_device_t *fridgeLock;
extern esp_rmaker_device_t *fridgeState;

void app_driver_init(void);

// ===== Sin Kiat's Fridge Declaration ===== //
// For setting up rainmaker app
void initESPDevice(esp_rmaker_node_t *node);
// For Fridge's servo
void initFridgeLock(void);
void fridgeLockFunc(bool state);
static void fridgeLockPushEvent(void *arg);
// For Fridge's button
void initFridgeLockBtn(void);
bool app_get_door_state(void);
// For Fridge's door sensor
void initDoorSensor(void);
static void fridgeDoorOpenEvent(void *arg);
static void fridgeDoorCloseEvent(void *arg);