#include <iostream>
#include <vector>

using namespace std;

int main() {
    int filas, columnas;

    bool esCoordenadaValida(int x, int y, int filas, int columnas) {
    // La lógica es: 0 <= x < filas  Y  0 <= y < columnas
    return (x >= 0 && x < filas) && (y >= 0 && y < columnas);
}

    // 1. Pedir dimensiones al usuario
    cout << "Introduce el numero de filas: ";
    cin >> filas;
    cout << "Introduce el numero de columnas: ";
    cin >> columnas;

    // 2. Crear la matriz e inicializarla con 1s
    // vector<vector<int>> nombre(filas, vector<int>(columnas, valor_inicial))
    vector<vector<int>> matriz(filas, vector<int>(columnas, 1));

    // 3. Imprimir la matriz
    cout << "\nMatriz resultante:" << endl;
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            cout << matriz[i][j] << " ";
        }
        cout << endl; // Salto de línea al terminar cada fila
        
    }

    
    return 0;
}




// Definición de la función: Recibe la coordenada actual (y, x) y el mapa original (por referencia '&' para modificarlo directamente).
void generarLaberinto(int y, int x, vector<vector<char>>& mapa) {
    
    // 1. LA MARCA: Al pisar esta celda, la transforma de pared ('#') a pasillo ('*').
    mapa[y][x] = '*'; 

    // 2. EL AZAR: Crea una lista con los números 0, 1, 2, 3 que representan las 4 direcciones (Arriba, Abajo, Izquierda, Derecha).
    vector<int> orden = {0, 1, 2, 3};
    
    // 3. LA SEMILLA: Prepara el generador de números aleatorios del sistema.
    random_device rd;
    
    // 4. EL MOTOR: Usa la semilla para crear un motor de mezcla matemática (Mersenne Twister).
    mt19937 g(rd());
    
    // 5. LA BARAJA: Desordena la lista 'orden'. Ejemplo: en vez de {0,1,2,3} ahora es {2,0,3,1}. Esto hace que el minero no cave siempre hacia el mismo lado primero.
    shuffle(orden.begin(), orden.end(), g);

    // 6. EL EXPLORADOR: Un bucle que intentará moverse en las 4 direcciones, una por una, según el orden barajado.
    for (int i = 0; i < 4; i++) {
        
        // 7. Selecciona la dirección actual del ciclo.
        int dir = orden[i];
        
        // 8. Calcula el destino Y saltando 2 casillas (para dejar espacio para un muro en medio).
        int ny = y + dy[dir];
        
        // 9. Calcula el destino X saltando 2 casillas.
        int nx = x + dx[dir];

        // 10. EL GUARDIA DE LÍMITES: ¿El salto cayó dentro del mapa sin tocar el borde exterior intocable?
        if (ny > 0 && ny < mapa.size() - 1 && nx > 0 && nx < mapa[0].size() - 1) {
            
            // 11. EL DETECTOR DE ROCA: ¿La casilla donde caímos sigue siendo una pared sólida ('#')?
            if (mapa[ny][nx] == '#') {
                
                // 12. EL MAZO: Si es pared, rompemos la casilla intermedia (dividiendo el salto entre 2) para conectar el pasillo actual con el nuevo.
                mapa[y + dy[dir] / 2][x + dx[dir] / 2] = '*';
                
                // 13. LA RECURSIÓN: ¡La magia! La función se "pausa" a sí misma y se vuelve a invocar desde la nueva celda descubierta.
                generarLaberinto(ny, nx, mapa); 
            }
        }
    }
} // 14. EL FIN: Cuando el bucle for termina (i llega a 4), la función se destruye y devuelve el control a quien la llamó.














