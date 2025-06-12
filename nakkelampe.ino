// Denne koden brukes for en prototype laget av gruppe Viva i emne IN1060 ved UiO. 
// Den bruker Adafruit NeoPixel-biblioteket til å styre lys, og simulerer batterinivå over tid. 

#include <Adafruit_NeoPixel.h> // Inkluderer bibliotek for å styre NeoPixel LED-striper

// Definerer hvilke pinner som er koblet til hvilke LED-striper
#define LEFT_LED_PIN 6
#define RIGHT_LED_PIN 5
#define BATTERY_LED_PIN 2

// Antall piksler på LED-stripene
#define LED_NUMPIXELS 2

#define DIMMER_PIN A0 // Analog pin koblet til potensiometer

// Oppretter tre NeoPixel-objekter (to for lys, én for batteriindikator)
Adafruit_NeoPixel left_strip(LED_NUMPIXELS, LEFT_LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel right_strip(LED_NUMPIXELS, RIGHT_LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel battery_strip(1, BATTERY_LED_PIN, NEO_GRB + NEO_KHZ800);


// Variabler for å simulere batterinivå
int batteryPercent = 100;               // Starter på 100 % batteri
unsigned long lastUpdate = 0;           // Tidspunkt for forrige oppdatering
const unsigned long interval = 7500;    // Hvor ofte batteriet skal synke (i millisekunder)

bool lysErPaa = false; // Brukes for å sjekke om lyset er på


void setup() {
  // Initialiserer alle LED-stripene
  left_strip.begin();
  right_strip.begin();
  battery_strip.begin();
}

void loop() {
  // Leser analog verdi fra potensiometer og konverterer til lysstyrke (0–255)
  int potValue = analogRead(DIMMER_PIN);
  int brightness = map(potValue, 0, 1023, 0, 255);

  // Håndterer fading av lys basert på potensiometerverdi og status
  if (potValue < 10 && lysErPaa){ // Hvis potensiometer er nær null og lyset er på, fade ut
    fadeOut(brightness);
    lysErPaa = false;

  }else if (potValue >= 10 && !lysErPaa){ // Hvis potensiometer skrus opp og lyset er av, fade inn
    fadeIn(brightness);
    lysErPaa = true;

  }else if (lysErPaa){  // Hvis lyset allerede er på, oppdater lysstyrken kontinuerlig
    left_strip.setBrightness(brightness);
    right_strip.setBrightness(brightness);
    for (int i = 0; i < LED_NUMPIXELS; i++) {
      left_strip.setPixelColor(i, left_strip.Color(255,255,255));
      right_strip.setPixelColor(i, right_strip.Color(255,255,255));
    }
    left_strip.show();
    right_strip.show();
  }

  
  // Simulerer at batteriet tømmes over tid
  if(millis() - lastUpdate > interval && batteryPercent > 0){
    batteryPercent --;
    lastUpdate = millis();  // Oppdater tid for siste endring

  }

  // Velger riktig farge på batteriindikator basert på gjenværende prosent
  uint32_t batteryColor; 
  if(batteryPercent > 60){
    batteryColor = battery_strip.Color(0,255,0); // Grønn hvis bra batteri
  }else if(batteryPercent > 20){
    batteryColor = battery_strip.Color(255,255,0); // Gul hvis mellomnivå
  }else{
    batteryColor = battery_strip.Color(255,0,0); // Rødt hvis lavt batteri
  }

  // Oppdaterer batteriindikator med valgt farge og lysstyrke
  battery_strip.setBrightness(50);
  battery_strip.setPixelColor(0, batteryColor);
  battery_strip.show();


  delay(10); // Kort pause for å stabilisere loop
}


// Funksjon som gradvis øker lysstyrken (fade in)
void fadeIn(int maxBrightness){
  for (int b = 0; b <= maxBrightness; b += 2) {
    left_strip.setBrightness(b);
    right_strip.setBrightness(b);
    for (int i = 0; i < LED_NUMPIXELS; i++) {
      left_strip.setPixelColor(i, left_strip.Color(255,255,255));
      right_strip.setPixelColor(i, right_strip.Color(255,255,255));
    }
    left_strip.show();
    right_strip.show();

    delay(10);
  }
}

// Funksjon som gradvis senker lysstyrken (fade out)
void fadeOut(int currentBrightness) {
  for (int b = currentBrightness; b > 0; b -= 2) {
    left_strip.setBrightness(b);
    right_strip.setBrightness(b);
    for (int i = 0; i < LED_NUMPIXELS; i++) {
      left_strip.setPixelColor(i, left_strip.Color(255,255,255));
      right_strip.setPixelColor(i, right_strip.Color(255,255,255));
    }
    left_strip.show();
    right_strip.show();
    delay(10);
  }

  // Slukk alle piksler helt etter fading
  for (int i = 0; i < LED_NUMPIXELS; i++) {
    left_strip.setPixelColor(i, left_strip.Color(0,0,0));
    right_strip.setPixelColor(i, right_strip.Color(0,0,0));
  }
  left_strip.show();
  right_strip.show();
  
}

