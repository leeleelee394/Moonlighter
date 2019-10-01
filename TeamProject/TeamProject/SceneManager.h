#pragma once
/***************************************************************
## SceneManager ##
***************************************************************/
class SceneManager
{
	SingletonHeader(SceneManager)
public:
	string nextSceneName;
private:
	enum class State
	{
		None,FadeOut,FadeIn,Load
	};

	typedef unordered_map<string, class SceneBase*>::iterator SceneIter;
private:
	unordered_map<string, class SceneBase*> sceneList;
	unordered_map<string, class SceneBase*> loadingSceneList;
	class SceneBase* nowScene;

	function<void(string,bool)> loadFunc;
	string loadSceneName;
	bool bInit;

	State state;
	float fadeAlpha;
private:
	void ChangeScene(string name, bool init);
public:
	void Update();
	void Render();
	void SceneQueue();

	void AddScene(string name, class SceneBase* scene);
	class SceneBase* FindScene(string name);
	void LoadScene(string name,bool init = true);
	void LoadSceneByLoading(string name);
};

#define _SceneManager SceneManager::Get()