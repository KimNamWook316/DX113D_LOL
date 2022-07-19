#pragma once

#include "Component/Node/ConditionNode.h"

class CBossKnightCutScenePlayCheck :
    public CConditionNode
{
    friend class CGameBehaviorTree;

protected:
    CBossKnightCutScenePlayCheck();
    CBossKnightCutScenePlayCheck(const CBossKnightCutScenePlayCheck& Node);
    virtual ~CBossKnightCutScenePlayCheck();

public:
    virtual NodeResult OnStart(float DeltaTime) override;
    virtual NodeResult OnUpdate(float DeltaTime) override;
    virtual NodeResult OnEnd(float DeltaTime) override;
};

