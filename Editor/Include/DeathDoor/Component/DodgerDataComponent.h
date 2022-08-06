#pragma once

#include "MonsterDataComponent.h"

class CDodgerDataComponent :
    public CMonsterDataComponent
{
    friend class CGameObject;

private:
    CDodgerDataComponent();
    CDodgerDataComponent(const CDodgerDataComponent& com);
    virtual ~CDodgerDataComponent();

public:
	virtual void Start() override;
    virtual void Update(float DeltaTime) override;

public:
    void OnAttack1ReadyEnd();
    void OnAttack2ReadyEnd();
    void OnAttackMoveStart();
    void OnAttackMove1End();
    void OnAttackMove2End();
    void OnAttackHit();
    void OnDashReady();
    void OnDashStart();
    void OnDashEnd();
    void OnPostAttackStart();
    void OnPostAttackEnd();

public:
    void OnIncreaseAttackCount()
    {
        m_AttackCount;
    }

    void OnResetAttackCount()
    {
        m_AttackCount = 0;
    }

    int GetAttackCount() const
    {
        return m_AttackCount;
    }

    bool IsDashReady() const
    {
        return m_DashReady;
    }

private:
    int m_AttackCount;
    bool m_DashRotaiting;
    bool m_DashReady;
    bool m_Dash;
    Vector3 m_PrevPlayerPos;
    Vector3 m_DashDest;
    Vector3 m_DashMovePerSec;
};

