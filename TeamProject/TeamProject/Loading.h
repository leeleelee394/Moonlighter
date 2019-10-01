#pragma once
/*************************************************************
## Loading ##
스레드 로딩 클래스
**************************************************************/
#include <thread>
class Loading
{
private:
	thread* loadingThread;				//로딩해줄 쓰레드 
	bool isLoadEnd;						//로딩이 끝났는가 
	function<void()> loadingFunc;		//로딩해야 할 함수
public:
	//생성자에서 로딩해야할 함수를 넣어준다. 
	Loading(function<void()> func);
	~Loading();
	//스레드 로딩 시작 
	void Start();
	//로딩이 끝났는지 여부
	bool GetIsLoadEnd()const { return this->isLoadEnd; }
private:
	//실제 쓰레딩돌아갈 함수
	void Threading();
};

