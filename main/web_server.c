#include "esp_http_server.h"
#include "esp_log.h"
#include "ota_app.h"

static const char *TAG = "WEB";

static httpd_handle_t server = NULL;

/* HTML Page */
static const char *html_page =
"<!DOCTYPE html>"
"<html><body>"
"<h2>ESP32 OTA Update</h2>"
"<button onclick=\"fetch('/update')\">Update Firmware</button>"
"</body></html>";

/* Root Page */
static esp_err_t root_get_handler(httpd_req_t *req)
{
    httpd_resp_send(req, html_page, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

/* OTA Trigger */
static esp_err_t update_get_handler(httpd_req_t *req)
{
    httpd_resp_sendstr(req, "Starting OTA... Check Serial Monitor");

    ESP_LOGI(TAG, "Web OTA Triggered");
    ota_start();   // 🔥 OTA CALL

    return ESP_OK;
}

/* Start Web Server */
void start_webserver(void)
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    if (httpd_start(&server, &config) == ESP_OK)
    {
        httpd_uri_t root = {
            .uri = "/",
            .method = HTTP_GET,
            .handler = root_get_handler
        };
        httpd_register_uri_handler(server, &root);

        httpd_uri_t update = {
            .uri = "/update",
            .method = HTTP_GET,
            .handler = update_get_handler
        };
        httpd_register_uri_handler(server, &update);

        ESP_LOGI(TAG, "Web Server Started");
    }
}
