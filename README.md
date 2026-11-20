# Cold Case — Thriller de Investigación 2D

**Enigma Byte Studio** | **Programación 3 — Universidad de Los Andes (ULA)**

Videojuego 2D de misterio e investigación policial desarrollado en **C++17** con **Godot Engine 4** (mediante **GDExtension** nativo, sin lógica en GDScript) y la librería de estructuras de datos **`DeSiGNAR`**.

---

## Arquitectura y Estructuras de Datos (`DeSiGNAR`)

Toda la lógica del juego reside en C++17 (`GameEngine`) y utiliza exclusivamente estructuras de datos de `DeSiGNAR`:

| Componente | Archivo | Estructura `DeSiGNAR` | Función en el Juego |
| :--- | :--- | :--- | :--- |
| **Inventario** | `InventarioHash` | `SeparateChainingHashTable` | Búsqueda $O(1)$ de objetos clave recogidos por la detective. |
| **Expediente de Pistas** | `ArbolAVL` | `AvlTree` | Almacenamiento balanceado $O(\log n)$ y recorrido ordenado de pistas. |
| **Bitácora de Acciones** | `PilaEventos` | `DynStack` (LIFO) | Historial cronológico de acciones y decisiones recientes. |
| **Sistema de Alertas** | `ColaAlertas` | `DynQueue` (FIFO) | Despacho ordenado de notificaciones y descubrimientos. |
| **Gestor de Escenas** | `GestorEscenas` | `SeparateChainingHashTable` + `DLList` | Acceso directo a escenas por ID y lista de decisiones condicionadas. |

---

## Estructura del Repositorio

* `CMakeLists.txt` — Configuración unificada (consola, tests y GDExtension).
* `docs/documento_tecnico.md` — Arquitectura, estado de módulos y Kanban + GitFlow.
* `godot_project/` — Proyecto de Godot 4 (`project.godot`, `cold_case.gdextension`, `bin/libcoldcase.dll` y escenas en `scenes/`).
* `include/` y `src/` — Núcleo lógico (`GameEngine`), estructuras con `DeSiGNAR` y puente `GDExtension` (`GameBridge`).
* `tests/` — Pruebas unitarias (`test_inventario`, `test_avl`, `test_pila_cola`).
* `third_party/` — Submódulos `DeSiGNAR` y `godot-cpp`.

---

## Clonación y Compilación

1. **Clonar con submódulos:**
   `git clone --recurse-submodules [https://github.com/DanielaBalza/proyecto-pr3.git](https://github.com/DanielaBalza/proyecto-pr3.git)`
   *(Si ya clonaste sin submódulos, ejecuta `git submodule update --init --recursive`.)*

2. **Compilar en Windows (MinGW + CMake) o Linux (GCC + CMake):**
   `cmake -S . -B build -DBUILD_TESTS=ON -DBUILD_GODOT=ON`
   `cmake --build build --parallel`

Esto genera `build/cold_case_consola`, las suites de pruebas unitarias y la extensión nativa `godot_project/bin/libcoldcase.dll` (o `.so` en Linux) lista para abrir `godot_project/project.godot` en **Godot 4**.

---

## Metodología de Trabajo

El proyecto se desarrolló aplicando **Kanban** (*Notes*, *To-Do*, *InProgress*, *Testing*, *Done*) sincronizado con **GitFlow** (`main`, `develop` y ramas `feature/*`). Ver detalles completos en [`docs/documento_tecnico.md`](docs/documento_tecnico.md).
