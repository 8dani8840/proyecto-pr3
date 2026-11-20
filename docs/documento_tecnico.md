# Documento Técnico — Cold Case

**Equipo:** Enigma Byte Studio  
**Asignatura:** Programación 3 — Escuela de Ingeniería de Sistemas (ULA)  
**Lenguaje y Estándar:** C++17  
**Motor Gráfico:** Godot Engine 4 (vía GDExtension / `godot-cpp`, sin lógica en GDScript)  
**Librería de Estructuras de Datos:** `DeSiGNAR` (`third_party/DeSiGNAR`)  
**Metodología de Gestión:** Kanban + GitFlow  

---

## 1. Arquitectura General del Sistema

**Cold Case** es un videojuego 2D de misterio e investigación policial diseñado bajo una separación estricta entre **lógica de negocio/estructuras de datos (C++17)** y **capa de presentación (Godot 4)**:

* **Núcleo Lógico (`include/` y `src/`):** Implementa el motor del juego (`GameEngine`) y las estructuras de datos respaldadas por `DeSiGNAR`. Puede compilarse de forma independiente como ejecutable de demostración en consola (`cold_case_consola`) o como biblioteca dinámica compartida (`libcoldcase.dll` / `libcoldcase.so`).
* **Puente Nativo GDExtension (`src/gdextension/`):** Expone la clase `GameBridge` (heredada de `godot::Node`) mediante `ColdCaseExtension`, permitiendo que las escenas de Godot 4 consulten escenas, decisiones, inventario, pistas e historial directamente desde C++17 sin escribir lógica de juego en GDScript.
* **Interfaz Gráfica 2D (`godot_project/`):** Contiene la configuración del proyecto (`project.godot`), el manifiesto nativo (`cold_case.gdextension`) y las escenas modulares (`MainScene.tscn`, `InvestigationPanel.tscn`, `InventoryPanel.tscn`, `CluePanel.tscn`).

---

## 2. Estado de Implementación por Módulo

Para garantizar transparencia técnica durante la evaluación, se distingue explícitamente entre componentes funcionales verificados y extensiones futuras:

| Módulo / Característica | Estado | Descripción Técnica |
| :--- | :--- | :--- |
| **Motor Central (`GameEngine`)** | **[IMPLEMENTADO]** | Orquesta narrativa, inventario, expediente de pistas, historial de acciones y cola de alertas en C++17. |
| **Inventario (`InventarioHash`)** | **[IMPLEMENTADO]** | Tabla Hash abierta con encadenamiento separado (`Designar::SeparateChainingHashTable`) y función `super_fast_hash` para búsqueda $O(1)$ promedio. |
| **Expediente de Pistas (`ArbolAVL`)** | **[IMPLEMENTADO]** | Árbol binario de búsqueda autobalanceado (`Designar::AvlTree`) ordenado por ID de pista para inserción, búsqueda y recorrido inorden $O(\log n)$. |
| **Historial de Acciones (`PilaEventos`)** | **[IMPLEMENTADO]** | Pila LIFO (`Designar::DynStack`) que registra cronológicamente las acciones de la investigadora. |
| **Cola de Alertas (`ColaAlertas`)** | **[IMPLEMENTADO]** | Cola FIFO (`Designar::DynQueue`) para despachar notificaciones y descubrimientos en orden de llegada. |
| **Gestor de Escenas (`GestorEscenas`)** | **[IMPLEMENTADO]** | Tabla Hash (`Designar::SeparateChainingHashTable`) indexada por ID de escena con lista doblemente enlazada (`Designar::DLList`) de decisiones condicionadas por objetos. |
| **Puente GDExtension (`GameBridge`)** | **[IMPLEMENTADO]** | Clase nativa registrada en `ClassDB` de Godot 4 y compilada en `godot_project/bin/libcoldcase.dll`. |
| **Escenas Base en Godot 4** | **[IMPLEMENTADO]** | Escena principal `MainScene.tscn` con nodo `GameBridge` y paneles UI (`InvestigationPanel`, `InventoryPanel`, `CluePanel`). |
| **Pruebas Unitarias Automatizadas** | **[IMPLEMENTADO]** | Suites `test_inventario`, `test_avl` y `test_pila_cola` (60 aserciones aprobadas, 0 fallos) más demo integral `cold_case_consola`. |
| **Arte 2D, Sprites y Audio Ambiental** | **[PLANIFICADO]** | Incorporación de fondos ilustrados de la mansión Blackwood, retratos de personajes y efectos sonoros en Godot 4. |
| **Guardado/Carga Persistente en Disco** | **[PLANIFICADO]** | Serialización del estado de `GameEngine` a archivo externo para continuar partidas entre sesiones. |
| **Capítulos Adicionales del Caso** | **[PLANIFICADO]** | Expansión de escenas y pistas más allá del caso piloto *La Mansión Blackwood*. |

---

## 3. Estructuras de Datos con `DeSiGNAR`

Todas las estructuras de datos del proyecto están respaldadas por la librería `DeSiGNAR` (integrada como submódulo Git en `third_party/DeSiGNAR` y enlazada estáticamente mediante CMake):

