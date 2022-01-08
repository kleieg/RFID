#include <Arduino.h>

#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

void setup() {
  
  Serial.begin(9600);
  Serial.println("Adafruit MLX90614 test"); 
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 init failed"));
    for(;;);
  }
  

  display.clearDisplay();
  display.setRotation(2);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 35);
  display.println("Init...");
  display.display();
  delay(500);
  display.clearDisplay();

  mlx.begin(); 
  
}

void loop() {

    display.clearDisplay();

    display.setTextSize(3);
    display.setCursor(5, 27);
    display.print(mlx.readObjectTempC()); //IR-Temperatur
    display.setCursor(100, 27);
    display.print("C");
    display.display();


  Serial.print("Umgebungstemperatur: ");
  Serial.println(mlx.readAmbientTempC());
  Serial.print("IR-Temperatur: ");
  Serial.println(mlx.readObjectTempC());
  delay(1000);
}
