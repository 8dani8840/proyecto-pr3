#ifndef COLD_CASE_EXTENSION_HPP
#define COLD_CASE_EXTENSION_HPP

// ColdCaseExtension.hpp — Punto de entrada de la GDExtension
#include <godot_cpp/core/class_db.hpp>

void initialize_coldcase_module(godot::ModuleInitializationLevel p_level);
void uninitialize_coldcase_module(godot::ModuleInitializationLevel p_level);

#endif // COLD_CASE_EXTENSION_HPP
