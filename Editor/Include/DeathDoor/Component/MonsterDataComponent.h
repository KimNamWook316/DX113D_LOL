#pragma once

#include "ObjectDataComponent.h"

#define HIT_EFFECT_FIRST	0x1
#define HIT_EFFECT_SECOND	0x2
#define HIT_EFFECT_THIRD	0x4
#define HIT_EFFECT_FOURTH	0x8
#define HIT_EFFECT_FIFTH	0xF

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
	void RightLookPlayer(float DeltaTime);
	void LeftLookPlayer(float DeltaTime);
	void MoveZ(float DeltaTime);

public:
    void OnEndAnimPostAttackDelayOn();
	void OnEndAnimPostAttackDelayOff();
    void OnEnableLookPlayer();
	void OnDisableLookPlayer();
	void OnEnableRightLookPlayer();
	void OnDisableRightLookPlayer();
	void OnEnableLeftLookPlayer();
	void OnDisableLeftLookPlayer();
	void OnEnableMoveZ();
	void OnDisableMoveZ();
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

	void SetCurMoveSpeed(float Speed)
	{
		m_CurMoveSpeed = Speed;
	}

	void SetCurrentNodeNull();

public:
	float GetRotateSpeed() const
	{
		return m_Data.RotateSpeedPerSec;
	}
	float GetMeleeAttackRange() const
	{
		return m_Data.MeleeAttackRange;
	}
	float GetCurMoveSpeed() const
	{
		return m_CurMoveSpeed;
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
	float DistToPlayer();
		
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
	// 왼쪽으로만 무조건 돌아야 하는 순간
	bool m_LeftLookPlayer;
	// 오른쪽으로만 무조건 돌아야 하는 순간
	bool m_RightLookPlayer;

public:
	class CMonsterNavAgent* GetMonsterNavAgent()	const;
};

