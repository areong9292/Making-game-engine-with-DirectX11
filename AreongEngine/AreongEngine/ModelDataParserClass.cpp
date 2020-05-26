#include "stdafx.h"
#include "ModelDataParserClass.h"

ModelDataParserClass::ModelDataParserClass()
{
}

ModelDataParserClass::~ModelDataParserClass()
{
}

bool ModelDataParserClass::ProcessParser(WCHAR* fileName, WCHAR* resultFileName, WCHAR* modelExtension)
{
	// �� ���� �ִ��� üũ
	if (!isExistModelFile(fileName))
	{
		return false;
	}
	
	// Ÿ�Կ� ���� �ٸ� �Ľ� ó��
	if (wcscmp(modelExtension, L".obj") == 0)
	{
		if (!ParsingOBJ(fileName, resultFileName))
		{
			MessageBox(0, L"ParsingOBJ failed", L"Error", MB_OK);
			return false;
		}
	}
	// Ÿ�Կ� ���� �ٸ� �Ľ� ó��
	if (wcscmp(modelExtension, L".fbx") == 0)
	{
		if (!ParsingFBX(fileName, resultFileName))
		{
			MessageBox(0, L"ParsingFBX failed", L"Error", MB_OK);
			return false;
		}
	}

	// ���� ���� ������
	cout << "\nFile has been converted." << endl;
	return true;
}

bool ModelDataParserClass::isExistModelFile(WCHAR * fileName)
{
	bool done;
	ifstream fin;

	// �ùٸ��� ������ ã���� ��� true ����
	fin.open(fileName);
	if (fin.good())
	{
		fin.close();
		return true;
	}
	else
	{
		fin.close();
		MessageBox(0, L"File not exist", L"Error", MB_OK);
		return false;
	}
}

bool ModelDataParserClass::ParsingOBJ(WCHAR* fileName, WCHAR* resultFileName)
{
	bool result;
	int vertexCount, textureCount, normalCount, faceCount;

	// �� ��ҵ��� ���ڸ� �о �����͸� �ʱ�ȭ�Ѵ�
	result = ReadFileCounts(fileName, vertexCount, textureCount, normalCount, faceCount);
	if (!result)
	{
		MessageBox(0, L"ReadFileCounts Failed", L"Error", MB_OK);
		return false;
	}

	// �� ���� ���
	cout << endl;
	cout << "Vertices: " << vertexCount << endl;
	cout << "UVs:      " << textureCount << endl;
	cout << "Normals:  " << normalCount << endl;
	cout << "Faces:    " << faceCount << endl;

	// ���� �� ������ �о �Ľ���
	result = LoadDataStructures(fileName, resultFileName, vertexCount, textureCount, normalCount, faceCount);
	if (!result)
	{
		MessageBox(0, L"LoadDataStructures Failed", L"Error", MB_OK);
		return false;
	}

	return true;
}

bool ModelDataParserClass::ParsingFBX(WCHAR * fileName, WCHAR * resultFileName)
{
	return false;
}

bool ModelDataParserClass::ReadFileCounts(WCHAR* filename, int& vertexCount, int& textureCount, int & normalCount, int& faceCount)
{
	ifstream fin;
	char inputLine[255];
	vertexCount = 0;
	textureCount = 0;
	normalCount = 0;
	faceCount = 0;

	fin.open(filename);

	// ���� �б⿡ �����ϸ� ����
	if (fin.fail())
		return false;

	while (fin)
	{
		fin.getline(inputLine, 255);
		if (fin)
		{

			// ������ �Ľ��ؼ� �ش� ����� ���ڸ� ����Ѵ�
			if (inputLine[0] == 'v')
			{
				// ���ؽ�
				if (inputLine[1] == ' ') { vertexCount++; }
				if (inputLine[1] == 't') { textureCount++; }
				if (inputLine[1] == 'n') { normalCount++; }
			}
			if (inputLine[0] == 'f')
			{
				if (inputLine[1] == ' ') { faceCount++; }
			}

		}
	}

	fin.close();

	return true;
}

