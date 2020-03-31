#pragma once

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class D3DClass;
class CameraClass;
class ModelClass;
class ShaderManager;
class RenderManager
{
public:
	RenderManager();
	~RenderManager();

	bool Init(int screenWidth, int screenHeight, HWND hWnd);
	bool Render();

	// DirectX 클래스
	D3DClass* m_d3dClass = nullptr;

	// 카메라 클래스
	CameraClass* m_camera = nullptr;

	// 모델 클래스
	ModelClass* m_model = nullptr;

	// 쉐이더 관리하는 매니저
	ShaderManager* m_shaderManager = nullptr;
};