
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

	if (m_NavAgent)
	{
		CSceneComponent* Root = m_Object->GetRootComponent();
		Vector3 PlayerPos = Player->GetWorldPos();
		std::vector<Vector3> vecPath;
		bool Straight = m_NavAgent->CheckStraightPath(Root->GetWorldPos(), PlayerPos, vecPath);

		if (Straight)
		{
			m_NavAgent->AddPath(PlayerPos);
		}

		else
			m_NavAgent->FindPath(Root, PlayerPos);
	}

	m_CallStart = true;

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

