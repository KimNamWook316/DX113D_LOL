
#include "BatDataComponent.h"
#include "../DataManager.h"
#include "Component/ColliderBox3D.h"
#include "Component/AnimationMeshComponent.h"
#include "MonsterNavAgent.h"
#include "GameStateComponent.h"

CBatDataComponent::CBatDataComponent() :
	m_RecognizeStart(false),
	m_RecognizeEnd(false)
{
	SetTypeID<CBatDataComponent>();

	m_ComponentType = Component_Type::ObjectComponent;
}

CBatDataComponent::CBatDataComponent(const CBatDataComponent& com)	:
	CMonsterDataComponent(com)
{
}

CBatDataComponent::~CBatDataComponent()
{
}

void CBatDataComponent::Start()
{
	CMonsterDataComponent::Start();

	m_Data = CDataManager::GetInst()->GetObjectData("Bat");

	m_HitBox = m_Object->FindComponentFromType<CColliderBox3D>();

	m_AnimMesh->GetAnimationInstance()->SetEndFunction("Recognize", this, &CBatDataComponent::OnRecognizeEnd);
	m_AnimMesh->GetAnimationInstance()->SetEndFunction("MeleeAttack", this, &CBatDataComponent::OnEndMeleeAttack);

	m_AnimMesh->GetAnimationInstance()->AddNotify<CBatDataComponent>("MeleeAttack", "MeleeAttackStart", 19, this, &CBatDataComponent::OnMeleeAttackColliderEnable);
	m_AnimMesh->GetAnimationInstance()->AddNotify<CBatDataComponent>("MeleeAttack", "MeleeAttackEnd", 32, this, &CBatDataComponent::OnMeleeAttackColliderDisable);


	if (m_MonsterNavAgent)
		m_MonsterNavAgent->SetMoveSpeed(m_Data.MoveSpeed);

	CMonsterDataComponent* Data = (CMonsterDataComponent*)m_Object->FindComponent("ObjectData");
	Data->SetAttackCoolDelayTimeMax(1.7f);
}

void CBatDataComponent::Update(float DeltaTime)
{
	CMonsterDataComponent::Update(DeltaTime);

	CAnimationSequenceInstance* Instance = m_AnimMesh->GetAnimationInstance();

	bool IsOnMeleeAttack = Instance->CheckCurrentAnimation("MeleeAttack");

	if (!IsOnMeleeAttack)
	{
		m_MeleeAttackCollider->Enable(false);
	}
}

void CBatDataComponent::SetIsHit(bool Hit)
{
	CMonsterDataComponent::SetIsHit(Hit);

	if (m_Data.HP <= 0)
	{
		m_HitBox->Enable(false);
		m_MeleeAttackCollider->Enable(false);
		m_AnimMesh->GetAnimationInstance()->GetCurrentAnimation()->SetPlayScale(0.f);
		OnDeadAnimEnd();
	}
}

void CBatDataComponent::OnRecognizeEnd()
{
 	m_RecognizeEnd = true;
}

void CBatDataComponent::OnEndMeleeAttack()
{
	CMonsterDataComponent* Data = (CMonsterDataComponent*)m_Object->FindComponent("ObjectData");

	Data->SetAttackCoolDelayTimeEnable(true);

	CGameStateComponent* State = m_Object->FindObjectComponentFromType<CGameStateComponent>();
	State->GetBehaviorTree()->SetCurrentNode(nullptr);

	//m_AnimMesh->GetAnimationInstance()->SetCurrentAnimation("Idle");
}

void CBatDataComponent::OnMeleeAttackColliderEnable()
{
	m_MeleeAttackCollider->Enable(true);
}

void CBatDataComponent::OnMeleeAttackColliderDisable()
{
	m_MeleeAttackCollider->Enable(false);
}

void CBatDataComponent::OnCollision(const CollisionResult& Result)
{
	if (Result.Dest->GetGameObject()->GetObjectType() == Object_Type::Player)
	{
		m_Object->Destroy();

		CGameObject* DestObject = Result.Dest->GetGameObject();

		CObjectDataComponent* Data = (CObjectDataComponent*)DestObject->FindComponent("ObjectData");
		float Attack = m_Data.Attack;

		Data->DecreaseHP(Attack);
	}
}
