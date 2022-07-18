
#include "FindPathNode.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Scene/Navigation3DManager.h"
#include "Component/BehaviorTree.h"	
#include "../MonsterPathFindCollider.h"	

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
	CScene* Scene = CSceneManager::GetInst()->GetScene();

	CGameObject* Player = Scene->GetPlayerObject();

	m_NavAgent = m_Object->FindObjectComponentFromType<CNavAgent>();

	if (m_NavAgent)
	{
		// 이미 길찾기해서 목표 지점으로 가고 있는 중이면 길찾기 X
		if (!m_NavAgent->IsEmptyPathList())
		{
			return NodeResult::Node_True;
		}

		// PathList에 경로는 채워지지 않았지만 길찾기 쓰레드가 돌아가고 있는중이라도 길찾기X
		if (m_NavAgent->GetPathFindStart())
			return NodeResult::Node_True;

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

