
#include "ParticleComponent.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Scene/SceneManager.h"
#include "../Scene/CameraManager.h"
#include "../PathManager.h"
#include "../Component/CameraComponent.h"
#include "../Resource/Shader/StructuredBuffer.h"

CParticleComponent::CParticleComponent()	:
	m_SpawnTime(0.f),
	m_SpawnTimeMax(0.01f),
	m_Info{},
	m_BillBoardEffect(false),
	m_BazierMoveEffect(false),
	m_GravityEnable(false),
	m_ParticleMoveSpeed(50.f),
	m_InfoShared{},
	m_CBuffer(nullptr)
{
	SetTypeID<CParticleComponent>();
	m_Render = true;

	m_LayerName = "Particle";
}

CParticleComponent::CParticleComponent(const CParticleComponent& com) :
	CSceneComponent(com)
{
	m_SpawnTime = 0.f;
	m_SpawnTimeMax = com.m_SpawnTimeMax;

	m_Particle = com.m_Particle;

	size_t	BufferCount = m_vecStructuredBuffer.size();

	for (size_t i = 0; i < BufferCount; ++i)
	{
		SAFE_DELETE(m_vecStructuredBuffer[i]);
	}

	m_vecStructuredBuffer.clear();

	if (m_Particle)
	{
		m_Particle->CloneStructuredBuffer(m_vecStructuredBuffer);

		m_UpdateShader = m_Particle->GetUpdateShader();

		SAFE_DELETE(m_CBuffer);

		m_CBuffer = m_Particle->CloneConstantBuffer();
	}
}

CParticleComponent::~CParticleComponent()
{
	// SAFE_DELETE(m_NormalDistributionBuffer);

	size_t	BufferCount = m_vecStructuredBuffer.size();

	for (size_t i = 0; i < BufferCount; ++i)
	{
		SAFE_DELETE(m_vecStructuredBuffer[i]);
	}

	SAFE_DELETE(m_CBuffer);
}

void CParticleComponent::SetParticle(const std::string& Name)
{
	SetParticle(m_Scene->GetResource()->FindParticle(Name));
}

void CParticleComponent::SetParticle(CParticle* Particle)
{
	m_Particle = Particle;

	m_Material = m_Particle->CloneMaterial();
	
	size_t	BufferCount = m_vecStructuredBuffer.size();

	for (size_t i = 0; i < BufferCount; ++i)
	{
		SAFE_DELETE(m_vecStructuredBuffer[i]);
	}

	SAFE_DELETE(m_CBuffer);

	// SAFE_DELETE(m_NormalDistributionBuffer);

	m_vecStructuredBuffer.clear();

	m_Particle->CloneStructuredBuffer(m_vecStructuredBuffer);

	// 정규 분포 정보의 경우, 딱 한번만 Update 해준다. (우선 사용 X)
	// m_Particle->CloneNormalDistStructuredBuffer(m_NormalDistributionBuffer);
	// std::vector<float> VecNormalDistVal = m_Particle->GetVecNormalDistVal();
	// m_NormalDistributionBuffer->UpdateBuffer(&VecNormalDistVal[0], (int)VecNormalDistVal.size());

	m_UpdateShader = m_Particle->CloneUpdateShader();

	m_CBuffer = m_Particle->CloneConstantBuffer();

	m_SpawnTimeMax = m_Particle->GetSpawnTimeMax();

	m_ParticleName = m_Particle->GetName();
}

void CParticleComponent::SetSpawnTime(float Time)
{
	m_SpawnTimeMax = Time;

	m_Particle->SetSpawnTimeMax(m_SpawnTimeMax);
}

void CParticleComponent::ApplyBillBoardEffect()
{
	Vector3 CameraPos = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera()->GetWorldPos();

	Vector3 View = CameraPos - GetWorldPos();

	View.Normalize();

	// float 
	Vector3 OriginDir = Vector3(0.f, 0.f, -1.f);

	m_Transform->SetRotationAxis(OriginDir, View);
}

void CParticleComponent::ApplyBazierMove()
{
	// 위치 이동
	AddWorldPos(m_ParticleMoveDir * m_ParticleMoveSpeed);

	// 이동 방향에 따라, Rotation 적용해준다.

	// 목표 위치로 거의 이동했다면, 다음 위치를 뽑아서 해당 위치로 이동한다.
	if (GetWorldPos().Distance(m_ParticleNextMovePos) < 0.1f)
	{
		if (!m_queueBazierMovePos.empty())
		{
			Vector3 NextPos = m_queueBazierMovePos.front();
			m_queueBazierMovePos.pop();
			m_ParticleNextMovePos = NextPos;
			m_ParticleMoveDir = m_ParticleNextMovePos - GetWorldPos();
			m_ParticleMoveDir.Normalize();
		}
	}
}

