# Cold Case — La Mansión Blackwood
### Thriller de Investigación y Terror Psicológico 2D
*Enigma Byte Studio | Programación 3 — C++17 + Qt6*

---

## 🕵️ Narrativa del Juego

La detective **Kate Warne** reabre el expediente N° 2004-078: la misteriosa desaparición de **Eleanor Blackwood** hace 20 años dentro de la mansión familiar.  
Armada con su intuición y sus herramientas de investigación, Sofía debe explorar habitaciones, recoger evidencias y confrontar secretos que alguien quiso enterrar para siempre.

> *"Los casos no se cierran solos. Alguien siempre sabe la verdad."*

**Dos finales posibles:**
- 🏆 **Caso Resuelto** — Kate Warne encuentra la grabación de Eleanor y confronta al culpable.
- 📁 **Expediente Sellado** — Sin la prueba definitiva, el caso regresa al archivo.

---

## 🏗️ Arquitectura del Proyecto

```
proyecto-pr3/
├── include/
│   ├── structures/
│   │   ├── InventarioHash.hpp     ← Tabla Hash (O(1))
│   │   ├── ArbolAVL.hpp           ← Árbol AVL (O(log n))
│   │   ├── PilaEventos.hpp        ← Stack LIFO
│   │   ├── ColaAlertas.hpp        ← Queue FIFO
│   │   └── GrafoNarrativo.hpp     ← Grafo + DFS/Backtracking
│   └── game/
│       ├── Pista.hpp              ← Struct Pista
│       ├── Evento.hpp             ← Struct Evento
│       └── GameEngine.hpp         ← Orquestador central
├── src/
│   ├── structures/                ← Implementaciones .cpp
│   └── game/
│       ├── GameEngine.cpp
│       └── main.cpp               ← Punto de entrada
├── tests/
│   ├── test_inventario.cpp        ← Tests InventarioHash
│   ├── test_avl.cpp               ← Tests Árbol AVL
│   ├── test_pila_cola.cpp         ← Tests Pila y Cola
│   └── test_grafo.cpp             ← Tests Grafo + Backtracking
├── docs/
├── CMakeLists.txt
└── README.md
```

---

## 📐 Estructuras de Datos

### 1. `InventarioHash` — Tabla Hash con Encadenamiento
| Operación | Complejidad |
|-----------|-------------|
| `insertar()` | **O(1)** amortizado |
| `buscar()` | **O(1)** amortizado |
| `eliminar()` | **O(1)** amortizado |

- Capacidad: 19 slots (número primo para minimizar colisiones)
- Función hash: **djb2** (`hash = hash * 33 + c`)
- Manejo de colisiones: **encadenamiento con listas enlazadas**
- Almacena: llaves, documentos, grabaciones y objetos recogibles

### 2. `ArbolAVL` — Árbol AVL Autobalanceado
| Operación | Complejidad |
|-----------|-------------|
| `insertar()` | **O(log n)** |
| `buscar()` | **O(log n)** |
| `eliminar()` | **O(log n)** |
| `filtrarPorCategoria()` | **O(n)** inorden |

- Rotaciones implementadas: simple derecha, simple izquierda, doble izq-der, doble der-izq
- Factor de balance mantenido en `[-1, 0, 1]`
- Recorrido **inorden** = pistas ordenadas por ID ascendente
- Categorías: `FISICA`, `DOCUMENTAL`, `TESTIMONIAL`, `DIGITAL`

### 3. `PilaEventos` — Pila LIFO
| Operación | Complejidad |
|-----------|-------------|
| `push()` | **O(1)** |
| `pop()` | **O(1)** |
| `peek()` | **O(1)** |

- Registra el **historial de acciones** de la investigadora
- Capacidad máxima: 100 eventos (descarta el más antiguo)
- `obtenerUltimos(n)`: muestra el historial reciente en la UI

### 4. `ColaAlertas` — Cola FIFO
| Operación | Complejidad |
|-----------|-------------|
| `enqueue()` | **O(1)** |
| `dequeue()` | **O(1)** |
| `frente()` | **O(1)** |

- Gestiona **alertas narrativas** en orden de llegada
- Implementada con `std::deque` para O(1) en ambos extremos
- Capacidad máxima: 50 alertas pendientes

### 5. `GrafoNarrativo` — Grafo Dirigido + DFS/Backtracking
| Operación | Complejidad |
|-----------|-------------|
| `moverA()` | **O(1)** |
| `getDecisiones()` | **O(grado)** |
| `encontrarCaminoAFinal()` | **O(V + E)** |

