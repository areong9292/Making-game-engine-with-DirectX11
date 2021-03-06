#pragma once

#include "resource.h"
#include "RenderManager.h"
#include "InputManager.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                RegisterWindowClass(HINSTANCE hInstance);
bool                InitInstance(HINSTANCE, int);

bool				InitClass();
int					MessageLoop();
bool				SaveProject();
bool				ReleaseClass();

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void ShutdownWindows();

// width and height of the window
int screenWidth = 0;
int screenHeight = 0;

HWND hWnd;

// is window full screen?
bool fullScreen = false;

RenderManager* m_renderManager = nullptr;
InputManager* m_InputManager = nullptr;