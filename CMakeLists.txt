cmake_minimum_required(VERSION 3.20)
project(ColdCase VERSION 1.0 LANGUAGES CXX)

# ============================================================
#  C++17 obligatorio
# ============================================================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ============================================================
#  Enlazado estático en Windows (MinGW) para portabilidad
# ============================================================
if(WIN32 AND MINGW)
    add_link_options(-static-libgcc -static-libstdc++)
endif()

# ============================================================
#  Opciones de compilación
# ============================================================
option(BUILD_GUI   "Compilar versión Qt6 (legado)"         OFF)
option(BUILD_TESTS "Compilar ejecutables de prueba"        ON)
option(BUILD_GODOT "Compilar librería GDExtension Godot"   ON)

# ============================================================
#  1. DeSiGNAR — librería de estructuras de datos
# ============================================================
set(DESIGNAR_DIR ${CMAKE_SOURCE_DIR}/third_party/DeSiGNAR)
if(EXISTS ${DESIGNAR_DIR}/CMakeLists.txt)
    # Apagamos los samples y tests internos de DeSiGNAR para compilar rápido
    set(DESIGNAR_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
    set(DESIGNAR_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    add_subdirectory(${DESIGNAR_DIR})
    set(DESIGNAR_AVAILABLE TRUE)
else()
    message(FATAL_ERROR "DeSiGNAR no encontrado en third_party/DeSiGNAR. Ejecuta: git submodule update --init")
endif()

# ============================================================
#  2. godot-cpp — bindings de Godot para C++
# ============================================================
set(GODOT_CPP_DIR ${CMAKE_SOURCE_DIR}/third_party/godot-cpp)
if(BUILD_GODOT AND EXISTS ${GODOT_CPP_DIR}/CMakeLists.txt)
    set(GODOT_CPP_AVAILABLE TRUE)
    add_subdirectory(${GODOT_CPP_DIR})
else()
    if(BUILD_GODOT)
        message(WARNING "godot-cpp no encontrado en third_party/godot-cpp. Se omitirá coldcase_gdext.")
    endif()
    set(GODOT_CPP_AVAILABLE FALSE)
endif()

# ============================================================
#  Fuentes compartidas (estructuras + motor)
# ============================================================
set(SOURCES_ESTRUCTURAS
    src/structures/InventarioHash.cpp
    src/structures/ArbolAVL.cpp
    src/structures/PilaEventos.cpp
    src/structures/ColaAlertas.cpp
    src/structures/GrafoNarrativo.cpp
)

set(SOURCES_GAME
    src/game/GameEngine.cpp
)

# ============================================================
#  Target 1: cold_case_consola — Modo consola (con DeSiGNAR)
# ============================================================
add_executable(cold_case_consola
    src/game/main.cpp
    ${SOURCES_ESTRUCTURAS}
    ${SOURCES_GAME}
)
target_include_directories(cold_case_consola PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(cold_case_consola PRIVATE Designar::Designar)
target_compile_definitions(cold_case_consola PRIVATE USE_DESIGNAR=1)

# ============================================================
#  Función auxiliar para registrar tests (con DeSiGNAR)
# ============================================================
function(add_test_target name source)
    add_executable(${name}
        ${source}
        ${SOURCES_ESTRUCTURAS}
        ${SOURCES_GAME}
    )
    target_include_directories(${name} PRIVATE ${CMAKE_SOURCE_DIR}/include)
    target_link_libraries(${name} PRIVATE Designar::Designar)
    target_compile_definitions(${name} PRIVATE USE_DESIGNAR=1)
endfunction()

if(BUILD_TESTS)
    add_test_target(test_inventario  tests/test_inventario.cpp)
    add_test_target(test_avl         tests/test_avl.cpp)
    add_test_target(test_pila_cola   tests/test_pila_cola.cpp)
    add_test_target(test_grafo       tests/test_grafo.cpp)
endif()

# ============================================================
#  Target 2: GDExtension (libcoldcase.dll / libcoldcase.so)
# ============================================================
if(GODOT_CPP_AVAILABLE)
    add_library(coldcase_gdext SHARED
        src/gdextension/ColdCaseExtension.cpp
        src/gdextension/GameBridge.cpp
        ${SOURCES_ESTRUCTURAS}
        ${SOURCES_GAME}
    )
    target_include_directories(coldcase_gdext PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${GODOT_CPP_DIR}/include
        ${GODOT_CPP_DIR}/gen/include
    )
    target_link_libraries(coldcase_gdext PRIVATE godot-cpp Designar::Designar)
    target_compile_definitions(coldcase_gdext PRIVATE USE_DESIGNAR=1)
endif()

# ============================================================
#  Target 3: cold_case — GUI con Qt6 (Opcional / Legado)
# ============================================================
if(BUILD_GUI)
    find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
    if(Qt6_FOUND)
        set(CMAKE_AUTOMOC ON)
        set(CMAKE_AUTOUIC ON)
        set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_SOURCE_DIR}/resources)

        add_executable(cold_case
            src/game/main_qt.cpp
            src/ui/MainWindow.cpp
            ${SOURCES_ESTRUCTURAS}
            ${SOURCES_GAME}
        )
        target_include_directories(cold_case PRIVATE
            ${CMAKE_SOURCE_DIR}/include
            ${CMAKE_SOURCE_DIR}/resources
            ${CMAKE_CURRENT_BINARY_DIR}
        )
        target_link_libraries(cold_case PRIVATE Qt6::Core Qt6::Widgets Designar::Designar)
        if(WIN32)
            set_target_properties(cold_case PROPERTIES WIN32_EXECUTABLE TRUE)
        endif()
    endif()
endif()

# ============================================================
#  Resumen al configurar
# ============================================================
message(STATUS "")
message(STATUS "=== Cold Case — Configuracion CMake ===")
message(STATUS "  Compilador  : ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "  Estandar    : C++${CMAKE_CXX_STANDARD}")
message(STATUS "  DeSiGNAR    : ${DESIGNAR_AVAILABLE}")
message(STATUS "  godot-cpp   : ${GODOT_CPP_AVAILABLE}")
message(STATUS "  Tests       : ${BUILD_TESTS}")
message(STATUS "  GUI Qt6     : ${BUILD_GUI}")
message(STATUS "")