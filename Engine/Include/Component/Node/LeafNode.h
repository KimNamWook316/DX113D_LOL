#pragma once

#include "Node.h"

class CLeafNode :
    public CNode
{
public:
    CLeafNode();
    CLeafNode(const CLeafNode& Node);
    virtual ~CLeafNode() = 0;


protected:
    virtual NodeResult OnStart(float DeltaTime) = 0;
    virtual NodeResult OnUpdate(float DeltaTime) = 0;
    virtual NodeResult OnEnd(float DeltaTime) = 0;

public:
    virtual bool Invoke(float DeltaTime) override;

protected:
    // Start를 최초로 한번 이미 호출 했는지
    bool m_CallStart;
};

