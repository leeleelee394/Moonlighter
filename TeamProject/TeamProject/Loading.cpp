#include "stdafx.h"
#include "Loading.h"

/***************************************************************
## Loading ##
@@ function<void()> func : 로딩할 함수
***************************************************************/
Loading::Loading(function<void()> func)
	:isLoadEnd(false), loadingFunc(func), loadingThread(nullptr)
{
}

/***************************************************************
## ~Loading ##
쓰레드 종료될때까지 대기 지점은 로딩 클래스가 삭제될 때 
***************************************************************/
Loading::~Loading()
{
	//쓰레드 끝날때까지 대기시키기
	if(loadingThread)
		loadingThread->join();
	//여기까지 왔다면 쓰레딩이 끝났다는 뜻이므로 메모리 해제해준다. 
	SafeDelete(loadingThread);
}
/***************************************************************
## Start ##
로딩을 시작한다. 
***************************************************************/
void Loading::Start()
{
	//만약 로딩쓰레드가 nullptr이라면 쓰레드를 생성한다. 
	//쓰레드는 생성과 동시에 비동기로 실행된다. 
	if (loadingThread == nullptr)
	{
		loadingThread = new thread(bind(&Loading::Threading, this));
		isLoadEnd = false;
	}
}
/***************************************************************
## Threading ##
실질적으로 쓰레드가 돌아갈 함수 
***************************************************************/
void Loading::Threading()
{
	//만약 로딩할 함수가 존재 한다면 해당 함수를 실행한다.
	if (loadingFunc)
		loadingFunc();
	//여기까지 왔다면 로딩이 끝났다는 뜻이므로 isLoadEnd값 true로 바꿔준다.
	isLoadEnd = true;
}

