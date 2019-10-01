#include "stdafx.h"
#include "DebugFrameObject.h"

#include "Animation.h"
DebugFrameObject::DebugFrameObject(string imageKey)
	:_frameUpdateTime(0.2f)
{
	_image = _ImageManager->FindImage(imageKey);
	_animation = new Animation;
	_animation->SetStartEndFrame(0, 0, _image->GetMaxFrameX() - 1, 0, false);
	_animation->SetFrameUpdateTime(_frameUpdateTime);
	_animation->SetIsLoop(true);
	_animation->Play();

	_size = _image->GetFrameSize(0);

	_pivot = Pivot::LEFT_TOP;
}


DebugFrameObject::~DebugFrameObject()
{
	SafeDelete(_animation);
}

void DebugFrameObject::Update()
{
	_animation->UpdateFrame();

	_position = _Camera->GetWorldMouse();
	this->UpdateMainRect();
}

void DebugFrameObject::Render()
{
	_image->SetSize(_size);
	_image->FrameRender(_mainRect.left, _mainRect.top,_animation->GetNowFrameX(),_animation->GetNowFrameY(), Pivot::LEFT_TOP, true);

	if (_isDebug)
	{
		ImGui::Begin("DebugFrameObject");
		{
			ImGui::Text("MousePos X : %d", CastingInt(_Camera->GetWorldMouse().x));
			ImGui::Text("MousePos Y : %d", CastingInt(_Camera->GetWorldMouse().y));
			if (ImGui::SliderFloat("Size", &_size.x, 10.f, 500.f))
			{
				_size.y = _size.x;
				this->UpdateMainRect();
			}
			if (ImGui::SliderFloat("FrameUpdate", &_frameUpdateTime, 0.1f, 0.5f))
				this->_animation->SetFrameUpdateTime(_frameUpdateTime);

		}
		ImGui::End();
	}
}





