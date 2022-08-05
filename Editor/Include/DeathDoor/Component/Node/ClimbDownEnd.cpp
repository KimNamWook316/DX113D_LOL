
#include "ClimbDownEnd.h"
#include "../PlayerDataComponent.h"
#include "../GameStateComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "Scene/Scene.h"
#include "Scene/Navigation3DManager.h"

CClimbDownEnd::CClimbDownEnd()
{
	SetTypeID(typeid(CClimbDownEnd).hash_code());
}

CClimbDownEnd::CClimbDownEnd(const CClimbDownEnd& Node) :
	CActionNode(Node)
{
}

CClimbDownEnd::~CClimbDownEnd()
{
}

NodeResult CClimbDownEnd::OnStart(float DeltaTime)
{
	CPlayerDataComponent* DataComp = dynamic_cast<CPlayerDataComponent*>(dynamic_cast<CGameStateComponent*>(m_Owner->GetOwner())->GetData());

	DataComp->SetClimbingLadder(false);

	//m_AnimationMeshComp->GetAnimationInstance()->ChangeAnimation("PlayerLadderFinish");

	Vector3 PlayerPos = m_Object->GetWorldPos();
	Vector3 PlayerZAxis = m_Object->GetWorldAxis(AXIS_Z);

	float Height = 0.f;
	int PolyIndex = 0;

	bool Result1 = m_Object->GetScene()->GetNavigation3DManager()->CheckNavMeshPoly(PlayerPos, Height, PolyIndex);
	if (Result1)
	{
		m_Object->GetScene()->GetNavigation3DManager()->RefreshPlayerNavMeshPoly(PlayerPos);
		DataComp->SetLadderDownEnable(false);
		DataComp->SetClimbingLadder(false);

		return NodeResult::Node_True;
	}

	PlayerPos = Vector3(PlayerPos.x, PlayerPos.y, PlayerPos.z + PlayerZAxis.z);

	bool Result2 = m_Object->GetScene()->GetNavigation3DManager()->CheckNavMeshPoly(PlayerPos, Height, PolyIndex);

	if (Result2)
	{
		m_Object->GetScene()->GetNavigation3DManager()->RefreshPlayerNavMeshPoly(PlayerPos);

		DataComp->SetLadderDownEnable(false);
		DataComp->SetClimbingLadder(false);

		return NodeResult::Node_True;
	}

	PlayerPos = Vector3(PlayerPos.x, PlayerPos.y, PlayerPos.z + PlayerZAxis.z);

	bool Result3 = m_Object->GetScene()->GetNavigation3DManager()->CheckNavMeshPoly(PlayerPos, Height, PolyIndex);

	if (Result3)
	{
		m_Object->GetScene()->GetNavigation3DManager()->RefreshPlayerNavMeshPoly(PlayerPos);

		DataComp->SetLadderDownEnable(false);
		DataComp->SetClimbingLadder(false);

		return NodeResult::Node_True;
	}


	else
	{
		// TODO: 사다리를 다 내려가고 발에 닿은 평지에 네비메쉬가 없다면
		assert(false);
	}

	return NodeResult::Node_True;
}

NodeResult CClimbDownEnd::OnUpdate(float DeltaTime)
{
	return NodeResult::Node_True;
}

NodeResult CClimbDownEnd::OnEnd(float DeltaTime)
{
	return NodeResult::Node_True;
}