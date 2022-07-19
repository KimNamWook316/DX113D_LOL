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

public:
    void OnActiveMeleeAttackCollider();
    void OnInActiveMeleeAttackCollider();
    void OnHitMeleeAttack(const CollisionResult& Result);
    void OnLookPlayerMove(float DeltaTime);
    void OnEndAnimJudgeContinueAttack();
    void OnWalk(float DeltaTime);
    void OnPlayerEnterZone(const CollisionResult& Result);

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

    class CColliderBox3D* GetMeleeAttackCollider() const
    {
        return m_MeleeAttackCollider;
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

    void OnEndCutScenePlaying()
    {
        m_IsCutScenePlaying = false;
    }

protected:
    Knight_Attack_Rot_Type m_AttackRot;
    Vector3 m_FinalTraceZAxis;
    bool m_ContinueAttack;
    float m_JumpAttackRange;

    int m_MeleeAttackCount;
    class CColliderBox3D* m_MeleeAttackCollider;

    bool m_PlayerEnterZone;
    class CColliderBox3D* m_PlayerEnterZoneTrigger;

    bool m_IsCutScenePlaying;
};

