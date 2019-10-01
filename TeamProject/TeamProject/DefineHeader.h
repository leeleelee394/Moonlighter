/*********************************************************
## DefineHeader.h ##
클래스들이 공통적으로 사용할 전처리기(define)들 여기에 추가
**********************************************************/
#pragma once

#define LockFPS 300.f

#define WinSizeX 1152	//윈도우창 가로길이
#define WinSizeY 719	//윈도우창 세로길이

#define ZeroToASC 48	//숫자 0 아스키값
#define NineToASC 57	//숫자 9 아스키값

#define PathResources(n) (wstring(L"../Resources/").append(n))
#define PathSounds(n)(string("../Sounds/").append(n))

/*************************************************************
## SafeRelease ##
@@ p : Release 함수가 있는 클래스 

SafeRelease는 보통 팩토리 패턴구현 클래스 또는 라이브러리를 사용할 
경우 사용한다. 고로 우리는 평상시에 쓸 일 없어요~
*************************************************************/
#define SafeRelease(p){ if(p){ (p)->Release(); (p) = NULL; } }
/*************************************************************
## SafeDelete ##
@@ p : 포인터

할당한 메모리를 delete를 통해 해제 하고 해당 포인터(p)를 NULL로 초기화 
*************************************************************/
#define SafeDelete(p){ if(p){ delete (p); (p) = NULL; } }
/*************************************************************
## SafeDeleteArray ## 
@@ p : 동적 배열 포인터 

동적 배열로 할당한 포인터를 메모리 해제 후 NULL로 초기화 
*************************************************************/
#define SafeDeleteArray(p){ if(p){ delete [] (p); (p) = NULL; } }
/*************************************************************
## CastingInt ## 
@@ n : 정수,실수 형 변수 

n을 int형으로 변환 (float)1.0f하는거랑 같음
*************************************************************/
#define CastingInt(n) static_cast<int>(n)
/*************************************************************
## CastingFloat ##
@@ n : 정수,실수 형 

n을 float형으로 변환 
*************************************************************/
#define CastingFloat(n) static_cast<float>(n)
/*************************************************************
## CastingFloat ## 
@@ VarType : 자료형
@@ VarName : 변수이름 
@@ FuncName : 함수이름 

변수,접근자,설정자 편하게 만드는 전처리기
*************************************************************/
#define Synthesize(VarType, VarName, FuncName)	\
protected:	VarType VarName;	\
public:	inline VarType Get##FuncName(void) const { return VarName; }	\
public:	inline void Set##FuncName(VarType value) { VarName = value; }

/*************************************************************
## HResult ##
@@ n : HRESULT 결과값 

n의 결과값에 따라 에러 메세지 박스와 함께 컴파일러 중단
*************************************************************/
#if defined(DEBUG) | defined(_DEBUG)
#ifndef HResult
#define HResult(x)                                       \
   {                                             \
      HRESULT hr = (x);                              \
      if(FAILED(hr))                                 \
      {                                          \
         LPWSTR output;                              \
         FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS |            \
         FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),   \
            (LPTSTR)&output, 0, NULL);                                             \
            MessageBox(NULL, output, L"Error", MB_OK);                                 \
      }                                                                     \
   }
#endif

#else
#ifndef HResult
#define HResult(x) (x)
#endif
#endif 
/*************************************************************
## Assert ##
@@ b : HRESULT 또는 bool 값

assert는 컴파일러를 정지 시키지만 이 전처리기는 컴파일러 중지 대신 디버깅창에 문제점을 출력
*************************************************************/
#ifndef Assert
#if defined(DEBUG) || defined(_DEBUG)
#define Assert(b) do {if (!(b)) {OutputDebugStringA("Assert : " #b "\n"); }} while (0)
#else
#define Assert(b)
#endif   //   DEBUG || _DEBUG
#endif
/*************************************************************
## Singleton ##
전처리기 싱글톤
*************************************************************/
#define SingletonHeader(ClassName)	\
public:								\
	static ClassName* Get();		\
	static void Create();			\
	static void Delete();			\
private:							\
	ClassName();					\
	~ClassName();					\
	static ClassName* instance;		

#define SingletonCpp(ClassName)				\
ClassName* ClassName::instance = nullptr;	\
ClassName * ClassName::Get()				\
{											\
	assert(instance != nullptr);			\
	return instance;						\
}											\
void ClassName::Create()					\
{											\
	assert(instance == nullptr);			\
	instance = new ClassName();				\
}											\
void ClassName::Delete()					\
{											\
	SafeDelete(instance);					\
}			

