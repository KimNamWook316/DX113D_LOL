
#include "EyeLaserComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "Animation/AnimationSequenceInstance.h"
#include "GameObject/GameObject.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"

CEyeLaserComponent::CEyeLaserComponent()	:
	m_TriggerHitCount(0),
	m_Player(nullptr),
	m_AnimComp(nullptr),
	m_WakeEnd(false)
{
	SetTypeID<CEyeLaserComponent>();
	m_ComponentType = Component_Type::SceneComponent;

	m_CurrentLaserDir = Vector3(0.f, 0.f, 1.f);

	m_LayerName = "Transparency";
	m_Render = true;
}

CEyeLaserComponent::CEyeLaserComponent(const CEyeLaserComponent& com)	:
	CSceneComponent(com)
{
}

CEyeLaserComponent::~CEyeLaserComponent()
{
}

void CEyeLaserComponent::SetWakeEnd()
{
	m_WakeEnd = true;
	m_AnimComp->GetAnimationInstance()->ChangeAnimation("EyeLaser_Idle");
}

void CEyeLaserComponent::Start()
{
	CSceneComponent::Start();

	m_Player = m_Object->GetScene()->GetPlayerObject();

	// Notify(ex. Wake 애니메이션 끝나면 TrackPlayer 함수 호출) 세팅

	m_AnimComp = m_Object->FindComponentFromType<CAnimationMeshComponent>();

	if (m_AnimComp)
	{
		CAnimationSequenceInstance* Instance = m_AnimComp->GetAnimationInstance();
		
		Instance->ChangeAnimation("EyeLaser_Sleep");
		Instance->SetEndFunction<CEyeLaserComponent>("EyeLaser_Wake", this, &CEyeLaserComponent::SetWakeEnd);
	}
	
	m_LaserPlaneMesh1 = CResourceManager::GetInst()->FindMesh("PlaneMesh");

	CMaterial* LaserMat = m_Scene->GetResource()->FindMaterial("LaserMaterial");

	if (!LaserMat)
	{
		m_Scene->GetResource()->CreateMaterial<CMaterial>("LaserMaterial");
		m_Material = m_Scene->GetResource()->FindMaterial("LaserMaterial");
	}

	else
		m_Material = LaserMat;

	m_Material->SetShader("LaserShader");
	m_Material->AddTexture(0, (int)Buffer_Shader_Type::Pixel,
		"LaserDif", TEXT("Laser.png"));
	m_Material->SetOpacity(0.3f);
}

bool CEyeLaserComponent::Init()
{
	if (!CSceneComponent::Init())
		return false;

	return true;
}

void CEyeLaserComponent::Update(float DeltaTime)
{
	CSceneComponent::Update(DeltaTime);

	if (m_AnimComp)
	{
		CAnimationSequenceInstance* Instance = m_AnimComp->GetAnimationInstance();
		if (m_TriggerHitCount == 1 && !m_WakeEnd)
		{
			// EyeLaser가 깨어나는 애니메니션으로 전환
			Instance->ChangeAnimation("EyeLaser_Wake");
		}

		if (m_TriggerHitCount == 1 && m_WakeEnd)
		{
			TrackPlayer(DeltaTime);
		}

		else if (m_TriggerHitCount == 4)
		{
			// EyeLaser가 파괴
		}
	}
}

void CEyeLaserComponent::PostUpdate(float DeltaTime)
{
	CSceneComponent::PostUpdate(DeltaTime);
}

void CEyeLaserComponent::PrevRender()
{
	CSceneComponent::PrevRender();
}

void CEyeLaserComponent::Render()
{
	// 카메라를 계속 바라보게 만든다.
	// 카메라의 위치를 얻어온다.
	Vector3 CameraPos = m_Scene->GetCameraManager()->GetCurrentCamera()->GetWorldPos();

	Vector3	View = CameraPos - GetWorldPos();
	View.Normalize();

	Vector3 OriginDir(0.f, 0.f, -1.f);

	m_Transform->SetRotationAxis(OriginDir, View);

	//if (m_TriggerHitCount == 1 && m_WakeEnd)
	//{
		CSceneComponent::Render();

		m_Material->Render();

		m_LaserPlaneMesh1->Render();
	//}
}

void CEyeLaserComponent::PostRender()
{
	CSceneComponent::PostRender();
}

CEyeLaserComponent* CEyeLaserComponent::Clone()
{
	return new CEyeLaserComponent(*this);
}

bool CEyeLaserComponent::Save(FILE* File)
{
	CSceneComponent::Save(File);

	return true;
}

bool CEyeLaserComponent::Load(FILE* File)
{
	CSceneComponent::Load(File);

	return true;
}

bool CEyeLaserComponent::SaveOnly(FILE* File)
{
	return false;
}

bool CEyeLaserComponent::LoadOnly(FILE* File)
{
	return false;
}

void CEyeLaserComponent::TrackPlayer(float DeltaTime)
{
	m_WakeEnd = true; 

	// 원래 레이저가 바라보는 방향은 플레이어가 입장하는 문을 바라보는 방향
	Vector3 LaserRot = Vector3(0.f, 0.f, 1.f);

	Matrix RotMatrix = m_Object->GetRootComponent()->GetTransform()->GetRotationMatrix();

	LaserRot = LaserRot.TransformCoord(RotMatrix);

	Vector3 PlayerPos = m_Player->GetWorldPos();
	Vector3 LaserPos = m_Object->GetWorldPos();

	Vector3 Dir = PlayerPos - LaserPos;
	Dir.Normalize();

	float Rad = Dir.Dot(LaserRot);
	float Degree = RadianToDegree(acosf(Rad));

	Vector3 CrossVector = Dir.Cross(LaserRot);

	// Player가 레이저를 바라봤을때, Player의 위치가 레이저 쏘는 위치보다 왼쪽에 존재 
	if (CrossVector.y < 0.f)
	{
		m_Object->AddWorldRotationY(DeltaTime * 6.f);
	}

	// Player가 레이저를 바라봤을때, Player의 위치가 레이저 쏘는 위치보다 오른쪽에 존재
	else
	{
		m_Object->AddWorldRotationY(-DeltaTime * 6.f);
	}

	Vector3 LaserTopPos = GetWorldPos();;
	Dir = PlayerPos - Vector3(LaserTopPos.x, LaserPos.y + 1.5f, LaserPos.z);

	Vector3 UpDownVec = Vector3(0.f, Dir.y, Dir.z);
	UpDownVec.Normalize();

	Rad = m_CurrentLaserDir.Dot(UpDownVec);
	Degree = RadianToDegree(acosf(Rad));

	CrossVector = m_CurrentLaserDir.Cross(UpDownVec);

	// LaserEye가 -y 방향으로 향해야 할때
	if (CrossVector.x > 0)
	{
		m_Object->AddWorldRotationX(DeltaTime * 3.f);
		m_LaserRotMatrix.Rotation(DeltaTime * 3.f, 0.f, 0.f);
	}

	// LaserEye가 +y 방향으로 향해야 할때
	else
	{
		m_Object->AddWorldRotationX(-DeltaTime * 3.f);
		m_LaserRotMatrix.Rotation(-DeltaTime * 3.f, 0.f, 0.f);
	}

	m_CurrentLaserDir = m_CurrentLaserDir.TransformCoord(m_LaserRotMatrix);
	m_CurrentLaserDir.Normalize();
}

void CEyeLaserComponent::ChangeToWakeAnimation()
{
}

void CEyeLaserComponent::ChangeToDieAnimation()
{
}

void CEyeLaserComponent::ChangeToIdleAnimation()
{
}
