#include "ota_app.h"
#include "esp_https_ota.h"
#include "esp_log.h"
#include "esp_system.h"

static const char *TAG = "OTA";

void ota_start(void)
{
    ESP_LOGI(TAG, "Starting OTA...");

    /* HTTP client config */
    esp_http_client_config_t http_config = {
    .url = "http://192.168.0.68:8000/ESP32_OTA.bin",
};

    /* OTA config wrapper (IDF v5 requirement) */
    esp_https_ota_config_t ota_config = {
        .http_config = &http_config,
    };

    esp_err_t ret = esp_https_ota(&ota_config);

    if (ret == ESP_OK)
    {
        ESP_LOGI(TAG, "OTA Success! Rebooting...");
        esp_restart();
    }
    else
    {
        ESP_LOGE(TAG, "OTA Failed!");
    }
}
