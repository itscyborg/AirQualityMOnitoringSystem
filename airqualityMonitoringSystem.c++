#define BLYNK_TEMPLATE_ID "TMPL3FTAP_IXO"
#define BLYNK_TEMPLATE_NAME "Gas Detect"
#define BLYNK_AUTH_TOKEN "o7u643JUtdSih4Xc3tMyi_mOQedXbDtP"
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

 
#define SCREEN_WIDTH 128    // OLED display width, in pixels
#define SCREEN_HEIGHT 64    // OLED display height, in pixels
#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
 
#define DHTPIN D4
#define DHTTYPE DHT11
DHT dht(DHTPIN,DHTTYPE);

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Laptop";  // Enter your WiFi name
char pass[] = "12345678";  // Enter your WiFi password
int smokeA0 = A0;
int sensorThres = 100; // Adjust this threshold value as needed

BlynkTimer timer;

void sendSensor() {
 
  float h= dht.readHumidity();
  float t= dht.readTemperature();
   if (isnan(h) || isnan(t)){
    Serial.println("Failed to read DHT sensor");
    return;
   }

  int data = analogRead(smokeA0);
  Blynk.virtualWrite(V0, data);
  Serial.print("Gas Value: ");
  Serial.println(data);

  Blynk.virtualWrite(V1,t);
  Blynk.virtualWrite(V2,h);

  Serial.print("Temprature : ");
  Serial.println(t);

  Serial.print("Humidity : ");
  Serial.println(h);

  if (data > sensorThres) {
    Blynk.logEvent("pollution_alert", "Bad Air Quality");
  }
}

void setup() {
  pinMode(smokeA0, INPUT);

  Serial.begin(115200);

  Blynk.begin(auth, ssid, pass);
  dht.begin();

  timer.setInterval(2500L, sendSensor); // Adjust the interval as needed
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //initialize with the I2C addr 0x3C (128x64)
  display.clearDisplay();
  delay(10);
 
  Serial.println("Connecting to ");
  Serial.println(ssid);
  
  display.clearDisplay();
  display.setCursor(0,0);  
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println("Connecting to ");
  display.setTextSize(2);
  display.print(ssid);
  display.display();
  
  WiFi.begin(ssid, pass);
 
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
    Serial.println("");
    Serial.println("WiFi connected");
    
    display.clearDisplay();
    display.setCursor(0,0);  
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.print("WiFi connected");
    display.display();
    delay(4000);
}

void loop() {
  Blynk.run();
  timer.run();
  int data = analogRead(smokeA0);
  
  // Display Air Quality
  Serial.print("Air Quality: ");  
  Serial.print(data);
  Serial.println("  PPM");   
  display.clearDisplay();
  display.setCursor(0,0);  
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println("Air Quality Index");
  display.setCursor(0,20);  
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.print(data);
  display.setTextSize(1.8);
  display.setTextColor(WHITE);
  display.println(" PPM");
  display.setCursor(0,40); 
  display.setTextSize(1.8);
  if(data<120) {
    display.print("Fresh Air");
    Serial.println("Fresh Air");
  }
  else if (data>121 && data<245) {
    display.print("Moderate Air");
    Serial.println("Moderate Air");
  }
  else {
    display.print("Hazardous");
    Serial.println("Hazardous");
  }
  display.display();
  delay(3000);

  // Display Temperature
  float t = dht.readTemperature();
  display.clearDisplay();
  display.setCursor(0,0);  
  display.setTextSize(1.75);
  display.setTextColor(WHITE);
  display.println("Temperature ");
  display.setCursor(0,20);  
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.print(t);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.println("C");
  display.display();
  delay(3000);

  // Display Humidity
  float h = dht.readHumidity();
  display.clearDisplay();
  display.setCursor(0,0);  
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.println("Humidity ");
  display.setCursor(0,20);  
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.print(h);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println("%");
  display.display();
  delay(3000);
}