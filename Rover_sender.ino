#include "ESP32_NOW.h"
#include "WiFi.h"
#include <esp_mac.h>  // For the MAC2STR and MACSTR macros

/* Definitions */

#define ESPNOW_WIFI_CHANNEL 6
#define SOIL_MOISTURE_PIN 34  // Define the pin where the soil moisture sensor is connected

#define MAX_MOISTURE_VALUE 4095  // Assuming the ADC resolution is 12-bit (0-4095)
#define MIN_MOISTURE_VALUE 0     // Minimum possible value from the sensor

/* Classes */

// Creating a new class that inherits from the ESP_NOW_Peer class is required.
class ESP_NOW_Broadcast_Peer : public ESP_NOW_Peer {
public:
  // Constructor of the class using the broadcast address
  ESP_NOW_Broadcast_Peer(uint8_t channel, wifi_interface_t iface, const uint8_t *lmk) : ESP_NOW_Peer(ESP_NOW.BROADCAST_ADDR, channel, iface, lmk) {}

  // Destructor of the class
  ~ESP_NOW_Broadcast_Peer() {
    remove();
  }

  // Function to properly initialize the ESP-NOW and register the broadcast peer
  bool begin() {
    if (!ESP_NOW.begin() || !add()) {
      log_e("Failed to initialize ESP-NOW or register the broadcast peer");
      return false;
    }
    return true;
  }

  // Function to send a message to all devices within the network
  bool send_message(const uint8_t *data, size_t len) {
    if (!send(data, len)) {
      log_e("Failed to broadcast message");
      return false;
    }
    return true;
  }
};

/* Global Variables */

uint32_t msg_count = 0;

// Create a broadcast peer object
ESP_NOW_Broadcast_Peer broadcast_peer(ESPNOW_WIFI_CHANNEL, WIFI_IF_STA, NULL);

/* Main */

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }

  // Initialize the Wi-Fi module
  WiFi.mode(WIFI_STA);
  WiFi.setChannel(ESPNOW_WIFI_CHANNEL);
  while (!WiFi.STA.started()) {
    delay(100);
  }

  Serial.println("ESP-NOW Example - Broadcast Master");
  Serial.println("Wi-Fi parameters:");
  Serial.println("  Mode: STA");
  Serial.println("  MAC Address: " + WiFi.macAddress());
  Serial.printf("  Channel: %d\n", ESPNOW_WIFI_CHANNEL);

  // Register the broadcast peer
  if (!broadcast_peer.begin()) {
    Serial.println("Failed to initialize broadcast peer");
    Serial.println("Rebooting in 5 seconds...");
    delay(5000);
    ESP.restart();
  }

  // Setup the soil moisture sensor pin
  pinMode(SOIL_MOISTURE_PIN, INPUT);

  Serial.println("Setup complete. Monitoring soil moisture and broadcasting status.");
}

void loop() {
  // Read the soil moisture level
  int soil_moisture_value = analogRead(SOIL_MOISTURE_PIN);
  
  // Convert the raw reading to a percentage
  int moisture_percentage = map(soil_moisture_value, MIN_MOISTURE_VALUE, MAX_MOISTURE_VALUE, 100, 0);
  moisture_percentage = constrain(moisture_percentage, 0, 100);  // Ensure the percentage is within 0-100%
  Serial.printf("Soil Moisture Value: %d%%\n", moisture_percentage);

  // Broadcast the integer value directly
  Serial.printf("Broadcasting integer value: %d\n", moisture_percentage);

  // Send the moisture percentage as an integer
  if (!broadcast_peer.send_message((uint8_t *)&moisture_percentage, sizeof(moisture_percentage))) {
    Serial.println("Failed to broadcast message");
  }

  // Wait before sending the next message
  delay(5000);
}
