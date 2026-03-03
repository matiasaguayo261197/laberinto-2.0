#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <queue>
#include <chrono>
#include <map>

using namespace std;

// Estructura para manejar coordenadas y reconstrucción de ruta
struct Punto { // crea una caja o molde donde va a reservar dos espacios
    int y, x;  // son el nombre de los espacios dentro de la caja 
    // Sobrecarga de operador para poder usar "Punto" como clave en un mapa
    bool operator<(const Punto& otro) const {  
        if (y != otro.y)  return y < otro.y; // PASO 1: ¿Están en diferentes filas?
                                            // dime: ¿MI fila está más arriba que la suya? Si es así, yo voy primero".
                                            // PASO 2: Si están en la misma fila, desempatamos con la columna
        return x < otro.x;
                            //TRADUCCIÓN: "Pero si estamos en la misma fila,
                            // entonces dime: ¿MI posición (x) está más a la izquierda que la suya?
                            // Si es así, yo voy primero".
    }
};

// Direcciones de movimiento (Salto de 2 para generación, paso de 1 para resolución)
int dy[] = {-2, 2, 0, 0};
int dx[] = {0, 0, -2, 2};
//(Generación): Tienen el número 2 porque le dices al "escultor" que dé pasos largos para saltar un muro y dejar un pasillo en medio.
int ry[] = {-1, 1, 0, 0};
int rx[] = {0, 0, -1, 1};
//(Resolución): Tienen el número 1 porque le dices al "explorador" que camine paso a paso, celda por celda, buscando la salida.

// ================================================================
// FUNCIÓN: VISUALIZACIÓN
// ================================================================

void mostrarLaberinto(const vector<vector<char>>& mapa) {// 1. LA CABECERA: Definimos el nombre y qué herramientas necesita 
    for (const auto& fila : mapa) {  // 2. EL BUCLE EXTERIOR: Recorremos las filas (de arriba hacia abajo)
        for (char celda : fila) {   // 3. EL BUCLE INTERIOR: Recorremos cada ladrillo de la fila (de izquierda a derecha)
            if (celda == '#') cout << "█ "; // Muro sólido
            else if (celda == '.') cout << "o "; // Camino de la solución // Si es parte de la solución, pon una 'o'
            else cout << celda << " ";      // Entrada (E), Salida (S) o Camino (*) / Si es 'E', 'S' o '*', ponlo tal cual
        }
        cout << endl; // 5. EL SALTO DE LÍNEA: Crucial para la forma del laberinto
    }
}

// ================================================================
// FUNCIÓN: GENERACIÓN (DFS Aleatorio)
// ================================================================
// 53. Define la función: recibe coordenadas iniciales (y, x) y el mapa por referencia (&) para modificarlo directamente.
void generarLaberinto(int y, int x, vector<vector<char>>& mapa) {
    mapa[y][x] = '*';                          // 54. Marca la casilla actual como un camino (espacio vacío).

    vector<int> orden = {0, 1, 2, 3};          // 56. Crea una lista con los índices de las 4 direcciones posibles.
    random_device rd;                          // 57. Generador de números aleatorios (el "grano" de azar).
    mt19937 g(rd());                           // 58. Motor que mezcla los números de forma impredecible.
    shuffle(orden.begin(), orden.end(), g);    // 59. ¡Baraja! Cambia el orden de las direcciones para que cada laberinto sea único.

    for (int i = 0; i < 4; i++) {              // 61. Intenta moverte en cada una de las 4 direcciones (en el orden barajado).
        int dir = orden[i];                    // 62. Toma la dirección actual del ciclo.
        int ny = y + dy[dir];                  // 63. Calcula la nueva coordenada Y (saltando una casilla para dejar pared).
        int nx = x + dx[dir];                  // 64. Calcula la nueva coordenada X (saltando una casilla para dejar pared).

        // 66. Verifica: ¿La nueva posición está dentro de los límites del mapa (dejando un borde de 1 bloque)?
        if (ny > 0 && ny < mapa.size() - 1 && nx > 0 && nx < mapa[0].size() - 1) {
            if (mapa[ny][nx] == '#') {         // 67. ¿Ese lugar todavía es una pared ('#')? Si es así, podemos ir ahí.
                // 68. Rompe la pared intermedia entre la posición actual y la nueva para conectar el camino.
                mapa[y + dy[dir] / 2][x + dx[dir] / 2] = '*'; 
                generarLaberinto(ny, nx, mapa); // 69. ¡RECURSIÓN! Se llama a sí misma para seguir excavando desde la nueva casilla.
            }
        }
    }
}

