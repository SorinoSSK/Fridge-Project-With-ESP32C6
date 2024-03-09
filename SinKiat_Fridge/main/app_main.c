/* Multi-Device Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <string.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <nvs_flash.h>

#include <esp_rmaker_standard_devices.h>
#include <esp_rmaker_schedule.h>
#include <esp_rmaker_scenes.h>

#include <app_wifi.h>
#include <app_insights.h>

#include "app_priv.h"

static const char *TAG = "app_main";

// Declare device
esp_rmaker_device_t *fridgeLock;
esp_rmaker_device_t *fridgeState;

/* Callback to handle commands received from the RainMaker cloud */
static esp_err_t write_cb(const esp_rmaker_device_t *device, const esp_rmaker_param_t *param,
            const esp_rmaker_param_val_t val, void *priv_data, esp_rmaker_write_ctx_t *ctx)
{
    if (ctx) 
    {
        ESP_LOGI(TAG, "Received write request via : %s", esp_rmaker_device_cb_src_to_str(ctx->src));
    }
    const char *device_name = esp_rmaker_device_get_name(device);
    const char *param_name = esp_rmaker_param_get_name(param);
    // If receive input from rainmaker app and is a power device
    if (strcmp(param_name, ESP_RMAKER_DEF_POWER_NAME) == 0) 
    {
        ESP_LOGI(TAG, "Received value = %s for %s - %s", val.val.b? "true" : "false", device_name, param_name);
        // If the power device is "Fridge Lock", run fridge's lock
        if (strcmp(device_name, "Fridge Lock") == 0) 
        {
            fridgeLockFunc(val.val.b);
        }
    } 
    else 
    {
        /* Silently ignoring invalid params */
        return ESP_OK;
    }
    esp_rmaker_param_update_and_report(param, val);
    return ESP_OK;
}

void app_main()
{
    app_driver_init();

    /* Initialize NVS. */
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK( err );

    /* Initialize Wi-Fi. Note that, this should be called before esp_rmaker_node_init()
     */
    app_wifi_init();
    
    /* Initialize the ESP RainMaker Agent.
     * Note that this should be called after app_wifi_init() but before app_wifi_start()
     * */
    esp_rmaker_config_t rainmaker_cfg = {
        .enable_time_sync = false,
    };
    esp_rmaker_node_t *node = esp_rmaker_node_init(&rainmaker_cfg, "Sin Kiat IoT Fridge", "IoT Fridge");
    if (!node) {
        ESP_LOGE(TAG, "Could not initialise node. Aborting!!!");
        vTaskDelay(5000/portTICK_PERIOD_MS);
        abort();
    }

    // Initialise fridge's function to rainmaker app
    initESPDevice(node);

    /* Enable OTA */
    esp_rmaker_ota_enable_default();

    /* Enable timezone service which will be require for setting appropriate timezone
     * from the phone apps for scheduling to work correctly.
     * For more information on the various ways of setting timezone, please check
     * https://rainmaker.espressif.com/docs/time-service.html.
     */
    esp_rmaker_timezone_service_enable();

    /* Enable scheduling. */
    esp_rmaker_schedule_enable();

    /* Enable Scenes */
    esp_rmaker_scenes_enable();

    /* Enable Insights. Requires CONFIG_ESP_INSIGHTS_ENABLED=y */
    app_insights_enable();

    /* Start the ESP RainMaker Agent */
    esp_rmaker_start();

    /* Start the Wi-Fi.
     * If the node is provisioned, it will start connection attempts,
     * else, it will start Wi-Fi provisioning. The function will return
     * after a connection has been successfully established
     */
    err = app_wifi_start(POP_TYPE_RANDOM);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Could not start Wifi. Aborting!!!");
        vTaskDelay(5000/portTICK_PERIOD_MS);
        abort();
    }
}

void initESPDevice(esp_rmaker_node_t *node)
{
    // ===== Fridge Lock ===== //
    fridgeLock = esp_rmaker_switch_device_create("Fridge Lock", NULL, DEFAULT_LOCK_STATE);
    esp_rmaker_device_add_cb(fridgeLock, write_cb, NULL);
    esp_rmaker_device_add_attribute(fridgeLock, "Lock Type", "SG92R Servo Motor");
    esp_rmaker_node_add_device(node, fridgeLock);

    // ===== Fridge Sensor ===== //
    fridgeState = esp_rmaker_temp_sensor_device_create("Fridge Door", NULL, app_get_door_state());
    esp_rmaker_node_add_device(node, fridgeState);
}