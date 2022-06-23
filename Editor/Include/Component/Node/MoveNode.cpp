
#include "MoveNode.h"
#include "Component/AnimationMeshComponent.h"
#include "Animation/AnimationSequenceInstance.h"
#include "../GameBehaviorTree.h"
#include "../GameDataComponent.h"
#include "Input.h"

CMoveNode::CMoveNode()
{
	SetTypeID(typeid(CMoveNode).hash_code());
}

CMoveNode::CMoveNode(const CMoveNode& Node)	:
	CActionNode(Node)
{
}

CMoveNode::~CMoveNode()
{
}

NodeResult CMoveNode::OnStart(float DeltaTime)
{
	m_AnimationMeshComp = m_Owner->GetAnimationMeshComp();

	std::string ChampionName = m_Object->GetName();

	std::string SequenceName = ChampionName + "_" + "Run";

	if (m_AnimationMeshComp)
	{
		m_AnimationMeshComp->GetAnimationInstance()->ChangeAnimation(SequenceName);
	}

	//m_CallStart = true;

	return NodeResult::Node_True;
}

NodeResult CMoveNode::OnUpdate(float DeltaTime)
{
	CGameDataComponent* Comp = m_Object->FindComponentFromType<CGameDataComponent>();

	if (!Comp)
		return NodeResult::Node_False;

	const keyState WState = CInput::GetInst()->FindKeyState('W');
	const keyState AState = CInput::GetInst()->FindKeyState('A');
	const keyState SState = CInput::GetInst()->FindKeyState('S');
	const keyState DState = CInput::GetInst()->FindKeyState('D');

	float Speed = Comp->GetMoveSpeed();

	Vector3 ZAxis = m_Object->GetWorldAxis(AXIS::AXIS_Z);

	Vector3 FrontVector = Vector3(-ZAxis.x, -ZAxis.y, -ZAxis.z);

	Vector3 MoveDir;

	if (WState.State[0] || WState.State[1])
	{
		MoveDir += Vector3(0.f, 0.f, 1.f);
	}

	if (AState.State[0] || AState.State[1])
	{
		MoveDir += Vector3(-1.f, 0.f, 0.f);
	}

	if (SState.State[1])
	{
		MoveDir += Vector3(0.f, 0.f, -1.f);
	}

	if (DState.State[1])
	{
		MoveDir += Vector3(1.f, 0.f, 0.f);
	}

	MoveDir.Normalize();
	
	/// 벡터 외적을 할땐 오른손 좌표계 기준이라서 z에 -1을 곱해준다
	Vector3 CrossVector = Vector3(FrontVector.x, FrontVector.y, -FrontVector.z).Cross(Vector3(MoveDir.x, MoveDir.y, -MoveDir.z));

	bool Over180 = false;

	if (CrossVector.y < 0.f)
		Over180 = true;

	float Degree = FrontVector.Dot(MoveDir);
	Degree = RadianToDegree(acosf(Degree));

	// 180도 넘으면 반시계로 회전
	if (Over180)
	{
		if(Degree > 1.5f)
			m_Object->AddWorldRotationY(360.f * DeltaTime);
	}

	else
	{
		if (Degree > 1.5f)
			m_Object->AddWorldRotationY(-360.f * DeltaTime);
	}

	m_Object->AddWorldPos(MoveDir.x * Speed * DeltaTime, 0.f, MoveDir.z * Speed * DeltaTime);

	return NodeResult::Node_True;
}

NodeResult CMoveNode::OnEnd(float DeltaTime)
{
	return NodeResult::Node_True;
}

NodeResult CMoveNode::Invoke(float DeltaTime)
{
	return CActionNode::Invoke(DeltaTime);
}