// ================================================================
// FUNCIÓN: RESOLUCIÓN (BFS con Reconstrucción de Camino)
// ================================================================
// 78. Define la función: recibe coordenadas de inicio, fin y el mapa del laberinto (por referencia '&' para no copiarlo todo)
bool resolverLaberinto(int startY, int startX, int endY, int endX, vector<vector<char>>& mapa) {
    queue<Punto> fila;                          // 79. Crea la 'fila' (cola) para el algoritmo BFS
    fila.push({startY, startX});               // 80. Mete el punto de partida en la fila para empezar a buscar

    map<Punto, Punto> padres;                  // 82. ¡CLAVE! Crea un "diccionario" para recordar quién te llevó a cada casilla
    // 83. Crea una matriz de booleanos del mismo tamaño que el mapa, inicializada en 'false' (nadie ha sido visitado aún)
    vector<vector<bool>> visitado(mapa.size(), vector<bool>(mapa[0].size(), false));

    visitado[startY][startX] = true;           // 85. Marca la casilla de inicio como visitada para no volver ahí
    bool encontrado = false;                   // 86. Variable "bandera" para saber si logramos llegar al final

    while (!fila.empty()) {                    // 88. Bucle: Mientras haya casillas pendientes por revisar...
        Punto actual = fila.front();           // 89. Mira la casilla que está al frente de la fila
        fila.pop();                            // 90. Sácala de la fila (ya la estamos procesando)

        if (actual.y == endY && actual.x == endX) { // 92. ¿Esta casilla es la meta?
            encontrado = true;                 // 93. Si sí, marcamos que lo logramos...
            break;                             // 94. ...y rompemos el bucle 'while' (no hace falta buscar más)
        }

        for (int i = 0; i < 4; i++) {          // 97. Revisa las 4 direcciones (Arriba, Abajo, Izquierda, Derecha)
            int ny = actual.y + ry[i];         // 98. Calcula la nueva coordenada Y (fila)
            int nx = actual.x + rx[i];         // 99. Calcula la nueva coordenada X (columna)

            // 101-102. Gran IF de seguridad: ¿Está dentro del mapa? ¿No es pared ('#')? ¿Y no la hemos visitado?
            if (ny >= 0 && ny < mapa.size() && nx >= 0 && nx < mapa[0].size() && 
                mapa[ny][nx] != '#' && !visitado[ny][nx]) {
                
                visitado[ny][nx] = true;       // 104. Marcamos la nueva casilla como visitada
                padres[{ny, nx}] = actual;     // 105. ¡IMPORTANTE! Guarda que a 'ny,nx' llegamos desde 'actual'
                fila.push({ny, nx});           // 106. Agrégala a la fila para explorar sus vecinos después
            }
        }
    }

    // 112. Si encontramos la salida, vamos a dibujar el camino
    if (encontrado) {
        Punto paso = padres[{endY, endX}];     // 113. Empezamos desde el final (antes de la meta)
        // 114. Mientras no hayamos regresado al punto de inicio...
        while (!(paso.y == startY && paso.x == startX)) {
            mapa[paso.y][paso.x] = '.';        // 115. Ponemos un puntito en el mapa
            paso = padres[paso];               // 116. Retrocedemos al "padre" de este punto
        }
        return true;                           // 118. Terminamos con éxito
    }
    return false;                              // 120. Si llegamos aquí, es que no hay salida
}
// ================================================================
// FUNCIÓN PRINCIPAL
// ================================================================
int main() {                                   // Inicio del programa: el punto de entrada obligatorio
    int n;                                     // Declaración: crea una variable entera para la dimensión
    cout << "=== GENERADOR DE LABERINTOS MAESTRO ===\n"; // Salida: imprime el título en consola
    cout << "Ingrese dimension (debe ser impar, ej. 21): "; // Pide al usuario el tamaño del mapa
    cin >> n;                                  // Entrada: guarda el número que el usuario teclea en 'n'

    if (n % 2 == 0) n++;                       // Operador %: si el número es par, le suma 1 para hacerlo impar

    // Crea el laberinto: un vector de vectores (matriz) de tamaño n x n, lleno de paredes '#'
    vector<vector<char>> laberinto(n, vector<char>(n, '#'));

    // Inicia el cronómetro: captura el tiempo exacto antes de empezar a trabajar
    auto inicio = chrono::high_resolution_clock::now();

    // 1. Generar estructura: llama a la función DFS para "excavar" los pasillos
    generarLaberinto(1, 1, laberinto);

    // 2. Definir Entrada y Salida: coordenadas fijas para el inicio y el fin
    int startY = 1, startX = 1;                // Coordenadas de inicio (siempre en 1,1)
    int endY = n - 2, endX = n - 2;            // Coordenadas de fin (ajustadas al tamaño del mapa)
    laberinto[startY][startX] = 'E';           // Coloca la 'E' de Entrada en el mapa
    laberinto[endY][endX] = 'S';               // Coloca la 'S' de Salida en el mapa

    // 3. Resolver: llama a la función BFS para encontrar el camino de E a S
    resolverLaberinto(startY, startX, endY, endX, laberinto);

    // Detiene el cronómetro: captura el tiempo justo después de terminar
    auto fin = chrono::high_resolution_clock::now();
    // Calcula la diferencia entre fin e inicio en milisegundos (ms)
    chrono::duration<double, milli> tiempo = fin - inicio;

    // 4. Mostrar resultado: llama a la función que imprime el dibujo final en pantalla
    mostrarLaberinto(laberinto);
    // Imprime información técnica: tamaño del laberinto y cuánto tardó la CPU en procesarlo
    cout << "\n[INFO] Laberinto de " << n << "x" << n << " generado y resuelto." << endl;
    cout << "[INFO] Tiempo de ejecucion: " << tiempo.count() << " ms" << endl;

    return 0;                                  // Fin: indica al sistema que el programa terminó con éxito
}