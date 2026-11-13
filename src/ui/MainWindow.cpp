#include "ui/MainWindow.hpp"
#include "ui_MainWindow.h"   // Generado por uic (Qt Designer → C++)

#include <QListWidgetItem>
#include <QPushButton>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QString>
#include <QTimer>

// ============================================================
// Helpers para convertir std::string ↔ QString
// ============================================================
static QString qs(const std::string& s) { return QString::fromStdString(s); }

// Colores por tipo de evento (Historial)
static QString colorEvento(TipoEvento tipo) {
    switch (tipo) {
        case TipoEvento::ACCION:          return "#d4c5a9";
        case TipoEvento::ALERTA:          return "#c8a04a";
        case TipoEvento::DESCUBRIMIENTO:  return "#7ec8a0";
        case TipoEvento::DECISION:        return "#a090d0";
        default:                          return "#555555";
    }
}

// Icono por tipo de evento
static QString iconoEvento(TipoEvento tipo) {
    switch (tipo) {
        case TipoEvento::ACCION:          return "▶ ";
        case TipoEvento::ALERTA:          return "⚠ ";
        case TipoEvento::DESCUBRIMIENTO:  return "✦ ";
        case TipoEvento::DECISION:        return "◈ ";
        default:                          return "· ";
    }
}

// Icono por categoría de pista
static QString iconoPista(CategoriaPista cat) {
    switch (cat) {
        case CategoriaPista::FISICA:      return "🔬 ";
        case CategoriaPista::DOCUMENTAL:  return "📄 ";
        case CategoriaPista::TESTIMONIAL: return "🎙 ";
        case CategoriaPista::DIGITAL:     return "💾 ";
        default:                          return "❓ ";
    }
}

// ============================================================
// Constructor y Destructor
// ============================================================

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , engine(new GameEngine)
    , timerAlertas(new QTimer(this)) {

    ui->setupUi(this);
    engine->setNombreJugadora("Sofía Ramos");

    // Centrar ventana
    setWindowTitle("Cold Case — La Mansión Blackwood");
    resize(1100, 720);

    // ---- Conectar Signals & Slots ----

    // Inventario: clic en un ítem
    connect(ui->listaInventario, &QListWidget::itemClicked,
            this, &MainWindow::onInventarioItemClicked);

    // Expediente: clic en una pista
    connect(ui->listaExpediente, &QListWidget::itemClicked,
            this, &MainWindow::onExpedienteItemClicked);

    // Expediente: cambio de filtro por categoría
    connect(ui->comboCategoria, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onFiltrarCategoria);

    // Cambio de pestaña
    connect(ui->tabWidget, &QTabWidget::currentChanged,
            this, &MainWindow::onTabChanged);

    // Acciones del menú
    connect(ui->actionSalir, &QAction::triggered, this, &QMainWindow::close);
    connect(ui->actionAcercaDe, &QAction::triggered, this, [this]() {
        QMessageBox::about(this, "Acerca de Cold Case",
            "<b>Cold Case — La Mansión Blackwood</b><br>"
            "Versión 1.0 | Enigma Byte Studio<br><br>"
            "Proyecto académico — Programación 3<br>"
            "Implementa: Tabla Hash, Árbol AVL, Pila, Cola y Grafo con Backtracking.<br><br>"
            "<i>C++17 + Qt6</i>");
    });
    connect(ui->actionNuevoJuego, &QAction::triggered, this, [this]() {
        // Reiniciar el engine con una nueva partida
        delete engine;
        engine = new GameEngine;
        engine->setNombreJugadora("Sofía Ramos");
        actualizarEscena();
        actualizarDecisiones();
        refrescarInventario();
        actualizarExpediente();
        actualizarHistorial();
        ui->statusBar->showMessage("Nueva investigación iniciada.", 3000);
    });

    // Timer de alertas: procesa la cola FIFO cada 2.5 segundos
    connect(timerAlertas, &QTimer::timeout, this, &MainWindow::procesarSiguienteAlerta);
    timerAlertas->start(2500);

    // ---- Render inicial ----
    actualizarEscena();
    actualizarDecisiones();
    refrescarInventario();
    actualizarExpediente();
    actualizarHistorial();

    ui->statusBar->showMessage("Expediente N° 2004-078 reabierto. Buena suerte, detective.", 5000);
}