1. **Tabla Hash (`InventarioHash` y `GestorEscenas`):**
   * **Clase `DeSiGNAR`:** `Designar::SeparateChainingHashTable<Key, Cmp>` junto con `Designar::super_fast_hash` (`<chainedhash.hpp>`, `<hash.hpp>`).
   * **Uso en el juego:** En `InventarioHash` permite verificar en tiempo constante $O(1)$ si Sofía Ramos posee un objeto clave (como `llave_b` o `grabacion_eleanor`) antes de habilitar una acción. En `GestorEscenas` almacena los registros de cada escena indexados por su `id`, acompañados de una lista doblemente enlazada `Designar::DLList<AristaDecision>` (`<list.hpp>`) con las opciones disponibles desde esa ubicación.
2. **Árbol AVL (`ArbolAVL`):**
   * **Clase `DeSiGNAR`:** `Designar::AvlTree<Pista, CmpPista>` (`<avltree.hpp>`).
   * **Uso en el juego:** Mantiene el expediente de pistas siempre ordenado ascendentemente por identificador único sin degradarse ante inserciones secuenciales, permitiendo recorridos ordenados inorden y filtrado por categoría (`Documental`, `Fisica`, `Testimonial`, `Digital`).
3. **Pila LIFO (`PilaEventos`):**
   * **Clase `DeSiGNAR`:** `Designar::DynStack<Evento>` (`<stack.hpp>`).
   * **Uso en el juego:** Registra la bitácora de eventos y decisiones tomadas por la jugadora, permitiendo consultar los últimos $N$ movimientos realizados desde el tope de la pila.
4. **Cola FIFO (`ColaAlertas`):**
   * **Clase `DeSiGNAR`:** `Designar::DynQueue<Evento>` (`<queue.hpp>`).
   * **Uso en el juego:** Gestiona la cola de mensajes y descubrimientos pendientes para mostrarlos al jugador estrictamente en el orden en que ocurrieron.

---

## 4. Integración con Godot 4 (GDExtension)

Para cumplir la restricción de **cero lógica en GDScript**, se construyó una extensión nativa en C++17:

* **`ColdCaseExtension.cpp`:** Registra el módulo ante `GDExtensionInterfaceGetProcAddress` en el nivel de inicialización `MODULE_INITIALIZATION_LEVEL_SCENE` y da de alta la clase `GameBridge` mediante `GDREGISTER_CLASS(GameBridge)`.
* **`GameBridge.hpp` / `GameBridge.cpp`:** Encapsula una instancia privada de `GameEngine` y expone métodos enlazados con `ClassDB::bind_method`:
  * `get_scene_title()` y `get_scene_description()`
  * `get_decisions()` y `take_decision(id)`
  * `collect_item(key, name, desc)` y `get_inventory()`
  * `get_clues()`, `get_recent_history(n)` e `is_game_over()`
* **Configuración (`godot_project/cold_case.gdextension`):** Vincula la biblioteca compilada (`res://bin/libcoldcase.dll` en Windows 64-bit y `res://bin/libcoldcase.so` en Linux 64-bit) con `entry_symbol = "coldcase_library_init"`.

---

## 5. Metodología de Desarrollo: Kanban + GitFlow

El desarrollo se organizó combinando un **tablero Kanban** de flujo continuo con el modelo de ramificación **GitFlow**:

### 5.1 Flujo del Tablero Kanban
El ciclo de vida de cada tarea se gestionó a través de cinco columnas explícitas:

1. **Notes:** Ideas de diseño narrativo, requisitos de evaluación y arquitectura GDExtension.
2. **To-Do:** Tareas priorizadas y listas para ejecución (migración de estructuras, configuración de CMake, montaje de escenas, redacción técnica).
3. **InProgress:** Tareas en desarrollo activo sobre una rama `feature/*` aislada.
4. **Testing:** Validación mediante compilación cruzada CMake, ejecución de suites unitarias (`test_inventario`, `test_avl`, `test_pila_cola`) y verificación de carga de `libcoldcase.dll` en Godot 4.
5. **Done:** Tareas aprobadas e integradas mediante merge `--no-ff` hacia `develop`.

### 5.2 Trazabilidad entre Kanban y Ramas GitFlow

| Rama GitFlow | Propósito Técnico | Estado Kanban |
| :--- | :--- | :--- |
| `main` | Versión estable de entrega del proyecto. | **Done** |
| `develop` | Rama principal de integración continua donde convergen todas las funcionalidades probadas. | **Done** |
| `feature/cmake-designar-godot` | Configuración unificada de `CMakeLists.txt` y registro de submódulos `DeSiGNAR` y `godot-cpp`. | **Done** |
| `feature/designar-migration` | Sustitución de estructuras manuales por contenedores oficiales de `DeSiGNAR` (`SeparateChainingHashTable`, `AvlTree`, `DynStack`, `DynQueue`). | **Done** |
| `feature/godot-scenes` | Creación de `GameBridge` (GDExtension), escenas base de Godot 4 y sustitución de grafos por `GestorEscenas` (Tabla Hash + `DLList`). | **Done** |
| `feature/technical-doc` | Elaboración de `docs/DOCUMENTO_TECNICO.md` con estado de módulos, arquitectura y trazabilidad Kanban. | **Done** |
| `feature/readme-final` | Actualización de `README.md` con guía de instalación, compilación y estructura del repositorio. | **Done** |
