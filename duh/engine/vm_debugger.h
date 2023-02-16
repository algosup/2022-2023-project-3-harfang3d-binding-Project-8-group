// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include <memory>

namespace hg {

void InstallVMDebugger();
void UninstallVMDebugger();

class RenderSystem;

void DrawVMDebugger(RenderSystem &render_system);

class VM;
class ScriptExecutionContext;

void NewExecutionContextInspector(std::shared_ptr<VM> vm, ScriptExecutionContext &exec_ctx);

} // namespace hg
