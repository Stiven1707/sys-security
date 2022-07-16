/*
 * @brief Configura la maquina de estados según la configuración que se necesite
 */

void setup_State_Machine()
{
  // Add transitions
  stateMachine.AddTransition(LOW_A, NORMAL_B, []() {
    return currentInput == Sign_T;
  });
  stateMachine.AddTransition(NORMAL_B, LOW_A, []() {
    return currentInput == Sign_D;
  });

  stateMachine.AddTransition(NORMAL_B, HIGH_C, []() {
    return currentInput == Sign_T;
  });
  stateMachine.AddTransition(HIGH_C, NORMAL_B, []() {
    return currentInput == Sign_D;
  });

  stateMachine.AddTransition(HIGH_C, ALERT_D, []() {
    return currentInput == Sign_T;
  });
  stateMachine.AddTransition(ALERT_D, NORMAL_B, []() {
    return currentInput == Sign_P;
  });

  // Add actions
  stateMachine.SetOnEntering(LOW_A, input_LowA);
  stateMachine.SetOnEntering(NORMAL_B, input_NormalB);
  stateMachine.SetOnEntering(HIGH_C, input_HighC);
  stateMachine.SetOnEntering(ALERT_D, input_AlertD);

  stateMachine.SetOnLeaving(LOW_A, output_LowA);
  stateMachine.SetOnLeaving(NORMAL_B, output_NormalB);
  stateMachine.SetOnLeaving(HIGH_C, output_HighC);
  stateMachine.SetOnLeaving(ALERT_D, output_AlertD);
}
/*
 * @brief Se encarga de leer los datos suministrados por el sensor de temperatura y humedad
 */
void read_data() {

  bool flag_data = false;
  while (Serial.available() > 0) {
    int inChar = Serial.read();
    if (isDigit(inChar)) {
      // convert the incoming byte to a char and add it to the string:
      inString += (char)inChar;
    }
    // if you get a newline, print the string, then the string's value:
    if (inChar == '\n') {
      Serial.print("Value:");
      value_Temp = inString.toInt();
      flag_data = true;
      Serial.println(value_Temp);
      Serial.print("String: ");
      Serial.println(inString);
      // clear the string for new input:
      inString = "";
    }
  }
  if (flag_data) {
    update_Input_State_Machine();
  }
}

/*
 * @brief Se encarga de evaluar si los datos del sensor de tempetatura se encuentra en el umbral bajo de remperatura
 */
void state_Low() {
  if (value_Temp > TH_TEMP_LOW) {
    currentInput = Input::Sign_T;
    Serial.println(value_Temp);
  }
  else {
    currentInput = Input::Unknown;
  }
}

/*
 * @brief Se encarga de evaluar si los datos del sensor de tempetatura se encuentra en el umbral normal de temperatura
 */
void state_Normal() {
  if (value_Temp < TH_TEMP_LOW) {
    currentInput = Input::Sign_D;
  }
  else if (value_Temp >= TH_TEMP_NORM) {
    currentInput = Input::Sign_T;
  }
  else {
    currentInput = Input::Unknown;
  }
}

/*
 * @brief Se encarga de evaluar si los datos del sensor de tempetatura se encuentra en el umbral alto de temperatura
 */
void state_High() {
  if (value_Temp < TH_TEMP_NORM) {
    currentInput = Input::Sign_D;
  }
  else if (value_Temp > TH_TEMP_HIGH) {
    currentInput = Input::Sign_T;
  }
  else {
    currentInput = Input::Unknown;
  }
}

/*
 * @brief Escribe el titulo de alerta en el puerto serial.
 */
void state_Alert() {
  Serial.println("state ALERT");
}


/*
 * @brief Evalua los estados de la máquina, segun el estado, va a ejecutar la acción del umbral que le corresponda, ya sea avanzar al siguiente estado o retornar al anterior.
 */
