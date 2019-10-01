#include "stdafx.h"
#include "CameraManager.h"

#include "GameObject.h"

SingletonCpp(CameraManager)

/***************************************************************************
## CameraManager ## 
***************************************************************************/
CameraManager::CameraManager()
	:mapSize((float)WinSizeX,(float)WinSizeY),pTarget(nullptr),zoomFactor(1.0f),
	speed(100.0f), state(CameraManager::FreeCamera), shakeDirection(1.f), moveStartDistance(CameraMoveStartDistance)
{
	this->position = Vector2(0.f,0.f);
	this->cameraRect = Figure::RectMake(
		(int)this->position.x,(int)this->position.y, WinSizeX, WinSizeY);
	this->saveMouse = _Input->GetMousePosition();
	shakeChangeDelayTime = CameraDefaultChangeDirectionDelay;
}

/***************************************************************************
## ~CameraManager ##
***************************************************************************/
CameraManager::~CameraManager()
{

}

/***************************************************************************
## Update ## 
카메라 상태에 따라 Update
***************************************************************************/
void CameraManager::Update()
{
	//TODO 타겟 카메라하고 카메라 보간 추가
	switch (state)
	{
	case CameraManager::None:
	case CameraManager::MoveToTarget:
		this->UpdateTargetCameraMode();
		break;
	case CameraManager::FreeCamera:
		this->UpdateFreeCameraMode();
		break;
	case CameraManager::End:
		this->ShakingUpdate();
		break;
	default:
		break;
	}
}


/***************************************************************************
## OnGui ## 
***************************************************************************/
void CameraManager::OnGui()
{
	static float strength = CameraDefaultStrength;
	static float time = CameraDefaultShakeTime;
	ImGui::Begin("Camera");
	{
		ImGui::SliderFloat("Strength", &strength, 0.5f, 10.f);
		ImGui::SliderFloat("ShakeTime", &time, 0.1f, 2.f);
		ImGui::SliderFloat("ShakeDelay", &shakeChangeDelayTime, 0.001f, 0.1f);
		if (ImGui::Button("Shake"))
			_Camera->Shake(strength, time);
	}
	ImGui::End();
}
/***************************************************************************
## GetRelaticeRect ## 
카메라 상대좌표로 RECT 보정
@@ RECT rc : 카메라 보정 받을 렉트 

@@ return RECT : 카메라 보정한 렉트 반환 
***************************************************************************/
RECT CameraManager::GetRelativeRect(RECT rc)
{
	Figure::MoveRect(&rc, (int)-position.x, (int)-position.y);
	rc.left =(LONG)(CastingFloat(rc.left) * zoomFactor);
	rc.top = (LONG)(CastingFloat(rc.top) * zoomFactor);
	rc.right = (LONG)(CastingFloat(rc.right) * zoomFactor);
	rc.bottom =(LONG)( CastingFloat(rc.bottom) * zoomFactor);
	return std::move(rc);
}
/***************************************************************************
## GetRelativePoint ## 
카메라 상대좌표로 POINT 보정 후 반환 
@@ POINT pt : 보정 받을 POINT

@@ return POINT : 보정 한 POINT 반환 
***************************************************************************/
POINT CameraManager::GetRelatviePoint(POINT pt)
{
	pt.x -= (LONG)position.x;
	pt.y -= (LONG)position.y;
	pt.x = (LONG)(CastingFloat(pt.x) * zoomFactor);
	pt.y = (LONG)(CastingFloat(pt.y) * zoomFactor);
	return std::move(pt);
}

