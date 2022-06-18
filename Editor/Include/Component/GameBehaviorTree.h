#pragma once

#include "Component\BehaviorTree.h"
#include "GameInfo.h"

class CGameBehaviorTree :
    public CBehaviorTree
{
    friend class CStateComponent;

public:
    CGameBehaviorTree();
    CGameBehaviorTree(const CGameBehaviorTree& Tree);
    ~CGameBehaviorTree();

public:
    virtual bool Init();
    virtual void Start();

public:
    virtual CNode* LoadNode(CNode* Parent, size_t TypeID);
};

