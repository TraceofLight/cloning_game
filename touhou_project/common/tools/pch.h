/**
 * @file pch.h
 * @brief 미리 컴파일 처리할 헤더 모음
 */

#ifndef TOUHOU_PCH_H_
#define TOUHOU_PCH_H_

// png 로딩
#include <objidl.h>
#include <gdiplus.h>
#pragma comment(lib, "GdiPlus.lib")
using namespace Gdiplus;

// 사운드
#include <mmsystem.h>
#include <dsound.h>
#include <dinput.h>
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dsound.lib")

// Window API from framework.h
#include <malloc.h>
#include <memory.h>
#include <stdlib.h>
#include <tchar.h>
#include <windows.h>

// standard library
#include <cassert>
#include <list>
#include <map>
#include <memory>
#include <string>
#include <vector>

using namespace std;

// transparent bit lock transfer
#pragma comment(lib, "Msimg32.lib")

#endif // TOUHOU_PCH_H_
