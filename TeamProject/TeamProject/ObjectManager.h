#pragma once
/***************************************************************
## ObjectManager ##
기존의 오브젝트 메니져와 동일
앞으로 라이팅 추가하게 되면 변경될 예정
***************************************************************/
class ObjectManager
{
	SingletonHeader(ObjectManager)
private:
	typedef unordered_map<ObjectType::Enum, vector<class GameObject*>> ObjectContainer; 
	typedef unordered_map<ObjectType::Enum, vector<class GameObject*>>::iterator ObjectContainerIter;
private:
	ObjectContainer _objectContainer;
	class LightSystem* _lightSystem;
	bool _isZorder;
public:
	void Init();
	void Release();
	void Update();
	void Render();
public:
	void AddObject(const ObjectType::Enum type, class GameObject * pObject);
    class GameObject* FindObject(const ObjectType::Enum type, const string name);
	vector<class GameObject*> FindObjects(const ObjectType::Enum type, const string name);
	vector<class GameObject*> GetObjectList(const ObjectType::Enum type);
	const vector<class GameObject*>* GetObjectListPointer(const ObjectType::Enum type);

	void ChangeZOrdering(bool b) { _isZorder = b; }
	class LightSystem* GetLightSystem()const { return this->_lightSystem; }
private:
	void ZOrder();

	void ObjectRender();
	void Lighting();
	void UIRender();
};
#define _ObjectManager ObjectManager::Get()
#define _LightingSystem ObjectManager::Get()->GetLightSystem()
