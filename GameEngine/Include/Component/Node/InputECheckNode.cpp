
#include "InputECheckNode.h"
#include "../../Input.h"

CInputECheckNode::CInputECheckNode()
{
	SetTypeID(typeid(CInputECheckNode).hash_code());
}

CInputECheckNode::CInputECheckNode(const CInputECheckNode& Node)
{
}

CInputECheckNode::~CInputECheckNode()
{
}

NodeResult CInputECheckNode::OnStart(float DeltaTime)
{
	const keyState State = CInput::GetInst()->FindKeyState('E');

	if (!State.State[KeyState_Up] && !State.State[KeyState_Down] && !State.State[KeyState_Push])
	{
		return NodeResult::Node_False;
	}

	else
	{
		return NodeResult::Node_True;
	}
}

NodeResult CInputECheckNode::OnUpdate(float DeltaTime)
{
	return NodeResult();
}

NodeResult CInputECheckNode::OnEnd(float DeltaTime)
{
	return NodeResult();
}

bool CInputECheckNode::Invoke(float DeltaTime)
{
	return CConditionNode::Invoke(DeltaTime);
}
