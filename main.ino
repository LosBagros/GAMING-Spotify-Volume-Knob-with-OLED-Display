#if defined(ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WiFi.h>
#endif
#include <WiFiClientSecure.h>
#include <Adafruit_NeoPixel.h>
#include <SpotifyArduino.h>
#include <SpotifyArduinoCert.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <Wire.h>
#include <SSD1306.h>


SSD1306 display(0x3C, D2, D1);

const int ledPin = D3;
const int numLeds = 24;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(numLeds, ledPin, NEO_GRB + NEO_KHZ800);

#include credentials.h

// Rotary encoder not implemented
// 

WiFiClientSecure client;
SpotifyArduino spotify(client, clientId, clientSecret, SPOTIFY_REFRESH_TOKEN);

unsigned long requestDueTime;
unsigned long delayBetweenRefresh;
unsigned long lastMillisCheck;
bool isOnEnd = true;
bool isPlaying = false;
int status = 0;
float percentage;
long progressMs = 0;
long durationMs = 1;

void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void displayCurrentlyPlaying(CurrentlyPlaying currentlyPlaying) {
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  if (status != 200) {
    display.drawString(0, 0, "Nothing playing");
    display.display();
    return;
  }
  isPlaying = currentlyPlaying.isPlaying;

  display.drawString(0, 0, currentlyPlaying.trackName);
  display.drawString(0, 16, currentlyPlaying.artists[0].artistName);
  display.drawString(0, 32, currentlyPlaying.albumName);

  if (isPlaying) {
  delayBetweenRefresh += millis() - lastMillisCheck;
  lastMillisCheck = millis();
  } else {
  lastMillisCheck = millis();
  }

  durationMs = currentlyPlaying.durationMs;
  progressMs = currentlyPlaying.progressMs;

  percentage = (((float)currentlyPlaying.progressMs + delayBetweenRefresh) / (float)currentlyPlaying.durationMs) * 100;
  if (percentage > 100) {
    isOnEnd = true;
  }
  isPlaying = currentlyPlaying.isPlaying;
  display.drawProgressBar(1, 50, 126, 10, (int)percentage);

  display.display();
}

void displayStatus() {
  if (isPlaying) {
  delayBetweenRefresh += millis() - lastMillisCheck;
  lastMillisCheck = millis();
  } else {
  lastMillisCheck = millis();
  }

  percentage = (((float)progressMs + delayBetweenRefresh) / (float)durationMs) * 100;
  if (percentage > 100) {
    isOnEnd = true;
  }
  
  display.drawProgressBar(1, 50, 126, 10, (int)percentage);

  display.display();
}

void setup() {
  Serial.begin(115200);
  strip.begin();
  strip.setBrightness(250);
  randomSeed(analogRead(0));

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Serial.println("Connected to WIFI!");

#if defined(ESP8266)
  client.setFingerprint(SPOTIFY_FINGERPRINT);
#elif defined(ESP32)
  client.setCACert(spotify_server_cert);
#endif

  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
}

void loop() {

  if ((millis() > requestDueTime) || (isOnEnd)) {
    isOnEnd = false;

    delayBetweenRefresh = 0;
    status = spotify.getCurrentlyPlaying(displayCurrentlyPlaying, SPOTIFY_MARKET);

    requestDueTime = millis() + delayBetweenRequests;
  }

  int red = random(256);    // 0 to 255
  int green = random(256);  // 0 to 255
  int blue = random(256);   // 0 to 255
  colorWipe(strip.Color(red, green, blue), 50);

  delay(1000);
  displayStatus();
}