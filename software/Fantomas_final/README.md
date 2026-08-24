# Código final – Fantoma ECG

Esta carpeta contiene la versión final del software desarrollado para el **Fantoma ECG**, utilizando una placa basada en RP2040.

## Archivos necesarios

- `Fantomas_final.ino`: código principal.
- `ECGBancos_PTBXL_recortado.h`: bancos de señales ECG.

Ambos archivos deben permanecer en la misma carpeta para abrir y compilar correctamente el proyecto en Arduino IDE.

## Funciones principales

- Generación de **8 canales de salida ECG** mediante PWM.
- Selección entre **5 bancos de señales**: Normal, Infarto de miocardio, cambios ST/T, trastornos de conducción e hipertrofia.
- Selección entre **3 velocidades de reproducción**.
- LEDs indicadores del banco y velocidad seleccionados.
- LED de estado para indicar que el programa se encuentra en ejecución.

## Requisito

El programa utiliza la librería `RP2040_PWM`.
