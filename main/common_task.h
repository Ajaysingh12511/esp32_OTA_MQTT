/*
 * common_task.h
 *
 *  WiFi Application Configuration
 */

#ifndef MAIN_COMMON_TASK_H_
#define MAIN_COMMON_TASK_H_

#include <stdbool.h>
#include "esp_wifi.h"
#include "esp_netif.h"

/* =====================================================
   WiFi Application Settings
   ===================================================== */

/* WiFi Credentials */
#define WIFI_APP_SSID              "Ajay"
#define WIFI_APP_PASSWORD          "12345677"

/* Retry Configuration */
#define WIFI_APP_MAX_RETRY         5

/* Authentication Mode */
#define WIFI_APP_AUTH_MODE         WIFI_AUTH_WPA2_PSK

/* Power Save (Disable for EV Charger) */
#define WIFI_APP_POWER_SAVE        WIFI_PS_NONE

/* Static IP Enable (1 = Enable, 0 = DHCP) */
#define WIFI_APP_USE_STATIC_IP 0
/* =====================================================
   Static IP Configuration
   (Only used if WIFI_APP_USE_STATIC_IP = 1)
   ===================================================== */

#define WIFI_APP_IP_A              192
#define WIFI_APP_IP_B              168
#define WIFI_APP_IP_C              1
#define WIFI_APP_IP_D              200

#define WIFI_APP_GW_A              192
#define WIFI_APP_GW_B              168
#define WIFI_APP_GW_C              1
#define WIFI_APP_GW_D              1

#define WIFI_APP_NETMASK_A         255
#define WIFI_APP_NETMASK_B         255
#define WIFI_APP_NETMASK_C         255
#define WIFI_APP_NETMASK_D         0

/* =====================================================
   Buffer Limits
   ===================================================== */

#define MAX_SSID_LENGTH            32
#define MAX_PASSWORD_LENGTH        64

#endif /* MAIN_COMMON_TASK_H_ */