MainWindow::~MainWindow() {
    delete ui;
    delete engine;
}

// ============================================================
// Helpers visuales
// ============================================================

void MainWindow::actualizarEscena() {
    NodoNarrativo* escena = engine->getEscenaActual();
    if (!escena) return;

    ui->lblTituloEscena->setText(qs(escena->titulo));
    ui->txtNarrativa->setPlainText(qs(escena->descripcion));

    // Actualizar barra de estado inferior
    ui->lblEstado->setText(QString("Pistas: %1  |  Objetos: %2  |  Escena: %3")
        .arg(engine->getTotalPistas())
        .arg(engine->getTotalObjetos())
        .arg(qs(escena->titulo)));

    // Si llegamos a un final, mostrar mensaje
    if (escena->esFinale) {
        timerAlertas->stop();
        QString finMsg = escena->id == 5
            ? "✦ CASO RESUELTO ✦\n\nSofía Ramos cierra el expediente N° 2004-078.\nLa verdad sobre Eleanor Blackwood sale a la luz."
            : "✕ EXPEDIENTE SELLADO ✕\n\nSin pruebas suficientes, el caso regresa al archivo.\nLa oscuridad de la mansión Blackwood permanece.";
        QMessageBox::information(this, "Fin de la Investigación", finMsg);
    }

    // Cargar objetos/pistas de la nueva escena
    cargarObjetosEscena(escena->id);
    agregarPistasEscena(escena->id);
}

