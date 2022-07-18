#pragma once

#include "Component\Node\ConditionNode.h"

class CMeleeRangeCheckNode :
    public CConditionNode
{
protected:
    CMeleeRangeCheckNode();
    CMeleeRangeCheckNode(const CMeleeRangeCheckNode& Node);
    virtual ~CMeleeRangeCheckNode();

public:
    virtual NodeResult OnStart(float DeltaTime) override;
};

