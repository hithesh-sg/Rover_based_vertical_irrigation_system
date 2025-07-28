#include <esp_now.h>
#include <WiFi.h>

/* Definitions */
#define ESPNOW_WIFI_CHANNEL 6

int received_value = 100;

#define m11 23
#define m12 22
#define m21 21
#define m22 19
#define pump 5

#define edgeL 13
#define edgeR 12

/* Callback function to handle incoming data */
void onDataReceive(const esp_now_recv_info *recv_info, const uint8_t *data, int data_len) {
  // Check if the received data length matches the size of an integer
  if (data_len == sizeof(int)) {
    // Copy the received data into the integer variable
    memcpy(&received_value, data, sizeof(received_value));

    // Print the received integer value to the serial monitor
    Serial.printf("Received integer value from sender: %d\n", received_value);
  } else {
    Serial.println("Received data size does not match expected integer size.");
  }
}

/* Main */
void setup() {
  Serial.begin(115200);

  // Initialize the Wi-Fi module in station mode
  WiFi.mode(WIFI_STA);
  WiFi.setChannel(ESPNOW_WIFI_CHANNEL);

  Serial.println("ESP-NOW Example - Broadcast Receiver");
  Serial.println("Wi-Fi parameters:");
  Serial.println("  Mode: STA");
  Serial.println("  MAC Address: " + WiFi.macAddress());
  Serial.printf("  Channel: %d\n", ESPNOW_WIFI_CHANNEL);

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Failed to initialize ESP-NOW");
    Serial.println("Rebooting in 5 seconds...");
    delay(5000);
    ESP.restart();
  }

  // Register the receive callback function
  esp_now_register_recv_cb(onDataReceive);

  Serial.println("Setup complete. Waiting for messages...");

  pinMode(m11, OUTPUT);
  pinMode(m12, OUTPUT);
  pinMode(m21, OUTPUT);
  pinMode(m22, OUTPUT);
  pinMode(pump, OUTPUT);
  pinMode(edgeL, INPUT);
  pinMode(edgeR, INPUT);
  digitalWrite(m11, LOW);
  digitalWrite(m12, LOW);
  digitalWrite(m21, LOW);
  digitalWrite(m22, LOW);
  digitalWrite(pump, LOW);
  delay(500);
}

void loop() {
  if (received_value < 30) {
    start();
    Serial.println("Start");
    delay(200);
  } else {
    stop();
    Serial.println("Stop");
  }
  delay(500);
}

void start() {
  if (digitalRead(edgeL) == LOW && digitalRead(edgeR) == LOW) {
    forward();
  } else if (digitalRead(edgeL) == HIGH && digitalRead(edgeR) == HIGH) {
    halt();
  } else if (digitalRead(edgeL) == HIGH) {
    left();
    delay(200);
  } else if (digitalRead(edgeR) == HIGH) {
    right();
    delay(200);
  }
}

void forward() {
  digitalWrite(m11, LOW);
  digitalWrite(m12, HIGH);
  digitalWrite(m21, LOW);
  digitalWrite(m22, HIGH);
}

void backward() {
  digitalWrite(m11, HIGH);
  digitalWrite(m12, LOW);
  digitalWrite(m21, HIGH);
  digitalWrite(m22, LOW);
}

void left() {
  digitalWrite(m11, LOW);
  digitalWrite(m12, HIGH);
  digitalWrite(m21, HIGH);
  digitalWrite(m22, LOW);
}

void right() {
  digitalWrite(m11, HIGH);
  digitalWrite(m12, LOW);
  digitalWrite(m21, LOW);
  digitalWrite(m22, HIGH);
}

void halt() {
  digitalWrite(m11, LOW);
  digitalWrite(m12, LOW);
  digitalWrite(m21, LOW);
  digitalWrite(m22, LOW);
  digitalWrite(pump, HIGH);
  delay(5000);
  digitalWrite(pump, LOW);
}
void stop() {
  digitalWrite(m11, LOW);
  digitalWrite(m12, LOW);
  digitalWrite(m21, LOW);
  digitalWrite(m22, LOW);
  digitalWrite(pump, LOW);
}