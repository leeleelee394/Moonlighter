/***************************************************************************************
## stdafx.h ## 
미리 컴파일 된 헤더 
****************************************************************************************/
#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <string>
#include <assert.h>
#include <functional>
#include <unordered_map>

using namespace std;

#include "LibraryHeader.h"
#include "DefineHeader.h"
#include "EnumStructHeader.h"
#include "ModuleHeader.h"

#define IsMouseOnGui ImGui::GetIO().WantCaptureMouse


#ifdef _DEBUG

//콘솔 창 출력
#pragma comment(linker , "/entry:WinMainCRTStartup /subsystem:console")

#else


#endif

extern bool _isDebug;