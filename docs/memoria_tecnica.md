
# Sistema de adquisición y visualización de señales de ECG
## Introducción.
El presente documento tiene como finalidad dejar constancia del proceso de investigación, desarrollo y ajuste llevado adelante para la implementación de un sistema de adquisición y visualización de señales de ECG.

El trabajo se desarrolló de manera progresiva. En una primera instancia se realizó una investigación teórica sobre el funcionamiento del corazón, la generación de la señal electrocardiográfica, las derivaciones utilizadas para su medición y el principio de funcionamiento de un electrocardiógrafo.

Posteriormente, se comenzó a trabajar sobre los archivos, circuitos y códigos proporcionados inicialmente para el proyecto. A partir de ese material se realizaron diferentes pruebas utilizando una placa Raspberry Pi Pico, avanzando gradualmente en la lectura de las señales, la organización de los canales o bancos de medición, la incorporación de controles y la adaptación del sistema a las necesidades específicas del prototipo.

Durante el desarrollo se presentaron distintas dificultades relacionadas con la programación, las conexiones eléctricas, la selección de entradas, el cambio entre bancos de señales y la velocidad de visualización de los datos. Cada uno de estos inconvenientes fue analizado y corregido mediante sucesivas pruebas, modificaciones del código y verificaciones sobre el circuito físico.

En las siguientes secciones se describen cronológicamente las diferentes etapas del proyecto, desde la investigación inicial hasta la obtención del prototipo desarrollado

## Primera etapa: investigación bibliográfica.
La primera etapa de la adscripción consistió en una revisión bibliográfica orientada a comprender los conceptos básicos necesarios para el desarrollo posterior del proyecto. Esta instancia permitió realizar una primera aproximación tanto a la electrocardiografía como al funcionamiento de los sistemas utilizados para generar, adquirir y representar señales cardíacas.

En primer lugar, se estudió el origen de la señal electrocardiográfica y el funcionamiento general de un electrocardiógrafo. Este equipo registra las pequeñas diferencias de potencial producidas por la actividad eléctrica del corazón mediante electrodos colocados sobre el cuerpo. Debido a la baja amplitud de estas señales, el electrocardiógrafo debe amplificarlas, reducir las interferencias y finalmente representarlas en papel o en una pantalla. También se analizaron algunas de sus principales etapas, como la selección de la derivación, la amplificación, el filtrado, el aislamiento eléctrico del paciente y el registro de la señal.

Uno de los temas centrales de esta etapa fue el estudio de las derivaciones electrocardiográficas. Se comprendió que una derivación no representa un electrodo individual, sino una forma determinada de medir la diferencia de potencial eléctrico entre distintos puntos del cuerpo. Las derivaciones bipolares I, II y III se obtienen a partir de electrodos colocados en los miembros y conforman el denominado triángulo de Einthoven. Estas derivaciones se encuentran relacionadas entre sí mediante la ley de Einthoven, según la cual la derivación II es igual a la suma de las derivaciones I y III.

También se estudiaron las derivaciones aumentadas aVR, aVL y aVF, que observan la actividad eléctrica desde cada uno de los miembros respecto de una referencia, y las derivaciones precordiales V1 a V6, obtenidas mediante electrodos ubicados sobre el tórax. De esta manera, las doce derivaciones permiten observar la actividad eléctrica del corazón desde diferentes posiciones o ángulos, por lo que una misma actividad cardíaca puede presentar formas distintas según la derivación analizada.

Posteriormente, se analizaron antecedentes sobre la generación artificial de señales electrocardiográficas. Los trabajos estudiados presentan modelos matemáticos capaces de representar la actividad de los principales marcapasos naturales del corazón y los procesos de despolarización y repolarización de las aurículas y los ventrículos. A partir de la combinación de estas señales es posible generar las formas características del electrocardiograma, como la onda P, el complejo QRS y la onda T, así como reproducir las doce derivaciones y distintos ritmos cardíacos.

