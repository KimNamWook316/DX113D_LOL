#include "SporeBoomerDataComponent.h"
#include "../DataManager.h"
#include "Component/AnimationMeshComponent.h"

CSporeBoomerDataComponent::CSporeBoomerDataComponent()
{
	SetTypeID<CSporeBoomerDataComponent>();
}

CSporeBoomerDataComponent::CSporeBoomerDataComponent(const CSporeBoomerDataComponent& com)	:
	CMonsterDataComponent(com)
{
}

CSporeBoomerDataComponent::~CSporeBoomerDataComponent()
{
}

void CSporeBoomerDataComponent::Start()
{
	CMonsterDataComponent::Start();

	m_Data = CDataManager::GetInst()->GetObjectData("SporeBoomer");

	CAnimationSequenceInstance* AnimInst = m_AnimMesh->GetAnimationInstance();
	AnimInst->AddNotify("Fire", "OnFire", 44, this, &CSporeBoomerDataComponent::OnShootBullet);
	AnimInst->SetEndFunction("Fire", (CMonsterDataComponent*)this, &CMonsterDataComponent::SetCurrentNodeNull);
}

void CSporeBoomerDataComponent::OnShootBullet()
{
}
