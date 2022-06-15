
#include "MinionAIComponent.h"
#include "GameObject/GameObject.h"
#include "Component/ColliderBox3D.h"


CMinionAIComponent::CMinionAIComponent()	:
	m_Target(nullptr)
{
	SetTypeID<CMinionAIComponent>();
	m_MinionType = Minion_Type::None;
}

CMinionAIComponent::~CMinionAIComponent()
{
}

CMinionAIComponent::CMinionAIComponent(const CMinionAIComponent& Com)	:
	CObjectComponent(Com)
{
}

bool CMinionAIComponent::Init()
{
	return true;
}

void CMinionAIComponent::Update(float DeltaTime)
{
}

void CMinionAIComponent::PostUpdate(float DeltaTime)
{
}

void CMinionAIComponent::PrevRender()
{
}

void CMinionAIComponent::Render()
{
}

void CMinionAIComponent::PostRender()
{
}

CMinionAIComponent* CMinionAIComponent::Clone()
{
	return new CMinionAIComponent(*this);
}

void CMinionAIComponent::Start()
{
	if (m_Object && m_MinionType != Minion_Type::None)
	{
		std::vector<CColliderBox3D*> vecColliderBox;
		m_Object->FindAllObjectComponentFromType<CColliderBox3D>(vecColliderBox);

		size_t Count = vecColliderBox.size();

		for (size_t i = 0; i < Count; ++i)
		{
			if (m_MinionType == Minion_Type::Melee)
			{
				if (vecColliderBox[i]->GetCollisionProfile()->Name == "MinionNormalAttack")
				{
					vecColliderBox[i]->AddCollisionCallback<CMinionAIComponent>(Collision_State::Begin, this, &CMinionAIComponent::OnAttack);
				}

				else if (vecColliderBox[i]->GetCollisionProfile()->Name == "MinionDetect")
				{
					vecColliderBox[i]->AddCollisionCallback<CMinionAIComponent>(Collision_State::Begin, this, &CMinionAIComponent::OnTrack);
				}
			}
		}
	}
}

bool CMinionAIComponent::Save(FILE* File)
{
	fwrite(&m_MinionType, sizeof(Minion_Type), 1, File);

	return true;
}

bool CMinionAIComponent::Load(FILE* File)
{
	fread(&m_MinionType, sizeof(Minion_Type), 1, File);

	return true;
}

bool CMinionAIComponent::SaveOnly(FILE* File)
{
	return false;
}

bool CMinionAIComponent::LoadOnly(FILE* File)
{
	return false;
}

void CMinionAIComponent::OnAttack(const CollisionResult& Result)
{
}

void CMinionAIComponent::OnTrack(const CollisionResult& Result)
{
	CGameObject* Dest = Result.Dest->GetGameObject();
	Vector3 DestPos = Dest->GetWorldPos();
}
