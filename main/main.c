/*
 * main.c
 * ESP32 OTA Project
 */
#include "mqtt_app.h"
#include "web_server.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "wifi.h"

static const char *TAG = "MAIN";

static void system_init_task(void *arg)
{
    ESP_LOGI(TAG, "System Initialization Started");

    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ESP_ERROR_CHECK(nvs_flash_init());
    }

    wifi_init();
   
    while (!wifi_is_connected())
    {
        ESP_LOGI(TAG, "Waiting for WiFi...");
        vTaskDelay(pdMS_TO_TICKS(2000));
    }

    ESP_LOGI(TAG, "WiFi Connected Successfully");
    mqtt_app_start();
    start_webserver();   
    ESP_LOGI(TAG, "System Ready");

    vTaskDelete(NULL);
}

void app_main(void)
{
    ESP_LOGI(TAG, "ESP32 Booting...");

    xTaskCreate(system_init_task,
                "system_init_task",
                4096,
                NULL,
                5,
                NULL);
}