void CParticleComponent::SetBazierTargetPos(const Vector3& D1, const Vector3& D2, const Vector3& D3)
{

}

void CParticleComponent::Start()
{
	CSceneComponent::Start();
}

bool CParticleComponent::Init()
{
	m_Mesh = m_Scene->GetResource()->FindMesh("ParticlePointMesh");

	return true;
}

void CParticleComponent::Update(float DeltaTime)
{
	if (!m_CBuffer)
		return;

	CSceneComponent::Update(DeltaTime);

	m_SpawnTime += DeltaTime;

	// CBuffer 의 DisableNewAlive 가 true 라면, 한번에 다 생성해버려야 하는 것
	// 이 방법 중 하나는 m_SpawnTimeMax 를 0으로 만들어버리는 것
	if (m_CBuffer->IsDisableNewAlive() == 1)
	{
		m_SpawnTimeMax = 0.f;
	}

	// Spawn Time 정보
	if (m_SpawnTime >= m_SpawnTimeMax)
	{
		m_SpawnTime -= m_SpawnTimeMax;
		m_CBuffer->SetSpawnEnable(1);

		// SpawnTime 은 0으로 만든다.
		if (m_CBuffer->IsDisableNewAlive() == 1)
		{
			m_SpawnTime = 0.f;
		}
	}
	else
	{
		m_CBuffer->SetSpawnEnable(0);
	}

	// Noise Texture 를 이용한 랜덤 사라짐 효과
	if (m_CBuffer->IsNoiseTextureSamplingApplied())
	{
		float NoiseTextureFilter = m_CBuffer->GetNoiseTextureFilter();

		NoiseTextureFilter += DeltaTime;

		if (NoiseTextureFilter > 1)
			NoiseTextureFilter = 1.f;

		m_CBuffer->SetNoiseTextureFilter(NoiseTextureFilter);
	}

	// 추가 : Particle 도 BillBoard 를 적용하기위해 OBJ 가 추가
	if (m_BillBoardEffect)
	{
		ApplyBillBoardEffect();
	}

	// Bazier 방식으로 움직이게 할 것인다
	if (m_BazierMoveEffect)
	{
		// Bazier 에 저장된 위치 정보로 이동할 것인다.
		ApplyBazierMove();
	}
}

void CParticleComponent::PostUpdate(float DeltaTime)
{
	if (!m_CBuffer)
		return;

	CSceneComponent::PostUpdate(DeltaTime);

	CParticleConstantBuffer* CBuffer = m_Particle->GetCBuffer();

	// Update Shader를 동작시킨다.
	// Start Min, Max 의 경우, 
	// 1) Particle Component 의 World Rot 을 반영하여 회전 시킨 이후
	// 2) Particle Component 의 Scalilng 까지 적용한다. (우선 RelativeScale 만 적용)
	// 3) Translation은 처리하지 않는다. StartMin, Max 는, Local Space 상에서의 Min, Max 를 의미하게 할 것이다.
	// ( Rot  -> Translation )
	Vector3	StartMin = CBuffer->GetStartMin() * GetWorldScale();
	StartMin.TransformCoord(GetRotationMatrix());

	Vector3	StartMax = CBuffer->GetStartMax() * GetWorldScale();
	StartMax.TransformCoord(GetRotationMatrix());
	
	// StartMin, Max 는, World Pos 를 더해주지 않고 넘겨줄 것이다.
	// 그저 Particle Component Local Space 상에서의 Min, Max 값 만을 세팅해줄 것이다.
	// StartMin += GetWorldPos();
	// StartMax += GetWorldPos();

	// (기존 코드)
	// StartMin = GetWorldPos() + CBuffer->GetStartMin();
	// StartMax = GetWorldPos() + CBuffer->GetStartMax();

	m_CBuffer->SetStartMin(StartMin);
	m_CBuffer->SetStartMax(StartMax);

	// Rotation Angle 정보를 세팅한다. Transform 의 정보로 만들어낼 것이다.
	m_CBuffer->SetRotationAngle(GetWorldRot());
	
	// Relative Scale 정보를 세팅한다.
	m_CBuffer->SetCommonWorldScale(GetWorldScale());
	m_CBuffer->SetCommonParticleComponentWorldPos(GetWorldPos());

	m_CBuffer->UpdateCBuffer();

	// Normal Dist 구조화 버퍼 정보를 넘겨준다.
	// m_NormalDistributionBuffer->SetShader();

	size_t	BufferCount = m_vecStructuredBuffer.size();

	for (size_t i = 0; i < BufferCount; ++i)
	{
		m_vecStructuredBuffer[i]->SetShader();
	}

	// UpdateShader의 Thread는 64, 1, 1을 사용하고 있다.
	// 생성되야할 파티클의 전체 수에서 64개를 나눈다. 만약 64개를 최대파티클 수로 지정해주었다면
	// 필요한 그룹의 수는 1개이다. 하지만 64개 미만이라면 64를 나눌 경우 0이 나오므로 여기에 1을 더해주어야 한다.
	// 100개일 경우 그룹은 2개가 생성된다. 이때 스레드는 128개가 되므로 100개를 제외한 나머지 28개는 처리가 안되게
	// 막아주면 되는것이다.
	int	GroupCount = m_Particle->GetSpawnCountMax() / 64 + 1;

	m_UpdateShader->Excute(GroupCount, 1, 1);

	// m_NormalDistributionBuffer->ResetShader();

	for (size_t i = 0; i < BufferCount; ++i)
	{
		m_vecStructuredBuffer[i]->ResetShader();
	}
	// 여기에서 Data를 CopyResource로 복제해서 테스트 해볼것.
}