Además, se revisó el desarrollo de un simulador de paciente basado en estos modelos matemáticos. El sistema estudiado utiliza microcontroladores para realizar los cálculos, conversores digital-analógicos para transformar los valores calculados en señales eléctricas y etapas de amplificación para obtener amplitudes similares a las de un ECG real. Este antecedente resultó especialmente importante porque permitió comprender de manera general la relación entre el modelo matemático, el código ejecutado por el microcontrolador y la señal eléctrica entregada finalmente al electrocardiógrafo.

Finalmente, se realizó una introducción a la programación de microcontroladores mediante el manual de Arduino. Se estudiaron conceptos básicos como la estructura de un programa, las funciones setup() y loop(), la declaración de variables, la lectura de entradas digitales y analógicas, el manejo de salidas, el uso de retardos y la comunicación serie. Estos conocimientos sirvieron como preparación para comenzar posteriormente con la lectura y modificación del código utilizado en el proyecto.

En conjunto, esta primera etapa funcionó como una introducción al trabajo que se desarrollaría después, ya que permitió comprender qué señal debía generarse, cómo se conformaban las distintas derivaciones y de qué manera podían integrarse los modelos matemáticos, la programación y el hardware dentro de un simulador electrocardiográfico.

## Segunda etapa: análisis inicial del código y familiarización con Arduino.
Una vez finalizada la revisión bibliográfica, se comenzó con la parte práctica de la adscripción. Para ello, se trabajó con un primer archivo de programación proporcionado por el equipo responsable del proyecto y con una placa Raspberry Pi Pico, que sería utilizada como unidad de procesamiento para la generación de las señales.

En esta etapa, el objetivo inicial fue comprender la estructura general del código y reconocer la función de sus distintas partes. Se analizaron las variables utilizadas, la configuración de los pines, las funciones de inicialización y el bloque principal que se ejecutaba de manera continua. También se buscó identificar de qué forma se encontraban incorporados los modelos matemáticos estudiados anteriormente y cómo los resultados de esos cálculos se transformaban en valores de salida.

Posteriormente, se realizaron las primeras pruebas de compilación y carga del programa en la Raspberry Pi Pico. Esto permitió familiarizarse con el entorno de programación, la conexión de la placa con la computadora y el procedimiento necesario para ejecutar el código. Durante estas pruebas también se utilizó la comunicación serie para observar los valores generados por el programa y verificar su funcionamiento.

Esta etapa fue principalmente exploratoria. Permitió pasar de los conceptos estudiados en la bibliografía a una primera implementación concreta, comprendiendo la relación entre el modelo matemático, el código y la placa encargada de procesar los datos. A partir de estas pruebas iniciales, se comenzó a detectar qué partes del sistema debían modificarse o adaptarse para avanzar con el desarrollo del simulador.

## Tercera etapa: visualización de las derivaciones.
Antes de conectar la Raspberry Pi Pico a la placa electrónica completa, se trabajó sobre el código de generación de las señales para comprender su funcionamiento y comprobar las salidas de manera independiente.

El programa utilizaba un conjunto de datos previamente almacenado, organizado en diferentes canales. Cada canal contenía las muestras correspondientes a una señal electrocardiográfica, y el código recorría esos valores de manera secuencial para reconstruir las distintas derivaciones a lo largo del tiempo.

En esta primera versión se trabajó con un único banco de datos, es decir, con un conjunto fijo de derivaciones. Todavía no se había incorporado la posibilidad de seleccionar o alternar entre diferentes bancos de señales. El objetivo inicial era verificar el funcionamiento básico del programa y comprobar que cada canal reprodujera correctamente la forma de onda que le correspondía.

Para realizar estas comprobaciones se utilizaron el Serial Monitor y el Serial Plotter del entorno de Arduino. A través de la comunicación serie, la Raspberry Pi Pico enviaba a la computadora los valores que iba leyendo del banco de datos. Esto permitió observar las diferentes derivaciones, verificar que las muestras se recorrieran en el orden correcto y comprobar que las señales se repitieran de manera continua.

