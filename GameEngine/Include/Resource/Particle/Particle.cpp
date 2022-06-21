
#include "Particle.h"
#include "../Shader/StructuredBuffer.h"
#include "../Shader/ShaderManager.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneResource.h"
#include "../../Scene/SceneManager.h"
#include "../ResourceManager.h"
#include "../Material/MaterialManager.h"
#include "../../PathManager.h"
#include "../../EngineUtil.h"

CParticle::CParticle()	:
	m_CBuffer(nullptr),
	m_SpawnTimeMax(0.001f),
	m_2D(true),
	m_SpawnCountMax(100),
	m_SaveLoadStruct{}
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
	// 1번째 Save Load
	CRef::Save(File);

	// 2번째 Save , Load => SaveLoad Struct
	m_SaveLoadStruct.MaterialEnable = m_Material ? true : false;
	strcpy_s(m_SaveLoadStruct.MaterialName, m_Material->GetName().c_str());

	m_SaveLoadStruct.UpdateShaderEnable = m_UpdateShader ? true : false;
	if (m_SaveLoadStruct.UpdateShaderEnable)
	{
		strcpy_s(m_SaveLoadStruct.UpdateShaderName, m_UpdateShader->GetName().c_str());
	}

	m_SaveLoadStruct.Info = m_Info;
	m_SaveLoadStruct.InfoShared = m_InfoShare;
	m_SaveLoadStruct.Is2D = m_2D;
	m_SaveLoadStruct.SpawnCountMax = m_SpawnCountMax;
	m_SaveLoadStruct.SpawnTimeMax = m_SpawnTimeMax;

	fwrite(&m_SaveLoadStruct, sizeof(ParticleSaveLoadStruct), 1, File);

	// 3번째 Save, Load => Material
	// 먼저 현재 저장하려는 Material 이 이미 하드디스크에 mtrl 형태로 존재하는지 확인
	const PathInfo* MaterialPath = CPathManager::GetInst()->FindPath(MATERIAL_PATH);

	bool MtrlFileExist = false;

	if (m_SaveLoadStruct.MaterialEnable)
	{
		char MaterialFileName[MAX_PATH] = {};

		strcpy_s(MaterialFileName, m_Material->GetName().c_str());
		strcat_s(MaterialFileName, ".mtrl");

		if (CEngineUtil::IsFileExistInDir(MATERIAL_PATH, MaterialFileName))
			MtrlFileExist = true;
	}

	// 현재 저장할 Material 이 Particle 에 있다면
	// if (m_SaveLoadStruct.MaterialEnable && MtrlFileExist == false) => 기존 Particle 용 Material 을 덮어써야 하는 경우도 있다.
	if (m_SaveLoadStruct.MaterialEnable)
	{
		// Material 을 Bin/Material Path 에 저장하기
		char MaterialBinPathMutlibyte[MAX_PATH] = {};

		if (MaterialPath)
			strcpy_s(MaterialBinPathMutlibyte, MaterialPath->PathMultibyte);

		strcat_s(MaterialBinPathMutlibyte, m_Material->GetName().c_str());
		strcat_s(MaterialBinPathMutlibyte, ".mtrl");

		// .mtrl 확장자가 붙어서 저장되도록 세팅해야 한다.
		m_Material->SaveFullPath(MaterialBinPathMutlibyte);
	}

	// 4번째 Save, Load : 상수 버퍼 저장 
	m_CBuffer->Save(File);


	return true;
}

bool CParticle::Load(FILE* File)
{
	// 1번째 Save Load
	CRef::Load(File);

	// 2번째 Save, Load
	fread(&m_SaveLoadStruct, sizeof(ParticleSaveLoadStruct), 1, File);

	// 3번째 Save, Load => Material 불러오기 
	// 3_1) Material 이름을 얻어와서 Material Manager 에서 찾기
	m_Material = CResourceManager::GetInst()->FindMaterial(m_SaveLoadStruct.MaterialName);

	// 3_2) 그래도 없으면, 하드디스크에서 찾기
	if (!m_Material)
	{
		const PathInfo* MaterialPath = CPathManager::GetInst()->FindPath(MATERIAL_PATH);

		// Material 을 Bin/Material Path 에 저장하기
		char MaterialBinPathMutlibyte[MAX_PATH] = {};

		if (MaterialPath)
			strcpy_s(MaterialBinPathMutlibyte, MaterialPath->PathMultibyte);

		strcat_s(MaterialBinPathMutlibyte, m_SaveLoadStruct.MaterialName);
		strcat_s(MaterialBinPathMutlibyte, ".mtrl");

		m_Material = CResourceManager::GetInst()->CreateMaterialEmpty<CMaterial>();

		m_Material->LoadFullPath(MaterialBinPathMutlibyte);

		// Material Manager 의 Material Map 에 추가
		// KeyName 은, Material 의 Name 로 되어 있을 것이다.
		CResourceManager::GetInst()->GetMaterialManager()->AddMaterial(m_Material);
	}

	m_UpdateShader = (CParticleUpdateShader*)CResourceManager::GetInst()->FindShader(m_SaveLoadStruct.UpdateShaderName);

	m_Info = m_SaveLoadStruct.Info;
	m_InfoShare = m_SaveLoadStruct.InfoShared;
	m_2D = m_SaveLoadStruct.Is2D;
	m_SpawnCountMax = m_SaveLoadStruct.SpawnCountMax;
	m_SpawnTimeMax = m_SaveLoadStruct.SpawnTimeMax;

	// 4번째 Save, Load : 상수 버퍼 저장 
	m_CBuffer = new CParticleConstantBuffer;
	m_CBuffer->Init();
	m_CBuffer->Load(File);


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

bool CParticle::SaveFile(const char* FullPath)
{
	FILE* File;

	fopen_s(&File, FullPath, "wb");

	if (!File)
		return false;

	bool Result = Save(File);

	fclose(File);

	return Result;
}

bool CParticle::LoadFile(const char* FullPath)
{
	FILE* File;

	fopen_s(&File, FullPath, "rb");

	if (!File)
		return false;

	bool Result = Load(File);

	fclose(File);

	return Result;
}

void CParticle::SetSpawnCountMax(unsigned int Count)
{
	m_CBuffer->SetSpawnCountMax(Count);

	m_SpawnCountMax = Count;

	ResizeBuffer("ParticleInfo", sizeof(ParticleInfo), m_SpawnCountMax, 0);
}