/***************************************************************************
## GetRelativeVector2 ## 
카메라 상대좌표로 Vector2 보정 후 반환
@@ Vector2 v : 보정 받을 Vector2 

@@ return Vector2 : 보정 한 Vector2반환 
***************************************************************************/
Vector2 CameraManager::GetRelativeVector2(Vector2 v)
{
	v.x -= position.x;
	v.y -= position.y;
	v = v * zoomFactor;
	return std::move(v);
}
Vector2 CameraManager::GetWorldVector2(Vector2 v)
{
	return Vector2(v.x / zoomFactor + position.x,v.y / zoomFactor + position.y);
}
/***************************************************************************
## GetWorldMouse ## 
카메라 공간 상의 마우스좌표 반환

@@ return Vector2 : 월드 공간상 마우스좌표 
***************************************************************************/
Vector2 CameraManager::GetWorldMouse()
{
	Vector2 mouse = _Input->GetMousePosition();
	return  Vector2(mouse.x / zoomFactor + position.x, mouse.y / zoomFactor + position.y);
}
/***************************************************************************
## GetWorldViewportCenter ## 

@@ return Vector2 : 월드 공간상의 화면 중심 반환
***************************************************************************/
Vector2 CameraManager::GetWorldViewportCenter()
{
	return Vector2((position.x + (float)(WinSizeX / 2) / zoomFactor) , (position.y + (float)(WinSizeY / 2) / zoomFactor) );
}

/***************************************************************************
## GetWorldMosuePoint ## 

@@ return POINT : 월드 공간상의 마우스 좌표 POINT 반환 
***************************************************************************/
POINT CameraManager::GetWorldMousePoint()
{
	Vector2 mouse = _Input->GetMousePosition();
	mouse = Vector2(mouse.x / zoomFactor + position.x, mouse.y / zoomFactor + position.y);
	return { (LONG)mouse.x,(LONG)mouse.y };
}
/***************************************************************************
## SetZoom ## 
@@ float Zoom : 줌 수치 
***************************************************************************/
void CameraManager::SetZoom(float zoom)
{
	this->zoomFactor = Math::Clampf(zoom, CameraZoomMin, CameraZoomMax);
}
/***************************************************************************
## UpdateRenderRect ## 
화면 Rect반환
***************************************************************************/
void CameraManager::UpdateRenderRect()
{
	this->cameraRect = Figure::RectMake(
		(int)this->position.x, (int)this->position.y, WinSizeX, WinSizeY);
}
/***************************************************************************
## UpdateFreeCameraMode ## 
FreeCaemra 상태일 때의 업데이트 
***************************************************************************/
void CameraManager::UpdateFreeCameraMode()
{
	if (IsMouseOnGui == false)
	{
		Vector2 currentMouse = _Input->GetMousePosition();
		if (_Input->GetKey(VK_RBUTTON))
		{
			if (this->saveMouse != currentMouse)
			{
				this->position += (saveMouse - currentMouse) / zoomFactor;
				this->UpdateRenderRect();
				saveMouse = currentMouse;
			}
		}
		saveMouse = currentMouse;
	}
}
/***************************************************************************
## UpdateTargetCameraMode ##

***************************************************************************/
void CameraManager::UpdateTargetCameraMode()
{
	if (pTarget)
	{
		{
			//target의 위치로 서서히 움직인다.
			Vector2 centerPos = { position.x + CastingFloat(WinSizeX) * 0.5f, position.y + CastingFloat(WinSizeY) * 0.5f };
			Vector2 toTarget = pTarget->GetPosition() - centerPos;

			switch (state)
			{
			case MoveState::None:
			{
				if (toTarget.GetLength() > moveStartDistance)
				{
					this->state = MoveState::MoveToTarget;
				}
			}
			break;

			case MoveState::MoveToTarget:
			{
				float length = toTarget.GetLength();
				this->speed = (length / (CameraMaxDistance - CameraMinDistance)) * (CameraMaxSpeed - CameraMinSpeed);
				this->speed = Math::Clampf(speed, CameraMinSpeed, CameraMaxSpeed);

				centerPos += toTarget.Normalize() * speed * _TimeManager->DeltaTime();
				this->position = centerPos - Vector2(WinSizeX / 2.f, WinSizeY / 2.f);
				this->cameraRect = Figure::RectMake(position, Vector2(WinSizeX, WinSizeY));

				if ((pTarget->GetPosition() - centerPos).GetLength() <= 50.f)
				{
					this->state = MoveState::None;
					this->position = centerPos - Vector2(WinSizeX / 2.f, WinSizeY / 2.f);
					this->cameraRect = Figure::RectMake(position, Vector2(WinSizeX, WinSizeY));
				}

				this->AmendCamera();

			}
			break;
			}
			this->ShakingUpdate();
		}

	}
}

