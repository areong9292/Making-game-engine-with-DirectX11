#include "stdafx.h"
#include "D3DClass.h"

D3DClass::D3DClass()
{
	MessageBox(0, L"D3DClass", L"Error", MB_OK);
}

D3DClass::~D3DClass()
{
	MessageBox(0, L"~D3DClass", L"Error", MB_OK);
}

bool D3DClass::InitializeD3DClass()
{
	return true;
}
