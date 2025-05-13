#include <WiFi.h>
#include <ThingerESP32.h>

// — Credenciales Wi-Fi —
#define SSID     "Gonet_Liliana_Abril_2.4"
#define PWD      "AbrilS1985@2209#"

// — Thinger.io —
#define USER     "DiegoSiguencia"
#define DEVICE   "Esp32_A"
#define CRED     "V&+oi2Mc3R$wrYNG"
ThingerESP32 thing(USER, DEVICE, CRED);

// — Pines —
const int PIN_MONO = 12;  // LED monocromo (ánodo al pin, cátodo a GND)
const int PIN_R    = 27;  // Cátodo Rojo (ánodo común a 3.3 V)
const int PIN_G    = 26;  // Cátodo Verde
const int PIN_B    = 25;  // Cátodo Azul

// — Estados internos —  
bool monoState = false;
uint8_t redState = 0, grnState = 0, bluState = 0;

void setup() {
  Serial.begin(115200);

  // Conecta Wi-Fi y Thinger.io
  thing.add_wifi(SSID, PWD);

  // Configura todos los pines como salida
  pinMode(PIN_MONO, OUTPUT);
  pinMode(PIN_R, OUTPUT);
  pinMode(PIN_G, OUTPUT);
  pinMode(PIN_B, OUTPUT);

  // — Recurso LED monocromo (on/off) —
  thing["LED_MONO"] << [](pson& in){
    if (in.is_empty()) {
      // Lectura de estado
      in = monoState;
    } else {
      monoState = (bool)in;
      digitalWrite(PIN_MONO, monoState ? HIGH : LOW);
      Serial.printf("Mono → %s\n", monoState ? "ON" : "OFF");
    }
  };

  // — Recurso Rojo (0–255) —
  thing["LED_R"] << [](pson& in){
    if (in.is_empty()) {
      in = redState;
    } else {
      redState = in;
      // Para ánodo común invertimos el valor
      analogWrite(PIN_R, 255 - redState);
      Serial.printf("R = %u\n", redState);
    }
  };

  // — Recurso Verde (0–255) —
  thing["LED_G"] << [](pson& in){
    if (in.is_empty()) {
      in = grnState;
    } else {
      grnState = in;
      analogWrite(PIN_G, 255 - grnState);
      Serial.printf("G = %u\n", grnState);
    }
  };

  // — Recurso Azul (0–255) —
  thing["LED_B"] << [](pson& in){
    if (in.is_empty()) {
      in = bluState;
    } else {
      bluState = in;
      analogWrite(PIN_B, 255 - bluState);
      Serial.printf("B = %u\n", bluState);
    }
  };

  // — Recurso “Todos” para setear los tres canales juntos —
  thing["LED_ALL"] << [](pson& in){
    if (!in.is_empty()) {
      uint8_t v = in; // 0=off, 1=on
      redState = grnState = bluState = (v ? 255 : 0);
      analogWrite(PIN_R, 255 - redState);
      analogWrite(PIN_G, 255 - grnState);
      analogWrite(PIN_B, 255 - bluState);
      Serial.printf("ALL → %s\n", v ? "ON" : "OFF");
      in = v;
    }
  };
}

void loop() {
  thing.handle();
}
