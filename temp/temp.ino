#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Definición de los pines de los pulsadores
#define BUTTON1 5
#define BUTTON2 18
#define BUTTON3 19
#define BUTTON4 23
#define BUTTON5 25

// Definición de los pines de los relés
#define RELAY_FOCO 26       // Ajusta según el pin que uses para el foco
#define RELAY_VENTILADOR 27  // Ajusta según el pin que uses para el ventilador

// Configuración del bus OneWire para el sensor de temperatura
#define ONE_WIRE_BUS 4       // Ajusta según el pin que uses para el sensor de temperatura
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Configuración del LCD
LiquidCrystal_I2C lcd(0x27, 20, 4); // Dirección I2C y tamaño de la pantalla

// Temperaturas asignadas a cada pulsador
float tempObjetivo;
float tempActual;
float tempButton1 = 20.0; // Ejemplo: 30°C
float tempButton2 = 23.0; // Ejemplo: 35°C
float tempButton3 = 26.0; // Ejemplo: 40°C
float tempButton4 = 29.0; // Ejemplo: 45°C
float tempButton5 = 31.0; // Ejemplo: 50°C

// Márgenes específicos por nivel de temperatura (foco y ventilador)
const float MARGIN_FOCO_BUTTON1 = -0.5;
const float MARGIN_VENTILADOR_BUTTON1 = 0.5;
const float MARGIN_FOCO_BUTTON2 = -0.5;
const float MARGIN_VENTILADOR_BUTTON2 = 0.5;
const float MARGIN_FOCO_BUTTON3 = -0.5;
const float MARGIN_VENTILADOR_BUTTON3 = 0.5;
const float MARGIN_FOCO_BUTTON4 = -0.5;
const float MARGIN_VENTILADOR_BUTTON4 = 0.5;
const float MARGIN_FOCO_BUTTON5 = 0.5;
const float MARGIN_VENTILADOR_BUTTON5 = 0.5;

void setup() {
  pinMode(RELAY_FOCO, OUTPUT);
  pinMode(RELAY_VENTILADOR, OUTPUT);
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  pinMode(BUTTON3, INPUT_PULLUP);
  pinMode(BUTTON4, INPUT_PULLUP);
  pinMode(BUTTON5, INPUT_PULLUP);

  Serial.begin(115200); // Inicia la comunicación con el Monitor Serie a 115200 bps
  lcd.begin(16, 2); // Inicializa la LCD con 16 columnas y 2 filas
  lcd.backlight();  // Enciende la luz de fondo de la pantalla LCD
  sensors.begin();
}

void loop() {
  float marginFoco = 0;
  float marginVentilador = 0;

  // Lectura de los pulsadores
  if (digitalRead(BUTTON1) == LOW) {
    tempObjetivo = tempButton1;
    marginFoco = MARGIN_FOCO_BUTTON1;
    marginVentilador = MARGIN_VENTILADOR_BUTTON1;
  } else if (digitalRead(BUTTON2) == LOW) {
    tempObjetivo = tempButton2;
    marginFoco = MARGIN_FOCO_BUTTON2;
    marginVentilador = MARGIN_VENTILADOR_BUTTON2;
  } else if (digitalRead(BUTTON3) == LOW) {
    tempObjetivo = tempButton3;
    marginFoco = MARGIN_FOCO_BUTTON3;
    marginVentilador = MARGIN_VENTILADOR_BUTTON3;
  } else if (digitalRead(BUTTON4) == LOW) {
    tempObjetivo = tempButton4;
    marginFoco = MARGIN_FOCO_BUTTON4;
    marginVentilador = MARGIN_VENTILADOR_BUTTON4;
  } else if (digitalRead(BUTTON5) == LOW) {
    tempObjetivo = tempButton5;
    marginFoco = MARGIN_FOCO_BUTTON5;
    marginVentilador = MARGIN_VENTILADOR_BUTTON5;
  }

  // Lectura de la temperatura actual
  sensors.requestTemperatures();
  tempActual = sensors.getTempCByIndex(0);

  // Control del foco
  if (tempActual - 0.2< tempObjetivo) {
    digitalWrite(RELAY_FOCO, LOW); // Enciende el foco
  } else if (tempActual > tempObjetivo + 0.7) {
    digitalWrite(RELAY_FOCO, HIGH); // Apaga el foco
  }

  // Control del ventilador
  if (tempActual - 0.2 > tempObjetivo) {
    digitalWrite(RELAY_VENTILADOR, LOW); // Enciende el ventilador
  } else if (tempActual <= tempObjetivo + 0.3) {
    digitalWrite(RELAY_VENTILADOR, HIGH); // Apaga el ventilador
  }

  // Cálculo del fallo
  float fallo = tempActual - tempObjetivo;

  // Mostrar en LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp Obj: ");
  lcd.print(tempObjetivo);
  lcd.setCursor(0, 1);
  lcd.print("Temp Act: ");
  lcd.print(tempActual);
  lcd.setCursor(0, 2);
  lcd.print("Fallo: ");
  lcd.print(fallo);

  // Mostrar en el Monitor Serie
  Serial.print("Temp Objetivo: ");
  Serial.print(tempObjetivo);
  Serial.print(" °C, Temp Actual: ");
  Serial.print(tempActual);
  Serial.print(" °C, Fallo: ");
  Serial.print(fallo);
  Serial.println(" °C");

  delay(1000); // Delay de 1 segundo entre lecturas
}
