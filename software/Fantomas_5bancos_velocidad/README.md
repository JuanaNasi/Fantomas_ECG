# Software del Fantoma ECG

Esta carpeta contiene el programa utilizado para el funcionamiento del Fantoma ECG, implementado sobre una placa basada en RP2040 y programado mediante Arduino IDE.

## Archivos

- `Fantomas_5bancos_velocidad.ino`: programa principal del dispositivo.
- `ECGBancos_PTBXL_recortado.h`: contiene los bancos de señales ECG utilizados por el programa.

## Funcionamiento

El software permite:

- Reproducir distintos bancos de señales electrocardiográficas.
- Cambiar entre los bancos de ECG mediante el control incorporado en el dispositivo.
- Modificar la velocidad de reproducción de las señales.
- Generar las señales correspondientes a las distintas salidas del Fantoma ECG.
- Utilizar un LED como indicador del funcionamiento del sistema.

## Uso con Arduino IDE

1. Descargar o clonar el repositorio.
2. Abrir la carpeta `software/Fantomas_5bancos_velocidad`.
3. Abrir el archivo `Fantomas_5bancos_velocidad.ino` con Arduino IDE.
4. Verificar que el archivo `ECGBancos_PTBXL_recortado.h` se encuentre en la misma carpeta que el archivo `.ino`.
5. Seleccionar en Arduino IDE la placa correspondiente.
6. Compilar y cargar el programa en el dispositivo.

> Es importante conservar el archivo `.ino` y el archivo `.h` dentro de la misma carpeta para que el programa pueda acceder correctamente a los bancos de señales ECG.

## Documentación

La descripción completa del desarrollo, las modificaciones realizadas, el origen y procesamiento de las señales ECG y el funcionamiento del dispositivo se encuentran en la [memoria técnica](../../docs/memoria_tecnica.md).

## Estado

El software se encuentra actualmente en desarrollo y puede ser actualizado a medida que se incorporen nuevas funciones o modificaciones al prototipo.