void CParticleComponent::PrevRender()
{
	CSceneComponent::PrevRender();
}

void CParticleComponent::RenderParticleEffectEditor()
{
	if (!m_CBuffer)
		return;

	// 상수 버퍼를 다시 한번 Setting 해준다.
	// 계산 셰이더 외에도, Render 과정에서도 상수 버퍼 정보를 사용할 수 있게 하는 것이다.
	m_CBuffer->UpdateCBuffer();

	CSceneComponent::RenderParticleEffectEditor();

	size_t	BufferCount = m_vecStructuredBuffer.size();

	for (size_t i = 0; i < BufferCount; ++i)
	{
		m_vecStructuredBuffer[i]->SetShader(30 + (int)i, (int)Buffer_Shader_Type::Geometry);
	}

	if (m_Material)
	{
		m_Material->Render();
		m_Material->EnableDecal(m_ReceiveDecal);
	}

	// 인스턴싱을 이용해서 그려준다.
	m_Mesh->RenderInstancing(m_CBuffer->GetSpawnCount());

	for (size_t i = 0; i < BufferCount; ++i)
	{
		m_vecStructuredBuffer[i]->ResetShader(30 + (int)i, (int)Buffer_Shader_Type::Geometry);
	}

	if (m_Material)
		m_Material->Reset();
}

void CParticleComponent::Render()
{
	if (!m_CBuffer)
		return;

	// 계산 셰이더 외에도, Render 과정에서도 상수 버퍼 정보를 사용할 수 있게 하는 것이다.
	m_CBuffer->UpdateCBuffer();

	CSceneComponent::Render();

	size_t	BufferCount = m_vecStructuredBuffer.size();

	for (size_t i = 0; i < BufferCount; ++i)
	{
		m_vecStructuredBuffer[i]->SetShader(30 + (int)i, (int)Buffer_Shader_Type::Geometry);
	}

	if (m_Material)
	{
		m_Material->Render();
	}

	// 인스턴싱을 이용해서 그려준다.
	m_Mesh->RenderInstancing(m_CBuffer->GetSpawnCount());

	for (size_t i = 0; i < BufferCount; ++i)
	{
		m_vecStructuredBuffer[i]->ResetShader(30 + (int)i, (int)Buffer_Shader_Type::Geometry);
	}

	if (m_Material)
		m_Material->Reset();
}

void CParticleComponent::PostRender()
{
	CSceneComponent::PostRender();

	// Restart 버튼을 해제 한다.
	// 일단 해당 상수 버퍼 내용이 실제 GPU 측에는 한번 넘어가야 하니까
	// 위에서 Render 혹은 PostUpdate 에서 넘겨주고 여기서 세팅
	if (m_CBuffer)
	{
		m_CBuffer->SetResetParticleSharedInfoSumSpawnCnt(0);
	}
}

CParticleComponent* CParticleComponent::Clone()
{
	return new CParticleComponent(*this);
}

