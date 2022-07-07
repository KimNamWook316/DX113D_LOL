#pragma once

#include "Component/Node/ConditionNode.h"
class CTriggerBoxHitCheck :
    public CConditionNode
{
public:
    CTriggerBoxHitCheck();
    CTriggerBoxHitCheck(const CTriggerBoxHitCheck& Node);
    virtual ~CTriggerBoxHitCheck();

public:
    virtual NodeResult OnStart(float DeltaTime);
    virtual NodeResult OnUpdate(float DeltaTime);
    virtual NodeResult OnEnd(float DeltaTime);

private:
    class CGameObject* m_TriggerTarget;


public:
    virtual NodeResult Invoke(float DeltaTime);
};

