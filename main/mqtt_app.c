#include "mqtt_app.h"
#include "wifi.h"
#include "ota_app.h"
#include "esp_log.h"
#include "mqtt_client.h"

static const char *TAG = "MQTT";
static esp_mqtt_client_handle_t client = NULL;
static bool mqtt_connected = true;   // ✅ MUST be here (top)


//Heart Beat

void heartbeat_task(void *arg)
{
    while (1)
    {
        if (mqtt_connected)
        {
            esp_mqtt_client_publish(client,
                                    "ev/charger/heartbeat",
                                    "{\"status\":\"alive\"}",
                                    0,
                                    1,
                                    0);

            ESP_LOGI(TAG, "Heartbeat Sent");
        }

        vTaskDelay(pdMS_TO_TICKS(30000));  // 30 sec
    }
}




/* ===============================
   MQTT Event Handler
   =============================== */
static void mqtt_event_handler(void *handler_args,
                               esp_event_base_t base,
                               int32_t event_id,
                               void *event_data)
{
    esp_mqtt_event_handle_t event = event_data;

    switch ((esp_mqtt_event_id_t)event_id)
    {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT Connected");

            // Subscribe topic
            esp_mqtt_client_subscribe(client, "ev/charger/control", 1);

            // Publish Boot message
            esp_mqtt_client_publish(client,
                                    "ev/charger/status",
                                    "BootNotification",
                                    0,
                                    1,
                                    0);
                                    
            ESP_LOGI(TAG, "BootNotification Sent");
            
            
             /* 2 Subscribe for Remote Control */
            esp_mqtt_client_subscribe(client,
                                      "ev/charger/control",
                                      1);
            
            ESP_LOGI(TAG, "Subscribed to control topic");
            
            /* 3️ Start Heartbeat Task */
            xTaskCreate(heartbeat_task,
                        "heartbeat_task",
                        4096,
                        NULL,
                        5,
                        NULL);
            
            break;

        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "Received Topic: %.*s", event->topic_len, event->topic);
            ESP_LOGI(TAG, "Received Data: %.*s", event->data_len, event->data);

            // Yaha control logic likh sakte ho
              
            /* 4 Remote Start/Stop Logic */
            if (strncmp(event->data, "start", event->data_len) == 0)
            {
                ESP_LOGI(TAG, "Remote START Command Received");

                // Yaha charger start logic likho
            }

            if (strncmp(event->data, "stop", event->data_len) == 0)
            {
                ESP_LOGI(TAG, "Remote STOP Command Received");

                // Yaha charger stop logic likho
            }
            
             /* 🔥 OTA Trigger */
         if (strncmp(event->data, "ota", event->data_len) == 0)
          {
           ESP_LOGI(TAG, "OTA Command Received");
            ota_start();
           }
            
            break;

        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "MQTT Disconnected");
            break;

        default:
            break;
    }
}

/* ===============================
   MQTT Start
   =============================== */
void mqtt_app_start(void)
{
    if (!wifi_is_connected())
    {
        ESP_LOGE(TAG, "WiFi not connected");
        return;
    }

    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = "mqtt://broker.hivemq.com"
    };

    client = esp_mqtt_client_init(&mqtt_cfg);

    esp_mqtt_client_register_event(client,
                                   ESP_EVENT_ANY_ID,
                                   mqtt_event_handler,
                                   NULL);

    esp_mqtt_client_start(client);
}
