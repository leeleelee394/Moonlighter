#pragma once
#include "SceneBase.h"
class HaScene : public SceneBase
{
public:
	void Init()override;
	void Release()override;
	void Update()override;
	void Render()override;

public:
	HaScene();
	~HaScene();
};