bool ModelDataParserClass::LoadDataStructures(WCHAR* filename, WCHAR* resultFileName, int vertexCount, int textureCount, int normalCount, int faceCount)
{
	VertexType *vertices, *texcoords, *normals;
	FaceType *faces;
	ifstream fin;
	int vertexIndex, texcoordIndex, normalIndex, faceIndex, vIndex, tIndex, nIndex;
	char inputLine[255];
	ofstream fout;

	// ������ ����ü �ʱ�ȭ
	vertices = new VertexType[vertexCount];
	if (!vertices)
	{
		return false;
	}

	texcoords = new VertexType[textureCount];
	if (!texcoords)
	{
		return false;
	}

	normals = new VertexType[normalCount];
	if (!normals)
	{
		return false;
	}

	faces = new FaceType[faceCount];
	if (!faces)
	{
		return false;
	}

	// �ε��� �� �ʱ�ȭ
	vertexIndex = 0;
	texcoordIndex = 0;
	normalIndex = 0;
	faceIndex = 0;

	fin.open(filename);

	// ���� �� �ε����� üũ
	if (fin.fail() == true)
	{
		return false;
	}

	char* tok1;
	char* context1 = NULL;

	char* tok2;
	char* context2 = NULL;

	while (fin)
	{
		fin.getline(inputLine, 255);
		if (fin)
		{
			if (inputLine[0] == 'v')
			{
				// ���ؽ�
				if (inputLine[1] == ' ')
				{
					tok1 = strtok_s(inputLine, " ", &context1);

					int cnt = 0;
					while (tok1 != NULL)
					{
						tok1 = strtok_s(NULL, " ", &context1);
						if (tok1 == NULL)
							break;

						if (cnt == 0)
							vertices[vertexIndex].x = atof(tok1);
						else if (cnt == 1)
							vertices[vertexIndex].y = atof(tok1);
						else if (cnt == 2)
							vertices[vertexIndex].z = atof(tok1);

						cnt++;
						if (cnt == 3)
							cnt = 0;
					}

					// �޼� ��ǥ��� ��ȯ
					vertices[vertexIndex].z = vertices[vertexIndex].z * -1.0f;
					vertexIndex++;
				}
				// uv
				else if (inputLine[1] == 't')
				{
					tok1 = strtok_s(inputLine, " ", &context1);

					int cnt = 0;
					while (tok1 != NULL)
					{
						tok1 = strtok_s(NULL, " ", &context1);
						if (tok1 == NULL)
							break;

						if (cnt == 0)
							texcoords[texcoordIndex].x = atof(tok1);
						else if (cnt == 1)
							texcoords[texcoordIndex].y = atof(tok1);
						else if (cnt == 2)
							texcoords[texcoordIndex].z = atof(tok1);

						cnt++;
						if (cnt == 3)
							cnt = 0;
					}

					// �޼� ��ǥ��� ��ȯ
					texcoords[texcoordIndex].y = 1.0f - texcoords[texcoordIndex].y;
					texcoordIndex++;
				}
				// ����
				else if (inputLine[1] == 'n')
				{
					tok1 = strtok_s(inputLine, " ", &context1);

					int cnt = 0;
					while (tok1 != NULL)
					{
						tok1 = strtok_s(NULL, " ", &context1);
						if (tok1 == NULL)
							break;

						if (cnt == 0)
							normals[normalIndex].x = atof(tok1);
						else if (cnt == 1)
							normals[normalIndex].y = atof(tok1);
						else if (cnt == 2)
							normals[normalIndex].z = atof(tok1);

						cnt++;
						if (cnt == 3)
							cnt = 0;
					}

					// �޼� ��ǥ��� ��ȯ
					normals[normalIndex].z = normals[normalIndex].z * -1.0f;
					normalIndex++;
				}
			}
			else if (inputLine[0] == 'f')
			{
				tok1 = strtok_s(inputLine, " ", &context1);

				int cnt = 0;
				while (tok1 != NULL)
				{
					tok1 = strtok_s(NULL, " ", &context1);
					if (tok1 == NULL)
						break;

					tok2 = strtok_s(tok1, "/", &context2);

					int cnt2 = 0;
					while (tok2 != NULL)
					{
						if (cnt == 0)
						{
							if (cnt2 == 0)
								faces[faceIndex].vIndex4 = atoi(tok2);
							else if (cnt2 == 1)
								faces[faceIndex].tIndex4 = atoi(tok2);
							else if (cnt2 == 2)
								faces[faceIndex].nIndex4 = atoi(tok2);
						}
						else if (cnt == 1)
						{
							if (cnt2 == 0)
								faces[faceIndex].vIndex3 = atoi(tok2);
							else if (cnt2 == 1)
								faces[faceIndex].tIndex3 = atoi(tok2);
							else if (cnt2 == 2)
								faces[faceIndex].nIndex3 = atoi(tok2);
						}
						else if (cnt == 2)
						{
							if (cnt2 == 0)
								faces[faceIndex].vIndex2 = atoi(tok2);
							else if (cnt2 == 1)
								faces[faceIndex].tIndex2 = atoi(tok2);
							else if (cnt2 == 2)
								faces[faceIndex].nIndex2 = atoi(tok2);
						}
						else if (cnt == 3)
						{
							faceCount++;
							if (cnt2 == 0)
								faces[faceIndex].vIndex1 = atoi(tok2);
							else if (cnt2 == 1)
								faces[faceIndex].tIndex1 = atoi(tok2);
							else if (cnt2 == 2)
								faces[faceIndex].nIndex1 = atoi(tok2);
						}

						cnt2++;
						if (cnt2 == 3)
							cnt2 = 0;

						tok2 = strtok_s(NULL, "/", &context2);
					}

					cnt++;
					if (cnt == 4)
						cnt = 0;
				}

				faceIndex++;

			}
		}


	}
	fin.close();

	fout.open(resultFileName);

	fout << "VertexCount: " << (faceCount * 3) << endl;
	fout << "Data:" << endl;

	for (int i = 0; i < faceIndex; i++)
	{
		vIndex = faces[i].vIndex1 - 1;
		tIndex = faces[i].tIndex1 - 1;
		nIndex = faces[i].nIndex1 - 1;

		// f�� 4����
		if (vIndex != -100000 && tIndex != -100000 && nIndex != -100000)
		{
			fout << vertices[vIndex].x << ' ' << vertices[vIndex].y << ' ' << vertices[vIndex].z << ' '
				<< texcoords[tIndex].x << ' ' << texcoords[tIndex].y << ' '
				<< normals[nIndex].x << ' ' << normals[nIndex].y << ' ' << normals[nIndex].z << endl;
			vIndex = faces[i].vIndex2 - 1;
			tIndex = faces[i].tIndex2 - 1;
			nIndex = faces[i].nIndex2 - 1;

			fout << vertices[vIndex].x << ' ' << vertices[vIndex].y << ' ' << vertices[vIndex].z << ' '
				<< texcoords[tIndex].x << ' ' << texcoords[tIndex].y << ' '
				<< normals[nIndex].x << ' ' << normals[nIndex].y << ' ' << normals[nIndex].z << endl;

			vIndex = faces[i].vIndex3 - 1;
			tIndex = faces[i].tIndex3 - 1;
			nIndex = faces[i].nIndex3 - 1;

			fout << vertices[vIndex].x << ' ' << vertices[vIndex].y << ' ' << vertices[vIndex].z << ' '
				<< texcoords[tIndex].x << ' ' << texcoords[tIndex].y << ' '
				<< normals[nIndex].x << ' ' << normals[nIndex].y << ' ' << normals[nIndex].z << endl;

			vIndex = faces[i].vIndex1 - 1;
			tIndex = faces[i].tIndex1 - 1;
			nIndex = faces[i].nIndex1 - 1;

			fout << vertices[vIndex].x << ' ' << vertices[vIndex].y << ' ' << vertices[vIndex].z << ' '
				<< texcoords[tIndex].x << ' ' << texcoords[tIndex].y << ' '
				<< normals[nIndex].x << ' ' << normals[nIndex].y << ' ' << normals[nIndex].z << endl;


			vIndex = faces[i].vIndex3 - 1;
			tIndex = faces[i].tIndex3 - 1;
			nIndex = faces[i].nIndex3 - 1;

			fout << vertices[vIndex].x << ' ' << vertices[vIndex].y << ' ' << vertices[vIndex].z << ' '
				<< texcoords[tIndex].x << ' ' << texcoords[tIndex].y << ' '
				<< normals[nIndex].x << ' ' << normals[nIndex].y << ' ' << normals[nIndex].z << endl;

			vIndex = faces[i].vIndex4 - 1;
			tIndex = faces[i].tIndex4 - 1;
			nIndex = faces[i].nIndex4 - 1;

			fout << vertices[vIndex].x << ' ' << vertices[vIndex].y << ' ' << vertices[vIndex].z << ' '
				<< texcoords[tIndex].x << ' ' << texcoords[tIndex].y << ' '
				<< normals[nIndex].x << ' ' << normals[nIndex].y << ' ' << normals[nIndex].z << endl;
		}
		// f�� 3����
		else
		{
			vIndex = faces[i].vIndex2 - 1;
			tIndex = faces[i].tIndex2 - 1;
			nIndex = faces[i].nIndex2 - 1;

			fout << vertices[vIndex].x << ' ' << vertices[vIndex].y << ' ' << vertices[vIndex].z << ' '
				<< texcoords[tIndex].x << ' ' << texcoords[tIndex].y << ' '
				<< normals[nIndex].x << ' ' << normals[nIndex].y << ' ' << normals[nIndex].z << endl;

			vIndex = faces[i].vIndex3 - 1;
			tIndex = faces[i].tIndex3 - 1;
			nIndex = faces[i].nIndex3 - 1;

			fout << vertices[vIndex].x << ' ' << vertices[vIndex].y << ' ' << vertices[vIndex].z << ' '
				<< texcoords[tIndex].x << ' ' << texcoords[tIndex].y << ' '
				<< normals[nIndex].x << ' ' << normals[nIndex].y << ' ' << normals[nIndex].z << endl;

			vIndex = faces[i].vIndex4 - 1;
			tIndex = faces[i].tIndex4 - 1;
			nIndex = faces[i].nIndex4 - 1;

			fout << vertices[vIndex].x << ' ' << vertices[vIndex].y << ' ' << vertices[vIndex].z << ' '
				<< texcoords[tIndex].x << ' ' << texcoords[tIndex].y << ' '
				<< normals[nIndex].x << ' ' << normals[nIndex].y << ' ' << normals[nIndex].z << endl;
		}
	}

	cout << "parse End" << endl;

	fout.close();

	if (vertices)
	{
		delete[] vertices;
		vertices = 0;
	}
	if (texcoords)
	{
		delete[] texcoords;
		texcoords = 0;
	}
	if (normals)
	{
		delete[] normals;
		normals = 0;
	}
	if (faces)
	{
		delete[] faces;
		faces = 0;
	}

	return true;
}
