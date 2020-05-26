#pragma once
#include <iostream>
#include <fstream>
using namespace std;

class ModelDataParserClass
{
public:
	ModelDataParserClass();
	~ModelDataParserClass();

	bool ProcessParser(WCHAR* fileName, WCHAR* resultFileName, WCHAR* modelExtension);

private:

	// ¹öÅØ½º ÁÂÇ¥
	typedef struct
	{
		float x, y, z;
	}VertexType;

	// Face(ÇÑ »ï°¢Çü) ÀÎµ¦½º
	typedef struct
	{
		int vIndex1 = -99999, vIndex2 = -99999, vIndex3 = -99999, vIndex4 = -99999;
		int tIndex1 = -99999, tIndex2 = -99999, tIndex3 = -99999, tIndex4 = -99999;
		int nIndex1 = -99999, nIndex2 = -99999, nIndex3 = -99999, nIndex4 = -99999;
	}FaceType;

	void GetModelFilename(WCHAR* fileName);

	bool isExistModelFile(WCHAR* fileName);

	bool ParsingOBJ(WCHAR* fileName, WCHAR* resultFileName);
	bool ParsingFBX(WCHAR* fileName, WCHAR* resultFileName);
	bool ReadFileCounts(WCHAR* filename, int& vertexCount, int& textureCount, int& normalCount, int& faceCount);
	bool LoadDataStructures(WCHAR* filename, WCHAR* resultFileName, int vertexCount, int textureCount, int normalCount, int faceCount);
};