void MainWindow::actualizarDecisiones() {
    // Limpiar botones anteriores
    QLayoutItem* item;
    while ((item = ui->layoutDecisiones->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    if (engine->estaTerminado()) return;

    auto decisiones = engine->getDecisionesDisponibles();
    if (decisiones.empty()) {
        QLabel* lbl = new QLabel("[ Sin opciones disponibles aún ]");
        lbl->setStyleSheet("color: #555; font-family: 'Courier New'; font-size: 11px; padding: 4px;");
        ui->layoutDecisiones->addWidget(lbl);
        return;
    }

    for (const auto& dec : decisiones) {
        QPushButton* btn = new QPushButton("▶  " + qs(dec.textoDecision));
        btn->setProperty("idDestino", dec.idDestino);
        btn->setStyleSheet(R"(
            QPushButton {
                background-color: #1a1510;
                color: #c8a04a;
                font-family: 'Courier New';
                font-size: 12px;
                border: 1px solid #3a2a10;
                border-radius: 4px;
                padding: 8px 14px;
                text-align: left;
            }
            QPushButton:hover {
                background-color: #2a2010;
                border-color: #c8a04a;
                color: #f0d890;
            }
            QPushButton:pressed {
                background-color: #3a2a10;
            }
        )");
        connect(btn, &QPushButton::clicked, this, &MainWindow::onDecisionClicked);
        ui->layoutDecisiones->addWidget(btn);
    }
}

void MainWindow::refrescarInventario() {
    ui->listaInventario->clear();
    auto objetos = engine->obtenerInventario();

    for (const auto& obj : objetos) {
        QString texto = QString("[%1]  %2%3")
            .arg(qs(obj.clave))
            .arg(qs(obj.nombre))
            .arg(obj.usado ? "  (usado)" : "");

        QListWidgetItem* item = new QListWidgetItem(texto);
        item->setData(Qt::UserRole, qs(obj.clave)); // Guardar clave para búsqueda O(1)
        if (obj.usado) {
            item->setForeground(QColor("#666655"));
        }
        ui->listaInventario->addItem(item);
    }

    // Si está en la tab de inventario, actualizar el estado también
    ui->lblEstado->setText(QString("Pistas: %1  |  Objetos: %2  |  Escena: %3")
        .arg(engine->getTotalPistas())
        .arg(engine->getTotalObjetos())
        .arg(engine->getEscenaActual() ? qs(engine->getEscenaActual()->titulo) : "—"));
}

void MainWindow::actualizarExpediente() {
    ui->listaExpediente->clear();
    int idx = ui->comboCategoria->currentIndex();

    std::vector<Pista> pistas;
    if (idx == 0) {
        pistas = engine->obtenerExpediente();
    } else {
        // Mapeo de índice → categoría string
        const char* cats[] = {"", "Fisica", "Documental", "Testimonial", "Digital"};
        pistas = engine->filtrarPistasPorCategoria(cats[idx]);
    }

    for (const auto& p : pistas) {
        QString texto = QString("%1[ID:%2]  %3")
            .arg(iconoPista(p.categoria))
            .arg(p.id)
            .arg(qs(p.titulo));

        QListWidgetItem* item = new QListWidgetItem(texto);
        item->setData(Qt::UserRole, p.id); // ID para búsqueda AVL O(log n)
        item->setForeground(p.analizada ? QColor("#556655") : QColor("#d4c5a9"));
        ui->listaExpediente->addItem(item);
    }
}

void MainWindow::actualizarHistorial() {
    ui->listaHistorial->clear();
    auto eventos = engine->obtenerHistorialReciente(30);

    for (const auto& e : eventos) {
        QString texto = iconoEvento(e.tipo) + qs(e.descripcion);
        QListWidgetItem* item = new QListWidgetItem(texto);
        item->setForeground(QColor(colorEvento(e.tipo)));
        ui->listaHistorial->addItem(item);
    }
}

void MainWindow::mostrarDetalleObjeto(const std::string& clave) {
    ObjetoInventario* obj = engine->consultarObjeto(clave); // O(1)
    if (!obj) return;

    QString detalle = QString(
        "OBJETO: %1\n"
        "CLAVE:  %2\n"
        "ESTADO: %3\n\n"
        "%4"
    ).arg(qs(obj->nombre))
     .arg(qs(obj->clave))
     .arg(obj->usado ? "Usado" : "Disponible")
     .arg(qs(obj->descripcion));

    ui->txtDetalleObjeto->setPlainText(detalle);
}

void MainWindow::mostrarDetallePista(int id) {
    Pista* p = engine->buscarPista(id); // O(log n)
    if (!p) return;

    QString detalle = QString(
        "PISTA #%1 — %2\n"
        "CATEGORÍA: %3\n"
        "ESTADO: %4\n\n"
        "%5"
    ).arg(p->id)
     .arg(qs(p->titulo))
     .arg(qs(categoriaAString(p->categoria)))
     .arg(p->analizada ? "Analizada" : "Pendiente")
     .arg(qs(p->descripcion));

    ui->txtDetallePista->setPlainText(detalle);
}

// ============================================================
// Objetos y pistas por escena (datos narrativos)
// ============================================================

void MainWindow::cargarObjetosEscena(int idEscena) {
    // Cada escena tiene objetos específicos para recoger
    struct ItemEscena { std::string clave, nombre, desc; };
    static const std::map<int, std::vector<ItemEscena>> objetosPorEscena = {
        {2, {{"llave_b",     "Llave con sello 'B'",       "Hallada entre las páginas del diario. Abre el estudio del Sr. Blackwood."},
             {"diario_roto", "Diario con páginas arrancadas", "Fechas borradas en el período de la desaparición."}}},
        {4, {{"grabacion_eleanor", "Grabación de Eleanor", "Voz de Eleanor: 'No pueden saberlo... el acuerdo...' La cinta está dañada."},
             {"nota_sotano",       "Nota del sótano",       "Escrita a mano. Dice: 'Si algo me pasa, busca la caja bajo el roble.'"}}},
        {3, {{"registros_fin",    "Registros Financieros", "Transferencias millonarias el día de la desaparición. Cuenta en Suiza."}}}
    };

    auto it = objetosPorEscena.find(idEscena);
    if (it == objetosPorEscena.end()) return;

    for (const auto& item : it->second) {
        engine->recogerObjeto(item.clave, item.nombre, item.desc);
    }
    refrescarInventario();
}

void MainWindow::agregarPistasEscena(int idEscena) {
    // Pistas que se descubren automáticamente al llegar a cada escena
    struct PistaEscena { std::string titulo, desc; CategoriaPista cat; };
    static const std::map<int, std::vector<PistaEscena>> pistasPorEscena = {
        {2, {{"Diario con páginas arrancadas",
              "Las fechas borradas coinciden exactamente con la semana de la desaparición de Eleanor.",
              CategoriaPista::DOCUMENTAL}}},
        {4, {{"Grabación de voz de Eleanor",
              "Menciona 'el acuerdo' con angustia. La voz está temblorosa. Grabada 3 días antes de desaparecer.",
              CategoriaPista::TESTIMONIAL}}},
        {3, {{"Registros financieros sospechosos",
              "Transferencias de $2.4M al exterior. Fecha: 14/08/2004. Día de la desaparición de Eleanor.",
              CategoriaPista::DOCUMENTAL},
             {"Cajón forzado en el escritorio",
              "Marcas de palanca recientes. Los documentos removidos recientemente.",
              CategoriaPista::FISICA}}}
    };

    auto it = pistasPorEscena.find(idEscena);
    if (it == pistasPorEscena.end()) return;

    for (const auto& p : it->second) {
        engine->agregarPista(p.titulo, p.desc, p.cat);
    }
    actualizarExpediente();
}

// ============================================================
// SLOTS
// ============================================================

void MainWindow::onDecisionClicked() {
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    int idDestino = btn->property("idDestino").toInt();
    bool ok = engine->tomarDecision(idDestino);

    if (!ok) {
        // El jugador no tiene el objeto requerido
        QMessageBox::warning(this, "Acceso bloqueado",
            "No tienes los elementos necesarios para avanzar por esta ruta.\n"
            "Busca más pistas antes de continuar.");
        return;
    }

    // Actualizar toda la UI
    actualizarEscena();
    actualizarDecisiones();
    actualizarHistorial();
    actualizarExpediente();
}

void MainWindow::onInventarioItemClicked() {
    QListWidgetItem* item = ui->listaInventario->currentItem();
    if (!item) return;
    QString clave = item->data(Qt::UserRole).toString();
    mostrarDetalleObjeto(clave.toStdString()); // Búsqueda O(1) en Tabla Hash
}

void MainWindow::onExpedienteItemClicked() {
    QListWidgetItem* item = ui->listaExpediente->currentItem();
    if (!item) return;
    int id = item->data(Qt::UserRole).toInt();
    mostrarDetallePista(id); // Búsqueda O(log n) en Árbol AVL
}

void MainWindow::onFiltrarCategoria(int index) {
    actualizarExpediente();
    (void)index;
}

void MainWindow::procesarSiguienteAlerta() {
    // Dequeue de la Cola FIFO de alertas
    if (!engine->hayAlertasPendientes()) return;
    Evento alerta = engine->siguienteAlerta();
    ui->lblAlerta->setText(qs(alerta.descripcion));
    actualizarHistorial();
}

void MainWindow::onTabChanged(int index) {
    switch (index) {
        case 0: refrescarInventario(); break;
        case 1: actualizarExpediente(); break;
        case 2: actualizarHistorial(); break;
    }
}
