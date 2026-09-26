#include "esp_camera.h"
#include <WiFi.h>
#include <WiFiClientSecure.h>

const char* ssid = "";
const char* password = "";
String BOTtoken = "";
String chat_id = "";

#define TRIGGER_PIN 13   // From Arduino D8
#define CAMERA_MODEL_AI_THINKER

// --- AI Thinker Pin Definitions ---
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22
// --- End Pin Definitions ---

WiFiClientSecure client;

void setup() {
  Serial.begin(115200);
  pinMode(TRIGGER_PIN, INPUT);
  delay(100);

  // ---- Connect Wi-Fi ----
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");

  // ---- Start camera ----
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_VGA;
  config.jpeg_quality = 12;
  config.fb_count = 1;

  if (esp_camera_init(&config) != ESP_OK) {
    Serial.println("Camera init failed!");
    return;
  }

  client.setInsecure();
  Serial.println("ESP32-CAM ready!");
}

void loop() {
  if (digitalRead(TRIGGER_PIN) == HIGH) {
    captureAndSendPhoto();
    delay(3000);
  }
}

void captureAndSendPhoto() {
  camera_fb_t* fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed!");
    return;
  }

  if (!client.connect("api.telegram.org", 443)) {
    Serial.println("Telegram connection failed!");
    esp_camera_fb_return(fb);
    return;
  }

  String head = "--Random\r\nContent-Disposition: form-data; name=\"chat_id\"\r\n\r\n" + chat_id + "\r\n--Random\r\nContent-Disposition: form-data; name=\"photo\"; filename=\"photo.jpg\"\r\nContent-Type: image/jpeg\r\n\r\n";
  String tail = "\r\n--Random--\r\n";

  client.printf("POST /bot%s/sendPhoto HTTP/1.1\r\n", BOTtoken.c_str());
  client.println("Host: api.telegram.org");
  client.println("Content-Type: multipart/form-data; boundary=Random");
  client.print("Content-Length: ");
  client.println(head.length() + fb->len + tail.length());
  client.println();
  client.print(head);
  client.write(fb->buf, fb->len);
  client.print(tail);

  Serial.println("Photo sent!");

  esp_camera_fb_return(fb);
  client.stop();
}
