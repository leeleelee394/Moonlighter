#include "stdafx.h"
#include "DebugSystem.h"

#include "LightSystem.h"
DebugSystem::DebugSystem()
{
}


DebugSystem::~DebugSystem()
{
}

void DebugSystem::DebugRender()
{

	if (_isDebug)
	{
		_TimeManager->Render();
		//_LightingSystem->OnGui();
		//_Camera->OnGui();
	}
}
