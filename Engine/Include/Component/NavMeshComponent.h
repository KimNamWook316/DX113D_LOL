#pragma once

#include "SceneComponent.h"
#include "../Resource/ResourceManager.h"
#include "../Render/RenderManager.h"
#include "../Render/RenderState.h"

class CNavMeshComponent :
    public CSceneComponent
{
	friend class CGameObject;
	friend class CCameraManager;

protected:
	CNavMeshComponent();
	CNavMeshComponent(const CNavMeshComponent& com);
	virtual ~CNavMeshComponent();

protected:
	CSharedPtr<CNavMesh>	m_NavMesh;
	CSharedPtr<class CShader> m_Shader;
	bool m_DebugRender;
	CSharedPtr<CRenderState> m_WireFrameState;
	//int		m_CountX;
	//int		m_CountZ;
	//std::vector<NavMeshPolygon>		m_vecNavMeshPolygon;
	//Vector3 m_Min;
	//Vector3 m_Max;

public:
	CNavMesh* GetNavMesh()	const
	{
		return m_NavMesh;
	}

	void SetDebugRender(bool Debug)
	{
		m_DebugRender = Debug;

		if (m_DebugRender)
		{
			m_Shader = CResourceManager::GetInst()->FindShader("Standard3DWireFrameShader");
			m_WireFrameState = CRenderManager::GetInst()->FindRenderState("Wireframe");
		}
	}

	bool GetDebugRender() const
	{
		return m_DebugRender;
	}

public:
	void SetNavMesh(const std::string& Name);
	void SetNavMesh(class CNavMesh* NavMesh);
	void GetNavPolgonVertexPos(int Index, std::vector<Vector3>& vecPos);
	void GetAdjPolyIndex(int Index, std::vector<int>& vecPolyIndex);
	const Vector3& GetVertexPos(int PolyIndex, int VertexIndex);

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void PrevRender();
	virtual void Render();
	virtual void PostRender();
	virtual CNavMeshComponent* Clone();
	virtual bool Save(FILE* File);
	virtual bool Load(FILE* File);
	virtual bool SaveOnly(FILE* File) override;
	virtual bool LoadOnly(FILE* File) override;

};

