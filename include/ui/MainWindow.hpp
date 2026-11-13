#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QTimer>
#include "game/GameEngine.hpp"

// Forward declarations de Qt
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// ============================================================
// MainWindow: Ventana principal del juego Cold Case.
// Coordina todos los paneles visuales y conecta la GUI con
// el GameEngine mediante el patrón Signals & Slots de Qt.
// ============================================================
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    // ---- Slots de Navegación (Grafo Narrativo) ----
    void onDecisionClicked();         // El jugador elige una opción narrativa

    // ---- Slots de Inventario (Tabla Hash) ----
    void onInventarioItemClicked();   // Ver detalle de un objeto
    void refrescarInventario();       // Actualiza la lista visual del inventario

    // ---- Slots de Expediente (Árbol AVL) ----
    void onExpedienteItemClicked();   // Ver detalle de una pista
    void onFiltrarCategoria(int index); // Filtrar pistas por categoría

    // ---- Slots de Alertas (Cola FIFO) ----
    void procesarSiguienteAlerta();   // Muestra la próxima alerta de la cola

    // ---- Slots de UI ----
    void onTabChanged(int index);     // Cambio de pestaña

private:
    Ui::MainWindow* ui;
    GameEngine* engine;
    QTimer* timerAlertas;             // Dispara alertas de la cola cada N ms

    // ---- Helpers visuales ----
    void actualizarEscena();          // Refresca el panel de narrativa
    void actualizarDecisiones();      // Reconstruye los botones de decisión
    void actualizarExpediente();      // Refresca el árbol AVL en la lista
    void actualizarHistorial();       // Refresca el panel de historial (Pila)
    void mostrarDetalleObjeto(const std::string& clave);
    void mostrarDetallePista(int id);
    void mostrarMensaje(const QString& titulo, const QString& mensaje);

    // ---- Objetos "recolectables" de la escena actual ----
    void cargarObjetosEscena(int idEscena);
    void agregarPistasEscena(int idEscena);
};

#endif // MAINWINDOW_HPP
