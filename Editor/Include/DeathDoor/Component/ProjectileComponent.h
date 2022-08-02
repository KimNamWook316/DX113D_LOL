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
	virtual void Reset() override;
    virtual void Update(float DeltaTime) override;
    virtual void PostUpdate(float DeltaTime) override;
    virtual void PrevRender() override;
    virtual void Render() override;
    virtual void PostRender() override;
    virtual CObjectComponent* Clone() override;

public:
	virtual bool Save(FILE* File) override;
	virtual bool Load(FILE* File) override;
	virtual bool SaveOnly(FILE* File) override;
	virtual bool LoadOnly(FILE* File) override;

public:
	void ShootByTargetPos(const Vector3& StartPos, float Speed, const Vector3& TargetPos, class CGameObject* EndParticleObj = nullptr);
	void ShootByLifeTime(const Vector3& StartPos, const Vector3& Dir, 
		float Speed, float LifeTime,
		class CGameObject* EndParticleObj = nullptr);
	void ShootByGravityTargetPos(const Vector3& StartPos, const Vector3& XZDir, float Angle,
		const Vector3& TargetPos, class CGameObject* EndParticleObj = nullptr);

	void ClearCollsionCallBack();
	bool CheckDestroy();
	void OnEnd();

public:
	template <typename T>
	void ShootByLifeTimeCollision(T* Obj, void(T::* CollisionCallBack)(const CollisionResult&), Collision_State CollisionState,
		const Vector3& StartPos, const Vector3& Dir,
		float Speed, float LifeTime,
		class CGameObject* EndParticleObj = nullptr)
	{
		if (m_Collider && CollisionCallBack)
		{
			m_Collider->AddCollisionCallback(CollisionState, Obj, CollisionCallBack);
		}

		ShootByLifeTime(StartPos, Dir, Speed, LifeTime, EndParticleObj);
	}

	template <typename T>
	void Shoot(T* Obj, void(T::* CollisionCallBack)(const CollisionResult&), Collision_State CollsionState,
		const Vector3& StartPos, const Vector3& Dir, float Speed,
		const Vector3& TargetPos, bool Gravity = false, class CGameObject* EndParticleObj = nullptr)
	{
		if (m_Collider && CollisionCallBack)
		{
			m_Collider->AddCollisionCallback(CollsionState, Obj, CollisionCallBack);
		}

		Shoot(StartPos, Dir, Speed, TargetPos, Gravity, EndParticleObj);
	}

	template <typename T>
	void Shoot(T* Obj, void(T::* CollisionCallBack)(const CollisionResult&), Collision_State CollsionState,
		const Vector3& StartPos, const Vector3& Dir, float Speed,
		float LifeTime, class CGameObject* EndParticleObj = nullptr)
	{
		if (m_Collider && CollisionCallBack)
		{
			m_Collider->AddCollisionCallback(CollsionState, Obj, CollisionCallBack);
		}

		Shoot(StartPos, Dir, Speed, LifeTime, EndParticleObj);
	}

	template <typename T>
	void SetEndCallBack(T* Obj, void(T::* Func)(const Vector3&))
	{
		m_EndCallBack = std::bind(Func, Obj, std::placeholders::_1);
	}

private:
	class CSceneComponent* m_Root;
	class CColliderComponent* m_Collider;
	class CGameObject* m_EndParticleObject;
	Vector3 m_Dir;
	float	m_Speed;
	Vector3 m_StartPos;
	Vector3 m_TargetPos;
	bool	m_IsShoot;

	bool	m_IsGravity;
	float	m_VelocityXZ;
	float	m_VelocityY;

	float	m_LifeTime;
	float	m_LifeTimer;
	bool	m_Destroy;
	bool	m_NoUpdate;

	std::function<void(const Vector3&)> m_EndCallBack;

public:
	void SetDestroy(bool NoDestroy)
	{
		m_Destroy = NoDestroy;
	}

	bool GetDestroy()	const
	{
		return m_Destroy;
	}

	void SetNoUpdate(bool Update)
	{
		m_NoUpdate = Update;
	}

	bool GetNoUpdate()	const
	{
		return m_NoUpdate;
	}

	void SetLifeTimer(float Timer)
	{
		m_LifeTimer = Timer;
	}
};