- Grafo dirigido representado con **lista de adyacencia** (`unordered_map`)
- Cada arista puede tener una **condición** (clave de objeto en inventario)
- **DFS con backtracking** para encontrar rutas alternativas hacia los finales

---

## 💻 Requisitos del Sistema y Prerrequisitos

Para compilar y ejecutar el proyecto se requieren las siguientes herramientas según tu sistema operativo:

### 1. Herramientas Generales
- **Compilador C++**: Soporte para **C++17** o superior (`GCC 9+`, `Clang 10+`, o `MSVC 2019+`).
- **Sistema de Construcción**: **CMake** versión 3.16 o superior.
- **Framework Gráfico (Opcional para GUI)**: **Qt6** (`Qt6Core`, `Qt6Widgets`).  
  *(Nota: Si no se cuenta con Qt6 instalado, el proyecto incluye el target `cold_case_consola` y todos los ejecutables de pruebas `test_*` que compilan únicamente con C++17 estándar sin dependencias externas).*

---

### 2. Instalación de Prerrequisitos por Sistema Operativo

#### 🪟 Windows
Se puede usar el entorno **MSYS2 (MinGW/UCRT64)** o **Visual Studio**:

- **Opción A (MSYS2 / UCRT64 - Recomendado para MinGW)**:
  Abrir la terminal de MSYS2 UCRT64 y ejecutar:
  ```bash
  pacman -Syu
  pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-ninja
  # Para soporte de GUI Qt6:
  pacman -S mingw-w64-ucrt-x86_64-qt6-base mingw-w64-ucrt-x86_64-qt6-tools
  ```
  *(Asegúrate de tener `C:\msys64\ucrt64\bin` agregado en tus Variables de Entorno PATH de Windows).*

