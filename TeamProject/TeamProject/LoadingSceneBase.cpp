#include "stdafx.h"
#include "LoadingSceneBase.h"

#include "Loading.h"
LoadingSceneBase::LoadingSceneBase()
	:_loading(nullptr)
{
}


LoadingSceneBase::~LoadingSceneBase()
{
}

void LoadingSceneBase::SetLoadingFunc(function<void()> func)
{
	if (_loading == nullptr)
	{
		_loading = new Loading(func);
	}
}
