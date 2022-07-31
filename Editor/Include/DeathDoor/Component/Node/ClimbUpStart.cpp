
#include "ClimbUpStart.h"
#include "../PlayerDataComponent.h"
#include "../GameStateComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "Scene/Scene.h"

CClimbUpStart::CClimbUpStart()
{
	SetTypeID(typeid(CClimbUpStart).hash_code());
}

CClimbUpStart::CClimbUpStart(const CClimbUpStart& Node) :
	CActionNode(Node)
{
}

CClimbUpStart::~CClimbUpStart()
{
}

NodeResult CClimbUpStart::OnStart(float DeltaTime)
{
	CPlayerDataComponent* DataComp = dynamic_cast<CPlayerDataComponent*>(dynamic_cast<CGameStateComponent*>(m_Owner->GetOwner())->GetData());

	DataComp->SetClimbingLadder(true);

	Vector3 PlayerPos = m_Object->GetWorldPos();

	CGameObject* Ladder = m_Object->GetScene()->FindObject("Ladder");

	if (Ladder)
	{
		// 플레이어가 사다리를 바라보도록 Rotation, 사다리 중앙에 오도록 x축 교정,  
		Vector3 LadderZAxis = Ladder->GetWorldAxis(AXIS_Z);
		LadderZAxis.z *= 1.f;

		Vector3 PlayerZAxis = m_Object->GetWorldAxis(AXIS_Z);

		float DotProduct = PlayerZAxis.Dot(LadderZAxis);
		float Degree = 0.f;
		if (DotProduct >= -0.99999999f && DotProduct <= 0.99999999f)
		{
			Degree = RadianToDegree(acosf(DotProduct));
			
			Vector3 CrossVec = PlayerZAxis.Cross(LadderZAxis);

			if (CrossVec.y < 0.f)
				Degree *= -1.f;

			m_Object->AddWorldRotationY(Degree);
		}

		Vector3 LadderPos = Ladder->GetWorldPos();
		m_Object->SetWorldPos(LadderPos.x - 0.3f, PlayerPos.y, LadderPos.z * 0.9f + PlayerPos.z * 0.1f);
	}


	m_AnimationMeshComp->GetAnimationInstance()->ChangeAnimation("PlayerLadderUp");

	return NodeResult::Node_True;
}

NodeResult CClimbUpStart::OnUpdate(float DeltaTime)
{
	return NodeResult::Node_True;
}

NodeResult CClimbUpStart::OnEnd(float DeltaTime)
{
	return NodeResult::Node_True;
}