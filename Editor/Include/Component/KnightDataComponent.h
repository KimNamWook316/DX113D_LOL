#pragma once

#include "ObjectDataComponent.h"

class CKnightDataComponent final :
    public CObjectDataComponent
{
    friend class CGameObject;

private:
    CKnightDataComponent();
    CKnightDataComponent(const CKnightDataComponent& com);
    virtual ~CKnightDataComponent();

public:
    int GetMeleeAttackCount() const
    {
        return m_MeleeAttackCount;
    }

    float GetJumpAttackRange() const
    {
        return m_JumpAttackRange;
    }

protected:
    int m_MeleeAttackCount;
    float m_JumpAttackRange;
};

