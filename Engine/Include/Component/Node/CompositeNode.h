#pragma once

#include "Node.h"

class CCompositeNode :
    public CNode
{
protected:
    CCompositeNode();
    CCompositeNode(const CCompositeNode& Node);
    virtual ~CCompositeNode();

public:
    void AddChild(CNode* node)
    {
        node->SetParent(this);
        m_ChildrenList.emplace_back(node);
    }

    const std::list<CNode*>& GetChildrenList()
    {
        return m_ChildrenList;
    }

    void SetChildrenCallStart(bool Start)
    {
        auto iter = m_ChildrenList.begin();
        auto iterEnd = m_ChildrenList.end();

        for (; iter != iterEnd; ++iter)
        {
            (*iter)->SetCallStart(false);
        }
    }

private:
    std::list<CNode*> m_ChildrenList;

public:
    virtual NodeResult OnStart(float DeltaTime) = 0;
    virtual NodeResult OnUpdate(float DeltaTime) = 0;
    virtual NodeResult OnEnd(float DeltaTime) = 0;
};

