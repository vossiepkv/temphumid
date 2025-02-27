#include <dht11.h>
#include <SPI.h>
#include <WiFiNINA.h>
#include "ThingSpeak.h"

#define DHT11PIN 2

dht11 DHT11;

#include "Secret.h" 
// Please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // Your network SSID (name)
char pass[] = SECRET_PASS;    // Your network password (use for WPA, or use as key for WEP)
int status = WL_IDLE_STATUS;     // The WiFi radio's status
WiFiClient  client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

void setup() {
  Serial.begin(9600);
  // Initialize ThingSpeak
  ThingSpeak.begin(client);

  // Attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);
    // Wait 10 seconds for connection:
    delay(10000);
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  // Check WiFi connection
  // (existing code to check and reconnect if necessary)

  Serial.println("Reading from DHT11...");
  int chk = DHT11.read(DHT11PIN); // Read data from the DHT11 sensor
  float temperatureC = DHT11.temperature; // Get the temperature in Celsius
  float humidity = DHT11.humidity; // Get the humidity

  // Print temperature and humidity to the serial monitor
  Serial.print("Temperature: ");
  Serial.print(temperatureC);
  Serial.print("C, ");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println("%");

  // Update ThingSpeak fields with the actual values
  ThingSpeak.setField(1, temperatureC);
  ThingSpeak.setField(2, humidity);


  // Write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if (x == 200) {
    Serial.println("Channel update successful.");
  } else {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }

  // Delay before the next read
  delay(20000); // Delay adjusted to 20 seconds to avoid hitting ThingSpeak's rate limit
}