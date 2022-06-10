#pragma once

#include "State.h"

class CAirBorneState :
    public CState
{
    friend class CGameObject;
    friend class CStateManager;

protected:
    CAirBorneState();
    CAirBorneState(const CAirBorneState& State);
    virtual ~CAirBorneState();

public:
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CAirBorneState* Clone();

private:
    float m_AccTime;

public:
    virtual void OnStart();
    virtual void OnEnd();
};

