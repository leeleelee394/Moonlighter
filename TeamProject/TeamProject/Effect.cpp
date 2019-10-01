#include "stdafx.h"
#include "Effect.h"
#include "Animation.h"

// 키 값 이름과 포지션을 받는 생성자
Effect::Effect()
{
	this->_name = "Effect";

	//소드 공격 이팩트 애니메이션 new 선언
	_effectAnimation = new Animation;


	this->_isActive = false;
}


Effect::~Effect()
{
	//애니메이션 소멸자에서 딜리트
	SafeDelete(_effectAnimation);
}

void Effect::Init()
{

}

void Effect::Release()
{
	
}

void Effect::Update()
{
	//프레임이 다 돌면 true를 반환하는 UpdateFrame

	if (_effectAnimation->UpdateFrame() == true)
	{
		//이미지 없애기
		this->SetActive(false);
	}
}

void Effect::Render()
{
	//사이즈 선언
	//현재 프레임 사이즈x, 현재 프레임 사이즈y
	_effectIMG->SetSize(Vector2(_effectIMG->GetFrameSize(0).x, _effectIMG->GetFrameSize(0).y));

	//프레임 렌더
	//현재 프레임 x,y 를 렌더
	_effectIMG->FrameRender(_position.x, _position.y, _effectAnimation->GetNowFrameX(), _effectAnimation->GetNowFrameY(), Pivot::CENTER, true);
}

void Effect::StartEffect(const string name,const Vector2 position)
{
	if (this->_isActive == false) 
	{
		//선언한 키 값을 이미지 이름으로 받아옴
		_effectIMG = _ImageManager->FindImage(name);

		this->_position = position;
		
		//시작 프레임 x,y와 끝 프레임 x,y
		_effectAnimation->SetStartEndFrame(0, 0, _effectIMG->GetMaxFrameX() - 1, 0, false);
		//루프 해야함
		_effectAnimation->SetIsLoop(true);
		//프레임 도는 시간
		_effectAnimation->SetFrameUpdateTime(0.1f);

		//애니메이션 실행
		_effectAnimation->Play();
		
		//액티브 true
		this->SetActive(true);
	}

}

void Effect::PlayEffect(const string name,const Vector2 position)
{
	//벡터 포인터로 묶어 담기
	const vector<GameObject*>* _effectTest;
	//오브젝트 타입 이팩트 찾아서 포인터로 전부 가져오기
	_effectTest = _ObjectManager->GetObjectListPointer(ObjectType::Effect);

	//사이즈만큼 돌리기
	for (UINT i = 0; i < _effectTest->size(); ++i)
	{
		//액티브가 false이면
		if (_effectTest->at(i)->GetActive() == false)
		{
			//static_cast<Effect*>(_effectTest->at(i));
			if (dynamic_cast<Effect*>(_effectTest->at(i)) != nullptr)
			{
				dynamic_cast<Effect*>(_effectTest->at(i))->StartEffect(name, position);
				break;
			}

		}

	}

}
