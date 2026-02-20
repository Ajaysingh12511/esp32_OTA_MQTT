#include "wifi.h"
#include "common_task.h"

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "lwip/ip4_addr.h"
#include <string.h>

static const char *TAG = "WIFI";

static EventGroupHandle_t wifi_event_group;
esp_netif_t *netif_sta = NULL;

static bool wifi_connected = false;
static int retry_count = 0;

/* ===============================
   WiFi Event Handler
   =============================== */
static void wifi_event_handler(void *arg,
                               esp_event_base_t event_base,
                               int32_t event_id,
                               void *event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
        esp_wifi_connect();
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        wifi_connected = false;

        if (retry_count < WIFI_APP_MAX_RETRY)
        {
            retry_count++;
            ESP_LOGI(TAG, "Retrying WiFi...");
            esp_wifi_connect();
        }
        else
        {
            ESP_LOGE(TAG, "WiFi Failed to Connect");
        }
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        retry_count = 0;
        wifi_connected = true;
        ESP_LOGI(TAG, "WiFi Connected");
    }
}

/* ===============================
   WiFi Init
   =============================== */
void wifi_init(void)
{
    esp_netif_init();
    esp_event_loop_create_default();

    netif_sta = esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);

    /* Register Events */
    esp_event_handler_instance_register(WIFI_EVENT,
                                        ESP_EVENT_ANY_ID,
                                        &wifi_event_handler,
                                        NULL,
                                        NULL);

    esp_event_handler_instance_register(IP_EVENT,
                                        IP_EVENT_STA_GOT_IP,
                                        &wifi_event_handler,
                                        NULL,
                                        NULL);

    /* Configure WiFi */
    wifi_config_t wifi_config = {0};

    strcpy((char *)wifi_config.sta.ssid, WIFI_APP_SSID);
    strcpy((char *)wifi_config.sta.password, WIFI_APP_PASSWORD);

    wifi_config.sta.threshold.authmode = WIFI_APP_AUTH_MODE;

    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
    esp_wifi_set_ps(WIFI_APP_POWER_SAVE);

#if WIFI_APP_USE_STATIC_IP

    ESP_LOGI(TAG, "Using Static IP Configuration");

    esp_netif_dhcpc_stop(netif_sta);

    esp_netif_ip_info_t ip_info = {0};

    IP4_ADDR(&ip_info.ip,
             WIFI_APP_IP_A,
             WIFI_APP_IP_B,
             WIFI_APP_IP_C,
             WIFI_APP_IP_D);

    IP4_ADDR(&ip_info.gw,
             WIFI_APP_GW_A,
             WIFI_APP_GW_B,
             WIFI_APP_GW_C,
             WIFI_APP_GW_D);

    IP4_ADDR(&ip_info.netmask,
             WIFI_APP_NETMASK_A,
             WIFI_APP_NETMASK_B,
             WIFI_APP_NETMASK_C,
             WIFI_APP_NETMASK_D);

    esp_netif_set_ip_info(netif_sta, &ip_info);

    /* 🔥 IMPORTANT: Set DNS (Fix MQTT hostname issue) */
    esp_netif_dns_info_t dns;
    IP4_ADDR(&dns.ip, 8, 8, 8, 8);  // Google DNS
    esp_netif_set_dns_info(netif_sta, ESP_NETIF_DNS_MAIN, &dns);

#endif

    esp_wifi_start();
}

/* ===============================
   WiFi Status
   =============================== */
bool wifi_is_connected(void)
{
    return wifi_connected;
}
