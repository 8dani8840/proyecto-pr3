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
#  DeSiGNAR — librería de estructuras de datos
# ============================================================
set(DESIGNAR_DIR ${CMAKE_SOURCE_DIR}/third_party/DeSiGNAR)
if(EXISTS ${DESIGNAR_DIR}/CMakeLists.txt)
    add_subdirectory(${DESIGNAR_DIR})
    set(DESIGNAR_AVAILABLE TRUE)
else()
    message(WARNING "DeSiGNAR no encontrado en third_party/. Usando implementaciones propias.")
    set(DESIGNAR_AVAILABLE FALSE)
endif()

# ============================================================
#  godot-cpp — bindings de Godot para C++
# ============================================================
set(GODOT_CPP_DIR ${CMAKE_SOURCE_DIR}/third_party/godot-cpp)
if(EXISTS ${GODOT_CPP_DIR}/CMakeLists.txt)
    set(GODOT_CPP_AVAILABLE TRUE)
    add_subdirectory(${GODOT_CPP_DIR})
else()
    message(WARNING "godot-cpp no encontrado. Las escenas Godot no estarán disponibles.")
    set(GODOT_CPP_AVAILABLE FALSE)
endif()

# ============================================================
#  Target GDExtension (libcoldcase.dll / libcoldcase.so)
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
    target_link_libraries(coldcase_gdext PRIVATE godot-cpp)
    if(DESIGNAR_AVAILABLE)
        target_link_libraries(coldcase_gdext PRIVATE DeSiGNAR)
        target_compile_definitions(coldcase_gdext PRIVATE USE_DESIGNAR=1)
    endif()
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
message(STATUS "  DeSiGNAR    : ${DESIGNAR_AVAILABLE}")
message(STATUS "  godot-cpp   : ${GODOT_CPP_AVAILABLE}")
message(STATUS "")
