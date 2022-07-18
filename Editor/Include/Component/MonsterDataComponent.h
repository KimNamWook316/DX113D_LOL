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
    void OnEndAnimPostAttackDelayOn();
	void OnEndAnimPostAttackDelayOff();
    void OnLookPlayer(float DeltaTime);

public:
	virtual void SetIsHit(bool Hit) override;

	void SetPostAttckDelaying(bool Enable)
	{
		m_PostAttackDelaying = Enable;
	}

public:
	float GetRotateSpeed() const
	{
		return m_Data.RotateSpeedPerSec;
	}

	bool IsPostAttackDelaying() const
	{
		return m_PostAttackDelaying;
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
	class CAnimationMeshComponent* m_AnimMesh;
	class CColliderBox3D* m_HitBox;

	bool m_PostAttackDelaying; // 공격 후딜레이 중인지 여부

	bool m_HitEffectStart;
	float m_HitEffectTimer;
	float m_HitEffectMax;
	int m_HitEffectFlag;

	size_t m_MeshMatSize;
	std::vector<Vector4> m_vecOriginDiffuse;
	std::vector<Vector4> m_vecOriginSpecluar;
	std::vector<Vector4> m_vecOriginAmbient;
	std::vector<Vector4> m_vecOriginEmissive;
};