La visualización también permitió analizar la velocidad de reproducción de las señales. El tiempo establecido entre una muestra y la siguiente determinaba qué tan rápido se representaba el electrocardiograma, por lo que fue necesario realizar distintas pruebas hasta conseguir una visualización adecuada.

Esta etapa fue fundamental para relacionar la organización de los datos con las señales observadas. Se pudo identificar qué canal correspondía a cada derivación y comprobar el funcionamiento del código antes de utilizar las salidas físicas de la Raspberry Pi Pico e integrarlas con el resto del circuito.

Una vez validado el funcionamiento del primer banco de señales, se avanzó posteriormente en la incorporación de nuevos bancos y en el desarrollo de un sistema que permitiera cambiar entre ellos.

## Cuarta etapa: integración de la Raspberry Pi Pico con la placa electrónica.
Una vez comprobado el funcionamiento del código mediante el Serial Monitor y el Serial Plotter, se avanzó con la integración física de la Raspberry Pi Pico a la placa electrónica proporcionada por la cátedra.

La Raspberry Pi Pico fue soldada a la placa respetando el esquema de conexiones y la distribución de sus pines. La placa contaba con ocho canales de entrada conectados a la Raspberry y con sus correspondientes etapas de filtrado y acondicionamiento, a partir de las cuales se obtenían las distintas señales de salida.

Durante las primeras pruebas se detectó un inconveniente con el LED indicador de funcionamiento. En lugar de parpadear durante la ejecución del programa, permanecía encendido constantemente. Luego de revisar el esquema, el código y la conexión física, se modificó la asignación utilizada en el programa y se configuró el LED en el pin 5. A partir de este cambio, el LED comenzó a parpadear correctamente, permitiendo utilizarlo como una indicación visual de que el programa se encontraba ejecutándose y recorriendo las muestras de las señales.

También fue necesario revisar la correspondencia entre los canales definidos en el código y las salidas físicas de la placa. Durante las pruebas se observó que una de las derivaciones aparecía en una salida diferente de la esperada, debido a que la asignación de uno de los canales no coincidía con el conexionado de la placa.

Para corregirlo, se compararon nuevamente el pinout de la Raspberry Pi Pico, el esquema electrónico y la distribución de los canales en el programa. Luego se modificaron las salidas utilizadas por el código para que cada derivación se obtuviera en el canal físico correspondiente.

Finalmente, se conectó la batería al circuito de alimentación previsto en la placa, lo que permitió comenzar a probar el dispositivo de manera autónoma, sin depender únicamente de la alimentación entregada por el puerto USB.

Esta etapa permitió comprobar que no era suficiente con que las señales se visualizaran correctamente en la computadora, sino que también debía existir una correspondencia precisa entre las asignaciones del código, los pines de la Raspberry Pi Pico y las salidas de la placa. Una vez corregidos estos aspectos, se pudo avanzar con la incorporación de nuevos bancos de señales y de los controles necesarios para seleccionarlos.

## Quinta etapa: incorporación de nuevos bancos de señales y control mediante botones.
Una vez comprobado el funcionamiento de la Raspberry Pi Pico junto con la placa electrónica, se avanzó en la ampliación de las señales disponibles en el simulador. Hasta ese momento se trabajaba con un único banco de datos, por lo que el dispositivo reproducía siempre el mismo registro electrocardiográfico. El objetivo de esta etapa fue incorporar diferentes condiciones cardíacas y permitir que el usuario pudiera seleccionarlas directamente desde la placa.

### Obtención de nuevos registros electrocardiográficos
Para incorporar nuevas señales se utilizó la base de datos PTB-XL, disponible públicamente en la plataforma PhysioNet. Se trata de una base de datos de electrocardiogramas clínicos de 12 derivaciones, con registros de 10 segundos de duración. PhysioNet proporciona los registros originales a 500 Hz (records500) y una versión reducida a 100 Hz (records100), que fue la utilizada para este desarrollo. La base también incluye el archivo ptbxl_database.csv, donde se encuentra la información asociada a cada ECG y su clasificación diagnóstica.

