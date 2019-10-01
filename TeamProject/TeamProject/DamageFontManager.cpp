#include "stdafx.h"
#include "DamageFontManager.h"
#include "StringHelper.h"

SingletonCpp(DamageFontManager)

DamageFontManager::DamageFontManager() {}
DamageFontManager::~DamageFontManager() {}

void DamageFontManager::Init()
{
}

void DamageFontManager::Release()
{
}

void DamageFontManager::Update()
{
	MoveDamage();
}

void DamageFontManager::Render()
{
	//텍스트 렌더
	for (UINT i = 0; i < _vDamageFont.size(); ++i) 
	{
		//텍스트 렌더
		//(폰트 포지션 x,y, 스트링헬퍼:: 해당 인덱스 데미지, 색상, 알파, 폰트 크기, 출력되는 피벗, 카메라 보정, 폰트 종류)
		_DXRenderer->RenderText(_vDamageFont[i].position.x - 15, _vDamageFont[i].position.y - 90,
			StringHelper::StringToWString(to_string(_vDamageFont[i].damage)),
			RGB(25, 42, 225), _vDamageFont[i].alpha, 22, DWRITE_TEXT_ALIGNMENT_LEADING, true,L"Cooper Std");

	}
}

void DamageFontManager::ShowDamage(Vector2 position, int damage)
{
	//구조체 데미지 폰트
	DamageFont sDamageFont;

	//x,y포지션 값
	sDamageFont.position = position;

	//데미지 값
	sDamageFont.damage = damage;

	//스피드 값
	sDamageFont.speed = 15.0f;

	//앵글 값
	sDamageFont.angle = Math::PI / 2;

	//알파 값
	sDamageFont.alpha = 1.0f;

	//벡터 데미지 폰트에 폰트 정보 담기
	_vDamageFont.push_back(sDamageFont);
}

void DamageFontManager::MoveDamage()
{
	//사이즈만큼 for문
	for (UINT i = 0; i < _vDamageFont.size(); ++i)
	{	
		//포지션 
		_vDamageFont.at(i).position.x += cosf(_vDamageFont.at(i).angle) * _vDamageFont.at(i).speed * _TimeManager->DeltaTime();
		_vDamageFont.at(i).position.y -= sinf(_vDamageFont.at(i).angle) * _vDamageFont.at(i).speed * _TimeManager->DeltaTime();

		//알파 값 초당 -0.9감소
		_vDamageFont.at(i).alpha -= 0.9f * _TimeManager->DeltaTime();

		//만일 알파 값이 0보다 낮거나 같으면
		if (_vDamageFont.at(i).alpha <= 0.f) 
		{
			//해당 인덱스 지우기
			_vDamageFont.erase(_vDamageFont.begin() + i);
		}
	}

	
	
	
}
