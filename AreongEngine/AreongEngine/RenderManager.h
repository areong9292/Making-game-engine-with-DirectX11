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

	// DirectX Ŭ����
	D3DClass* m_d3dClass = nullptr;

	// ī�޶� Ŭ����
	CameraClass* m_camera = nullptr;

	// �� Ŭ����
	ModelClass* m_model = nullptr;

	// ���̴� �����ϴ� �Ŵ���
	ShaderManager* m_shaderManager = nullptr;
};