#pragma once
#include "SceneBase.h"
class LoadingSceneBase :public SceneBase
{
protected:
	class Loading* _loading; 	
	string _nextSceneName;
public:
	LoadingSceneBase();
	~LoadingSceneBase();

	void SetLoadingFunc(function<void()> func);
	void SetNextSceneName(const string name) { _nextSceneName = name; }
};

