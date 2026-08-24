#include "ECGBancos_PTBXL_recortado.h"

// Tecla 1: cambia el banco de datos.
// Tecla 2: cambia la velocidad agregando o sacando zona muerta entre latidos.
#define PinTeclaBanco 20
#define PinTeclaVelocidad 21
#define PinLed 5   // LED general de blink, si lo querés seguir usando

#define _PWM_LOGLEVEL_ 0
#include "RP2040_PWM.h"

#define FrecuenciaPWM 100000

#define CantidadDeBancos 5
#define CantidadDeCanales 8
#define CantidadVelocidades 3

// Mapeo según la placa física:
// GPIO5 queda para el LED, por eso las salidas PWM saltan de GPIO4 a GPIO6.
const byte PinesPWM[CantidadDeCanales] = {0, 1, 2, 3, 4, 6, 7, 8};


// LEDs indicadores de banco.
// Cambiar estos GPIO según dónde conectes cada LED.
const byte PinesLedBanco[CantidadDeBancos] = {
  11,   // LED Banco 0: Normal (blanco)
  12,  // LED Banco 1: Infarto (amarillo)
  13,  // LED Banco 2: ST/T (verde)
  14,  // LED Banco 3: Conducción (azul)
  15   // LED Banco 4: Hipertrofia (rojo)
};


// LEDs indicadores de velocidad.
// Cambiar estos GPIO según dónde conectes cada LED.
const byte PinesLedVelocidad[CantidadVelocidades] = {
  27,  // LED velocidad 0: rápido (rojo)
  26,  // LED velocidad 1: normal (amarillo)
  10   // LED velocidad 2: lento (verde)
};


int i = 0;
int j;

volatile byte Banco = 0;
volatile byte ModoVelocidad = 1;
// 0 = rápido
// 1 = normal
// 2 = lento

byte BancoAnterior = 255;
uint16_t CicloActivo;

// Cada muestra dura 10 ms porque los datos son de PTB-XL records100.
// Los bancos recortados tienen 80 muestras = 0,8 s de latido.
// La velocidad se cambia agregando zona muerta al final del latido.
const int ZonaMuertaExtra[CantidadVelocidades] = {
  0,   // rápido: 80 muestras = 0,8 s aprox -> 75 lat/min
  20,  // normal: 80 + 20 = 1,0 s aprox -> 60 lat/min
  60   // lento: 80 + 60 = 1,4 s aprox -> 43 lat/min
};

bool EnZonaMuerta = false;
int ContadorZonaMuerta = 0;

// Blink del LED general
unsigned long TiempoAnteriorBlink = 0;
const unsigned long IntervaloBlink = 500;
bool EstadoLed = LOW;

// Antirrebote de teclas
bool TeclaBancoAnterior = HIGH;
bool TeclaVelocidadAnterior = HIGH;
unsigned long TiempoAnteriorTeclaBanco = 0;
unsigned long TiempoAnteriorTeclaVelocidad = 0;
const unsigned long TiempoAntirrebote = 200;

// Bancos de datos:
// 0 Normal
// 1 Infarto de miocardio
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

int PeriodoDeMuestreo[CantidadDeBancos] = {10, 10, 10, 10, 10};

RP2040_PWM* PWM_Canal[CantidadDeCanales];


// Apaga todos los LEDs de banco y prende solo el banco actual
void actualizarLedsBanco() {
  for (int k = 0; k < CantidadDeBancos; k++) {
    digitalWrite(PinesLedBanco[k], LOW);
  }

  digitalWrite(PinesLedBanco[Banco], HIGH);
}


// Apaga todos los LEDs de velocidad y prende solo la velocidad actual
void actualizarLedsVelocidad() {
  for (int k = 0; k < CantidadVelocidades; k++) {
    digitalWrite(PinesLedVelocidad[k], LOW);
  }

  digitalWrite(PinesLedVelocidad[ModoVelocidad], HIGH);
}


void setup() {
  Serial.begin(115200);

  for (j = 0; j < CantidadDeCanales; j++) {
    PWM_Canal[j] = new RP2040_PWM(PinesPWM[j], FrecuenciaPWM, 0);
  }
}


