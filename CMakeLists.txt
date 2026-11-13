cmake_minimum_required(VERSION 3.16)
project(ColdCase VERSION 1.0 LANGUAGES CXX)

# ============================================================
#  C++17 obligatorio
# ============================================================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ============================================================
#  Opciones de compilación
# ============================================================
option(BUILD_GUI  "Compilar versión Qt6 con interfaz gráfica" ON)
option(BUILD_TESTS "Compilar ejecutables de prueba"           ON)

# ============================================================
#  Directorios de headers compartidos
# ============================================================
include_directories(${CMAKE_SOURCE_DIR}/include)

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
#  Target 1: cold_case_consola — Modo consola (sin Qt)
# ============================================================
add_executable(cold_case_consola
    src/game/main.cpp
    ${SOURCES_ESTRUCTURAS}
    ${SOURCES_GAME}
)
target_include_directories(cold_case_consola PRIVATE ${CMAKE_SOURCE_DIR}/include)

# ============================================================
#  Target 2: cold_case — GUI con Qt6 (si BUILD_GUI=ON)
# ============================================================
if(BUILD_GUI)
    # Buscar Qt6 (requiere Qt instalado)
    find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
    if(NOT Qt6_FOUND)
        message(WARNING "Qt6 no encontrado. Desactiva BUILD_GUI con -DBUILD_GUI=OFF para omitir la GUI.")
    else()
        # Habilitar MOC automático (procesa Q_OBJECT) y UIC (procesa .ui)
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
            ${CMAKE_SOURCE_DIR}/resources   # Para que uic encuentre el .ui
            ${CMAKE_CURRENT_BINARY_DIR}      # Para ui_MainWindow.h generado
        )

        target_link_libraries(cold_case PRIVATE Qt6::Core Qt6::Widgets)

        # En Windows: ocultar la consola negra del CMD detrás de la ventana Qt
        if(WIN32)
            set_target_properties(cold_case PROPERTIES WIN32_EXECUTABLE TRUE)
        endif()

        message(STATUS "Target 'cold_case' (Qt6 GUI) configurado correctamente.")
    endif()
endif()

# ============================================================
#  Función auxiliar para registrar tests (sin Qt)
# ============================================================
function(add_test_target name source)
    add_executable(${name}
        ${source}
        ${SOURCES_ESTRUCTURAS}
        ${SOURCES_GAME}
    )
    target_include_directories(${name} PRIVATE ${CMAKE_SOURCE_DIR}/include)
endfunction()

# ============================================================
#  Targets de prueba
# ============================================================
if(BUILD_TESTS)
    add_test_target(test_inventario  tests/test_inventario.cpp)
    add_test_target(test_avl         tests/test_avl.cpp)
    add_test_target(test_pila_cola   tests/test_pila_cola.cpp)
    add_test_target(test_grafo       tests/test_grafo.cpp)
endif()

# ============================================================
#  Resumen al configurar
# ============================================================
message(STATUS "")
message(STATUS "=== Cold Case — Configuracion CMake ===")
message(STATUS "  Compilador  : ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "  Estandar    : C++${CMAKE_CXX_STANDARD}")
message(STATUS "  Build type  : ${CMAKE_BUILD_TYPE}")
message(STATUS "  GUI Qt6     : ${BUILD_GUI}")
message(STATUS "  Tests       : ${BUILD_TESTS}")
message(STATUS "")