Dentro de PTB-XL, los diagnósticos se agrupan en cinco grandes superclases: NORMAL (ECG normal), MI (infarto de miocardio), STTC (cambios del segmento ST y/o de la onda T), CD (trastornos de conducción) y HYP (hipertrofia). Estas cinco categorías fueron tomadas como referencia para conformar los cinco bancos del simulador.

La fuente utilizada puede consultarse en la página oficial de PTB-XL en PhysioNet.

De cada categoría se seleccionó un registro:
- Banco 0 – Normal (NORM): registro 00003_lr.
- Banco 1 – Infarto de miocardio (MI): registro 00177_lr.
- Banco 2 – Cambios ST/T (STTC): registro 00022_lr.
- Banco 3 – Trastorno de conducción (CD): registro 00180_lr.
- Banco 4 – Hipertrofia (HYP): registro 00138_lr.

Los archivos originales contienen las doce derivaciones convencionales. Sin embargo, como la placa desarrollada dispone de ocho canales de salida, se seleccionaron las derivaciones I, II, V1, V2, V3, V4, V5 y V6. De esta manera se conservaron dos derivaciones de los miembros y las seis derivaciones precordiales.

### Recorte y adaptación de las señales
Como los registros utilizados provenían de records100, cada segundo de señal contenía 100 muestras, es decir, existía una separación temporal de 10 ms entre muestras. Los registros completos tenían una duración mayor a la necesaria para el funcionamiento del simulador, por lo que se decidió trabajar con un único latido representativo de cada condición.

Para realizar el recorte se identificó aproximadamente la posición del pico R de cada registro y se conservaron 80 muestras por banco, tomando 25 muestras anteriores al pico R y 55 muestras a partir de esa zona posterior. De esta manera, cada banco quedó constituido por un fragmento de aproximadamente 0,8 segundos.

Los recortes utilizados fueron:
- NORM – 00003_lr: pico R aproximadamente en la muestra 857; se utilizaron aproximadamente las muestras 832 a 911.
- MI – 00177_lr: pico R aproximadamente en la muestra 211; se utilizaron aproximadamente las muestras 186 a 265.
- STTC – 00022_lr: pico R aproximadamente en la muestra 243; se utilizaron aproximadamente las muestras 218 a 297.
- CD – 00180_lr: pico R aproximadamente en la muestra 512; se utilizaron aproximadamente las muestras 487 a 566.
- HYP – 00138_lr: pico R aproximadamente en la muestra 721; se utilizaron aproximadamente las muestras 696 a 775.

El mismo intervalo temporal fue utilizado para las ocho derivaciones de cada registro, manteniendo así la sincronización entre todos los canales.

Además del recorte, los valores originales de las señales debieron adaptarse al rango utilizado por las salidas PWM de la Raspberry Pi Pico. Para esto se realizó un escalado de los datos tomando como valor de línea de base 30000 y aplicando la relación:

valor PWM = 30000 + 4000 × señal ECG centrada

Finalmente, los valores se limitaron al rango de 0 a 60000, compatible con la función utilizada para controlar las salidas PWM. Todos los datos procesados se almacenaron en el archivo ECGBancos_PTBXL_recortado.h, organizados como matrices de 80 muestras × 8 canales.

### Lógica para el cambio de banco
Una vez preparados los cinco bancos, se modificó el programa para que pudiera seleccionar cuál de ellos debía reproducirse. Para esto se incorporó una variable denominada Banco, cuyo valor podía variar entre 0 y 4.

Los bancos quedaron organizados de la siguiente manera:

0 Normal → 1 Infarto → 2 Cambios ST/T → 3 Trastorno de conducción → 4 Hipertrofia → 0 Normal

