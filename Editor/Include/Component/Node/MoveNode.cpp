
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

	m_CallStart = true;

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
	Vector3 Rot = m_Object->GetWorldRot();

	// 모든 각도를 0 ~ 360 사이로 만든다
	while (Rot.y > 360.f)
		Rot.y -= 360.f;
	while (Rot.y < 0.f)
		Rot.y += 360.f;

	Vector3 XAxis = m_Object->GetWorldAxis(AXIS::AXIS_X);
	Vector3 ZAxis = m_Object->GetWorldAxis(AXIS::AXIS_Z);
	
	Vector3 FrontVector = Vector3(-ZAxis.x, -ZAxis.y, -ZAxis.z);

	if (WState.State[1])
	{
		// 0 ~ 180도 사이에 존재한다면 시계 방향 회전
		if (Rot.y > 0 && Rot.y < 180.f)
		{
			if (Rot.y - 180.f >= -1.5f)
				m_Object->AddWorldRotationY(90.f * DeltaTime);
		}

		// 180 ~ 360도 사이에 존재한다면 반시계 방향 회전
		else
		{
			if (Rot.y - 180.f <= 1.5f)
				m_Object->AddWorldRotationY(-90.f * DeltaTime);
		}

		m_Object->AddWorldPos(0.f, 0.f, Speed * DeltaTime);
	}

	if (AState.State[1])
	{
		// 0 ~ 180도 사이
		float Rotation = Vector3(-1.f, 0.f, 0.f).Dot(FrontVector);

		if (FrontVector.z < 0.f)
		{
			if (Rotation < 1.5f)
				m_Object->AddWorldRotationY(180.f * DeltaTime);
		}

		else
		{
			if (Rotation < 1.5f)
				m_Object->AddWorldRotationY(-180.f * DeltaTime);
		}

		m_Object->AddWorldPos(-Speed * DeltaTime, 0.f, 0.f);
	}

	if (SState.State[1])
	{
		// 0 ~ 180도 사이에 존재한다면 반시계 방향 회전
		if (Rot.y > 0 && Rot.y < 180.f)
		{
			if (Rot.y - 180.f >= -1.5f)
				m_Object->AddWorldRotationY(-90.f * DeltaTime);
		}

		// 180 ~ 360도 사이에 존재한다면 시계 방향 회전
		else
		{
			if (Rot.y - 180.f <= 1.5f)
				m_Object->AddWorldRotationY(90.f * DeltaTime);
		}

		m_Object->AddWorldPos(0.f, 0.f, -Speed * DeltaTime);
	}

	if (DState.State[1])
	{
		// 0 ~ 180도 사이
		float Rotation = Vector3(1.f, 0.f, 0.f).Dot(FrontVector);

		if (FrontVector.z < 0.f)
		{
			if (Rotation < 1.5f)
				m_Object->AddWorldRotationY(-180.f * DeltaTime);
		}

		else
		{
			if (Rotation < 1.5f)
				m_Object->AddWorldRotationY(180.f * DeltaTime);
		}

		m_Object->AddWorldPos(Speed * DeltaTime, 0.f, 0.f);
	}


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
