
#include "RollInputCheck.h"
#include "Input.h"

CRollInputCheck::CRollInputCheck()
{
	SetTypeID(typeid(CRollInputCheck).hash_code());
}

CRollInputCheck::CRollInputCheck(const CRollInputCheck& Node)	:
	CConditionNode(Node)
{
}

CRollInputCheck::~CRollInputCheck()
{
}

NodeResult CRollInputCheck::OnStart(float DeltaTime)
{
	const keyState SpaceState = CInput::GetInst()->FindKeyState(VK_SPACE);

	if (SpaceState.State[KeyState_Down] || SpaceState.State[KeyState_Push])
	{
		return NodeResult::Node_True;
	}

	else
	{
		return NodeResult::Node_False;
	}

}

NodeResult CRollInputCheck::OnUpdate(float DeltaTime)
{
	return NodeResult();
}

NodeResult CRollInputCheck::OnEnd(float DeltaTime)
{
	return NodeResult();
}
