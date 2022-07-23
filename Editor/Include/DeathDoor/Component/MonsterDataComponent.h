#pragma once

#include "ObjectDataComponent.h"

#define HIT_EFFECT_FIRST	0x01
#define HIT_EFFECT_SECOND	0x02
#define HIT_EFFECT_THIRD	0x04
#define HIT_EFFECT_FOURTH	0x08
#define HIT_EFFECT_FIFTH	0x10

class CMonsterDataComponent :
    public CObjectDataComponent
{
	friend class CGameObject;

protected:
	CMonsterDataComponent();
	CMonsterDataComponent(const CMonsterDataComponent& com);
	virtual ~CMonsterDataComponent();

public:
	virtual void Start() override;
	virtual void Update(float DeltaTime) override;
	virtual CMonsterDataComponent* Clone();

public:
	void LookPlayer(float DeltaTime);
	void MoveZ(float DeltaTime);

public:
    virtual void OnActiveMeleeAttackCollider();
    virtual void OnInActiveMeleeAttackCollider();
    virtual void OnHitMeleeAttack(const CollisionResult& Result);
    void OnEndAnimPostAttackDelayOn();
	void OnEndAnimPostAttackDelayOff();
    void OnEnableLookPlayer();
	void OnDisableLookPlayer();
	void OnSetMoveSpeedZero(float DeltaTime)
	{
		m_CurMoveSpeed = 0.f;
	}

public:
	virtual void SetIsHit(bool Hit) override;

	void SetPostAttckDelaying(bool Enable)
	{
		m_PostAttackDelaying = Enable;
	}

	void OnCombatStart()
	{
		m_IsCombat = true;
	}

	void SetCurrentNodeNull();

public:
	float GetRotateSpeed() const
	{
		return m_Data.RotateSpeedPerSec;
	}

    class CColliderBox3D* GetMeleeAttackCollider() const
    {
        return m_MeleeAttackCollider;
    }

	bool IsPostAttackDelaying() const
	{
		return m_PostAttackDelaying;
	}

	bool IsCombat() const
	{
		return m_IsCombat;
	}

	float GetAnglePlayer();
	bool IsPlayerLeftBasedInLookDir(); // 플레이어가 몬스터가 보는 방향 기준 왼쪽에 있는지 판별
	bool IsPlayerInMeleeAttackRange();
	Vector3	ToPlayer();

public:
	virtual bool Save(FILE* File);
	virtual bool Load(FILE* File);
	virtual bool SaveOnly(FILE* File) override;
	virtual bool LoadOnly(FILE* File) override;

private:
	void ActiveHitEffect(float DeltaTime);
	void ChangeHitColor(int EffectNum);

protected:
	class CMonsterNavAgent* m_MonsterNavAgent;
	class CAnimationMeshComponent* m_AnimMesh;
	class CColliderBox3D* m_HitBox;
	class CColliderBox3D* m_MeleeAttackCollider;
	class CGameStateComponent* m_State;

	bool m_PostAttackDelaying; // 공격 후딜레이 중인지 여부
	bool m_IsCombat;

	bool m_HitEffectStart;
	float m_HitEffectTimer;
	float m_HitEffectMax;
	int m_HitEffectFlag;

	size_t m_MeshMatSize;
	std::vector<Vector4> m_vecOriginDiffuse;
	std::vector<Vector4> m_vecOriginSpecluar;
	std::vector<Vector4> m_vecOriginAmbient;
	std::vector<Vector4> m_vecOriginEmissive;

	float m_CurMoveSpeed;
	bool m_LookPlayer;
	bool m_MoveZ;

public:
	class CMonsterNavAgent* GetMonsterNavAgent()	const;
};

