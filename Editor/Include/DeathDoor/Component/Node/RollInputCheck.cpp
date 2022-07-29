
#include "RollInputCheck.h"
#include "Input.h"

CRollInputCheck::CRollInputCheck()	:
	m_FrameCount(0)
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

	++m_FrameCount;

	if (m_FrameCount >= 10 && SpaceState.State[KeyState_Down] || SpaceState.State[KeyState_Push])
	{
		m_FrameCount = 0;
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
