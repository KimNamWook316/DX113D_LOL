#pragma once

#include "Component/Node/ConditionNode.h"

class CFindEnemyChampion :
    public CConditionNode
{
public:
    CFindEnemyChampion();
    CFindEnemyChampion(const CFindEnemyChampion& Node);
    virtual ~CFindEnemyChampion();

public:
    virtual NodeResult OnStart(float DeltaTime);
    virtual NodeResult OnUpdate(float DeltaTime);
    virtual NodeResult OnEnd(float DeltaTime);

public:
    virtual NodeResult Invoke(float DeltaTime);
};

