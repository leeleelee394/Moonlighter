#include "stdafx.h"
#include "DebugObject.h"


DebugObject::DebugObject(string name)
{
	_image = _ImageManager->FindImage(name);
}


DebugObject::~DebugObject()
{
}

void DebugObject::Update()
{
	this->_position = _Camera->GetWorldMouse();
}

void DebugObject::Render()
{
	_image->Render(_position.x, _position.y, Pivot::LEFT_TOP, true);

	if (_isDebug)
	{
		ImGui::Begin("DebugObject");
		ImGui::Text("MousePos X : %d", CastingInt(_Camera->GetWorldMouse().x));
		ImGui::Text("MousePos Y : %d", CastingInt(_Camera->GetWorldMouse().y));
		ImGui::End();
	}
}
