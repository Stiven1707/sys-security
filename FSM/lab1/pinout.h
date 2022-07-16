#define LED_RED_PIN 10  
#define LED_GREEN_PIN 9  
#define LED_BLUE_PIN 8  
#define DHT11_PIN 5
#define BUZZER_PASIVO 7
#define NOTE_FS6 1480
#define NOTE_G6  1568  

int melodia[] = {
  NOTE_FS6, NOTE_G6
};

int duraciones[] = {    // array con la duracion de cada nota
  8, 8
};

const byte TH_TEMP_LOW =25;
const byte TH_TEMP_NORM = 29;
const byte TH_TEMP_HIGH = 30;
