#pragma once

#include "ConditionNode.h"

// 플레이어가 피킹했을때 ray가 (지형이 아니라)특정 오브젝트(ex. 적 챔피언, 미니언)의 충돌체와 충돌하는지
class CCheckPickingObject :
    public CConditionNode
{
public:
    CCheckPickingObject();
    CCheckPickingObject(const CCheckPickingObject& Node);
    virtual ~CCheckPickingObject();

public:
    virtual NodeResult OnStart(float DeltaTime);
    virtual NodeResult OnUpdate(float DeltaTime);
    virtual NodeResult OnEnd(float DeltaTime);

public:
    virtual bool Invoke(float DeltaTime);
};

