
#include "CrowBossShootNode.h"
#include "../GameBehaviorTree.h"
#include "../GameStateComponent.h"
#include "../CrowBossDataComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "../MonsterNavAgent.h"
#include "Scene/Scene.h"

CCrowBossShootNode::CCrowBossShootNode()
{
	SetTypeID(typeid(CCrowBossShootNode).hash_code());
}

CCrowBossShootNode::CCrowBossShootNode(const CCrowBossShootNode& Node) :
	CActionNode(Node)
{
}

CCrowBossShootNode::~CCrowBossShootNode()
{
}

NodeResult CCrowBossShootNode::OnStart(float DeltaTime)
{
	m_AnimationMeshComp->GetAnimationInstance()->ChangeAnimation("SlidingReady");

	return NodeResult::Node_True;
}

NodeResult CCrowBossShootNode::OnUpdate(float DeltaTime)
{
	CCrowBossDataComponent* Data = dynamic_cast<CCrowBossDataComponent*>(dynamic_cast<CGameStateComponent*>(m_Owner->GetOwner())->GetData());

	Vector3 FaceDir = Data->GetMonsterNavAgent()->GetCurrentFaceDir();

	CGameObject* Player = m_Object->GetScene()->GetPlayerObject();
	Vector3 PlayerPos = Player->GetWorldPos();
	Vector3 MyPos = m_Object->GetWorldPos();
	Vector3 Dir = PlayerPos - MyPos;
	Dir.Normalize();

	float DotProduct = Dir.Dot(FaceDir);

	if (DotProduct > -0.99999f && DotProduct < 0.99999f)
	{
		float Degree = RadianToDegree(acosf(DotProduct));
		if (Degree > 90.f || Degree < -90.f)
		{
			Vector3 CrossVec = FaceDir.Cross(Dir);

			if (CrossVec.y > 0.f)
			{
				m_Object->AddWorldRotationY(Degree);
			}

			else
			{
				m_Object->AddWorldRotationY(-Degree);
			}

			return NodeResult::Node_True;
		}

	}

	Data->SetShootState(CrowBossShootState::Shoot);
	Data->ShootChain(FaceDir, DeltaTime);

	return NodeResult::Node_True;
}

NodeResult CCrowBossShootNode::OnEnd(float DeltaTime)
{
	return NodeResult();
}