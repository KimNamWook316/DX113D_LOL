#pragma once

#include "Component/Node/ConditionNode.h"

// 스킬 사용중인지(그래서 다른 상태로 전환되면 안되는 상태인지 체크)
class CInSkillCheck :
    public CConditionNode
{
public:
    CInSkillCheck();
    CInSkillCheck(const CInSkillCheck& Node);
    virtual ~CInSkillCheck();

public:
    virtual NodeResult OnStart(float DeltaTime);
    virtual NodeResult OnUpdate(float DeltaTime);
    virtual NodeResult OnEnd(float DeltaTime);

public:
    virtual NodeResult Invoke(float DeltaTime);
};

