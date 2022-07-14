
#include "FindPathNode.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Scene/Navigation3DManager.h"
#include "Component/BehaviorTree.h"	

CFindPathNode::CFindPathNode()	:
	m_NavAgent(nullptr)
{
	SetTypeID(typeid(CFindPathNode).hash_code());
}

CFindPathNode::CFindPathNode(const CFindPathNode& Node)	:
	CActionNode(Node)
{
}

CFindPathNode::~CFindPathNode()
{
}

NodeResult CFindPathNode::OnStart(float DeltaTime)
{
	m_Owner->SetCurrentNode(this);

	CScene* Scene = CSceneManager::GetInst()->GetScene();

	CGameObject* Player = Scene->GetPlayerObject();

	m_NavAgent = m_Object->FindObjectComponentFromType<CNavAgent>();

	if(m_NavAgent)
		m_NavAgent->FindPath(m_Object->GetRootComponent(), Player->GetWorldPos());

	return NodeResult::Node_True;
}

NodeResult CFindPathNode::OnUpdate(float DeltaTime)
{

	return NodeResult::Node_True;
}

NodeResult CFindPathNode::OnEnd(float DeltaTime)
{
	return NodeResult::Node_True;
}

NodeResult CFindPathNode::Invoke(float DeltaTime)
{
	return CActionNode::Invoke(DeltaTime);
}

