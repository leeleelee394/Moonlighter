#include "stdafx.h"
#include "TestDeadman.h"


TestDeadman::TestDeadman(Vector2 pos, Vector2 size)
	:GameObject("TestDeadman",pos,size,Pivot::CENTER)
{
	_isMove = true;

	this->AddCallbackMessage("InventoryOpen", [this](TagMessage message) {this->MoveOff();});
}

TestDeadman::~TestDeadman()
{
}

void TestDeadman::Init()
{
}

void TestDeadman::Update()
{
	if (_isMove)
	{
		this->_position += Vector2(Math::Random(-1.f, 1.f), Math::Random(-1.f, 1.f)) * 100.f * _TimeManager->DeltaTime();
		this->UpdateMainRect();
	}
}
void TestDeadman::Render()
{
	_DXRenderer->DrawRectangle(_mainRect, DefaultBrush::green, false, 3.0f);
}
