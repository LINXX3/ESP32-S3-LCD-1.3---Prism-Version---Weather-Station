# Weather Display with ESP32 and TFT Display by Niklas Heinzel

This project demonstrates a weather information display using an ESP32 microcontroller and a TFT screen. It fetches real-time weather data from the OpenWeatherMap API and displays temperature, humidity, wind speed, and an animated weather icon on the screen. 

![1745228095133](https://github.com/user-attachments/assets/ecc58a68-1beb-49a8-804b-dde1ff8e87c3)

## Features
- **Real-Time Weather Data**: Fetches temperature, humidity, wind speed, and weather conditions.
- **Animated Weather Icons**: Displays dynamic icons for sun, clouds, rain, and snow.
- **Smooth Transitions**: Provides smooth temperature updates with animation effects.
- **WiFi Connectivity**: Uses WiFi to connect to the OpenWeatherMap API.
- **Time Synchronization**: Syncs with an NTP server to display current date and time.

## Components
- **ESP32 Microcontroller**
- **TFT Display** (configured with the `TFT_eSPI` library)
- **WiFi Connection**
- **OpenWeatherMap API Key**

## Dependencies
This project uses the following libraries:
- [TFT_eSPI](https://github.com/Bodmer/TFT_eSPI)
- [WiFi](https://www.arduino.cc/en/Reference/WiFi)
- [HTTPClient](https://www.arduino.cc/reference/en/libraries/httpclient/)
- [ArduinoJson](https://arduinojson.org/)
- [time.h](https://www.arduino.cc/reference/en/language/functions/time/)

Install these libraries in your Arduino IDE before uploading the code.

## Configuration
1. Replace the placeholders in the code with your details:
   - `YOUR_SSID`: Your WiFi SSID.
   - `YOUR_PASSWORD`: Your WiFi password.
   - `YOUR_API_KEY`: Your OpenWeatherMap API key.
   - `YOUR_CITY`: The name of your city.
   - `YOUR_COUNTRY`: Your country code.

2. Adjust the TFT display settings in the `TFT_eSPI` library configuration file to match your hardware.

## Setup
1. Clone or download this repository.
2. Open the `WeatherDisplay.ino` file in the Arduino IDE.
3. Upload the code to your ESP32.
4. Connect the ESP32 to your TFT display and power it on.

## How It Works
1. The ESP32 connects to WiFi and synchronizes the time with an NTP server.
2. Weather data is fetched from the OpenWeatherMap API every 10 minutes.
3. The data is parsed and displayed on the TFT screen.
4. The screen shows:
   - Date and time.
   - Current temperature with a smooth transition effect.
   - Humidity and wind speed.
   - An animated icon representing the current weather.

## Troubleshooting
- **WiFi Connection Issues**: Ensure your WiFi credentials are correct and the ESP32 is in range of your router.
- **Incorrect Weather Data**: Verify your city, country code, and API key.
- **Display Problems**: Confirm the TFT display connections and `TFT_eSPI` configuration.
- **Boot-Loop-Problem**: Confirm that the board library published by Espressif is <3.0.0!

## License
This project is open-source and licensed under the MIT License. Feel free to modify and distribute the code.

## Acknowledgements
Special thanks to the developers of the libraries used in this project and Waveshare for sponsoring the boards! Their contributions made this project possible.

- Product Link: https://www.waveshare.com/esp32-s3-lcd-1.3.htm?&aff_id=Waveshare
- Wiki Link：http://www.waveshare.com/wiki/ESP32-S3-LCD-1.3
