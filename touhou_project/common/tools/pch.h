/**
 * @file pch.h
 * @brief 미리 컴파일 처리할 헤더 모음
 */

#ifndef TOUHOU_PCH_H_
#define TOUHOU_PCH_H_

// byte error clear
#define NOMINMAX 1
#define byte win_byte_override
#include <windows.h>
#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#else
#error max macro is already defined
#endif
#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#else
#error min macro is already defined
#endif
#include <gdiplus.h>
#undef min
#undef max
#undef byte

// Window API from framework.h
#include <malloc.h>
#include <memory.h>
#include <stdlib.h>
#include <tchar.h>
// #include <windows.h>

// standard library
#include <cassert>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <deque>

using namespace std;

// png 로딩
// #include <gdiplus.h>
#include <objidl.h>
#pragma comment(lib, "GdiPlus.lib")
using namespace Gdiplus;

// 사운드
#include <dinput.h>
#include <dsound.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dsound.lib")

// transparent bit lock transfer
#pragma comment(lib, "Msimg32.lib")

#endif // TOUHOU_PCH_H_
