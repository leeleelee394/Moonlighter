#include "stdafx.h"
#include "DebugCollider.h"


DebugCollider::DebugCollider()
{
	_state = State::None;
}


DebugCollider::~DebugCollider()
{
}

void DebugCollider::Update()
{
	switch (_state)
	{
	case DebugCollider::State::None:
		if (IsMouseOnGui == false)
		{
			if (_Input->GetKeyDown(VK_LBUTTON))
			{
				_state = State::Drag;
				_startPos = _Camera->GetWorldMouse();
			}
		}
		break;
	case DebugCollider::State::Drag:
		_endPos = _Camera->GetWorldMouse();
		if (_Input->GetKeyUp(VK_LBUTTON))
		{
			_state = State::None;
		}
		_rect = { (LONG)_startPos.x,(LONG)_startPos.y,(LONG)_endPos.x,(LONG)_endPos.y };
		break;
	default:
		break;
	}
}

void DebugCollider::Render()
{
	_DXRenderer->DrawRectangle(_rect, DefaultBrush::blue, true);

	if (_isDebug)
	{
		ImGui::Begin("DebugCollider");
		ImGui::Text("Start : %d , %d", (int)_startPos.x, (int)_startPos.y);
		ImGui::Text("End : %d , %d", (int)_endPos.x, (int)_endPos.y);
		ImGui::Text("Width : %d", (int)(_endPos.x - _startPos.y));
		ImGui::Text("Height : %d", (int)(_endPos.y - _startPos.y));
		ImGui::End();
	}
}
