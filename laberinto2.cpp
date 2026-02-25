
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <queue>
#include <chrono>
#include <map>

using namespace std;

// Estructura para manejar coordenadas y reconstrucción de ruta
struct Punto { // objeto con dos ranuras para guardar ej y eje x 
    int y, x;  // dentro de la caja solo se guardaran numeros enteros 
    // Sobrecarga de operador para poder usar Punto como clave en un mapa
    bool operator<(const Punto& otro) const {  // la respuesta solo puede ser T o F 
        if (y != otro.y) return y < otro.y;
        return x < otro.x;
    }
};

// Direcciones de movimiento (Salto de 2 para generación, paso de 1 para resolución)
int dy[] = {-2, 2, 0, 0};
int dx[] = {0, 0, -2, 2};
int ry[] = {-1, 1, 0, 0};
int rx[] = {0, 0, -1, 1};

// ================================================================
// FUNCIÓN: VISUALIZACIÓN
// ================================================================
void mostrarLaberinto(const vector<vector<char>>& mapa) {
    for (const auto& fila : mapa) {
        for (char celda : fila) {
            if (celda == '#') cout << "█ "; // Muro sólido
            else if (celda == '.') cout << "o "; // Camino de la solución
            else cout << celda << " ";      // Entrada (E), Salida (S) o Camino (*)
        }
        cout << endl;
    }
}

// ================================================================
// FUNCIÓN: GENERACIÓN (DFS Aleatorio)
// ================================================================
void generarLaberinto(int y, int x, vector<vector<char>>& mapa) {
    mapa[y][x] = '*'; // Requisito: Camino representado por '*'

    vector<int> orden = {0, 1, 2, 3};
    random_device rd;
    mt19937 g(rd());
    shuffle(orden.begin(), orden.end(), g);

    for (int i = 0; i < 4; i++) {
        int dir = orden[i];
        int ny = y + dy[dir];
        int nx = x + dx[dir];

        if (ny > 0 && ny < mapa.size() - 1 && nx > 0 && nx < mapa[0].size() - 1) {
            if (mapa[ny][nx] == '#') {
                mapa[y + dy[dir] / 2][x + dx[dir] / 2] = '*';
                generarLaberinto(ny, nx, mapa);
            }
        }
    }
}

// ================================================================
// FUNCIÓN: RESOLUCIÓN (BFS con Reconstrucción de Camino)
// ================================================================
bool resolverLaberinto(int startY, int startX, int endY, int endX, vector<vector<char>>& mapa) {
    queue<Punto> fila;
    fila.push({startY, startX});
    
    map<Punto, Punto> padres; // Para rastrear de dónde venimos
    vector<vector<bool>> visitado(mapa.size(), vector<bool>(mapa[0].size(), false));
    
    visitado[startY][startX] = true;
    bool encontrado = false;

    while (!fila.empty()) {
        Punto actual = fila.front();
        fila.pop();

        if (actual.y == endY && actual.x == endX) {
            encontrado = true;
            break;
        }

        for (int i = 0; i < 4; i++) {
            int ny = actual.y + ry[i];
            int nx = actual.x + rx[i];

            if (ny >= 0 && ny < mapa.size() && nx >= 0 && nx < mapa[0].size() &&
                mapa[ny][nx] != '#' && !visitado[ny][nx]) {
                
                visitado[ny][nx] = true;
                padres[{ny, nx}] = actual;
                fila.push({ny, nx});
            }
        }
    }

    // Reconstrucción del camino con puntos '.'
    if (encontrado) {
        Punto paso = padres[{endY, endX}];
        while (!(paso.y == startY && paso.x == startX)) {
            mapa[paso.y][paso.x] = '.'; // Marcamos la ruta ganadora
            paso = padres[paso];
        }
        return true;
    }
    return false;
}

// ================================================================
// FUNCIÓN PRINCIPAL
// ================================================================
int main() {
    int n;
    cout << "=== GENERADOR DE LABERINTOS MAESTRO ===\n";
    cout << "Ingrese dimension (debe ser impar, ej. 21): ";
    cin >> n;

    if (n % 2 == 0) n++; // Asegurar que sea impar para la lógica de muros

    vector<vector<char>> laberinto(n, vector<char>(n, '#'));

    auto inicio = chrono::high_resolution_clock::now();

    // 1. Generar estructura
    generarLaberinto(1, 1, laberinto);

    // 2. Definir Entrada y Salida
    int startY = 1, startX = 1;
    int endY = n - 2, endX = n - 2;
    laberinto[startY][startX] = 'E';
    laberinto[endY][endX] = 'S';

    // 3. Resolver
    resolverLaberinto(startY, startX, endY, endX, laberinto);

    auto fin = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> tiempo = fin - inicio;

    // 4. Mostrar resultado
    mostrarLaberinto(laberinto);
    cout << "\n[INFO] Laberinto de " << n << "x" << n << " generado y resuelto." << endl;
    cout << "[INFO] Tiempo de ejecucion: " << tiempo.count() << " ms" << endl;

    return 0;
}