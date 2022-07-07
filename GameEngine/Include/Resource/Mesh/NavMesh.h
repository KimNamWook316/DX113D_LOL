#pragma once

#include "Mesh.h"

class CNavMesh :
    public CMesh
{
	friend class CMeshManager;
	friend class CNavMeshComponent;

protected:
	CNavMesh();
	virtual ~CNavMesh();

private:
	std::vector<NavMeshPolygon>			m_vecNavMeshPolygon;
	std::vector<Vector3>				m_vecVertexPos;
	std::vector<int>					m_vecIndex;
	Vector3								m_Min;
	Vector3								m_Max;

	int m_PolyCount;

public:
	const Vector3& GetMin()	const
	{
		return m_Min;
	}

	const Vector3& GetMax()	const
	{
		return m_Max;
	}

	size_t GetNavMeshPolygonCount() const
	{
		return m_vecNavMeshPolygon.size();
	}

	const NavMeshPolygon& GetNavMeshPolygon(int Idx)	const
	{
		return m_vecNavMeshPolygon[Idx];
	}

	size_t GetVertexPosCount()	const
	{
		return m_vecVertexPos.size();
	}

	const Vector3& GetVertexPos(int Index)	const
	{
		return m_vecVertexPos[Index];
	}

	// m_mapIndex�� �̹� Vertex�� �����ϸ� �� Vertex�� Index�� �����ϰ� ������ -1�� �����ؼ�
	// �� Vertex���� ���ο� Index�� �ο�
	int CheckAndAssignIndex(const Vector3& Vertex);

public:
	virtual bool Init();
	virtual bool LoadMeshFullPathMultibyte(const char* FullPath);
	virtual bool LoadMeshFullPathMultibyte(std::string& OutName, const char* FullPath) override;

public:
	virtual bool ConvertFBX(class CFBXLoader* Loader, const char* FullPath);
	virtual bool SaveMesh(FILE* File);
	virtual bool LoadMesh(FILE* File);
	bool CreateNavMesh(const std::string& Name);

private:
	int CheckPointIndex(const Vector3& Point);
	void SplitByDelimit(std::string Input, char delimit, std::vector<std::string>& vecOut);
	void CreatePolygonInfo();
	void CheckAdjInfo();
	bool CheckAdjVector(int PolyIndex, int CheckIndex);
};
