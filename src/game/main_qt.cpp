// ============================================================
// main_qt.cpp — Punto de entrada de la versión Qt6 (GUI)
// ============================================================

#include <QApplication>
#include <QFontDatabase>
#include "ui/MainWindow.hpp"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    // ---- Identidad de la aplicación ----
    QApplication::setApplicationName("Cold Case");
    QApplication::setApplicationVersion("1.0");
    QApplication::setOrganizationName("Enigma Byte Studio");

    // ---- Paleta oscura global ----
    QPalette paleta;
    paleta.setColor(QPalette::Window,          QColor("#0d0d0d"));
    paleta.setColor(QPalette::WindowText,      QColor("#d4c5a9"));
    paleta.setColor(QPalette::Base,            QColor("#0a0a0a"));
    paleta.setColor(QPalette::AlternateBase,   QColor("#111111"));
    paleta.setColor(QPalette::ToolTipBase,     QColor("#1a1a1a"));
    paleta.setColor(QPalette::ToolTipText,     QColor("#d4c5a9"));
    paleta.setColor(QPalette::Text,            QColor("#d4c5a9"));
    paleta.setColor(QPalette::Button,          QColor("#1a1510"));
    paleta.setColor(QPalette::ButtonText,      QColor("#c8a04a"));
    paleta.setColor(QPalette::Highlight,       QColor("#3a2a10"));
    paleta.setColor(QPalette::HighlightedText, QColor("#f0d890"));
    app.setPalette(paleta);

    // ---- Fuente monoespaciada global ----
    QFont fuenteBase("Courier New", 11);
    app.setFont(fuenteBase);

    // ---- Lanzar ventana principal ----
    MainWindow ventana;
    ventana.show();

    return app.exec();
}
