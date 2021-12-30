// Copyright 2001-2019 Crytek GmbH / Crytek Group. All rights reserved.
#include "EditorCommonInit.h"

#include "IPlugin.h"

#include <IEditor.h>

#include <BlackBox/System/IConsole.hpp>
#include <BlackBox/Core/Platform/platform_impl.inl>

static IEditor* g_editor = nullptr;

IEditor* GetIEditor() { return g_editor; }

void EDITOR_COMMON_API EditorCommon::SetIEditor(IEditor* editor)
{
	g_editor = editor;
	auto system = GetIEditor()->GetSystem();
	gEnv = system->GetGlobalEnvironment();
}

void EDITOR_COMMON_API EditorCommon::Initialize()
{
	ModuleInitISystem(GetIEditor()->GetSystem(), "EditorCommon");

	RegisterPlugin();

	//QMetaType::registerComparators<CItemModelAttribute*>();
}

void EDITOR_COMMON_API EditorCommon::Deinitialize()
{
	UnregisterPlugin();
	g_editor = nullptr;
	gEnv = nullptr;
}
