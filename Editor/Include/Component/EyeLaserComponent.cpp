
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

	m_CurrentLaserDir = Vector3(0.f, 0.f, -1.f);

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

	SetWorldRotation(90.f, -90.f, 0.f);
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
	/*Vector3 CameraPos = m_Scene->GetCameraManager()->GetCurrentCamera()->GetWorldPos();

	Vector3	View = CameraPos - GetWorldPos();
	View.Normalize();

	Vector3 OriginDir(0.f, 0.f, -1.f);

	m_Transform->SetRotationAxis(OriginDir, View);*/

	//if (m_TriggerHitCount == 1 && m_WakeEnd)
	//{
		CSceneComponent::Render();

		m_Material->Render();

		m_LaserPlaneMesh->Render();
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

	Vector3 LaserTopPos = GetWorldPos();
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
