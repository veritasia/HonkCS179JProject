#include "esp_camera.h"
#include "esp_http_server.h"
#include "esp_timer.h"
#include "WiFi.h"

// WiFi credentials
const char* ssid = "WIFINAME";
const char* password = "PASSWORD";

// Camera pins for ESP32-S3-WROOM-1
#define CAM_PIN_PWDN 38
#define CAM_PIN_RESET -1
#define CAM_PIN_VSYNC 6
#define CAM_PIN_HREF 7
#define CAM_PIN_PCLK 13
#define CAM_PIN_XCLK 15
#define CAM_PIN_SIOD 4
#define CAM_PIN_SIOC 5
#define CAM_PIN_D0 11
#define CAM_PIN_D1 9
#define CAM_PIN_D2 8
#define CAM_PIN_D3 10
#define CAM_PIN_D4 12
#define CAM_PIN_D5 18
#define CAM_PIN_D6 17
#define CAM_PIN_D7 16

// Stream settings
#define PART_BOUNDARY "123456789000000000000987654321"
static const char* _STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;boundary=" PART_BOUNDARY;
static const char* _STREAM_BOUNDARY = "\r\n--" PART_BOUNDARY "\r\n";
static const char* _STREAM_PART = "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n";

httpd_handle_t stream_httpd = NULL;

// Configure camera
bool setupCamera() {
    camera_config_t config;
    config.pin_pwdn = CAM_PIN_PWDN,
    config.pin_reset = CAM_PIN_RESET,
    config.pin_xclk = CAM_PIN_XCLK,
    config.pin_sccb_sda = CAM_PIN_SIOD,
    config.pin_sccb_scl = CAM_PIN_SIOC,

    config.pin_d7 = CAM_PIN_D7,
    config.pin_d6 = CAM_PIN_D6,
    config.pin_d5 = CAM_PIN_D5,
    config.pin_d4 = CAM_PIN_D4,
    config.pin_d3 = CAM_PIN_D3,
    config.pin_d2 = CAM_PIN_D2,
    config.pin_d1 = CAM_PIN_D1,
    config.pin_d0 = CAM_PIN_D0,
    config.pin_vsync = CAM_PIN_VSYNC,
    config.pin_href = CAM_PIN_HREF,
    config.pin_pclk = CAM_PIN_PCLK,
    config.xclk_freq_hz = 20000000;
    config.frame_size = FRAMESIZE_QVGA;
    config.pixel_format = PIXFORMAT_JPEG;
    config.grab_mode = CAMERA_GRAB_LATEST;
    config.fb_location = CAMERA_FB_IN_DRAM;
    config.jpeg_quality = 12;
    config.fb_count = 2;

    // Initialize camera
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        Serial.printf("Camera init failed with error 0x%x", err);
        return false;
    }
    return true;
}

// Stream handler
static esp_err_t stream_handler(httpd_req_t *req) {
    camera_fb_t * fb = NULL;
    esp_err_t res = ESP_OK;
    size_t _jpg_buf_len = 0;
    uint8_t * _jpg_buf = NULL;
    char * part_buf[64];

    res = httpd_resp_set_type(req, _STREAM_CONTENT_TYPE);
    if(res != ESP_OK) {
        return res;
    }

    while(true) {
        fb = esp_camera_fb_get();
        if (!fb) {
            Serial.println("Camera capture failed");
            res = ESP_FAIL;
        } else {
            _jpg_buf_len = fb->len;
            _jpg_buf = fb->buf;
        }
        if(res == ESP_OK) {
            size_t hlen = snprintf((char *)part_buf, 64, _STREAM_PART, _jpg_buf_len);
            res = httpd_resp_send_chunk(req, (const char *)part_buf, hlen);
        }
        if(res == ESP_OK) {
            res = httpd_resp_send_chunk(req, (const char *)_jpg_buf, _jpg_buf_len);
        }
        if(res == ESP_OK) {
            res = httpd_resp_send_chunk(req, _STREAM_BOUNDARY, strlen(_STREAM_BOUNDARY));
        }
        if(fb) {
            esp_camera_fb_return(fb);
            fb = NULL;
            _jpg_buf = NULL;
        }
        if(res != ESP_OK) {
            break;
        }
    }
    return res;
}

void startCameraServer() {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.server_port = 80;
    
    httpd_uri_t index_uri = {
        .uri       = "/",
        .method    = HTTP_GET,
        .handler   = stream_handler,
        .user_ctx  = NULL
    };

    if (httpd_start(&stream_httpd, &config) == ESP_OK) {
        httpd_register_uri_handler(stream_httpd, &index_uri);
        Serial.println("Camera stream server started");
    }
}

void setup() {
    Serial.begin(115200);
    
    // Initialize WiFi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.print("Camera Stream Ready! Go to: http://");
    Serial.println(WiFi.localIP());

    // Initialize camera
    if(setupCamera()) {
        startCameraServer();
    }
}

void loop() {
    // The main streaming work is done in the HTTP handler
    delay(1);
}