void CameraManager::AmendCamera()
{
	if (position.x < 0.f)
	{
		position.x -= position.x;
		this->cameraRect = Figure::RectMake(position, Vector2(WinSizeX, WinSizeY));
	}
	else if (cameraRect.right > (LONG)mapSize.x)
	{
		position.x -= ((float)cameraRect.right) - mapSize.x;
		this->cameraRect = Figure::RectMake(position, Vector2(WinSizeX, WinSizeY));
	}
	if (cameraRect.top < 0)
	{
		position.y -= position.y;
		this->cameraRect = Figure::RectMake(position, Vector2(WinSizeX, WinSizeY));
	}
	else if (cameraRect.bottom > (LONG)mapSize.y)
	{
		position.y -= (float(cameraRect.bottom)) - mapSize.y;
		this->cameraRect = Figure::RectMake(position, Vector2(WinSizeX, WinSizeY));
	}
}

void CameraManager::ShakingUpdate()
{
	if (isShake == true)
	{
		shakeTime -= _TimeManager->DeltaTime();
		shakeChangeDirectionTime += _TimeManager->DeltaTime();
		if (shakeChangeDirectionTime >= shakeChangeDelayTime)
		{
			shakeDirection = -1.f * shakeDirection;
			while (shakeChangeDirectionTime >= shakeChangeDelayTime)
				shakeChangeDirectionTime -= shakeChangeDelayTime;
		}
		shakeStrength -= (shakeTime / totalShakeTime) *_TimeManager->DeltaTime();
		float strengh = shakeStrength * shakeDirection;
		position += Vector2(strengh, 0.f);
		this->cameraRect = Figure::RectMake(position, Vector2(WinSizeX, WinSizeY));

		if (shakeTime <= 0.f)
		{
			isShake = false;
			shakeChangeDelayTime = CameraDefaultChangeDirectionDelay;
		}
		this->AmendCamera();
	}
}

/***************************************************************************
## CameraProc ## 
마우스 휠에 따라 줌
***************************************************************************/
void CameraManager::CameraProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (state == MoveState::FreeCamera)
	{
		if (IsMouseOnGui == false)
		{
			if ((SHORT)HIWORD(wParam) > 0)
			{
				this->zoomFactor += 0.1f;
				this->zoomFactor = Math::Clampf(zoomFactor, CameraZoomMin, CameraZoomMax);
			}
			else
			{
				this->zoomFactor -= 0.1f;
				this->zoomFactor = Math::Clampf(zoomFactor, CameraZoomMin, CameraZoomMax);
			}
		}
	}
}

void CameraManager::SetTarget(GameObject * object)
{
	if (object)
	{
		this->pTarget = object;
		this->state = MoveState::None;
		this->position = object->GetPosition() - Vector2(WinSizeX/2,WinSizeY/2);
		this->cameraRect = Figure::RectMake(position, Vector2(WinSizeX, WinSizeY));
		this->AmendCamera();
	}
}

void CameraManager::ChangeTarget(GameObject * object)
{
	pTarget = object;
	state = MoveState::None;

}

void CameraManager::SetFreeCamera()
{
	state = MoveState::FreeCamera;
}

void CameraManager::Shake(float strength,float shakeTime,float changeDirectionTime)
{
	if (isShake == false)
	{
		this->isShake = true; 
		this->shakeStrength = strength;
		this->shakeTime = this->totalShakeTime = shakeTime;
		this->shakeChangeDirectionTime = 0.f;
		this->shakeChangeDelayTime = changeDirectionTime;
	}
}

void CameraManager::InitCameraToPlayer()
{
	GameObject* object = _ObjectManager->FindObject(ObjectType::Object, "Will");
	if (object)
	{
		this->SetTarget(object);
		zoomFactor = 1.f;
	}
}