// 68. Recibe inicio, fin y el mapa. Retorna 'true' si hay salida, 'false' si no.
bool resolverLaberinto(int startY, int startX, int endY, int endX, vector<vector<char>>& mapa) {
    
    queue<Punto> fila;             // 69. Crea la fila de espera.
    fila.push({startY, startX});   // 70. Mete la celda de inicio a la fila para detonar la búsqueda.

    map<Punto, Punto> padres;      // 73. Crea la libreta para rastrear quién descubrió a quién.
    // 74. Crea una matriz del mismo tamaño que el mapa, llena de 'false' (nadie visitado aún).
    vector<vector<bool>> visitado(mapa.size(), vector<bool>(mapa[0].size(), false));

    visitado[startY][startX] = true; // 76. Marca el inicio como visitado. ¡No queremos volver aquí!
    bool encontrado = false;         // 77. Variable bandera. Se alzará cuando toquemos la salida.

    // 79. EL MOTOR: Este bucle se repite mientras haya casillas en la fila de espera.
    while (!fila.empty()) { 
        Punto actual = fila.front(); // 80. Llama a la siguiente casilla en la fila.
        fila.pop();                  // 81. La saca de la fila (ya la estamos atendiendo).

        // 83. CONDICIÓN DE VICTORIA: ¿Esta casilla que estoy mirando es la salida?
        if (actual.y == endY && actual.x == endX) { 
            encontrado = true;       // 84. ¡Alzamos la bandera de victoria!
            break;                   // 85. 'break' destruye el bucle 'while' por completo. Ya no buscamos más.
        }

        // 88. Revisa las 4 celdas vecinas a la casilla actual.
        for (int i = 0; i < 4; i++) { 
            int ny = actual.y + ry[i]; // 89. Calcula coordenada Y del vecino.
            int nx = actual.x + rx[i]; // 90. Calcula coordenada X del vecino.

            // 93. FILTRO DE SEGURIDAD: ¿Está en el mapa? Y ¿No es muro? Y ¿No lo hemos visitado?
            if (ny >= 0 && ny < mapa.size() && nx >= 0 && nx < mapa[0].size() && 
                mapa[ny][nx] != '#' && !visitado[ny][nx]) { 
                
                visitado[ny][nx] = true;   // 96. Lo marcamos para no volver a explorarlo.
                padres[{ny, nx}] = actual; // 97. ¡La Libreta! Anota: "Al vecino (ny,nx) llegué desde 'actual'".
                fila.push({ny, nx});       // 98. Mete al vecino al final de la fila para explorar sus propios caminos luego.
            }
        }
    }

    // 104. FASE DE RECONSTRUCCIÓN: Solo se ejecuta si la bandera se alzó (llegamos al final).
    if (encontrado) { 
        Punto paso = padres[{endY, endX}]; // 105. Lee la libreta buscando quién nos trajo a la meta.
        
        // 106. Camina hacia atrás, leyendo la libreta, hasta llegar al inicio.
        while (!(paso.y == startY && paso.x == startX)) { 
            mapa[paso.y][paso.x] = '.';    // 107. Deja una miguita de pan (el punto de la solución) en el mapa.
            paso = padres[paso];           // 108. Pregunta a la libreta: "¿Y a ti, quién te trajo?".
        }
        return true;                       // 110. Termina la función con éxito.
    }
    return false; // 112. Si la fila se vació y nunca encontramos la salida, retorna falso.
}















// 1. Recibe el 'mapa'. Usamos 'const' para asegurar que la impresora NO modifique el laberinto, solo lo lea. El '&' evita copiar todo el mapa de nuevo.
void mostrarLaberinto(const vector<vector<char>>& mapa) {
    
    // 2. Bucle "For-Each": "Por cada 'fila' (que el compilador deduce con 'auto' que es un vector de letras) dentro del 'mapa'..."
    for (const auto& fila : mapa) { 
        
        // 3. Otro bucle: "Por cada letra ('celda') dentro de esa 'fila'..."
        for (char celda : fila) { 
            
            // 4. Transformación visual: Si en la memoria hay un muro '#', imprime en la pantalla el bloque sólido "█ ".
            if (celda == '#') cout << "█ "; 
            
            // 5. Si en la memoria hay un punto '.', imprime el círculo "o " (la ruta ganadora).
            else if (celda == '.') cout << "o "; 
            
            // 6. Si es cualquier otra cosa (espacio vacío, 'E', o 'S'), imprímelo tal cual con un espacio al lado para que sea cuadrado.
            else cout << celda << " "; 
        }
        // 7. Salto de línea: Al terminar de imprimir todas las celdas de una fila, presiona "Enter" en la consola para empezar la fila de abajo.
        cout << endl; 
    }
}









int main() {
    int n; // 1. Prepara la caja para guardar la dimensión.
    cout << "Ingrese dimension (debe ser impar): "; // 2. Pide el dato al jugador.
    cin >> n; // 3. Guarda lo que el jugador tecleó en 'n'.

    // 4. Si el usuario metió un número par (el residuo de n entre 2 es 0), le suma 1. Esto salva tu algoritmo DFS de romperse.
    if (n % 2 == 0) n++; 

    // 5. Crea la cuadrícula gigante (n x n) y la rellena toda con muros '#'.
    vector<vector<char>> laberinto(n, vector<char>(n, '#')); 
    
    // 6. Arranca el cronómetro usando el reloj del sistema. 'auto' nos salva de escribir el tipo gigante.
    auto inicio = chrono::high_resolution_clock::now(); 

    // 7. Manda a la excavadora (DFS) a tallar los pasillos empezando en la coordenada (1, 1).
    generarLaberinto(1, 1, laberinto); 

    // 8. Define matemáticamente dónde está el inicio (arriba a la izquierda) y el final (abajo a la derecha, evitando el muro exterior).
    int startY = 1, startX = 1; 
    int endY = n - 2, endX = n - 2; 
    
    // 9. Coloca literalmente las letras 'E' y 'S' en esas coordenadas del mapa.
    laberinto[startY][startX] = 'E'; 
    laberinto[endY][endX] = 'S'; 

    // 10. Manda al explorador (BFS) a encontrar el camino desde la 'E' hasta la 'S' y dejar el rastro de puntos.
    resolverLaberinto(startY, startX, endY, endX, laberinto); 

    // 11. Detiene el cronómetro.
    auto fin = chrono::high_resolution_clock::now(); 
    
    // 12. Calcula cuánto tiempo pasó entre el inicio y el fin, y convierte ese resultado a milisegundos (milli).
    chrono::duration<double, milli> tiempo = fin - inicio; 

    // 13. Llama a la función "impresora" para mostrar la obra de arte terminada.
    mostrarLaberinto(laberinto); 
    
    // 14. Imprime el reporte técnico con el tiempo exacto que tardó el CPU.
    cout << "\n[INFO] Tiempo de ejecucion: " << tiempo.count() << " ms" << endl; 

    return 0; // 15. Le avisa a Windows que el programa terminó de forma exitosa.
}