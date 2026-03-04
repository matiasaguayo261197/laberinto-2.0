#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <queue>
#include <chrono>
#include <map>

using namespace std;

// Estructura para coordenadas y reconstrucción de ruta
struct Punto {
    int y, x;
    
    // Sobrecarga necesaria para usar Punto como clave en un std::map
    bool operator<(const Punto& otro) const {  
        if (y != otro.y) return y < otro.y;
        return x < otro.x;
    }
};

// Direcciones: dy/dx con salto de 2 para generar caminos saltando muros
int dy[] = {-2, 2, 0, 0};
int dx[] = {0, 0, -2, 2};
// ry/rx con paso de 1 para que el explorador avance celda por celda
int ry[] = {-1, 1, 0, 0};
int rx[] = {0, 0, -1, 1};

// Dibuja el laberinto en consola sustituyendo caracteres por símbolos legibles
void mostrarLaberinto(const vector<vector<char>>& mapa) {
    for (const auto& fila : mapa) {
        for (char celda : fila) {
            if (celda == '#') cout << "█ "; 
            else if (celda == '.') cout << "o "; 
            else cout << celda << " "; 
        }
        cout << endl;
    }
}

// Genera el laberinto usando DFS (Depth First Search) aleatorio
void generarLaberinto(int y, int x, vector<vector<char>>& mapa) {
    mapa[y][x] = '*'; // Marca la celda actual como pasillo

    // Baraja las direcciones para que el laberinto sea distinto cada vez
    vector<int> orden = {0, 1, 2, 3};
    random_device rd;
    mt19937 g(rd());
    shuffle(orden.begin(), orden.end(), g);

    for (int i = 0; i < 4; i++) {
        int dir = orden[i];
        int ny = y + dy[dir];
        int nx = x + dx[dir];

        // Verifica límites y si la celda destino es un muro sin visitar
        if (ny > 0 && ny < mapa.size() - 1 && nx > 0 && nx < mapa[0].size() - 1) {
            if (mapa[ny][nx] == '#') {
                // Rompe el muro intermedio para conectar el nuevo camino
                mapa[y + dy[dir] / 2][x + dx[dir] / 2] = '*'; 
                generarLaberinto(ny, nx, mapa); // Recursión para seguir excavando
            }
        }
    }
}

// Resuelve el laberinto mediante BFS (Breadth First Search) para hallar la ruta corta
bool resolverLaberinto(int startY, int startX, int endY, int endX, vector<vector<char>>& mapa) {
    queue<Punto> fila;
    fila.push({startY, startX});

    // Registra la procedencia de cada celda para reconstruir el camino al final
    map<Punto, Punto> padres;
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

            // Solo avanza si es camino ('*'), entrada o salida, y no ha sido visitado
            if (ny >= 0 && ny < mapa.size() && nx >= 0 && nx < mapa[0].size() && 
                mapa[ny][nx] != '#' && !visitado[ny][nx]) {
                
                visitado[ny][nx] = true;
                padres[{ny, nx}] = actual; // Guarda el "padre" para el rastro
                fila.push({ny, nx});
            }
        }
    }

    // Reconstrucción del camino trazando puntos '.' desde el final hacia el inicio
    if (encontrado) {
        Punto paso = padres[{endY, endX}];
        while (!(paso.y == startY && paso.x == startX)) {
            mapa[paso.y][paso.x] = '.';
            paso = padres[paso];
        }
        return true;
    }
    return false;
}

int main() {
    int n;
    cout << "Ingrese dimension (debe ser impar): ";
    cin >> n;

    if (n % 2 == 0) n++; // Asegura que el tamaño sea impar para la lógica de muros

    vector<vector<char>> laberinto(n, vector<char>(n, '#'));
    auto inicio = chrono::high_resolution_clock::now();

    generarLaberinto(1, 1, laberinto);

    int startY = 1, startX = 1;
    int endY = n - 2, endX = n - 2;
    laberinto[startY][startX] = 'E';
    laberinto[endY][endX] = 'S';

    resolverLaberinto(startY, startX, endY, endX, laberinto);

    auto fin = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> tiempo = fin - inicio;

    mostrarLaberinto(laberinto);
    cout << "\n[INFO] Tiempo de ejecucion: " << tiempo.count() << " ms" << endl;

    return 0;
}