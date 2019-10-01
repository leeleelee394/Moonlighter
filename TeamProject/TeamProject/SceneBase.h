#pragma once
class SceneBase
{
public:
	SceneBase();
	~SceneBase();

	virtual void Init() = 0;
	virtual void Release() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
};

