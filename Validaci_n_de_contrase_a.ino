/*
  LiquidCrystal Library - Hello World

  Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
  library works with all LCD displays that are compatible with the
  Hitachi HD44780 driver. There are many of them out there, and you
  can usually tell them by the 16-pin interface.

  This sketch prints "Hello World!" to the LCD
  and shows the time.

  The circuit:
   LCD RS pin to digital pin 12
   LCD Enable pin to digital pin 11
   LCD D4 pin to digital pin 31
   LCD D5 pin to digital pin 32
   LCD D6 pin to digital pin 33
   LCD D7 pin to digital pin 34
   LCD R/W pin to ground
   LCD VSS pin to ground
   LCD VCC pin to 5V
   10K resistor:
   ends to +5V and ground
   wiper to LCD VO pin (pin 3)

  Library originally added 18 Apr 2008
  by David A. Mellis
  library modified 5 Jul 2009
  by Limor Fried (http://www.ladyada.net)
  example added 9 Jul 2009
  by Tom Igoe
  modified 22 Nov 2010
  by Tom Igoe
  modified 7 Nov 2016
  by Arturo Guadalupi

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/LiquidCrystalHelloWorld

*/
////////////////////////////////////////////////////////
#define LED_RED 16
#define LED_VERDE 17
#define LED_AZUL 18



// include the library code:
#include <Keypad.h> //http://www.arduino.cc/playground/uploads/Code/Keypad.zip
unsigned long sysloked_Actual;
unsigned long sysloked_delta;
unsigned long sysloked_Anterior = 0;
int contador = 0;

int indice = 0;
int intentos = 0; //3
const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
byte letra1[8] = { B00000, B01010, B11111, B11111, B11111, B01110, B00100, B00000 };
byte letra2[8] = { B00000, B01010, B10101, B10001, B10001, B01010, B00100, B00000 };
char PASSWORD[5] =  "2001";
char PASSWORD_USER[5];
byte rowPins[ROWS] = {5, 4, 3, 2}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {6, 7, 8}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 31, d5 = 32, d6 = 33, d7 = 34;//cambio
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


void setup() {
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_AZUL, OUTPUT);

  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_AZUL, LOW);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("PASSWORD:");
  lcd.createChar(1, letra1);      // asigna los nuevos caracteres por numeros
  lcd.createChar(2, letra2);
}

void loop() {
  validarIntentos();
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_AZUL, LOW);   // turn the LED on (HIGH is the voltage level)
  //delay(100);
  //key == PASSWORD && indice < 4
  leerTeclado();
  //delay(100);
  if (indice == 4)      // si ya se almacenaron los 4 digitos
  {
    validarPassword();
    limpiarPasswrd();
  }
}
/*
 * @brief Se encarga de verificar cuantos intentos a realizado el usuario al ingresar la contraseña
 */
void validarIntentos() {
  switch ( intentos )
  {
    case 0:
      lcd.setCursor(13, 0);   // Posicion del dibujo en 13, 0
      lcd.write(byte(1));
      lcd.setCursor(14, 0);   // Posicion del dibujo en 14, 0
      lcd.write(byte(1));
      lcd.setCursor(15, 0);   // Posicion del dibujo en 15, 0
      lcd.write(byte(1));
      break;
    case 1 :
      lcd.setCursor(13, 0);   // Posicion del dibujo en 13, 0
      lcd.write(byte(1));
      lcd.setCursor(14, 0);   // Posicion del dibujo en 14, 0
      lcd.write(byte(1));
      lcd.setCursor(15, 0);   // Posicion del dibujo en 15, 0
      lcd.write(byte(2));
      break;
    case 2 :
      lcd.setCursor(13, 0);   // Posicion del dibujo en 13, 0
      lcd.write(byte(1));
      lcd.setCursor(14, 0);   // Posicion del dibujo en 14, 0
      lcd.write(byte(2));
      lcd.setCursor(15, 0);   // Posicion del dibujo en 15, 0
      lcd.write(byte(2));
      break;
    case 3 :
      sysloked_Actual = millis();
      lcd.clear();
      lcd.print("SYS LOCKED");
      lcd.setCursor(13, 0);   // Posicion del dibujo en 13, 0
      lcd.write(byte(2));
      lcd.setCursor(14, 0);   // Posicion del dibujo en 14, 0
      lcd.write(byte(2));
      lcd.setCursor(15, 0);   // Posicion del dibujo en 15, 0
      lcd.write(byte(2));
      digitalWrite(LED_RED, LOW);
      digitalWrite(LED_VERDE, LOW);
      digitalWrite(LED_AZUL, LOW);  // turn the LED on (HIGH is the voltage level)
      delay(100);

      digitalWrite(LED_VERDE, LOW);
      digitalWrite(LED_AZUL, LOW);
      digitalWrite(LED_RED, HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(100);

      digitalWrite(LED_RED, LOW);
      digitalWrite(LED_VERDE, LOW);
      digitalWrite(LED_AZUL, HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(100);

      digitalWrite(LED_RED, LOW);
      digitalWrite(LED_VERDE, HIGH);
      digitalWrite(LED_AZUL, LOW);  // turn the LED on (HIGH is the voltage level)
      delay(100);

      tiempoMuerto();
      break;
  }
}
/*
 * @brief Se encarga de capturar el dígito ingresado por el usuario
 */
void leerTeclado() {
  char key = keypad.getKey();
  if (key != 0 and intentos < 3) {
    PASSWORD_USER[indice] = key;
    // set the cursor to column 0, line 1
    // (note: line 1 is the second row, since counting begins with 0):
    lcd.setCursor(indice, 1);
    // print the number of seconds since reset:
    lcd.print("*");
    indice++;
  }
}
/*
 * @brief Compara la clave ingresada por el usuario con la clave maestra 
 */
void validarPassword() {
  if (!strcmp(PASSWORD_USER, PASSWORD))
  { // compara clave ingresada con clave maestra
    lcd.setCursor(0, 0);
    lcd.print(" Correcta");  // imprime en monitor serial que es correcta la clave
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_VERDE, HIGH);
    digitalWrite(LED_AZUL, LOW);
    intentos = 0;
    delay(2000);
  }
  else {
    intentos++;
    lcd.setCursor(0, 0);
    lcd.print(" Incorrecta");  // imprime en monitor serial que es incorrecta la clave
    if (intentos == 3) {
      digitalWrite(LED_VERDE, LOW);
      digitalWrite(LED_AZUL, LOW);
      digitalWrite(LED_RED, HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(2000);
    }
    else {
      digitalWrite(LED_RED, LOW);
      digitalWrite(LED_VERDE, LOW);
      digitalWrite(LED_AZUL, HIGH);
      delay(2000);
    }
  }
}
/*
 * @brief Establece un tiempo de espera cuando el sistema se bloquea por el exceso de intentos permitidos
 */
void tiempoMuerto() {
  sysloked_delta = sysloked_Actual - sysloked_Anterior;
  if (sysloked_delta > 1000) {
    contador++;
    sysloked_Anterior = sysloked_Actual;
  }

  if (contador == 30) {
    lcd.clear();
    lcd.print("PASSWORD:");
    intentos = 0;
    contador = 0;
  }
}
/*
 * * @brief limpia el vector de contraseña ingresada
 */
void limpiarPasswrd() {
  lcd.clear();
  lcd.print("PASSWORD:");
  indice = 0;
  PASSWORD_USER[0] = ' ';
  PASSWORD_USER[1] = ' ';
  PASSWORD_USER[2] = ' ';
  PASSWORD_USER[3] = ' ';
  PASSWORD_USER[4] = '\0';
}
