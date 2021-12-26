#include "WebInterface.hpp"

#include <esp_http_server.h>
#include <esp_log.h>
#include "esp_vfs.h"
#include <board.h>
#include "periph_sdcard.h"
#include <errno.h>

#include <fcntl.h>
#include "esp_system.h"

static const char *TAG = "WebInterface";

#define FILE_PATH_MAX (ESP_VFS_PATH_MAX + 128)
#define SCRATCH_BUFSIZE (10240)

#define CHECK_FILE_EXTENSION(filename, ext) (strcasecmp(&filename[strlen(filename) - strlen(ext)], ext) == 0)

typedef struct rest_server_context {
	char base_path[ESP_VFS_PATH_MAX + 1];
	char scratch[SCRATCH_BUFSIZE];
} rest_server_context_t;

WebInterface::WebInterface() {

}

void WebInterface::start() {
	initSD();
	server = start_webserver();
}

void WebInterface::finishResourceRegistrations() {
	registerFileServer();
}

void WebInterface::initSD() {
	esp_periph_config_t periph_cfg = DEFAULT_ESP_PERIPH_SET_CONFIG();
	esp_periph_set_handle_t set = esp_periph_set_init(&periph_cfg);

	// Initialize SD Card peripheral
	if (audio_board_sdcard_init(set, SD_MODE_4_LINE) == ESP_FAIL) {
		ESP_LOGE(TAG, "Sdcard not mounted");
	} else {
		ESP_LOGI(TAG, "SDCard mounted");
	}
}

/* Set HTTP response content type according to file extension */
esp_err_t set_content_type_from_file(httpd_req_t *req, const char *filepath)
{
	const char *type = "text/plain";
	if (CHECK_FILE_EXTENSION(filepath, ".html")) {
		type = "text/html";
	} else if (CHECK_FILE_EXTENSION(filepath, ".js")) {
		type = "application/javascript";
	} else if (CHECK_FILE_EXTENSION(filepath, ".css")) {
		type = "text/css";
	} else if (CHECK_FILE_EXTENSION(filepath, ".png")) {
		type = "image/png";
	} else if (CHECK_FILE_EXTENSION(filepath, ".ico")) {
		type = "image/x-icon";
	} else if (CHECK_FILE_EXTENSION(filepath, ".svg")) {
		type = "text/xml";
	}
	return httpd_resp_set_type(req, type);
}

/* Send HTTP response with the contents of the requested file */
esp_err_t rest_common_get_handler(httpd_req_t *req)
{
	char filepath[FILE_PATH_MAX];

	rest_server_context_t *rest_context = (rest_server_context_t *)req->user_ctx;
	strlcpy(filepath, rest_context->base_path, sizeof(filepath));

	if (req->uri[strlen(req->uri) - 1] == '/') {
		strlcat(filepath, "/index.html", sizeof(filepath));
	} else {
		strlcat(filepath, req->uri, sizeof(filepath));
	}

	int fd = open(filepath, O_RDONLY);
	if (fd == -1) {
		ESP_LOGE(TAG, "Failed to open file : %s. Errno: %d", filepath, errno);
		/* Respond with 500 Internal Server Error */

		char responseError[255];
		char* error_name;

		switch (errno)
		{
			case EACCES:
				error_name = "EACCESS";
				break;
			case ENOENT:
				error_name = "ENOENT";
				break;
			default:
				error_name = "not parsed";
		}

		sprintf(responseError, "Failed to read existing file. %d => %s", errno, error_name);

		httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, responseError);
		return ESP_FAIL;
	}

	set_content_type_from_file(req, filepath);

	char *chunk = rest_context->scratch;
	ssize_t read_bytes;
	do {
		/* Read file in chunks into the scratch buffer */
		read_bytes = read(fd, chunk, SCRATCH_BUFSIZE);
		if (read_bytes == -1) {
			ESP_LOGE(TAG, "Failed to read file : %s", filepath);
		} else if (read_bytes > 0) {
			/* Send the buffer contents as HTTP response chunk */
			if (httpd_resp_send_chunk(req, chunk, read_bytes) != ESP_OK) {
				close(fd);
				ESP_LOGE(TAG, "File sending failed!");
				/* Abort sending file */
				httpd_resp_sendstr_chunk(req, NULL);
				/* Respond with 500 Internal Server Error */
				httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to send file");
				return ESP_FAIL;
			}
		}
	} while (read_bytes > 0);
	/* Close file after sending complete */
	close(fd);
	ESP_LOGI(TAG, "File sending complete");
	/* Respond with an empty chunk to signal HTTP response completion */
	httpd_resp_send_chunk(req, NULL, 0);
	return ESP_OK;
}


httpd_handle_t WebInterface::start_webserver(void) {
	httpd_handle_t server = NULL;
	httpd_config_t config = HTTPD_DEFAULT_CONFIG();
	config.lru_purge_enable = true;
	config.uri_match_fn = httpd_uri_match_wildcard;

	// Start the httpd server
	ESP_LOGI(TAG, "Starting server on port: '%d'", config.server_port);
	if (httpd_start(&server, &config) == ESP_OK) {
		return server;
	}


	ESP_LOGI(TAG, "Error starting server!");
	return NULL;
}

void WebInterface::registerResource(httpd_uri_t* config) {
	ESP_LOGI(TAG, "Register resource on path %s", config->uri);
	httpd_register_uri_handler(server, config);
}

void WebInterface::registerFileServer() {
	rest_server_context_t *fileContext = (rest_server_context_t*)calloc(1, sizeof(rest_server_context_t));

	if (! fileContext) {
		ESP_LOGE(TAG, "No memory for rest context");
		return;
	}

	strlcpy(fileContext->base_path, "/sdcard/www", sizeof(fileContext->base_path));    

	/* URI handler for getting web server files */
	httpd_uri_t common_get_uri = {
		.uri = "/*",
		.method = HTTP_GET,
		.handler = rest_common_get_handler,
		.user_ctx = fileContext
	};
	
	registerResource(&common_get_uri);
}