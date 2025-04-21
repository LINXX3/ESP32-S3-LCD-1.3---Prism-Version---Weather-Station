#include <TFT_eSPI.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <time.h>

// Initialize display and sprite
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);

// WiFi and API configuration
const char* ssid = "YOUR_SSID"; // WiFi network name
const char* password = "YOUR_PASSWORD"; // WiFi password
const char* apiKey = "YOUR_API_KEY"; // API key for weather data
const char* city = "YOUR_CITY"; // City for weather data
const char* country = "YOUR_COUNTRY"; // Country code
const char* apiUrl = "http://api.openweathermap.org/data/2.5/weather?q=%s,%s&appid=%s&units=metric";

// Variables for weather data
String weatherDescription = "Loading..."; // Description of the weather
float temperature = 0.0; // Current temperature
float targetTemperature = 0.0; // Target temperature for smooth animation
int humidity = 0; // Current humidity
float windSpeed = 0.0; // Current wind speed
String weatherIcon = "clear"; // Weather icon type
float animationOffset = 0; // Animation state

void setup() {
  // Initialize serial communication and display
  Serial.begin(115200);
  tft.init();
  tft.setRotation(4); // Set display orientation
  tft.fillScreen(TFT_BLACK);

  // Configure the sprite
  sprite.setColorDepth(16); // Set sprite color depth
  sprite.createSprite(240, 240); // Create a sprite with display dimensions

  // Connect to WiFi and synchronize time
  connectToWiFi();
  configTime(3600, 3600, "pool.ntp.org", "time.nist.gov");
  fetchWeatherData(); // Get initial weather data
}

void loop() {
  static unsigned long lastUpdate = 0;

  // Update weather data every 10 minutes
  if (millis() - lastUpdate > 600000) {
    fetchWeatherData();
    lastUpdate = millis();
  }

  // Update animation state
  animationOffset += 6.0; 
  if (animationOffset > 360) animationOffset = 0;

  // Smooth temperature transition
  temperature = temperature + (targetTemperature - temperature) * 0.1;

  drawWeatherDisplay(); // Render weather display
  delay(50); // Small delay for smoother updates
}

void connectToWiFi() {
  // Display connection progress on the screen
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(10, 10);
  tft.println("Connecting to WiFi...");

  // Attempt to connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
}

void fetchWeatherData() {
  // Create the API request URL
  char url[256];
  snprintf(url, sizeof(url), apiUrl, city, country, apiKey);

  // Send the HTTP GET request
  HTTPClient http;
  http.begin(url);
  int httpResponseCode = http.GET();

  // Process the HTTP response
  if (httpResponseCode == 200) {
    String payload = http.getString();
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, payload);

    // Extract weather data from the JSON response
    targetTemperature = doc["main"]["temp"];
    humidity = doc["main"]["humidity"];
    windSpeed = doc["wind"]["speed"];
    weatherDescription = doc["weather"][0]["description"].as<const char*>();

    // Map weather condition to icon type
    String weatherMain = doc["weather"][0]["main"].as<const char*>();
    if (weatherMain == "Clear") weatherIcon = "sun";
    else if (weatherMain == "Clouds") weatherIcon = "cloud";
    else if (weatherMain == "Rain") weatherIcon = "rain";
    else if (weatherMain == "Snow") weatherIcon = "snow";
    else weatherIcon = "unknown";

    // Log the extracted data to the serial console
    Serial.printf("Temp: %.1f °C, RH: %d %% Wind: %.1f m/s Weather: %s\n", 
                  targetTemperature, humidity, windSpeed, weatherDescription.c_str());
  } else {
    // Log an error if the request fails
    Serial.printf("Error fetching weather data: %d\n", httpResponseCode);
  }

  http.end(); // Close the HTTP connection
}

