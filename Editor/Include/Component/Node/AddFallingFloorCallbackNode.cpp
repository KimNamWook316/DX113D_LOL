
#include "AddFallingFloorCallbackNode.h"
#include "Component/BehaviorTree.h"
#include "Component/ColliderBox3D.h"
#include "Component/StateComponent.h"

CAddFallingFloorCallbackNode::CAddFallingFloorCallbackNode()
{
}

CAddFallingFloorCallbackNode::CAddFallingFloorCallbackNode(const CAddFallingFloorCallbackNode& Node)	:
	CActionNode(Node)
{
}

CAddFallingFloorCallbackNode::~CAddFallingFloorCallbackNode()
{
}

NodeResult CAddFallingFloorCallbackNode::OnStart(float DeltaTime)
{

	m_CallStart = true;
	m_Owner->GetOwner()->SetTreeUpdate(false);

	CColliderBox3D* Box = m_Object->FindComponentFromType<CColliderBox3D>();

	if (Box)
	{
		Box->AddCollisionCallback<CAddFallingFloorCallbackNode>(Collision_State::Begin, this, &CAddFallingFloorCallbackNode::Trigger);
	}

	return NodeResult::Node_True;
}

NodeResult CAddFallingFloorCallbackNode::OnUpdate(float DeltaTime)
{
	return NodeResult();
}

NodeResult CAddFallingFloorCallbackNode::OnEnd(float DeltaTime)
{
	return NodeResult();
}

NodeResult CAddFallingFloorCallbackNode::Invoke(float DeltaTime)
{
	return CActionNode::Invoke(DeltaTime);
}

void CAddFallingFloorCallbackNode::Trigger(const CollisionResult& Result)
{

}
