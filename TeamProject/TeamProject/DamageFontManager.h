#pragma once
class DamageFontManager
{
	SingletonHeader(DamageFontManager)

	struct DamageFont
	{
		Vector2 position;				//포지션
		int damage;						//데미지
		float speed;					//스피드
		float angle;					//앵글
		float alpha;					//알파
		bool isAddFont;					//폰트 bool
	};
private:
	vector<DamageFont> _vDamageFont;	//벡터 데미지 폰트

	bool _isAddFont;					//데미지 폰트 생성 bool 체크
public:
	void Init();
	void Release();
	void Update();
	void Render();

public:
	//데미지 폰트 출력
	void ShowDamage(Vector2 position, int damage);

	//데미지 이동
	void MoveDamage();
};

#define _DamageFontManager DamageFontManager::Get()