void loop() {
  byte bancoActual = Banco;
  byte velocidadActual = ModoVelocidad;

  // Si cambia el banco, arranca desde el comienzo del latido.
  if (bancoActual != BancoAnterior) {
    i = 0;
    BancoAnterior = bancoActual;
    EnZonaMuerta = false;
    ContadorZonaMuerta = 0;
  }

  delay(PeriodoDeMuestreo[bancoActual]);

  for (j = 0; j < CantidadDeCanales; j++) {
    if (EnZonaMuerta) {
      // Línea de base durante la pausa entre latidos.
      CicloActivo = VALOR_LINEA_BASE_ECG;
    } else {
      CicloActivo = *(Datos[bancoActual] + (i * CantidadDeCanales) + j);
    }

    PWM_Canal[j]->setPWM_Int(PinesPWM[j], FrecuenciaPWM, CicloActivo, true);
  }

  // Serial Plotter: imprime los primeros 3 canales y dos referencias.
  for (int col = 0; col < CantidadDeCanales; col++) {
    uint16_t sample;

    if (EnZonaMuerta) {
      sample = VALOR_LINEA_BASE_ECG;
    } else {
      sample = *(Datos[bancoActual] + (i * CantidadDeCanales) + col);
    }

    if (col < 3) {
      Serial.print(sample);
      Serial.print(',');
    }
  }

  Serial.print("0,");
  Serial.print("60000");
  Serial.println();

  // Avance de muestras y manejo de zona muerta.
  if (!EnZonaMuerta) {
    i++;

    if (i >= CantidadDeDatos[bancoActual]) {
      i = 0;

      if (ZonaMuertaExtra[velocidadActual] > 0) {
        EnZonaMuerta = true;
        ContadorZonaMuerta = 0;
      }
    }
  } else {
    ContadorZonaMuerta++;

    if (ContadorZonaMuerta >= ZonaMuertaExtra[velocidadActual]) {
      EnZonaMuerta = false;
      ContadorZonaMuerta = 0;
      i = 0;
    }
  }
}


void setup1() {
  pinMode(PinTeclaBanco, INPUT_PULLUP);
  pinMode(PinTeclaVelocidad, INPUT_PULLUP);

  pinMode(PinLed, OUTPUT);
  digitalWrite(PinLed, LOW);

  // Configuración de LEDs de banco
  for (int k = 0; k < CantidadDeBancos; k++) {
    pinMode(PinesLedBanco[k], OUTPUT);
    digitalWrite(PinesLedBanco[k], LOW);
  }

  // Configuración de LEDs de velocidad
  for (int k = 0; k < CantidadVelocidades; k++) {
    pinMode(PinesLedVelocidad[k], OUTPUT);
    digitalWrite(PinesLedVelocidad[k], LOW);
  }

  // Estado inicial de indicadores
  actualizarLedsBanco();
  actualizarLedsVelocidad();
}


void loop1() {
  unsigned long TiempoActual = millis();

  // Blink del LED general para saber que el programa está corriendo.
  if (TiempoActual - TiempoAnteriorBlink >= IntervaloBlink) {
    TiempoAnteriorBlink = TiempoActual;
    EstadoLed = !EstadoLed;
    digitalWrite(PinLed, EstadoLed);
  }

  // Tecla 1: cambio de banco.
  bool TeclaBancoActual = digitalRead(PinTeclaBanco);

  if (TeclaBancoAnterior == HIGH && TeclaBancoActual == LOW) {
    if (TiempoActual - TiempoAnteriorTeclaBanco >= TiempoAntirrebote) {
      TiempoAnteriorTeclaBanco = TiempoActual;

      Banco++;
      if (Banco >= CantidadDeBancos) {
        Banco = 0;
      }

      actualizarLedsBanco();

      Serial.print("Banco actual: ");
      Serial.println(Banco);
    }
  }

  TeclaBancoAnterior = TeclaBancoActual;

  // Tecla 2: cambio de velocidad.
  bool TeclaVelocidadActual = digitalRead(PinTeclaVelocidad);

  if (TeclaVelocidadAnterior == HIGH && TeclaVelocidadActual == LOW) {
    if (TiempoActual - TiempoAnteriorTeclaVelocidad >= TiempoAntirrebote) {
      TiempoAnteriorTeclaVelocidad = TiempoActual;

      ModoVelocidad++;
      if (ModoVelocidad >= CantidadVelocidades) {
        ModoVelocidad = 0;
      }

      actualizarLedsVelocidad();

      Serial.print("Modo velocidad: ");
      Serial.println(ModoVelocidad);
    }
  }

  TeclaVelocidadAnterior = TeclaVelocidadActual;
}