El cambio se realizó mediante una tecla conectada al GPIO20. El pulsador se configuró como INPUT_PULLUP, por lo que normalmente la entrada permanece en estado alto (HIGH) y la pulsación se reconoce cuando pasa a estado bajo (LOW).

Cada vez que se detecta una pulsación válida, el valor de Banco aumenta en una unidad. Al superar el último banco, vuelve automáticamente a cero. Además, cuando se produce un cambio de banco, el índice de lectura se reinicia para que la nueva señal comience desde el principio del latido.

Durante el desarrollo fue necesario incorporar también un antirrebote de 200 ms. Esto evita que una única pulsación física sea interpretada por el microcontrolador como varios accionamientos consecutivos.

El banco seleccionado se mostraba además mediante el Serial Monitor, lo que permitió verificar durante las pruebas que el pulsador estuviera siendo leído correctamente y que la secuencia de bancos coincidiera con la esperada.

### Incorporación del cambio de velocidad
Posteriormente se incorporó una segunda tecla para modificar la velocidad o frecuencia con la que se repetían los latidos. En la versión del programa desarrollada en esta etapa, esta segunda entrada se encontraba configurada en el GPIO21.

Una decisión importante fue mantener siempre el período original de muestreo de PTB-XL en 10 ms. De esta manera, el latido almacenado conserva su forma temporal y no se deforma al seleccionar otra velocidad.

En lugar de modificar el tiempo existente entre muestras, se incorporó una zona muerta entre el final de un latido y el comienzo del siguiente. Durante este intervalo, las ocho salidas permanecen en el valor correspondiente a la línea de base (30000). Al aumentar o disminuir la duración de esta pausa se modifica el tiempo total entre dos latidos y, por lo tanto, la frecuencia cardíaca simulada.

Se establecieron tres modos:
- Modo rápido: 80 muestras de señal y sin zona muerta adicional. Duración aproximada del ciclo: 0,8 s, equivalente a aproximadamente 75 latidos por minuto.
- Modo normal: 80 muestras de señal más 20 muestras de línea de base. Duración aproximada: 1,0 s, equivalente a aproximadamente 60 latidos por minuto.
- Modo lento: 80 muestras de señal más 60 muestras de línea de base. Duración aproximada: 1,4 s, equivalente a aproximadamente 43 latidos por minuto.

El programa inicia en el modo normal y cada pulsación de la segunda tecla permite avanzar de forma cíclica entre los tres modos:

Normal → Lento → Rápido → Normal

Al igual que en el cambio de banco, se incorporó un antirrebote de 200 ms y el modo seleccionado se mostraba en el Serial Monitor durante las pruebas.

### Organización del funcionamiento en la Raspberry Pi Pico
Para gestionar simultáneamente la generación de las señales y la lectura de los controles se aprovecharon los dos núcleos disponibles en la Raspberry Pi Pico. El ciclo principal se encargó de recorrer los datos del banco seleccionado y actualizar las ocho salidas PWM, mientras que el segundo núcleo se utilizó para controlar los botones y el LED indicador.

Las ocho salidas PWM quedaron asignadas a los GPIO 0, 1, 2, 3, 4, 6, 7 y 8. El GPIO5, que anteriormente había generado inconvenientes en la correspondencia entre las salidas y el LED, quedó reservado exclusivamente para este último. El LED se programó para cambiar de estado cada 500 ms, funcionando como una indicación visual de que el programa continuaba ejecutándose.

Durante las pruebas se continuó utilizando el Serial Plotter, enviando algunos de los canales generados para observar gráficamente las señales, mientras que el Serial Monitor permitió verificar los cambios de banco y de velocidad.

Con estas modificaciones, el simulador pasó de reproducir un único ECG fijo a disponer de cinco condiciones electrocardiográficas diferentes y tres frecuencias de repetición, seleccionables mediante controles incorporados a la propia placa, sin necesidad de modificar y volver a cargar el programa para cada prueba.
