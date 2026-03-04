# 🏥  El Laberinto Maestro

> *"Este es solo el comienzo. Escapa si puedes."*

Tras la batalla mental entre el Gato y el Ratón, las luces parpadearon y el código cobró vida. Este proyecto es la respuesta al reto del laboratorio subterráneo: un sistema escrito en C++ capaz de generar estructuras laberínticas complejas y, lo más importante, encontrar la salida antes de quedar atrapado en la memoria.

## 🛠️ 1. ¿Cómo se generó el laberinto?

Para construir los pasillos del hospital, implementé el algoritmo **DFS (Depth-First Search) con Backtracking**. 

Imagina a una excavadora ciega pero metódica:
1. Comienza en una coordenada inicial y marca el espacio como pasillo (`*` o espacio vacío).
2. Obtiene las 4 direcciones posibles (Arriba, Abajo, Izquierda, Derecha) y las **baraja aleatoriamente** utilizando `std::shuffle` y el motor `mt19937`. Esto garantiza que el laberinto sea único en cada ejecución.
3. Avanza dando "saltos" de dos en dos casillas. Si la casilla de destino es una pared no visitada, rompe el muro intermedio y hace una llamada recursiva desde esa nueva posición.
4. Cuando se topa con un callejón sin salida, el *backtracking* hace que retroceda sobre sus propios pasos hasta encontrar una intersección con paredes disponibles para seguir excavando.

## 🧠 2. ¿Qué algoritmo se usó para resolverlo?

No todo en la vida se resuelve con fuerza bruta. Para encontrar el camino de escape desde la Entrada ('E') hasta la Salida ('S'), el programa utiliza **BFS (Breadth-First Search)**.

* **Exploración justa:** A diferencia de DFS que se lanza por un solo camino hasta el fondo, BFS explora el laberinto "por capas", como una mancha de agua que se expande uniformemente.
* **El camino más corto:** Gracias a esta expansión, la primera vez que el algoritmo toca la casilla de salida, está matemáticamente garantizado que es la ruta más corta posible.
* **Reconstrucción de ruta:** Utilicé la estructura de datos `std::queue` para manejar los turnos de exploración y un `std::map<Punto, Punto>` para crear un "diccionario de padres". Una vez hallada la meta, el programa lee este mapa hacia atrás, dejando un rastro de puntos (`.`) hasta regresar al inicio.

## ⏱️ 3. Mediciones de Tiempo y Descubrimientos

Utilizando la librería `<chrono>` de C++, medí el rendimiento del procesador al generar y resolver la estructura.

**Descubrimientos:**
* El tiempo de ejecución (CPU time) para laberintos estándar (21x21 o 51x51) toma apenas una fracción de milisegundo. Las estructuras de datos como `std::vector` y `std::queue` en C++ son extremadamente eficientes para estas tareas de cuadrícula.
* El cuello de botella real de estos algoritmos no es el procesamiento lógico, sino la impresión por consola (`std::cout`). Escribir caracteres en la pantalla toma considerablemente más tiempo que calcular la matriz completa en la memoria.

**¿Qué haría distinto la próxima vez?**
* **Recursión vs. Iteración:** Actualmente, la generación DFS utiliza la pila de llamadas del sistema (recursividad). En laberintos de dimensiones masivas (ej. 1000x1000), esto causaría un error de *Stack Overflow*. La próxima evolución sería refactorizar el DFS a un formato iterativo utilizando una estructura `std::stack` manual.
* **Diseño Orientado a Objetos:** Encapsularía la lógica en una clase `Laberinto` para separar completamente los datos de la lógica de renderizado, permitiendo instanciar múltiples pisos del hospital simultáneamente.

## ⚙️ Instrucciones de Uso

El tamaño del laberinto no está quemado en el código (Hardcoded). Al ejecutar el programa, la consola solicitará la dimensión deseada.
* *Nota:* Por la matemática de los algoritmos de generación, la dimensión ingresada debe ser un número impar (ej. 15, 21, 55). Si se ingresa un número par, el programa lo ajustará automáticamente al siguiente impar para evitar colapso de muros.

---
*Desarrollado con lógica, C++ y mucha resiliencia para Penguin Academy.*