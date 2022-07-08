#pragma once

#include "Component/SceneComponent.h"

class CEyeLaserComponent :
    public CSceneComponent
{
	friend class CGameObject;

protected:
	CEyeLaserComponent();
	CEyeLaserComponent(const CEyeLaserComponent& com);
	virtual ~CEyeLaserComponent();

private:
	// TriggerBox를 플레이어가 타격한 횟수 -> 1개만 하면 EyeLaser깨어남, 4개 모두 부시면 EyeLaser도 파괴
	int m_TriggerHitCount;
	class CGameObject* m_Player;
	class CAnimationMeshComponent* m_AnimComp;
	bool	m_WakeEnd;
	Vector3 m_CurrentLaserDir;
	Matrix m_LaserRotMatrix;

	CSharedPtr<CShader>	m_Shader;
	CSharedPtr<CMesh> m_LaserPlaneMesh1;

public:
	void SetWakeEnd();

	void AddTriggerCount()
	{
		++m_TriggerHitCount;
	}

	int GetTriggerCount() const
	{
		return m_TriggerHitCount;
	}

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void PrevRender();
	virtual void Render();
	virtual void PostRender();
	virtual CEyeLaserComponent* Clone();


public:
	virtual bool Save(FILE* File);
	virtual bool Load(FILE* File);
	virtual bool SaveOnly(FILE* File) override;
	virtual bool LoadOnly(FILE* File) override;

private:
	void TrackPlayer(float DeltaTime);
	void ChangeToWakeAnimation();
	void ChangeToDieAnimation();
	void ChangeToIdleAnimation();
};

