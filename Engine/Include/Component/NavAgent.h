#pragma once

#include "ObjectComponent.h"
//#include "../Scene/Scene.h"
//#include "../Scene/SceneManager.h"

class CNavAgent :
    public CObjectComponent
{
	friend class CGameObject;

protected:
	CNavAgent();
	CNavAgent(const CNavAgent& com);
	virtual ~CNavAgent();

private:
	CSharedPtr<class CSceneComponent> m_UpdateComponent;
	std::list<Vector3>		m_PathList;
	float					m_MoveSpeed;
	bool					m_ApplyNavMesh;
	int						m_OccupyPolygonIndex;

public:
	void SetUpdateComponent(class CSceneComponent* UpdateComponent);
	bool Move(const Vector3& EndPos);
	// 길찾기를 하지 않고, NavMesh위에서 움직이고, NavMesh안에서만 움직이는 모든 오브젝트들은 이 함수들을 이용해서 움직여야 한다
	bool MoveOnNavMesh(const Vector3 EndPos);
	void SetMoveSpeed(float Speed)
	{
		m_MoveSpeed = Speed;
	}

	void SetApplyNavMesh(bool Apply)
	{
		m_ApplyNavMesh = Apply;
	}

	bool GetApplyNavMesh()	const
	{
		return m_ApplyNavMesh;
	}

public:
	void AddTargetPos(const Vector3& TargetPos)
	{
		m_PathList.push_back(TargetPos);
	}

	void ClearPathList()
	{
		m_PathList.clear();
	}

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void PrevRender();
	virtual void Render();
	virtual void PostRender();
	virtual CNavAgent* Clone();
	virtual bool Save(FILE* File);
	virtual bool Load(FILE* File);
	virtual bool SaveOnly(FILE* File);
	virtual bool LoadOnly(FILE* File);


private:
	void PathResult(const std::list<Vector3>& PathList);


public:
	void FillPathList(const std::list<Vector3>& PathList);
	bool FindPath(class CSceneComponent* OwnerComponent, const Vector3& End);
};

