
#include "CancleShootNode.h"
#include "Component/AnimationMeshComponent.h"
#include "Animation/AnimationSequenceInstance.h"
#include "Component/BehaviorTree.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"

CCancleShootNode::CCancleShootNode()
{
	SetTypeID(typeid(CCancleShootNode).hash_code());
}

CCancleShootNode::CCancleShootNode(const CCancleShootNode& Node) :
	CActionNode(Node)
{
}

CCancleShootNode::~CCancleShootNode()
{
}

NodeResult CCancleShootNode::OnStart(float DeltaTime)
{
	return NodeResult::Node_True;
}

NodeResult CCancleShootNode::OnUpdate(float DeltaTime)
{
	CScene* Scene = CSceneManager::GetInst()->GetScene();

	bool RestoreEnd = Scene->RestoreCamera(50.f, DeltaTime);

	if (RestoreEnd)
	{
		m_Object->SetNoInterrupt(false);

		m_IsEnd = true;
		m_CallStart = false;
		m_Owner->SetCurrentNode(nullptr);

		return NodeResult::Node_True;
	}

	else
	{
		m_Owner->SetCurrentNode(this);
		return NodeResult::Node_Running;
	}

	return NodeResult::Node_True;
}

NodeResult CCancleShootNode::OnEnd(float DeltaTime)
{
	return NodeResult::Node_True;
}

NodeResult CCancleShootNode::Invoke(float DeltaTime)
{
	return CActionNode::Invoke(DeltaTime);
}