- **Opción B (Visual Studio 2019/2022 con C++)**:
  - Instalar el instalador de Visual Studio y marcar: *"Desarrollo para el escritorio con C++"* (incluye MSVC y CMake).
  - Instalar Qt6 mediante el [Instalador Oficial de Qt](https://www.qt.io/download-qt-installer).

- **Opción C (Instaladores con winget)**:
  ```powershell
  winget install Kitware.CMake
  winget install MSYS2.MSYS2
  ```

---

#### 🐧 Linux (Ubuntu / Debian / Fedora / Arch)

- **Ubuntu / Debian / Linux Mint**:
  ```bash
  sudo apt update
  sudo apt install -y build-essential cmake git
  # Para soporte de GUI Qt6:
  sudo apt install -y qt6-base-dev qt6-tools-dev qt6-tools-dev-tools
  ```

- **Fedora / RHEL**:
  ```bash
  sudo dnf groupinstall -y "Development Tools"
  sudo dnf install -y cmake gcc-c++ git
  # Para soporte de GUI Qt6:
  sudo dnf install -y qt6-qtbase-devel qt6-qttools-devel
  ```

- **Arch Linux / Manjaro**:
  ```bash
  sudo pacman -S base-devel cmake git
  # Para soporte de GUI Qt6:
  sudo pacman -S qt6-base qt6-tools
  ```

---

#### 🍏 macOS (Intel & Apple Silicon M1/M2/M3)
Se recomienda utilizar [Homebrew](https://brew.sh/):

1. Instalar las herramientas de desarrollo de Xcode:
   ```bash
   xcode-select --install
   ```
2. Instalar CMake, Git y Qt6 con Homebrew:
   ```bash
   brew update
   brew install cmake git
   # Para soporte de GUI Qt6:
   brew install qt@6
   ```
3. Si utilizas Qt6 de Homebrew, agrega la ruta a tus variables:
   ```bash
   # En Apple Silicon:
   export CMAKE_PREFIX_PATH="/opt/homebrew/opt/qt@6"
   # En Intel Mac:
   export CMAKE_PREFIX_PATH="/usr/local/opt/qt@6"
   ```

---

## 🔧 Instrucciones Detalladas de Compilación y Ejecución

### 🪟 En Windows

#### Método 1: Con CMake y MinGW (PowerShell / CMD)
```powershell
# 1. Clonar el repositorio (si aún no lo tienes)
git clone https://github.com/tu-usuario/proyecto-pr3.git
cd proyecto-pr3

# 2. Crear y entrar a la carpeta de compilación
mkdir build
cd build

# 3. Configurar el proyecto (desactivar GUI si no tienes Qt6 instalado: -DBUILD_GUI=OFF)
cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release

# 4. Compilar todos los objetivos
cmake --build . --config Release

# 5. Ejecución:
# Ejecutar juego (Modo Consola):
.\cold_case_consola.exe

# Ejecutar juego (Modo GUI Qt6, si se compiló con Qt):
.\cold_case.exe

# Ejecutar baterías de pruebas unitarias:
.\test_inventario.exe
.\test_avl.exe
.\test_pila_cola.exe
.\test_grafo.exe
```

#### Método 2: Con Visual Studio (MSVC)
```powershell
cd proyecto-pr3
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release

# Ejecutar desde build/Release/
.\Release\cold_case_consola.exe
.\Release\test_inventario.exe
```

#### Método 3: Compilación Rápida Directa con g++ (Sin CMake ni Qt)
Si solo deseas compilar y evaluar rápidamente la lógica del juego y estructuras:
```powershell
# Compilar y ejecutar juego en consola:
g++ -std=c++17 -Iinclude src/structures/*.cpp src/game/GameEngine.cpp src/game/main.cpp -o cold_case_consola.exe
.\cold_case_consola.exe

# Compilar y ejecutar pruebas de estructuras:
g++ -std=c++17 -Iinclude src/structures/InventarioHash.cpp tests/test_inventario.cpp -o test_inventario.exe
.\test_inventario.exe
```

---

### 🐧 En Linux

```bash
# 1. Clonar el repositorio
git clone https://github.com/tu-usuario/proyecto-pr3.git
cd proyecto-pr3

# 2. Crear y acceder al directorio de build
mkdir -p build && cd build

# 3. Configurar CMake (usar -DBUILD_GUI=OFF si no instalaste Qt6)
cmake .. -DCMAKE_BUILD_TYPE=Release

# 4. Compilar aprovechando todos los núcleos del procesador
cmake --build . -j$(nproc)

# 5. Ejecutar:
# Modo consola:
./cold_case_consola

# Modo gráfico Qt6 (si está habilitado):
./cold_case

# Pruebas unitarias:
./test_inventario
./test_avl
./test_pila_cola
./test_grafo
```

---

### 🍏 En macOS

```bash
# 1. Clonar el repositorio
git clone https://github.com/tu-usuario/proyecto-pr3.git
cd proyecto-pr3

# 2. Crear y acceder al directorio de build
mkdir -p build && cd build

# 3. Configurar CMake
# Si usas Qt6 con Homebrew en Apple Silicon:
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="/opt/homebrew/opt/qt@6"
# O si compilarás sin GUI Qt6:
# cmake .. -DCMAKE_BUILD_TYPE=Release -DBUILD_GUI=OFF

# 4. Compilar
cmake --build . -j$(sysctl -n hw.ncpu)

# 5. Ejecutar:
./cold_case_consola

# Modo gráfico Qt6 (en macOS crea un .app o binario según configuración):
./cold_case

# Pruebas unitarias:
./test_inventario
./test_avl
./test_pila_cola
./test_grafo
```

---

## 🎮 Targets de Compilación Disponibles

| Target | Tipo | Descripción | Dependencias |
|--------|------|-------------|--------------|
| `cold_case` | Aplicación GUI | Juego completo con interfaz visual en Qt Designer / Qt6 | C++17 + Qt6 Core & Widgets |
| `cold_case_consola` | Ejecutable CLI | Sesión completa de simulación e integración de motor | C++17 Estándar (Sin dependencias externas) |
| `test_inventario` | Test Suite | Pruebas de inserción, búsqueda $O(1)$, eliminación y colisiones | C++17 Estándar |
| `test_avl` | Test Suite | Pruebas de inserción, balanceo, 4 rotaciones y orden $O(\log n)$ | C++17 Estándar |
| `test_pila_cola` | Test Suite | Pruebas de orden LIFO (historial) y FIFO (alertas) | C++17 Estándar |
| `test_grafo` | Test Suite | Pruebas de navegación dirigida y backtracking narrativo | C++17 Estándar |

---

## 🗺️ Mapa Narrativo

```
[0] Entrada ──► [1] Vestíbulo ──► [2] Biblioteca ──► (recoge llave_b)
                     │                  │
                     ▼                  ▼ (requiere llave_b)
                [4] Sótano         [3] Estudio
                (recoge grabación)      │
                     │          ┌───────┴───────────┐
                     │          ▼ (requiere grab.)   ▼
                     └────► [5] FINAL VERDAD    [6] FINAL SELLADO
```

---

## 📋 Stack Tecnológico

| Componente | Tecnología |
|------------|------------|
| Lenguaje | **C++17** |
| GUI (próxima fase) | **Qt6 + Qt Designer** |
| Build System | **CMake 3.16+** |
| Control de versiones | **Git + GitHub** |
| Testing | Executables de prueba con `assert` |

---

## 👩‍💻 Equipo de Desarrollo

**Enigma Byte Studio** — Programación 3