void update_Input_State_Machine()
{
  int currentState = stateMachine.GetState();
  switch (currentState)
  {
    case LOW_A:     state_Low();
      break;
    case NORMAL_B:  state_Normal();
      break;
    case HIGH_C:    state_High();
      break;
    case ALERT_D:   state_Alert();
      break;
    default:        Serial.println("state Unknown");
      break;
  }
}

/*
 * @brief Impresión de datos en la LCD y Puerto Serial cuando la máquina entra en el primer estado (Temperatura Baja)
 */
void input_LowA()
{
  currentInput = Input::Unknown;
  DEBUG("State_LOW_A");
  Serial.println("A   B   C   D");
  Serial.println("X            ");
  Serial.println();
  digitalWrite(LED_BLUE_PIN, HIGH);

  lcd.clear();
  lcd.setCursor(0, 0);
  //lcd.setRGB(0, 255, 0);
  lcd.print("State:LOW_A");
}

/*
 * @brief la máquina sale del primer estado 
 */
void output_LowA()
{
  //DEBUG("Leaving_State_LOW_A");
  Serial.println("Leaving State_LOW_A");
  digitalWrite(LED_BLUE_PIN, LOW);
}

/*
 * @brief Impresión de datos en la LCD y Puerto Serial cuando la máquina entra en el segundo estado (Temperatura Normal)
 */
void input_NormalB()
{
  currentInput = Input::Unknown;
  DEBUG("State_NORMAL_B");
  Serial.println("A   B   C   D");
  Serial.println("    X        ");
  Serial.println();
  digitalWrite(LED_GREEN_PIN, HIGH);

  lcd.clear();
  lcd.setCursor(0, 0);
  //lcd.setRGB(128, 128, 128);
  lcd.print("State:NORMAL_B");
}

/*
 * @brief la máquina sale del Segundo estado 
 */
void output_NormalB()
{
  //DEBUG("Leaving State_NORMAL_B");
  Serial.println("Leaving State_NORMAL_B");
  digitalWrite(LED_GREEN_PIN, LOW);
}

/*
 * @brief Impresión de datos en la LCD y Puerto Serial cuando la máquina entra en el tercer estado (Termperatura Alta)
 */
void input_HighC()
{
  currentInput = Input::Unknown;
  DEBUG("State_HIGH_C");
  Serial.println("A   B   C   D");
  Serial.println("        X    ");
  Serial.println();
  digitalWrite(LED_RED_PIN, HIGH);

  lcd.clear();
  lcd.setCursor(0, 0);
  ////lcd.setRGB(0, 0, 255);
  lcd.print("State:HIGH_C");
}

/*
 * @brief la máquina sale del tercero estado 
 */
void output_HighC()
{
  //DEBUG("Leaving State_HIGH_C");
  Serial.println("Leaving State_HIGH_C");
  digitalWrite(LED_RED_PIN, LOW);
}

/*
 * @brief Impresión de datos en la LCD y Puerto Serial cuando la máquina entra en el cuertos estado (Alerta)
 */
void input_AlertD()
{
  int nota = 1568;
  asyncTask1.Start();
  currentInput = Input::Unknown;
  DEBUG("State_ALERT_D");
  Serial.println("A   B   C   D");
  Serial.println("            X");
  Serial.println();
  digitalWrite(LED_RED_PIN, HIGH);
  for (int i = 0; i < 2; i++) {      // bucle repite 25 veces
    int duracion = 1000 / duraciones[i];    // duracion de la nota en milisegundos
    tone(BUZZER_PASIVO, melodia[i] , duracion);  // ejecuta el tono con la duracion
    int pausa = duracion * 1.30;      // calcula pausa
    delay(pausa);         // demora con valor de pausa
    noTone(BUZZER_PASIVO);        // detiene reproduccion de tono
  }


  lcd.clear();
  lcd.setCursor(0, 0);
  ////lcd.setRGB(255, 0, 0);
  lcd.print("State:ALERT_D");
}

/*
 * @brief la máquina sale del cuarto estado 
 */
void output_AlertD()
{
  //DEBUG("Leaving State_ALERT_D");
  Serial.println("Leaving State_ALERT_D");
  digitalWrite(LED_RED_PIN, LOW);
}