void drawWeatherDisplay() {
  // Clear the sprite background
  sprite.fillSprite(TFT_BLACK);

  // Display date and time
  sprite.setTextFont(4);
  sprite.setTextColor(TFT_WHITE, TFT_BLACK);
  sprite.setCursor(10, 10);
  sprite.println(getCurrentDateTime());

  // Draw a horizontal divider
  sprite.drawLine(0, 40, 240, 40, TFT_DARKGREY);

  // Display city name
  sprite.setTextFont(4);
  sprite.setTextColor(TFT_CYAN, TFT_BLACK);
  sprite.setCursor(10, 50); 
  sprite.println(city);

  // Display temperature
  sprite.setTextFont(7);
  sprite.setTextColor(TFT_YELLOW, TFT_BLACK);
  sprite.setCursor(0, 85); 
  sprite.printf("%.1f", temperature);

  // Draw degree symbol and 'C'
  int xCircle = 120;
  int yCircle = 120;
  sprite.drawCircle(xCircle, yCircle, 5, TFT_YELLOW);
  sprite.setTextFont(4);
  sprite.setTextColor(TFT_YELLOW, TFT_BLACK);
  sprite.setCursor(xCircle + 10, yCircle - 6);
  sprite.print("C");

  // Draw another horizontal divider
  sprite.drawLine(0, 140, 240, 140, TFT_DARKGREY);

  // Display humidity and wind speed
  sprite.setTextFont(4);
  sprite.setTextColor(TFT_WHITE, TFT_BLACK);
  sprite.setCursor(10, 150);
  sprite.printf("Humidity: %d %%", humidity);
  sprite.setCursor(10, 180); 
  sprite.printf("Wind: %.1f m/s", windSpeed);

  // Display weather icon with animation
  drawWeatherIcon(weatherIcon, 190, 95 + sin(animationOffset * DEG_TO_RAD) * 21);

  // Push the sprite to the display
  sprite.pushSprite(0, 0);
}

void drawWeatherIcon(String icon, int x, int y) {
  // Draws the weather icon based on the given type
  if (icon == "sun") {
    int radius = 20; // Radius of the sun
    int lineLength = 30; // Length of the sun rays
    sprite.fillCircle(x, y, radius, TFT_YELLOW); // Draw the sun's core
    for (int i = 0; i < 360; i += 45) { // Draw sun rays
      int dx = cos(radians(i)) * lineLength;
      int dy = sin(radians(i)) * lineLength;
      sprite.drawLine(x, y, x + dx, y + dy, TFT_YELLOW);
    }
  } else if (icon == "cloud") {
    int cloudRadius = 20; // Radius for cloud parts
    sprite.fillCircle(x - 10, y, cloudRadius, TFT_WHITE); // Draw left cloud circle
    sprite.fillCircle(x + 10, y, cloudRadius, TFT_WHITE); // Draw right cloud circle
    sprite.fillRect(x - 20, y, 40, 10, TFT_WHITE); // Connect circles with a rectangle
  } else if (icon == "rain") {
    int dropSize = 12; // Size of raindrops
    sprite.fillCircle(x, y, dropSize, TFT_WHITE); // Draw raindrop head
    for (int i = 0; i < 5; i++) { // Draw falling raindrops
      sprite.drawLine(x - 10 + i * 5, y + 10, x - 5 + i * 5, y + 20, TFT_BLUE);
    }
  } else if (icon == "snow") {
    int snowflakeSize = 20; // Radius for snowflake
    sprite.fillCircle(x, y, snowflakeSize, TFT_WHITE); // Draw snowflake center
    for (int i = 0; i < 360; i += 60) { // Draw snowflake arms
      int dx = cos(radians(i)) * snowflakeSize;
      int dy = sin(radians(i)) * snowflakeSize;
      sprite.drawLine(x, y, x + dx, y + dy, TFT_WHITE);
    }
  } else {
    // Display a placeholder if the icon type is unknown
    sprite.setTextFont(1);
    sprite.setCursor(x, y);
    sprite.print("?");
  }
}

String getCurrentDateTime() {
  // Fetches and formats the current date and time
  struct tm timeInfo;
  if (!getLocalTime(&timeInfo)) {
    return "N/A"; // Return "N/A" if time is unavailable
  }

  char dateTimeStr[20]; // Buffer for formatted date and time
  strftime(dateTimeStr, sizeof(dateTimeStr), "%A     %H:%M:%S", &timeInfo); 
  return String(dateTimeStr); // Return the formatted date and time string
}