bool CParticleComponent::Save(FILE* File)
{
	CSceneComponent::Save(File);

	bool Result = SaveOnly(File);

	return Result;
}

bool CParticleComponent::Load(FILE* File)
{
	CSceneComponent::Load(File);

	LoadOnly(File);

	return true;
}

// Shader 측에 넘겨진 구조화 버퍼 정보를 초기화 해주는 코드이다.
void CParticleComponent::ResetParticleStructuredBufferInfo()
{
	if (!m_Particle)
		return;

	m_CBuffer->SetResetParticleSharedInfoSumSpawnCnt(1);
}

bool CParticleComponent::SaveOnly(FILE* File)
{
	// Mesh
	std::string	MeshName = m_Mesh->GetName();
	int	Length = (int)MeshName.length();
	fwrite(&Length, sizeof(int), 1, File);
	fwrite(MeshName.c_str(), sizeof(char), Length, File);

	if (m_Particle->GetName() == "")
	{
		// Particle 의 경우, Particle Editor 를 통해서 저장할 때, 반드시 Particle 이름을 저장하게 되어 있다.
		// Particle 의 이름과, Particle File이 저장된 파일 이름은 동일하다.
		assert(false); 
	}

	// 혹시 모르니 한번 더 저장해준다.
	m_ParticleName = m_Particle->GetName();
	int	 ParticleNameLength = (int)m_ParticleName.length();
	fwrite(&ParticleNameLength, sizeof(int), 1, File);
	fwrite(m_ParticleName.c_str(), sizeof(char), ParticleNameLength, File);

	// Particle
	// m_Particle->Save(File);
	std::string SaveParticleFileFullPath;
	SaveParticleFileFullPath.reserve(100);

	const PathInfo* Info = CPathManager::GetInst()->FindPath(PARTICLE_PATH);

	if (Info)
		SaveParticleFileFullPath = Info->PathMultibyte;
	SaveParticleFileFullPath.append(m_Particle->GetName().c_str());

	// .ptrc 가 해당 Name에 있는지 확인 후, 없으면 Add
	if (SaveParticleFileFullPath.find(".prtc") == std::string::npos)
		SaveParticleFileFullPath.append(".prtc");

	m_Particle->SaveFile(SaveParticleFileFullPath.c_str());

	fwrite(&m_BillBoardEffect, sizeof(bool), 1, File);
	fwrite(&m_SpawnTimeMax, sizeof(float), 1, File);

	return true;
}

bool CParticleComponent::LoadOnly(FILE* File)
{
	// Mesh
	char	MeshName[256] = {};
	int	Length = 0;
	fread(&Length, sizeof(int), 1, File);
	fread(MeshName, sizeof(char), Length, File);
	m_Mesh = (CSpriteMesh*)m_Scene->GetResource()->FindMesh(MeshName);

	// Particle 생성
	m_Particle = CSceneManager::GetInst()->GetScene()->GetResource()->CreateParticleEmpty<CParticle>();
	// m_Particle->Load(File);

	char LoadedParticleName[MAX_PATH] = {};
	int	 ParticleNameLength = (int)m_ParticleName.length();
	fread(&ParticleNameLength, sizeof(int), 1, File);
	fread(LoadedParticleName, sizeof(char), ParticleNameLength, File);

	// Particle
	// m_Particle->Save(File);
	std::string LoadParticleFileFullPath;
	LoadParticleFileFullPath.reserve(100);

	const PathInfo* Info = CPathManager::GetInst()->FindPath(PARTICLE_PATH);

	if (Info)
		LoadParticleFileFullPath = Info->PathMultibyte;
	LoadParticleFileFullPath.append(LoadedParticleName);

	// .ptrc 가 해당 Name에 있는지 확인 후, 없으면 Add
	if (LoadParticleFileFullPath.find(".prtc") == std::string::npos)
		LoadParticleFileFullPath.append(".prtc");

	bool Result = m_Particle->LoadFile(LoadParticleFileFullPath.c_str());

	if (!Result)
	{
		assert(false);
	}

	m_ParticleName = m_Particle->GetName();

	SetParticle(m_Particle);

	// Load 한 Particle 은 Particle Manager 에 추가해준다.
	CResourceManager::GetInst()->GetParticleManager()->AddParticle(m_Particle);

	fread(&m_BillBoardEffect, sizeof(bool), 1, File);
	fread(&m_SpawnTimeMax, sizeof(float), 1, File);

	return true;
}
