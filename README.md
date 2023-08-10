# GAMING Spotify Volume Knob with OLED Display

This is a simple project to control/display Spotify playback using a rotary encoder and an OLED display.

It's GAMING because it has a cool RGB LED ring around the encoder.

## Hardware
ESP8266 (I used a Wemos D1 Mini)
Rotary Encoder
SSD1306 OLED Display
WS2812 RGB LED Ring (24 Bit, but you can use any size you want, just change the code)


## Wiring

TODO

## Software

### Arduino IDE
Install the following libraries:
- WiFiClientSecure
- Adafruit NeoPixel
- SpotifyArduino
- ArduinoJson
- SSD1306

### Spotify

Create a Spotify application and add the following redirect URI: `http://localhost/callback`

In order to obtain Spotify app client id and secret key, user must create a new Spotify app with her own account:
 1. Go to https://developer.spotify.com/my-applications/#!/applications/create
 2. Fill the form. Set *Redirect URIs* to `http://localhost/callback` and other fields to values of your choice.
 3. When the app is created, copy *Client ID* and *Client Secret* values into the device.

Next, we need to obtain the authorization code:
 1. In the following URL replace <client_id> with your app client id and open the URL with your browser. `https://accounts.spotify.com/authorize/?client_id=<client_id>&response_type=code&redirect_uri=http%3A%2F%2Flocalhost%2Fcallback&scope=user-read-private%20user-read-currently-playing%20user-read-playback-state%20user-modify-playback-state`
 2. Authorize the app and you will be redirected to http://localhost/callback.
 3. Copy the code from the browser address bar. It is the authorization code.

 

### Configuration

Copy `credentialsExample.h` to `credentials.h` and fill in the required information.


