

Vamos  a ver qué necesitamos para el programa

- Todas las constantes: G, las masas, etc

- Posiciones (la del sol es opcional, nos quitamos un vector)

- Velocidades iniciales

Tener en cuenta que los vectores velocidad tienen direción perpendicular a las posiciones.

## Podemos hacerlo de forma 'perezosa' en 2D

Ponemos todos los planetas en filas. El programa funciona igual, es una simplificación que no cambia mucho. 1D.

## Reescala

Si tenemos las posiciones podemos calcular las aceleraciones con la fórmula de Newton.

El espacio se mide en metros, pero nosotros REESCALAREMOS a UNIDADES ASTRONÓMICAS.
El tiempo se cambia de forma proporcional, las velocidades se reescalan también.

Si queremos simular muchos pasos debemos bajar la precisión para que no pete. 
Por ejemplo si hacemos 1000 pasos la precisión puede ser de 0,01 UA.

La energía PUEDE OSCILAR pero cuando hacemos la suma debe salir bien. NO DEBERIA NI CRECER MUCHO NI DECRECER MUCHO. 
Lo mejor es calcular la energía AL PRINCIPIO para 'depurar' esos fallos.


