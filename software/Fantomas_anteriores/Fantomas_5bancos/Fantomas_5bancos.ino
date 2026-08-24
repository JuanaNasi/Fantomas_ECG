#include "ECGBancos_PTBXL.h"

#define PinTecla 20
#define PinLed 5

#define _PWM_LOGLEVEL_ 0
#include "RP2040_PWM.h"

#define FrecuenciaPWM 100000

#define CantidadDeBancos 5
#define CantidadDeCanales 8

// Mapeo según la placa física:
// GPIO5 queda para el LED.
// Las señales PWM se corren desde ese punto.
const byte PinesPWM[CantidadDeCanales] = {0, 1, 2, 3, 4, 6, 7, 8};

int i = 0;
int j;

volatile byte Banco = 0;
byte BancoAnterior = 255;

uint16_t CicloActivo;

// Variables para el parpadeo del LED
unsigned long TiempoAnteriorBlink = 0;
const unsigned long IntervaloBlink = 500;
bool EstadoLed = LOW;

// Variables para la tecla
bool TeclaAnterior = HIGH;
unsigned long TiempoAnteriorTecla = 0;
const unsigned long TiempoAntirrebote = 200;

// Bancos de datos:
// 0 Normal
// 1 Infarto
// 2 Cambios ST/T
// 3 Trastorno de conducción
// 4 Hipertrofia
const uint16_t *Datos[CantidadDeBancos] = {
  &DatosNormal[0][0],
  &DatosMI[0][0],
  &DatosSTTC[0][0],
  &DatosCD[0][0],
  &DatosHYP[0][0]
};

int CantidadDeDatos[CantidadDeBancos] = {
  N_DatosNormal,
  N_DatosMI,
  N_DatosSTTC,
  N_DatosCD,
  N_DatosHYP
};

// PTB-XL records100 tiene 100 Hz, entonces cada muestra corresponde a 10 ms.
int PeriodoDeMuestreo[CantidadDeBancos] = {30, 30, 30, 30, 30};

// Crea las instancias PWM
RP2040_PWM* PWM_Canal[CantidadDeCanales];

void setup() {
  Serial.begin(115200);

  for (j = 0; j < CantidadDeCanales; j++) {
    PWM_Canal[j] = new RP2040_PWM(PinesPWM[j], FrecuenciaPWM, 0);
  }
}

void loop() {
  byte bancoActual = Banco;

  // Si cambia de banco, reinicio el índice para no leer fuera de rango.
  if (bancoActual != BancoAnterior) {
    i = 0;
    BancoAnterior = bancoActual;
  }

  delay(PeriodoDeMuestreo[bancoActual]);

  for (j = 0; j < CantidadDeCanales; j++) {
    CicloActivo = *(Datos[bancoActual] + (i * CantidadDeCanales) + j);
    PWM_Canal[j]->setPWM_Int(PinesPWM[j], FrecuenciaPWM, CicloActivo, true);
  }

  // Salida para Serial Plotter / Serial Oscilloscope.
  // Imprime los primeros 3 canales y dos referencias.
  for (int col = 0; col < CantidadDeCanales; col++) {
    uint16_t sample = *(Datos[bancoActual] + (i * CantidadDeCanales) + col);

    if (col < 3) {
      Serial.print(sample);
      Serial.print(',');
    }
  }

  Serial.print("0,");
  Serial.print("60000");
  Serial.println();

  i++;

  if (i >= CantidadDeDatos[bancoActual]) {
    i = 0;
  }
}

void setup1() {
  // Pull-up interno:
  // sin apretar -> HIGH
  // apretado contra GND -> LOW
  pinMode(PinTecla, INPUT_PULLUP);

  pinMode(PinLed, OUTPUT);
  digitalWrite(PinLed, LOW);
}

void loop1() {
  unsigned long TiempoActual = millis();

  // Blink del LED para saber que el programa está corriendo
  if (TiempoActual - TiempoAnteriorBlink >= IntervaloBlink) {
    TiempoAnteriorBlink = TiempoActual;

    EstadoLed = !EstadoLed;
    digitalWrite(PinLed, EstadoLed);
  }

  // Lectura de tecla para cambiar banco
  bool TeclaActual = digitalRead(PinTecla);

  // Detecta el momento en que apretás:
  // antes estaba HIGH y ahora pasa a LOW
  if (TeclaAnterior == HIGH && TeclaActual == LOW) {
    if (TiempoActual - TiempoAnteriorTecla >= TiempoAntirrebote) {
      TiempoAnteriorTecla = TiempoActual;

      Banco++;

      if (Banco >= CantidadDeBancos) {
        Banco = 0;
      }

      Serial.print("Banco actual: ");
      Serial.println(Banco);
    }
  }

  TeclaAnterior = TeclaActual;
}