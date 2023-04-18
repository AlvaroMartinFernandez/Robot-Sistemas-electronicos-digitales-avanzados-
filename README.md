# Robot-Sistemas-electronicos-digitales-avanzados-
Robot con traccion diferencial
Se propone diseñar un sistema empotrado basado en el microcontrolador LPC1768 (Cortex-M3) con
objeto de implementar un robot con tracción diferencial capaz de evitar obstáculos mientras sigue una
fuente de luz, grabar y ser telecontrolado con el mando de la Wii. Para ello, se partirá de una plataforma
mecánica que incluye dos motores de corriente continua que proporcionarán la tracción a las dos ruedas
delanteras, además de una tercera rueda que da la estabilidad a la plataforma mecánica. Dos sensores de
luz situados sobre un servomotor permitirán identificar la fuente de luz y un sensor de ultrasonidos,
situado en la parte frontal, permitirá que el robot detecte obstáculos. Además, el robot enviará y recibirá
información mediante una comunicación serie asíncrona y tendrá implementada una página web para su
configuración. 

Especificaciones del sistema
El robot deberá cumplir con las siguientes especificaciones:
1. El robot tendrá tres modos de funcionamiento: Manual, Automático y Depuración.
2. Modo Manual:
a. El movimiento del robot será controlado con el mando Nunchuck de la Wii con el joystick.
b. El movimiento del servomotor se controlará con inclinaciones la del mando Nunchuck.
c. Si detecta un objeto delante del robot, el robot se detendrá y emitirá un pitido discontinuo
de un segundo de periodo.
d. Al pulsar el botón C del mando Nunchuck, el robot grabará audio y al pulsar el botón Z del
mando lo reproducirá.
e. El robot pasará al modo Automático cuando se pulsen los dos botones del mando de la Wii
de forma simultánea durante menos de 2 segundos o los pulsadores Key1 y Key2 de la
tarjeta, y pasará al modo Depuración cuando se pulsen los dos botones durante más de 2
segundos o los pulsadores Key1 y Key2 de la tarjeta.
f. El display visualizará la información recibida por los sensores y la velocidad de los
motores.
3. Modo Automático:
a. El robot se moverá aleatoriamente buscando una fuente de luz lo suficientemente fuerte
interesa ser seguida. Cuando se detecte la fuente de luz, el robot la seguirá.
b. Si en el recorrido el robot detecta un objeto frente a él, intentará evitarlo para mantenerse
siguiendo la luz y emitirá un pitido discontinuo de un segundo de periodo.
c. Si el robot deja de detectar la luz, intentará buscarla con dos barridos de 180º del
servomotor y si no la encuentra, se iniciará el movimiento aleatorio de navegación.
d. El robot pasará a modo Manual al pulsar el botón C del mando Nunchuck durante más de
2 segundos.
e. El robot pasará a modo Configuración cuando se pulsen los dos pulsadores Key1 y Key2
de la tarjeta durante más de 2 segundos.
4. Modo Depuración:
a. Utilizando el display táctil se podrá controlar el movimiento de cada motor por separado
(motores derecho e izquierdo y el servomotor), visualizar las medidas de los sensores, y
las variables que proporciona el mando Nunchuck.
b. También se visualizará la dirección IP de la tarjeta.
c. Al pulsar Key 1, grabará audio durante un segundo y al pulsar Key 2, reproducirá el audio
grabado.
d. El robot pasará al modo Automático cuando se pulsen simultáneamente durante menos
de 2 segundos los pulsadores Key1 y Key2 de la tarjeta, y pasará al modo Manual cuando
se pulsen los pulsadores Key1 y Key2 de la tarjeta durante más de 2 segundos.
5. En todo momento, el robot debe responder a una comunicación serie asíncrona donde un terminal
remoto solicite al robot el valor de una variable (velocidad de los motores, posición del servo,
medida de los sensores de luz, medida del sensor de ultrasonidos). La solicitud la hará con un
comendo que podrá solicitar una medida o un determinado número de medidas indicando el
tiempo entre medidas. Habrá un comando para detener la transmisión. La conexión podría ser
4
mediante un cable USB a través del adaptador USB-serie o mediante un módulo que permita la
comunicación inalámbrica Bluettoth (HC05 o HC06 por ejemplo).
6. Servidor Web empotrado para, a través de la una conexión TCP-IP:
a. Visualizar algunos parámetros del sistema (los que se crean más interesantes)
b. Visualizar el modo en el que está configurado el robot
c. Controlar la activación y velocidad de los motores y/o del servomotor cuando el robot esté
en modo Depuración. Si no está en este modo, no responderá.
7. El watchdog debe estar activado en todo momento y el programa debe evitar esperas activas no
controladas
