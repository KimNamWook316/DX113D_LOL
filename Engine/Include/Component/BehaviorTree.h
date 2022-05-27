#pragma once

#include "../GameInfo.h"


class CBehaviorTree
{
    friend class CStateComponent;

public:
    CBehaviorTree();
    CBehaviorTree(const CBehaviorTree& Tree);
    ~CBehaviorTree();

private:
    class CNode* m_Root;
    class CStateComponent* m_Owner;
    std::list<class CNode*> m_NodeList;

    class CAnimationMeshComponent* m_AnimationMeshComp;

public:
    void SetAnimationMeshComponent(class CAnimationMeshComponent* Mesh);
    class CAnimationMeshComponent* GetAnimationMeshComp()   const;

    class CNode* GetRootNode()    const;
    void SetRoot(class CNode* Node);
    CNode* FindNode(const std::string& Name);
    void AddNode(class CNode* Node);


public:
    bool Init();
    void Start();
    void Update(float DeltaTime);
    void PostUpdate(float DeltaTime);
    void PrevRender();
    void Render();
    void PostRender();
    CBehaviorTree* Clone();

   /* template <typename T>
    T* CreateNode(const std::string& Name)
    {
        T* Node = new T;

        Node->SetName(Name);

        if (m_Owner)
            Node->SetObject(m_Owner->GetGameObject());

        Node->SetOwner(this);

        if (!m_Root)
            m_Root = Node;

        m_NodeList.push_back(Node);

        return Node;
    }*/
};

