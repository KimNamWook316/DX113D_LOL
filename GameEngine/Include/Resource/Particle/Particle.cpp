
#include "Particle.h"
#include "../Shader/StructuredBuffer.h"
#include "../Shader/ShaderManager.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneResource.h"
#include "../../Scene/SceneManager.h"
#include "../ResourceManager.h"

CParticle::CParticle()	:
	m_CBuffer(nullptr),
	m_SpawnTime(0.001f),
	m_2D(true),
	m_SpawnCountMax(100)
{
}

CParticle::CParticle(const CParticle& particle)
{
	m_CBuffer = particle.m_CBuffer->Clone();
	m_UpdateShader = particle.m_UpdateShader;
}

CParticle::~CParticle()
{
	size_t	BufferCount = m_vecStructuredBuffer.size();

	for (size_t i = 0; i < BufferCount; ++i)
	{
		SAFE_DELETE(m_vecStructuredBuffer[i]);
	}

	SAFE_DELETE(m_CBuffer);
}

bool CParticle::Init()
{
	m_CBuffer = new CParticleConstantBuffer;

	if (!m_CBuffer->Init())
		return false;

	if (m_Scene)
		m_UpdateShader = (CParticleUpdateShader*)m_Scene->GetResource()->FindShader("ParticleUpdateShader");

	else
		m_UpdateShader = (CParticleUpdateShader*)CResourceManager::GetInst()->FindShader("ParticleUpdateShader");

	AddStructuredBuffer("ParticleInfo", sizeof(ParticleInfo), m_SpawnCountMax, 0);
	AddStructuredBuffer("ParticleInfoShared", sizeof(ParticleInfoShared), 1, 1);

	return true;
}

bool CParticle::Save(FILE* File)
{
	CRef::Save(File);

	bool MaterialEnable = false;
	if (m_Material)
		MaterialEnable = true;
	fwrite(&MaterialEnable, sizeof(bool), 1, File);

	if (MaterialEnable)
	{
		m_Material->Save(File);
	}

	bool UpdateShaderEnable = false;
	if (m_UpdateShader)
		UpdateShaderEnable = true;
	fwrite(&UpdateShaderEnable, sizeof(bool), 1, File);

	if (m_UpdateShader)
	{
		// 이름 저장
		std::string	UpdateShaderName = m_UpdateShader->GetName();
		int	 ShaderNameLength = (int)UpdateShaderName.length();
		fwrite(&ShaderNameLength, sizeof(int), 1, File);
		fwrite(UpdateShaderName.c_str(), sizeof(char), ShaderNameLength, File);
	}

	fwrite(&m_Info, sizeof(ParticleInfo), 1, File);
	fwrite(&m_InfoShare, sizeof(ParticleInfoShared), 1, File);

	// 상수 버퍼 저장 
	m_CBuffer->Save(File);

	fwrite(&m_2D, sizeof(bool), 1, File);
	fwrite(&m_SpawnCountMax, sizeof(int), 1, File);

	return true;
}

bool CParticle::Load(FILE* File)
{
	CRef::Load(File);

	// Material
	bool	MaterialEnable = false;
	fread(&MaterialEnable, sizeof(bool), 1, File);

	if (MaterialEnable)
	{
		m_Material = CSceneManager::GetInst()->GetScene()->GetResource()->CreateMaterialEmpty<CMaterial>();
		m_Material->Load(File);
	}

	bool UpdateShaderEnable = false;
	fread(&UpdateShaderEnable, sizeof(bool), 1, File);

	if (UpdateShaderEnable)
	{
		// 이름 저장
		char	ShaderName[256] = {};
		int	 ShaderNameLength = 0;
		fread(&ShaderNameLength, sizeof(int), 1, File);
		fread(ShaderName, sizeof(char), ShaderNameLength, File);

		m_UpdateShader = (CParticleUpdateShader*)CResourceManager::GetInst()->FindShader(ShaderName);
	}

	fread(&m_Info, sizeof(ParticleInfo), 1, File);
	fread(&m_InfoShare, sizeof(ParticleInfoShared), 1, File);

	// 상수 버퍼 저장 X
	m_CBuffer = new CParticleConstantBuffer;
	m_CBuffer->Init();
	// 상수 버퍼 Load
	m_CBuffer->Load(File);

	m_SpawnTime = 0;
	fread(&m_2D, sizeof(bool), 1, File);
	fread(&m_SpawnCountMax, sizeof(int), 1, File);


	// 구조화 버퍼 세팅
	AddStructuredBuffer("ParticleInfo", sizeof(ParticleInfo), m_SpawnCountMax, 0);
	AddStructuredBuffer("ParticleInfoShared", sizeof(ParticleInfoShared), 1, 1);

	return true;
}

void CParticle::AddStructuredBuffer(const std::string& Name, unsigned int Size, unsigned int Count, 
	int Register, bool Dynamic, int StructuredBufferShaderType)
{
	CStructuredBuffer* Buffer = new CStructuredBuffer;

	if (!Buffer->Init(Name, Size, Count, Register, Dynamic, StructuredBufferShaderType))
	{
		SAFE_DELETE(Buffer);
		return;
	}

	m_vecStructuredBuffer.push_back(Buffer);

}

bool CParticle::ResizeBuffer(const std::string& Name, unsigned int Size, unsigned int Count,
	int Register, bool Dynamic, int StructuredBufferShaderType)
{
	size_t	BufferCount = m_vecStructuredBuffer.size();

	for (size_t i = 0; i < BufferCount; ++i)
	{
		if (m_vecStructuredBuffer[i]->GetName() == Name)
		{
			m_vecStructuredBuffer[i]->Init(Name, Size, Count, Register, Dynamic, StructuredBufferShaderType);

			return true;
		}
	}

	return false;
}

void CParticle::CloneStructuredBuffer(std::vector<CStructuredBuffer*>& vecBuffer)
{
	size_t	BufferCount = m_vecStructuredBuffer.size();

	for (size_t i = 0; i < BufferCount; ++i)
	{
		CStructuredBuffer* Buffer = m_vecStructuredBuffer[i]->Clone();

		vecBuffer.push_back(Buffer);
	}
}

void CParticle::SetSpawnCountMax(unsigned int Count)
{
	m_CBuffer->SetSpawnCountMax(Count);

	m_SpawnCountMax = Count;

	ResizeBuffer("ParticleInfo", sizeof(ParticleInfo), m_SpawnCountMax, 0);
}