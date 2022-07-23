#pragma once

#include "MonsterDataComponent.h"

enum class Knight_Attack_Rot_Type
{
    Front,
    CW,
    CCW,
};

class CKnightDataComponent final :
    public CMonsterDataComponent 
{
    friend class CGameObject;

private:
    CKnightDataComponent();
    CKnightDataComponent(const CKnightDataComponent& com);
    virtual ~CKnightDataComponent();

public:
    virtual void Start() override;
	virtual void Update(float DeltaTime) override;

public:
    virtual void OnActiveMeleeAttackCollider() override;
    void OnLookPlayerMove(float DeltaTime);
    void OnEndAnimJudgeContinueAttack();
    void OnEnableLookAndMove();
    void OnDisableLookAndMove();
    void OnWalk(float DeltaTime);
    void OnPlayerEnterZone(const CollisionResult& Result);
    void OnEndCutScenePlaying();
    void OnStartJumpAttackMove();
    void OnEndJumpAttackMove();
    void OnEndJumpAttack();
    void OnEndContinueAttack();

public:
    int GetMeleeAttackCount() const
    {
        return m_MeleeAttackCount;
    }

    float GetJumpAttackRange() const
    {
        return m_JumpAttackRange;
    }

    bool IsContinueAttack() const
    {
        return m_ContinueAttack;
    }

    Knight_Attack_Rot_Type GetAttackRotationType() const
    {
        return m_AttackRot;
    }

    const Vector3& GetFinalTraceAxisZ()
    {
        return m_FinalTraceZAxis;
    }

    bool IsCutScenePlaying() const
    {
        return m_IsCutScenePlaying;
    }

    bool IsPlayerEnterZone() const
    {
        return m_PlayerEnterZone;
    }

public:
    void IncreaseMeleeAttackCount()
    {
        ++m_MeleeAttackCount;
    }

    void ResetMeleeAttackCount()
    {
        m_MeleeAttackCount = 0;
    }

    void SetContinueAttack(bool Continue)
    {
        m_ContinueAttack = Continue;
    }

    void SetAttackRot(Knight_Attack_Rot_Type Type)
    {
        m_AttackRot = Type;
    }

    void OnStartCutScene()
    {
        m_IsCutScenePlaying = true;
    }

protected:
    Knight_Attack_Rot_Type m_AttackRot;
    Vector3 m_FinalTraceZAxis;
    bool m_ContinueAttack;
    float m_JumpAttackRange;

    int m_MeleeAttackCount;

    bool m_PlayerEnterZone;
    class CColliderBox3D* m_PlayerEnterZoneTrigger;

    bool m_IsCutScenePlaying;
    class CCameraComponent* m_CutSceneCam;
};

