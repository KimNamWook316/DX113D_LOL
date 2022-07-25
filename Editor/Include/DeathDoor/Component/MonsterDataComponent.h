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
	void RightLookPlayer(float DeltaTime);
	void LeftLookPlayer(float DeltaTime);
	void MoveZ(float DeltaTime);
	void ChangeColorBossDeath(float DeltaTime);	// 죽을 때 색상 변화가 필요한 보스나 몬스터의 경우 이 함수를 Update에서 호출

public:
    virtual void OnActiveMeleeAttackCollider();
    virtual void OnInActiveMeleeAttackCollider();
    virtual void OnHitMeleeAttack(const CollisionResult& Result);
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
    virtual void OnDeadPaperBurnEnd();			// PaperBurn 종료시 호출
    virtual void OnDeadAnimStart();				// Death Animation 시작시 호출
    virtual void OnDeadAnimEnd();				// Death Animation 종료시 호출
    virtual void OnPlayerEnterZone(const CollisionResult& Result);		// Boss Cutscene Trigger
	virtual void OnStartCutScene();
    virtual void OnEndCutScene();

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

	void SetPlayerEnterZone(bool Enter)
	{
		m_PlayerEnterZone = Enter;
	}

	void SetPlayerEnterZoneFalse()
	{
		m_PlayerEnterZone = false;
	}

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

    class CColliderBox3D* GetMeleeAttackCollider() const
    {
        return m_MeleeAttackCollider;
    }

	CColliderBox3D* GetPlayerEnterZoneTrigger()	const
	{
		return m_PlayerEnterZoneTrigger;
	}

	bool IsPostAttackDelaying() const
	{
		return m_PostAttackDelaying;
	}

	bool IsCombat() const
	{
		return m_IsCombat;
	}

	bool IsCutScenePlaying() const
	{
		return m_IsCutScenePlaying;
	}

	bool IsPlayerEnterZone() const
	{
		return m_PlayerEnterZone;
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

protected:
	void ActiveHitEffect(float DeltaTime);
	void ChangeHitColor(int EffectNum);

protected:
	class CMonsterNavAgent* m_MonsterNavAgent;
	class CAnimationMeshComponent* m_AnimMesh;
	class CColliderBox3D* m_HitBox;
	class CColliderBox3D* m_MeleeAttackCollider;
	class CGameStateComponent* m_State;

	bool m_PostAttackDelaying;  // 공격 후딜레이 중인지 여부
	bool m_IsCombat;			// 전투 시작 여부

	// Hit Effect 관련 변수
	bool m_HitEffectStart;
	float m_HitEffectTimer;
	float m_HitEffectMax;
	int m_HitEffectFlag;

	size_t m_MeshMatSize;
	std::vector<Vector4> m_vecOriginDiffuse;
	std::vector<Vector4> m_vecOriginSpecluar;
	std::vector<Vector4> m_vecOriginAmbient;
	std::vector<Vector4> m_vecOriginEmissive;

	// Move 관련 변수
	float m_CurMoveSpeed;
	bool m_LookPlayer;
	bool m_MoveZ;
	// 왼쪽으로만 무조건 돌아야 하는 순간
	bool m_LeftLookPlayer;
	// 오른쪽으로만 무조건 돌아야 하는 순간
	bool m_RightLookPlayer;

	// Death Effect 관련 변수
    class CPaperBurnComponent* m_PaperBurn;
    bool m_DeathColorChangeStart;
    float m_DeathTimer;
    float m_DeathColorChangeTimeMax;
	Vector4 m_DeathColorStart;
	Vector4	m_DeathColorEnd;

	// CutScene관련 변수
    bool m_PlayerEnterZone;
    class CColliderBox3D* m_PlayerEnterZoneTrigger;

    bool m_IsCutScenePlaying;
    class CCameraComponent* m_CutSceneCam;
    Vector3 m_CutSceneBeforePlayerPos;

public:
	class CMonsterNavAgent* GetMonsterNavAgent()	const;
};

