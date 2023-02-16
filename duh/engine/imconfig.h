//-----------------------------------------------------------------------------
// USER IMPLEMENTATION
// This file contains compile-time options for ImGui.
// Other options (memory allocation overrides, callbacks, etc.) can be set at runtime via the ImGuiIO structure - ImGui::GetIO().
//-----------------------------------------------------------------------------

#pragma once

#include "foundation/vector2.h"
#include "foundation/vector4.h"
#include "foundation/color.h"
#include "foundation/assert.h"

//---- Define assertion handler. Defaults to calling assert().
#define IM_ASSERT(_EXPR)  __ASSERT__(_EXPR)

//---- Define attributes of all API symbols declarations, e.g. for DLL under Windows.
//#define IMGUI_API __declspec( dllexport )
//#define IMGUI_API __declspec( dllimport )

//---- Include imgui_user.h at the end of imgui.h
//#define IMGUI_INCLUDE_IMGUI_USER_H

//---- Don't implement default handlers for Windows (so as not to link with OpenClipboard() and others Win32 functions)
//#define IMGUI_DISABLE_WIN32_DEFAULT_CLIPBOARD_FUNCS
//#define IMGUI_DISABLE_WIN32_DEFAULT_IME_FUNCS

//---- Don't implement help and test window functionality (ShowUserGuide()/ShowStyleEditor()/ShowTestWindow() methods will be empty)
#define IMGUI_DISABLE_TEST_WINDOWS

//---- Don't define obsolete functions names
#define IMGUI_DISABLE_OBSOLETE_FUNCTIONS

//---- Implement STB libraries in a namespace to avoid conflicts
//#define IMGUI_STB_NAMESPACE     ImGuiStb

//---- Define constructor and implicit cast operators to convert back<>forth from your math types and ImVec2/ImVec4.
#define IM_VEC2_CLASS_EXTRA                                                    \
        ImVec2(const hg::Vector2& f) { x = f.x; y = f.y; }                     \
        operator hg::Vector2() const { return hg::Vector2(x,y); }

#define IM_VEC4_CLASS_EXTRA                                                    \
        ImVec4(const hg::Vector4& f) { x = f.x; y = f.y; z = f.z; w = f.w; }   \
		ImVec4(const hg::Color& c) { x = c.r; y = c.g; z = c.b; w = c.a; }     \
        operator hg::Vector4() const { return hg::Vector4(x,y,z,w); }          \
        operator hg::Color() const { return hg::Color(x, y, z, w); }

//---- Tip: You can add extra functions within the ImGui:: namespace, here or in your own headers files.
//---- e.g. create variants of the ImGui::Value() helper for your low-level math types, or your own widgets/helpers.
