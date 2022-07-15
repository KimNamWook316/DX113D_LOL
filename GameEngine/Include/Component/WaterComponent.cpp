#include "WaterComponent.h"
#include "../Resource/Shader/WaterCBuffer.h"
#include "../Scene/Scene.h"

CWaterComponent::CWaterComponent()
{
	SetTypeID<CWaterComponent>();
	
	m_CBuffer = new CWaterCBuffer;
	m_CBuffer->Init();

	m_LayerName = "PostParticle";
	m_Render = true;
}

CWaterComponent::CWaterComponent(const CWaterComponent& com)	:
	CSceneComponent(com)
{
	m_Mesh = com.m_Mesh;
	m_Material = com.m_Material->Clone();
	m_CBuffer = com.m_CBuffer->Clone();
}

CWaterComponent::~CWaterComponent()
{
	SAFE_DELETE(m_CBuffer);
}

void CWaterComponent::Start()
{
	CSceneComponent::Start();

	if (!m_Mesh)
	{
		m_Mesh = m_Scene->GetResource()->FindMesh("DefaultPlane");
	}
}

bool CWaterComponent::Init()
{
	CSceneComponent::Init();

	m_Mesh = m_Scene->GetResource()->FindMesh("DefaultPlane");

	return true;
}

void CWaterComponent::Render()
{
	CSceneComponent::Render();

	if (m_Material)
	{
		m_Material->Render();
		m_CBuffer->UpdateCBuffer();
		m_Mesh->Render();
		m_Material->Reset();
	}
}

CWaterComponent* CWaterComponent::Clone()
{
	return new CWaterComponent(*this);
}

bool CWaterComponent::Save(FILE* File)
{
	CSceneComponent::Save(File);

	float Speed = m_CBuffer->GetSpeed();
	float FoamDepth = m_CBuffer->GetFoamDepth();

	fwrite(&Speed, sizeof(float), 1, File);
	fwrite(&FoamDepth, sizeof(float), 1, File);

	std::string MaterialName = m_Material->GetName();
	int Length = (int)MaterialName.length();
	fwrite(&Length, sizeof(int), 1, File);
	fwrite(MaterialName.c_str(), sizeof(char), Length, File);

	m_Material->Save(File);

	return true;
}

bool CWaterComponent::Load(FILE* File)
{
	CSceneComponent::Load(File);

	float Speed, FoamDepth;

	fread(&Speed, sizeof(float), 1, File);
	fread(&FoamDepth, sizeof(float), 1, File);

	char MatName[128] = {};
	int Length = 0;
	fread(&Length, sizeof(int), 1, File);
	fread(MatName, sizeof(char), Length, File);

	bool Success = m_Scene->GetResource()->CreateMaterial<CMaterial>(MatName);

	if (!Success)
	{
		return false;
	}

	m_Material = m_Scene->GetResource()->FindMaterial(MatName);

	m_Material->Load(File);

	return true;
}

bool CWaterComponent::SaveOnly(FILE* File)
{
	CSceneComponent::SaveOnly(File);

	float Speed = m_CBuffer->GetSpeed();
	float FoamDepth = m_CBuffer->GetFoamDepth();

	fwrite(&Speed, sizeof(float), 1, File);
	fwrite(&FoamDepth, sizeof(float), 1, File);

	std::string MaterialName;
	int Length = (int)MaterialName.length();
	fwrite(&Length, sizeof(int), 1, File);
	fwrite(MaterialName.c_str(), sizeof(char), Length, File);

	m_Material->Save(File);


	return true;
}

bool CWaterComponent::LoadOnly(FILE* File)
{
	CSceneComponent::LoadOnly(File);

	float Speed, DistortThreshold;
	int DepthOffsetX, DepthOffsetY;

	fread(&Speed, sizeof(float), 1, File);
	fread(&DistortThreshold, sizeof(float), 1, File);
	fread(&DepthOffsetX, sizeof(int), 1, File);
	fread(&DepthOffsetY, sizeof(int), 1, File);

	char MatName[128] = {};
	int Length = 0;
	fread(&Length, sizeof(int), 1, File);
	fread(MatName, sizeof(char), Length, File);

	bool Success = m_Scene->GetResource()->CreateMaterial<CMaterial>(MatName);

	if (!Success)
	{
		return false;
	}

	m_Material = m_Scene->GetResource()->FindMaterial(MatName);

	return true;
}

void CWaterComponent::SetMaterial(CMaterial* Mat)
{
	m_Material = Mat;
	m_Material->SetShader("WaterShader");
	m_Material->SetTransparency(true);
	m_Material->EnableBump();
}

void CWaterComponent::SetSpeed(float Speed)
{
	m_CBuffer->SetSpeed(Speed);
}

void CWaterComponent::SetFoamDepth(float Depth)
{
	m_CBuffer->SetFoamDepth(Depth);
}

float CWaterComponent::GetSpeed() const
{
	return m_CBuffer->GetSpeed();
}

float CWaterComponent::GetFoamDepth() const
{
	return m_CBuffer->GetFoamDepth();
}
