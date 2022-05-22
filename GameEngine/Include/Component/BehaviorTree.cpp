

#include "BehaviorTree.h"
#include "Node/Node.h"
#include "Node/ActionNode.h"

CBehaviorTree::CBehaviorTree() :
	m_Root(nullptr)
{

}

CBehaviorTree::CBehaviorTree(const CBehaviorTree& Tree)
{
}

CBehaviorTree::~CBehaviorTree()
{
	auto iter = m_NodeList.begin();
	auto iterEnd = m_NodeList.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(*iter);
	}
}

void CBehaviorTree::SetAnimationMeshComponent(CAnimationMeshComponent* Mesh)
{
	m_AnimationMeshComp = Mesh;
}

CAnimationMeshComponent* CBehaviorTree::GetAnimationMeshComp() const
{
	return m_AnimationMeshComp;
}

CNode* CBehaviorTree::GetRootNode() const
{
	return m_Root;
}

void CBehaviorTree::SetRoot(class CNode* Node)
{
	m_Root = Node;
}

CNode* CBehaviorTree::FindNode(const std::string& Name)
{
	auto iter = m_NodeList.begin();
	auto iterEnd = m_NodeList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetName() == Name)
			return (*iter);
	}

	return nullptr;
}

void CBehaviorTree::AddNode(CNode* Node)
{
	m_NodeList.push_back(Node);
}

void CBehaviorTree::Start()
{

}

bool CBehaviorTree::Init()
{
	return true;
}

void CBehaviorTree::Update(float DeltaTime)
{

}

void CBehaviorTree::PostUpdate(float DeltaTime)
{
	if (m_Root)
	{
		m_Root->Invoke(DeltaTime);
	}
}

void CBehaviorTree::PrevRender()
{

}

void CBehaviorTree::Render()
{
}

void CBehaviorTree::PostRender()
{
}

CBehaviorTree* CBehaviorTree::Clone()
{
	return new CBehaviorTree(*this);
}
