
#include "EyeLaserComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/ColliderRay.h"
#include "Animation/AnimationSequenceInstance.h"
#include "GameObject/GameObject.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "ObjectDataComponent.h"

CEyeLaserComponent::CEyeLaserComponent()	:
	m_TriggerHitCount(0),
	m_Player(nullptr),
	m_RayCollider(nullptr),
	m_FaceCameraOnce(false)
{
	SetTypeID<CEyeLaserComponent>();
	m_ComponentType = Component_Type::SceneComponent;

	m_CurrentLaserLeftRightDir = Vector3(0.f, 0.f, -1.f);
	//m_CurrentLaserUpDownDir = Vector3(0.f, 0.f, 1.f);
	m_NormalDir = Vector3(0.f, 1.f, 0.f);

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

}

void CEyeLaserComponent::Start()
{
	CSceneComponent::Start();

	m_Player = m_Object->GetScene()->GetPlayerObject();

	// Notify(ex. Wake 애니메이션 끝나면 TrackPlayer 함수 호출) 세팅

	
	m_LaserPlaneMesh = CResourceManager::GetInst()->FindMesh("PlaneMesh");

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
	m_Material->SetOpacity(1.f);
	m_Material->SetTransparency(true);

	m_RayCollider = m_Object->FindComponentFromType<CColliderRay>();

	if (m_RayCollider)
	{
		m_RayCollider->AddCollisionCallback<CEyeLaserComponent>(Collision_State::Begin, this, &CEyeLaserComponent::RaserCollision);
	}

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


	FaceCamera();

	if (m_TriggerHitCount == 1)
	{
		TrackPlayer(DeltaTime);
	}

	else if (m_TriggerHitCount == 4)
	{
		// EyeLaser가 파괴
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
	if (m_TriggerHitCount == 1)
	{
		CSceneComponent::Render();

		m_Material->Render();

		m_LaserPlaneMesh->Render();

		m_Material->Reset();
	}
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
	
	m_Material->Save(File);
	m_LaserPlaneMesh->Save(File);


	return true;
}

bool CEyeLaserComponent::Load(FILE* File)
{
	CSceneComponent::Load(File);

	m_Material = new CMaterial;

	m_Material->Load(File);

	m_LaserPlaneMesh = CResourceManager::GetInst()->FindMesh("PlaneMesh");

	m_LaserPlaneMesh->Load(File);

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
	// 원래 레이저가 바라보는 방향은 플레이어가 입장하는 문을 바라보는 방향. 
	// 이때 플레이어가 레이저를 바라볼 때 오른쪽이 -x, 왼쪽이 +x이다
	//m_CurrentLaserRot = Vector3(0.f, 0.f, 1.f);
	m_CurrentLaserLeftRightDir = m_CurrentLaserLeftRightDir.TransformCoord(m_LaserLeftRightRotMatrix);

	Vector3 PlayerPos = m_Player->GetWorldPos();
	//Vector3 LaserPos = m_Object->GetWorldPos();
	Vector3 LaserPos = GetWorldPos();

	Vector3 Dir = PlayerPos - LaserPos;
	Dir.y = 0.f;
	Dir.Normalize();

	float Rad = Dir.Dot(m_CurrentLaserLeftRightDir);
	float Degree = RadianToDegree(acosf(Rad));

	Vector3 CrossVector = Dir.Cross(m_CurrentLaserLeftRightDir);

	// Player가 레이저를 바라봤을때, Player의 위치가 레이저 쏘는 위치보다 오른쪽에 존재
	// 로컬축 기준으로 회전하니까 Z방향으로 회전해야함
	if (CrossVector.y > 0.f)
	{
		// 회전은 로컬축 기준이라 Z축 회전을 하지만 World기준은 Y방향이므로 Matrix는 Y축 회전 Matrix를 만든다
		//m_AnimComp->AddWorldRotationZ(-DeltaTime * 7.f);
		AddWorldRotationY(-DeltaTime * 7.f);
		if (m_RayCollider)
			m_RayCollider->AddWorldRotationY(-DeltaTime * 7.f);
		m_LaserLeftRightRotMatrix.RotationY(-DeltaTime * 7.f);
	}

	// Player가 레이저를 바라봤을때, Player의 위치가 레이저 쏘는 위치보다 왼쪽에 존재
	else if(CrossVector.y < 0.f)
	{
		// 회전은 로컬축 기준이라 Z축 회전을 하지만 World기준은 Y방향이므로 Matrix는 Y축 회전 Matrix를 만든다
		//m_AnimComp->AddWorldRotationZ(DeltaTime * 7.f);
		AddWorldRotationY(DeltaTime * 7.f);
		if(m_RayCollider)
			m_RayCollider->AddWorldRotationY(DeltaTime * 7.f);
		m_LaserLeftRightRotMatrix.RotationY(DeltaTime * 7.f);
	}

	//m_CurrentLaserUpDownDir = m_CurrentLaserUpDownDir.TransformCoord(m_LaserUpDownRotMatrix);

	//Dir = Vector3(PlayerPos.x, PlayerPos.y + 1.f, PlayerPos.z) - LaserPos;
	//Dir.x = 0.f;
	//Dir.Normalize();

	//Rad = Dir.Dot(m_CurrentLaserUpDownDir);
	//Degree = RadianToDegree(acosf(Rad));

	//CrossVector = Dir.Cross(m_CurrentLaserUpDownDir);

	//// 레이저가 위로 올라가야하는 상황
	//if (CrossVector.x > 0.f)
	//{
	//	m_AnimComp->AddWorldRotationX(DeltaTime * 4.f);
	//	if (m_RayCollider)
	//		m_RayCollider->AddWorldRotationZ(DeltaTime * 4.f);
	//	m_LaserUpDownRotMatrix.RotationX(-DeltaTime * 4.f);

	//	float Angle = Vector3(0.f, 0.f, 1.f).Angle(Dir);
	//	Vector3 Axis = Vector3(0.f, 0.f, 1.f).Cross(Dir);
	//	Axis.Normalize();

	//	// 레이저 위아래 회전 보류 -> 회전은 로컬축으로 회전하는데 회전하면 로컬축이 바뀌어서 월드X축으로 여기서 회전하기 힘듬
	//	//Matrix RotMatDest = XMMatrixRotationAxis(Axis.Convert(), DegreeToRadian(-Angle));

	//	//Vector3 CurrentPos = GetWorldPos();

	//	//CurrentPos = CurrentPos.TransformCoord(RotMatDest);

	//	//SetWorldPos(CurrentPos);
	//}

	//// 레이저가 아래 방향으로 내려가야하는 상황
	//else if (CrossVector.x < 0.f)
	//{
	//	m_AnimComp->AddWorldRotationX(-DeltaTime * 4.f);
	//	if (m_RayCollider)
	//		m_RayCollider->AddWorldRotationZ(-DeltaTime * 4.f);
	//	m_LaserUpDownRotMatrix.RotationX(DeltaTime * 4.f);

	//	float Angle = Vector3(0.f, 0.f, 1.f).Angle(Dir);
	//	Vector3 Axis = Vector3(0.f, 0.f, 1.f).Cross(Dir);
	//	Axis.Normalize();

	//	// 레이저 위아래 회전 보류 -> 회전은 로컬축으로 회전하는데 회전하면 로컬축이 바뀌어서 월드X축으로 여기서 회전하기 힘듬
	//	//Matrix RotMatDest = XMMatrixRotationAxis(Axis.Convert(), DegreeToRadian(Angle));

	//	//Vector3 CurrentPos = GetWorldPos();

	//	//CurrentPos = CurrentPos.TransformCoord(RotMatDest);

	//	//SetWorldPos(CurrentPos);
	//}
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

void CEyeLaserComponent::FaceCamera()
{
	// 카메라를 계속 바라보게 만든다.
	Vector3 CameraPos = m_Scene->GetCameraManager()->GetCurrentCamera()->GetWorldPos();
	Vector3 MyPos = GetWorldPos();

	CameraPos.z = MyPos.z;
	Vector3	View = CameraPos - MyPos;

	View.Normalize();

	float Dist = m_NormalDir.Distance(View);

	if (Dist < 0.001f)
		return;

	float Angle = Vector3(0.f, 1.f, 0.f).Angle(View);

	if (Angle < 0.1f)
		return;

	Vector3 RotDir = Vector3(0.f, 1.f, 0.f).Cross(View);
	RotDir.Normalize();

	if (RotDir.z < 0.f)
		Angle *= -1.f;

	//m_Transform->SetRotationAxis(m_NormalDir, View);
	SetWorldRotationZ(Angle);

	//Matrix MatRot = XMMatrixRotationAxis(RotDir.Convert(), DegreeToRadian(-Angle));
	//m_NormalDir = m_NormalDir.TransformCoord(MatRot);
	//m_NormalDir.Normalize();
}

void CEyeLaserComponent::RaserCollision(const CollisionResult& Result)
{
	CGameObject* Object = Result.Dest->GetGameObject();

	CObjectDataComponent* Comp = (CObjectDataComponent*)(Object->FindObjectComponent("ObjectData"));

	if (Comp)
	{
		Comp->DecreaseHP(1);
	}
}

void CEyeLaserComponent::SetBaseColor(const Vector4& Color, int Index)
{
	m_Material->SetBaseColor(Color);
}

void CEyeLaserComponent::SetBaseColor(float r, float g, float b, float a, int Index)
{
	m_Material->SetBaseColor(r, g, b, a);
}

void CEyeLaserComponent::SetEmissiveColor(const Vector4& Color, int Index)
{
	m_Material->SetEmissiveColor(Color);
}

void CEyeLaserComponent::SetEmissiveColor(float r, float g, float b, float a, int Index)
{
	m_Material->SetEmissiveColor(r, g, b, a);
}

void CEyeLaserComponent::SetTransparencyMaterial(bool Enable)
{
	if (Enable)
	{
		bool AlreadyTransparent = m_LayerName == "Transparency";

		if (!AlreadyTransparent)
		{
			m_LayerName = "Transparency";
		}

		m_Material->SetTransparency(true);

		// 모든 Material의 Shader 교체
		SetMaterialShader("Transparent3DShader");

		// 인스턴싱 레이어를 바꾼다.
		if (!AlreadyTransparent)
		{
			ChangeInstancingLayer();
		}
	}

	else
	{
		bool AlreadyOpaque = m_LayerName == "Default";

		if (!AlreadyOpaque)
		{
			m_LayerName = "Default";
		}

		// 이전 쉐이더로 되돌림
		m_Material->SetTransparency(false);
		m_Material->RevertShader();

		// 인스턴싱 레이어를 바꾼다.
		if (!AlreadyOpaque)
		{
			ChangeInstancingLayer();
		}
	}
}

void CEyeLaserComponent::SetMaterialShader(const std::string& Name)
{
	CGraphicShader* Shader = dynamic_cast<CGraphicShader*>(CResourceManager::GetInst()->FindShader(Name));

	if (!Shader)
		return;

	m_Material->SetShader(Shader);
	OnChangeMaterialShader(Shader);

}

void CEyeLaserComponent::ChangeInstancingLayer()
{
	auto iter = m_InstancingCheckList.begin();
	auto iterEnd = m_InstancingCheckList.end();

	bool CheckCountExist = false;

	bool AddOnNewLayer = false;
	bool DeleteOnOldLayer = false;

	for (; iter != iterEnd;)
	{
		if ((*iter)->Mesh == m_LaserPlaneMesh)
		{
			// 같은 레이어일 경우 추가한다.
			if ((*iter)->LayerName == m_LayerName)
			{
				CheckCountExist = true;

				(*iter)->InstancingList.push_back(this);

				auto iterInst = (*iter)->InstancingList.begin();
				auto iterInstEnd = (*iter)->InstancingList.end();

				// 이 컴포넌트를 추가하면서 인스턴싱으로 출력하게 되는 경우
				if ((*iter)->InstancingList.size() == 10)
				{
					for (; iterInst != iterInstEnd; ++iterInst)
					{
						(*iterInst)->SetInstancing(true);
					}
				}
				// 이미 인스턴싱 출력중인 경우
				else if ((*iter)->InstancingList.size() > 10)
				{
					SetInstancing(true);
				}
				// 인스턴싱 하지 않는 경우
				else
				{
					SetInstancing(false);
				}

				AddOnNewLayer = true;
			}

			// Layer가 다를 경우 해당 레이어에서 뺀다
			else
			{
				// 이 컴포넌트가 빠지면서 인스턴싱을 하지 않게 처리해야 하는 경우
				bool InstancingOff = (*iter)->InstancingList.size() == 10;

				auto iterInst = (*iter)->InstancingList.begin();
				auto iterInstEnd = (*iter)->InstancingList.end();

				if (InstancingOff)
				{
					for (; iterInst != iterInstEnd;)
					{
						(*iterInst)->SetInstancing(false);

						if ((*iterInst) == this)
						{
							iterInst = (*iter)->InstancingList.erase(iterInst);
							continue;
						}

						++iterInst;
					}
				}
				else
				{
					for (; iterInst != iterInstEnd; ++iterInst)
					{
						if ((*iterInst) == this)
						{
							// 현재 레이어의 인스턴싱 리스트에서 제거
							(*iter)->InstancingList.erase(iterInst);
							break;
						}
					}
				}

				DeleteOnOldLayer = true;

				if ((*iter)->InstancingList.empty())
				{
					SAFE_DELETE(*iter);
					iter = m_InstancingCheckList.erase(iter);
					continue;
				}
			}
		}

		// 새 레이어에 넣고, 이전 레이어에서 빼는 작업 완료한 경우 루프 종료
		if (AddOnNewLayer && DeleteOnOldLayer)
		{
			break;
		}

		++iter;
	}

	// 이 컴포넌트가 속한 레이어에 처음 추가되는 경우
	if (!CheckCountExist)
	{
		OnCreateNewInstancingCheckCount();
	}
}

void CEyeLaserComponent::OnCreateNewInstancingCheckCount()
{
	InstancingCheckCount* CheckCount = new InstancingCheckCount;

	m_InstancingCheckList.push_back(CheckCount);

	CheckCount->InstancingList.push_back(this);
	CheckCount->Mesh = m_LaserPlaneMesh;
	CheckCount->LayerName = m_LayerName;

	// Material별로 어떤 Instanicng Shader로 렌더해야 하는지 받아온다.
	size_t SlotSize = 1;

	CGraphicShader* NoInstancingShader = nullptr;
	CGraphicShader* InstancingShader = nullptr;
	CMaterial* Mat = nullptr;
	ShaderParams MatShaderParams = {};

	CheckCount->vecInstancingShader.resize(SlotSize);
	CheckCount->vecShaderParams.resize(SlotSize);

	for (size_t i = 0; i < SlotSize; ++i)
	{
		// Material별 Instancing Shader, Shader Paramerter 정보 저장
		Mat = m_Material;
		NoInstancingShader = Mat->GetShader();
		InstancingShader = CResourceManager::GetInst()->FindInstancingShader(NoInstancingShader);
		MatShaderParams = Mat->GetShaderParams();

		CheckCount->vecInstancingShader[i] = InstancingShader;
		CheckCount->vecShaderParams[i] = MatShaderParams;
	}

	SetInstancing(false);
}

void CEyeLaserComponent::SetOpacity(float Opacity)
{
	m_Material->SetOpacity(Opacity);
}

void CEyeLaserComponent::AddOpacity(float Opacity)
{
	m_Material->AddOpacity(Opacity);
}

void CEyeLaserComponent::OnChangeMaterialShader(CGraphicShader* NewShader)
{
	auto iter = m_InstancingCheckList.begin();
	auto iterEnd = m_InstancingCheckList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->Mesh == m_LaserPlaneMesh)
		{
			if ((*iter)->LayerName == m_LayerName)
			{
				// Instancing Shader 교체
				CGraphicShader* NewInstancingShader = CResourceManager::GetInst()->FindInstancingShader(NewShader);
				(*iter)->vecInstancingShader[0] = NewInstancingShader;
				break;
			}
		}
	}
}
