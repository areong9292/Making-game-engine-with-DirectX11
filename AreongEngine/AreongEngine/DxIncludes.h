#pragma once

// 객체 모듈을 사용하기 위해 링크하는 라이브러리들 명시
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

// 링크한 라이브러리들, DirectX타입 정의와 같은 것들에 대한 헤더 추가
#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;
using namespace std;
#include "AlignedAllocationPolicy.h"