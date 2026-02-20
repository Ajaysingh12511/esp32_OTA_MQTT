/*
 * wifi.h
 * ESP32 WiFi Interface
 */

#ifndef MAIN_WIFI_H_
#define MAIN_WIFI_H_

#include <stdbool.h>
#include "esp_netif.h"
#include "esp_err.h"

void wifi_init(void);
bool wifi_is_connected(void);

extern esp_netif_t *netif_sta;

#endif /* MAIN_WIFI_H_ */
