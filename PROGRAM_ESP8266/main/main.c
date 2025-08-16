#include <string.h>
#include <stdlib.h>
#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_system.h"
#include "tcpip_adapter.h"
#include "esp_http_server.h"
#include "driver/uart.h"

#define WIFI_SSID 	"ArcDetectorHotspot"
#define WIFI_PASS 	"we_love_arcs"
#define MAX_STA_CONN 	1
#define UART_NUM 	UART_NUM_0 //Uart module
#define BUF_SIZE	(1024)
#define BIN_NUMBER	90

//0 for disable, 1 for enable
#define ENABLE_LOGS 	0

#if ENABLE_LOGS
	#define LOGI(tag, fmt, ...) ESP_LOGI(tag,fmt, ##__VA_ARGS__)
	#define LOGW(tag, fmt, ...) ESP_LOGW(tag,fmt, ##__VA_ARGS__)
	#define LOGE(tag, fmt, ...) ESP_LOGE(tag,fmt, ##__VA_ARGS__)
#else
	#define LOGI(tag, fmt, ...)
	#define LOGW(tag, fmt, ...)
	#define LOGE(tag, fmt, ...)
#endif

static bool command_with_value = 0;
static const char *TAG = "wifi_softAP";//software access point
static uint32_t power_data[BIN_NUMBER];

static volatile bool capture_power = false;

static void uart_init(void){
	uart_config_t uart_config = {
		.baud_rate = 921600,
		.data_bits = UART_DATA_8_BITS,
		.parity    = UART_PARITY_DISABLE,
		.stop_bits = UART_STOP_BITS_1,
		.flow_ctrl = UART_HW_FLOWCTRL_DISABLE
	};
	uart_param_config(UART_NUM, &uart_config);
	uart_driver_install(UART_NUM, BUF_SIZE * 2, 0, 0, NULL, 0);
}


static void wifi_init_softap(void){
	tcpip_adapter_init();

	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
	ESP_ERROR_CHECK(esp_wifi_init(&cfg));

	wifi_config_t wifi_config = {
		.ap = {
			.ssid = WIFI_SSID,
			.ssid_len = strlen(WIFI_SSID),
			.password = WIFI_PASS,
			.max_connection = MAX_STA_CONN,
			.authmode = WIFI_AUTH_WPA_WPA2_PSK

		},
	};
	
	if(strlen(WIFI_PASS) == 0){
		wifi_config.ap.authmode = WIFI_AUTH_OPEN;
	}

	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
	ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_config));
	ESP_ERROR_CHECK(esp_wifi_start());

	LOGI(TAG, "WIFI Access Point started. Access Point name:%s ; Password:%s ", WIFI_SSID, WIFI_PASS);
}

esp_err_t data_post_handler(httpd_req_t *req){
	char content[100];
	size_t recv_size = MIN(req->content_len, sizeof(content)-1);

	int ret = httpd_req_recv(req, content, recv_size);
	if(ret <= 0){
		return ESP_FAIL;
	}

	content[ret] = '\0';
	LOGI(TAG, "Received data: %s", content);

	uint8_t command_code = 0;
	uint32_t command_val = 0;
	if      (strcmp(content, "ATx10") == 0) command_code = 1;
	else if (strcmp(content, "ATx5") == 0)  command_code = 2;
	else if (strcmp(content, "AMx1") == 0)  command_code = 3;
	else if (strcmp(content, "AMx5") == 0)  command_code = 4;
	else if (strcmp(content, "AMx10") == 0) command_code = 5;
	else if (strcmp(content, "START") == 0) command_code = 6;
	else if (strcmp(content, "STOP") == 0)  command_code = 7;
	else if	(strcmp(content, "FREQ") == 0)  {
		command_with_value = 1;
		command_code = 8;
	}
	else if (strcmp(content, "POW") == 0)   {
		command_with_value = 1;
		command_code = 9;
	}
	else if (strcmp(content, "R_POW") == 0){
		command_code = 10;
		capture_power = true;
	}
	else if (command_with_value == 1) command_val = atoi(content);

	if ((command_code > 0) && (command_code < 11)){
		uart_write_bytes(UART_NUM, (const char*) &command_code,1);
		LOGI(TAG, "Command code %d sent to UART, the code isn't translated to char!!!", command_code);
	} else if(command_with_value == 1){	
		uart_write_bytes(UART_NUM, (const char*) &command_val,4);
		command_with_value = 0;
	} else {
		LOGW(TAG, "Unknown command received");
	}

	httpd_resp_send(req, "Data received", strlen("Data received"));
	return ESP_OK;
}

static void uart_capture_task(void *arg){

	while (1){
		if (!capture_power){
			vTaskDelay(pdMS_TO_TICKS(100));
			continue;
		}	
		int bytes_needed = BIN_NUMBER * sizeof(uint32_t);
		int bytes_read = 0;
	
		while (bytes_read < bytes_needed){
			uint32_t r = uart_read_bytes(UART_NUM, (uint8_t *)power_data + bytes_read, bytes_needed - bytes_read, pdMS_TO_TICKS(1000));
			if (r > 0) bytes_read += r;
		}
		capture_power = false;
	}
}
esp_err_t graph_data_get_handler(httpd_req_t *req){
	char json[3000];//for the data and commas
	int len = 0;
	
	len += snprintf(json + len, sizeof(json) - len, "[");

	for (int i = 0; i < BIN_NUMBER; i++){
		len += snprintf(json + len, sizeof(json) - len, "%d%s", power_data[i], (i<BIN_NUMBER-1) ? ",":"");
	}
	len += snprintf(json + len, sizeof(json) - len, "]");


	httpd_resp_set_type(req, "application/json");
	httpd_resp_send(req, json, len);
	return ESP_OK;
}

httpd_uri_t uri_get_graph = {
	.uri= "/graph",
	.method = HTTP_GET,
	.handler = graph_data_get_handler,
	.user_ctx = NULL
};

httpd_uri_t uri_post = {
	.uri = "/data",
	.method = HTTP_POST,
	.handler = data_post_handler,
	.user_ctx = NULL
};

static httpd_handle_t start_webserver(void){
	httpd_config_t config = HTTPD_DEFAULT_CONFIG();
	httpd_handle_t server = NULL;

	if(httpd_start(&server, &config) == ESP_OK){
		httpd_register_uri_handler(server, &uri_post);
		httpd_register_uri_handler(server, &uri_get_graph);
	}
	return server;
}

void app_main(void){
	#if !ENABLE_LOGS
		esp_log_level_set("*", ESP_LOG_WARN);	
	#endif
	ESP_ERROR_CHECK(nvs_flash_init());
	tcpip_adapter_init();	
	ESP_ERROR_CHECK(esp_event_loop_create_default());
	
	uart_init();
	wifi_init_softap();
	start_webserver();
	xTaskCreate(uart_capture_task, "uart_capture_task", 4096, NULL, 5, NULL);	
}

