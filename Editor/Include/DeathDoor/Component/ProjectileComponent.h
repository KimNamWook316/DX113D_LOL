#pragma once

#include "Component/ObjectComponent.h"
#include "Component/ColliderComponent.h"

class CProjectileComponent :
    public CObjectComponent
{
	friend class CGameObject;

protected:
	CProjectileComponent();
	CProjectileComponent(const CProjectileComponent& com);
	virtual ~CProjectileComponent();

public:
    virtual bool Init() override;
	virtual void Start() override;
    virtual void Update(float DeltaTime) override;
    virtual void PostUpdate(float DeltaTime) override;
    virtual void PrevRender() override;
	virtual void Reset() override;
    virtual void Render() override;
    virtual void PostRender() override;
    virtual CObjectComponent* Clone() override;

public:
	virtual bool Save(FILE* File) override;
	virtual bool Load(FILE* File) override;
	virtual bool SaveOnly(FILE* File) override;
	virtual bool LoadOnly(FILE* File) override;

public:
	void Shoot(const Vector3& StartPos, const Vector3& Dir, 
		float Speed, const Vector3& TargetPos = Vector3::Zero, bool Gravity = false);

public:
	template <typename T>
	void Shoot(T* Obj, void(T::* CollisionCallBack)(const CollisionResult&), Collision_State CollsionState,
		const Vector3& StartPos, const Vector3& Dir, float Speed, 
		const Vector3& TargetPos = Vector3::Zero, bool Gravity = false)
	{
		if (m_Collider && CollisionCallBack)
		{
			m_Collider->AddCollisionCallback(CollsionState, Obj, CollisionCallBack);
		}

		Shoot(StartPos, Dir, Speed, TargetPos, Gravity);
	}

private:
	class CSceneComponent* m_Root;
	class CColliderComponent* m_Collider;
	class CParticleComponent* m_EndParticle;
	Vector3 m_Dir;
	float	m_Speed;
	Vector3 m_StartPos;
	Vector3 m_TargetPos;
	bool	m_IsGravity;
	bool	m_IsShoot;
	float	m_Timer;
};

