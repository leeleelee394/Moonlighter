#include "stdafx.h"
#include "MoveItem.h"
#include "UIWeaponAndBag.h"
#include "Inventory.h"
#include "Enemy.h"


//¿¡³Ê¹Ì ¾ÆÀÌÅÛ »Ñ¸®±â
void Enemy::SprayItemEnemy(string name)
{
	//x, y Æ÷Áö¼Ç 
	Vector2 randomPos;

	//·£´ý ÁÂÇ¥ X -30 ~ +30
	randomPos.x = Math::Random(this->_position.x - 30.f, this->_position.x + 30.f);
	//·£´ý ÁÂÇ¥ Y -30 ~ +30
	randomPos.y = Math::Random(this->_position.y - 30.f, this->_position.y + 30.f);

	//¾ÆÀÌÅÛ »Ñ¸®±â
	_ObjectManager->AddObject(ObjectType::Object, new MoveItem(name, randomPos));
}