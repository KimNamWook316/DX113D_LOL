
#include "TinyCrowDataComponent.h"
#include "Component/ColliderBox3D.h"
#include "ObjectPool.h"
#include "Scene/Scene.h"

CTinyCrowDataComponent::CTinyCrowDataComponent()	:
	m_AccTime(0.f)
{
	SetTypeID<CTinyCrowDataComponent>();

	m_ComponentType = Component_Type::ObjectComponent;
}

CTinyCrowDataComponent::CTinyCrowDataComponent(const CTinyCrowDataComponent& com)	:
	CMonsterDataComponent(com)
{
}

CTinyCrowDataComponent::~CTinyCrowDataComponent()
{
}

void CTinyCrowDataComponent::Start()
{
	m_Object->GetRootComponent()->SetWorldScale(0.01f, 0.01f, 0.01f);

	m_Object->GetRootComponent()->GetTransform()->ForceUpdateMat();

	CColliderBox3D* Body = m_Object->FindComponentFromType<CColliderBox3D>();

	Body->AddCollisionCallback<CTinyCrowDataComponent>(Collision_State::Begin, this, &CTinyCrowDataComponent::OnCollisionPlayer);
}

void CTinyCrowDataComponent::Update(float DeltaTime)
{
	m_AccTime += DeltaTime;

	Vector3 ZDir = m_Object->GetWorldAxis(AXIS_Z);
	Vector3 PlayerPos = m_Object->GetScene()->GetPlayerObject()->GetWorldPos();
	Vector3 MyPos = m_Object->GetWorldPos();
	Vector3 PlayerDir = PlayerPos - MyPos;
	PlayerDir.Normalize();

	m_Object->AddWorldPos(ZDir * 4.f * DeltaTime);

	Vector3 CrossVec = ZDir.Cross(PlayerDir);
	
	if(CrossVec.y < 0.f)
		m_Object->AddWorldRotationY(-8.f * m_AccTime * DeltaTime);
	else
		m_Object->AddWorldRotationY(8.f * m_AccTime * DeltaTime);
}

void CTinyCrowDataComponent::OnCollisionPlayer(const CollisionResult& Result)
{
	if (Result.Dest->GetGameObject()->GetObjectType() == Object_Type::Player)
	{
		m_Scene->GetCameraManager()->GetCurrentCamera()->Shake(0.2f, 0.2f);
		m_Object->Destroy();

		CGameObject* DestObject = Result.Dest->GetGameObject();

		CObjectDataComponent* Data = (CObjectDataComponent*)DestObject->FindComponent("ObjectData");
		Data->SetIsHit(true);
	}

	else if (Result.Dest->GetCollisionProfile()->Name == "PlayerAttack")
	{
		m_Scene->GetCameraManager()->GetCurrentCamera()->Shake(0.2f, 0.2f);
		m_Object->Destroy();
	}
}
