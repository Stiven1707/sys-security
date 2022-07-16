/*!
  \file   lab1.ino
  \date   2022-08-02
  \author Fulvio Vivas <fulvio.vivas@unicauca.edu.co>
  \brief  Temperature control program.

  \par Copyright
  Information contained herein is proprietary to and constitutes valuable
  confidential trade secrets of Unicauca, and
  is subject to restrictions on use and disclosure.

  \par
  Copyright (c) Unicauca 2022. All rights reserved.

  \par
  The copyright notices above do not evidence any actual or
  intended publication of this material.
******************************************************************************
*/

#include "pinout.h"

/********************************************//**
    Temperature sensor control functions
 ***********************************************/
#include "DHTStable.h"
DHTStable DHT;

/********************************************//**
    LCD control functions
 ***********************************************/
#include <Wire.h>
//#include "rgb_lcd.h"
//rgb_lcd lcd;

#include "AsyncTaskLib.h"
//LDC
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 31, 32, 33, 34);

#define DEBUG(a) Serial.print(millis()); Serial.print(": "); Serial.println(a);

/********************************************//**
    Asynchronous Task control functions
 ***********************************************/
#include "AsyncTaskLib.h"
void timeout_T1();
void measure_Temp();

AsyncTask asyncTask1(2500, timeout_T1);
AsyncTask asyncTask3(500, true, measure_Temp);

/********************************************//**
    State Machine control functions
 ***********************************************/
#include "StateMachineLib.h"

/*
 * @brief define los estados de la máquina
 */
enum State
{
  LOW_A = 0,
  NORMAL_B = 1,
  HIGH_C = 2,
  ALERT_D = 3
};

/*
 * @brief Define las enttradas de estados de la máquina  
 */
enum Input
{
  Sign_R = 0,
  Sign_T = 1,
  Sign_D = 2,
  Sign_P = 3,
  Unknown = 4,
};

/*! Stores last user input */
Input currentInput;

/*! Create new StateMachine 4 states and 6 transitions */
StateMachine stateMachine(4, 6);

/********************************************//**
    Define global variables
 ***********************************************/
int temp_value;
String inString = "";    // string to hold input
int value_Temp;
bool  flag_data = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println ("INIT");

  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(LED_GREEN_PIN, OUTPUT);
  pinMode(LED_BLUE_PIN, OUTPUT);

  lcd.begin(16, 2);
  //lcd.setRGB(128, 128, 128);
  // Print a message to the LCD.
  lcd.print("State Machine!");

  delay(1000);

  //default values
  digitalWrite(LED_RED_PIN, LOW); //cerrar valvula de oxigeno
  digitalWrite(LED_GREEN_PIN, LOW); //cerrar valvula de oxigeno
  digitalWrite(LED_BLUE_PIN, LOW); //cerrar valvula de oxigeno

  //Starting State Machine
  Serial.println("Starting State Machine...");
  setup_State_Machine();
  Serial.println("Start Machine Started");


  stateMachine.SetState(State::LOW_A, false, true);

  //Starting Asynchronous Task
  asyncTask3.Start();
  DEBUG("START");
}

void loop() {
  // Update Asynchronous Task
  asyncTask1.Update();
  asyncTask3.Update();

  //Read value by Serial port
  //read_data();
  if (flag_data) {
    update_Input_State_Machine();
      flag_data = false;
  }

  // Update State Machine
  stateMachine.Update();
}


/*
 * @brief Define un periodo de tiempo para la transición del estado D al estado B 
 */
void timeout_T1() {
  DEBUG("T1_END");
  currentInput = Input::Sign_P;
}

/*
 * @brief Se encarga de evaluar los valores de temperatura y los imprime en la LCD y puerto Serial  
 */
void measure_Temp() {
  DEBUG("TEMPERATURE");
  int chk = DHT.read11(DHT11_PIN);
  switch (chk)
  {
    case DHTLIB_OK:
      Serial.print("OK,\t");
      break;
    case DHTLIB_ERROR_CHECKSUM:
      Serial.print("Checksum error,\t");
      break;
    case DHTLIB_ERROR_TIMEOUT:
      Serial.print("Time out error,\t");
      break;
    default:
      Serial.print("Unknown error,\t");
      break;
  }
  // DISPLAY DATA
  int value_Hum = DHT.getHumidity();
  value_Temp = DHT.getTemperature();
  Serial.print(value_Hum, 1);
  Serial.print(",\t");
  Serial.println(value_Temp, 1);

  lcd.setCursor(0, 1);
  lcd.print("TEMP:");
  lcd.print(value_Temp);
  lcd.print(" ");
  lcd.print("HUM:");
  lcd.print(value_Hum);
  flag_data = true;
}
