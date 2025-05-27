#include <Wire.h>
#include <Adafruit_AHTX0.h>   
#include <TFT_eSPI.h>         
#include <Button2.h>          

#define SDA_PIN        21
#define SCL_PIN        22

#define LED_R_PIN      25
#define LED_G_PIN      26
#define LED_B_PIN      27

#define LED_R_CHANNEL 0    
#define LED_G_CHANNEL 1     
#define LED_B_CHANNEL 2     

#define BUTTON_UNIT_PIN  0     

#define PWM_FREQ 5000       
#define PWM_RESOLUTION 8    

Adafruit_AHTX0 aht;
TFT_eSPI       tft = TFT_eSPI();
Button2        btnUnit(BUTTON_UNIT_PIN);

bool useCelsius = true;        
uint32_t lastReadMs = 0;       
const uint32_t READ_INTERVAL = 5000;   

void setup() {
  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  Wire.begin(SDA_PIN, SCL_PIN);
  if (!aht.begin()) {
    tft.println("AHT20 未偵測到");
    while (1) delay(10);
  }
  tft.println("AHT20 OK");
  delay(1500);
  tft.fillScreen(TFT_BLACK);

  ledcSetup(LED_R_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
  ledcSetup(LED_G_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
  ledcSetup(LED_B_CHANNEL, PWM_FREQ, PWM_RESOLUTION);

  ledcAttachPin(LED_R_PIN, LED_R_CHANNEL);
  ledcAttachPin(LED_G_PIN, LED_G_CHANNEL);
  ledcAttachPin(LED_B_PIN, LED_B_CHANNEL);

  setLedColor(0, 0, 0);        

  btnUnit.setPressedHandler([](Button2 &){
    useCelsius = !useCelsius;  
  });
}

void loop() {
  btnUnit.loop();              
  uint32_t now = millis();
  if (now - lastReadMs >= READ_INTERVAL) {
    lastReadMs = now;

    sensors_event_t humidity, temp;
    aht.getEvent(&humidity, &temp);
    float tC  = temp.temperature;
    float hum = humidity.relative_humidity;

    float tDisp = useCelsius ? tC : tC * 1.8 + 32.0;
    char  unit  = useCelsius ? 'C' : 'F';

    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, 30);
    tft.printf("Temp: %.1f%c\n", tDisp, unit);
    tft.setCursor(10, 70);
    tft.printf("Humi: %.1f%%", hum);

    if (hum < 70) {
      setLedColor(0, 0, 255);   
    } else if (hum <= 80) {
      setLedColor(0, 255, 0);      
    } else {
      setLedColor(255, 0, 0);      
    }
  }
}

void setLedColor(uint8_t r, uint8_t g, uint8_t b) {
  ledcWrite(LED_R_CHANNEL, r);
  ledcWrite(LED_G_CHANNEL, g);
  ledcWrite(LED_B_CHANNEL, b);
}