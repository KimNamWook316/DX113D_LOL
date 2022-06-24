#pragma once

#include "../../Ref.h"
#include "../Material/Material.h"
#include "../Shader/ParticleUpdateShader.h"
#include "../Shader/ParticleConstantBuffer.h"

struct ParticleSaveLoadStruct
{
	bool MaterialEnable;
	char MaterialName[MAX_PATH];
	bool UpdateShaderEnable;
	char UpdateShaderName[MAX_PATH];
	ParticleInfo Info;
	ParticleInfoShared InfoShared;
	bool Is2D;
	int SpawnCountMax;
	float SpawnTimeMax;
};

class CParticle :
	public CRef
{
	friend class CParticleManager;
	friend class  CEffectEditor;
private:
	CParticle();
	CParticle(const CParticle& particle);
	~CParticle();

private:
	class CScene* m_Scene;
	std::vector<class CStructuredBuffer*>   m_vecStructuredBuffer;
	CSharedPtr<CMaterial>                   m_Material;
	CSharedPtr<CParticleUpdateShader>		m_UpdateShader;
	ParticleInfo							m_Info;
	ParticleInfoShared						m_InfoShare;
	CParticleConstantBuffer*				m_CBuffer;
	float									m_SpawnTimeMax;
	bool									m_2D;
	int										m_SpawnCountMax;
	ParticleSaveLoadStruct       m_SaveLoadStruct;
private :
	// 정규 분포 형태의 Y 값들을 지닌 구조화 버퍼
	// 원소 개수 (X 축 원소 개수) : SpawnCount 만큼
	// Y값 범위 : 0 ~ 1 (정규분포)
	class CStructuredBuffer* m_NormalDistributionBuffer;
	std::vector<float>	m_vecNormalDistVal;
public:
	CMaterial* CloneMaterial()	const
	{
		return m_Material->Clone();
	}
	std::vector<float> GetVecNormalDistVal() 
	{
		return m_vecNormalDistVal;
	}
	CParticleUpdateShader* GetUpdateShader()	const
	{
		return m_UpdateShader;
	}

	CParticleUpdateShader* CloneUpdateShader()	const
	{
		return m_UpdateShader->Clone();
	}

	CParticleConstantBuffer* CloneConstantBuffer()	const
	{
		return m_CBuffer->Clone();
	}

	float GetSpawnTimeMax()	const
	{
		return m_SpawnTimeMax;
	}

public:
	bool Init();
	virtual bool Save(FILE* File) override;
	virtual bool Load(FILE* File) override;
public:
	void AddStructuredBuffer(const std::string& Name, unsigned int Size, unsigned int Count,
		int Register, bool Dynamic = false,
		int StructuredBufferShaderType = (int)Buffer_Shader_Type::Compute);
	void CreateNormalDistStructuredBuffer(const std::string& Name, unsigned int Size, unsigned int Count,
		int Register, bool Dynamic = false,
		int StructuredBufferShaderType = (int)Buffer_Shader_Type::Compute);
	bool ResizeBuffer(const std::string& Name, unsigned int Size, unsigned int Count,
		int Register, bool Dynamic = false,
		int StructuredBufferShaderType = (int)Buffer_Shader_Type::Compute);
	bool ResizeNormalDistStructuredBuffer(const std::string& Name, unsigned int Size, unsigned int Count,
		int Register, bool Dynamic = false,
		int StructuredBufferShaderType = (int)Buffer_Shader_Type::Compute);
	void CloneStructuredBuffer(std::vector<CStructuredBuffer*>& vecBuffer);
	void CloneNormalDistStructuredBuffer(CStructuredBuffer*& NormalDistBuffer);
private :
	// m_NormalDistributionBuffer 에 정규 분포 계산 값들을 적용하는 함수
	void GenerateNormalDistribution();
public :
	bool SaveFile(const char* FullPath);
	bool LoadFile(const char* FullPath);
public:
	CParticleConstantBuffer* GetCBuffer()	const
	{
		return m_CBuffer;
	}
	CMaterial* GetMaterial() const
	{
		return m_Material;
	}

	// int GetApplyRandom() const
	// {
	// 	return m_CBuffer->GetApplyRandom();
	// }

	int GetSpawnCountMax()	const
	{
		return m_SpawnCountMax;
	}
	const Vector3& GetStartMin()	const
	{
		return m_CBuffer->GetStartMin();
	}

	const Vector3& GetStartMax()	const
	{
		return m_CBuffer->GetStartMax();
	}

	int GetSpawnCount()	const
	{
		return m_CBuffer->GetSpawnCount();
	}
	bool GetSpawnEnable()
	{
		return m_CBuffer->GetSpawnEnable();
	}
	const Vector3& GetStartMin()
	{
		return m_CBuffer->GetStartMin();
	}

	const Vector3& GetStartMax()
	{
		return m_CBuffer->GetStartMax();
	}

	unsigned int GetSpawnCountMax()
	{
		return m_CBuffer->GetSpawnCountMax();
	}

	const Vector3& GetScaleMin()
	{
		return m_CBuffer->GetScaleMin();
	}

	const Vector3& GetScaleMax()
	{
		return m_CBuffer->GetScaleMax();
	}


	float GetLifeTimeMin()
	{
		return m_CBuffer->GetLifeTimeMin();
	}

	float GetLifeTimeMax()
	{
		return m_CBuffer->GetLifeTimeMax();
	}

	const Vector4& GetColorMin()
	{
		return m_CBuffer->GetColorMin();
	}

	const Vector4& GetColorMax()
	{
		return m_CBuffer->GetColorMax();
	}

	float GetSpeedMin()
	{
		return m_CBuffer->GetSpeedMin();
	}

	float GetSpeedMax()
	{
		return m_CBuffer->GetSpeedMax();
	}

	bool GetMove()
	{
		return m_CBuffer->GetMove();
	}

	bool GetGravity()
	{
		return m_CBuffer->GetGravity();
	}

	const Vector3& GetMoveDir()
	{
		return m_CBuffer->GetMoveDir();
	}

	bool Is2D()
	{
		return m_CBuffer->Is2D();
	}

	bool IsMoveDirRandom()
	{
		return m_CBuffer->IsMoveDirRandom();
	}

	const Vector3& GetMoveAngle()
	{
		return m_CBuffer->GetMoveAngle();
	}

	const Vector3& GetRotationAngle()
	{
		return m_CBuffer->GetRotationAngle();
	}
	// Bounce
	int IsBounceEnable() const
	{
		return m_CBuffer->IsBounceEnable();
	}
	float GetBounceResistance() const
	{
		return m_CBuffer->GetParticleBounceResist();
	}
	// Ring
	int IsGenerateRing() const
	{
		return m_CBuffer->IsGenerateRing();
	}
	float GetGenerateRingRadius() const
	{
		return m_CBuffer->GetGenerateRingRadius();
	}
	int IsLoopGenerateRing() const
	{
		return m_CBuffer->IsLoopGenerateRing();
	}
	// Circle
	int IsGenerateCircle() const
	{
		return m_CBuffer->IsGenerateCircle();
	}
	float GetGenerateCircleRadius() const
	{
		return m_CBuffer->GetGenerateCircleRadius();
	}
	// Torch
	int IsGenerateTorch() const
	{
		return m_CBuffer->IsGenerateTorch();
	}
	float GetGenerateTorchRadius() const
	{
		return m_CBuffer->GetGenerateTorchRadius();
	}
	// Alpha
	float GetMinAlpha() const
	{
		return m_CBuffer->GetMinAlpha();
	}
	float GetMaxAlpha() const
	{
		return m_CBuffer->GetMaxAlpha();
	}
public:
	// Alpha
	void SetMinAlpha(float Alpha) 
	{
		m_CBuffer->SetMinAlpha(Alpha);
	}
	void SetMaxAlpha(float Alpha)
	{
		m_CBuffer->SetMaxAlpha(Alpha);
	}
	// Ring
	void SetLoopGenerateRing(bool Enable)
	{
		m_CBuffer->SetLoopGenerateRing(Enable);
	}
	void SetGenerateRingEnable(bool Enable)
	{
		m_CBuffer->SetGenerateRingEnable(Enable);
	}
	void SetGenerateRingRadius(float Radius)
	{
		m_CBuffer->SetGenerateRingRadius(Radius);
	}
	// Torch
	void SetGenerateTorchEnable(bool Enable)
	{
		m_CBuffer->SetGenerateTorchEnable(Enable);
	}
	void SetGenerateTorchRadius(float Radius)
	{
		m_CBuffer->SetGenerateTorchRadius(Radius);
	}
	// Circle
	void SetGenerateCircleEnable(bool Enable)
	{
		m_CBuffer->SetGenerateCircleEnable(Enable);
	}
	void SetGenerateCircleRadius(float Radius)
	{
		m_CBuffer->SetGenerateCircleRadius(Radius);
	}
	// Bounce
	void SetBounceEnable(bool Enable)
	{
		m_CBuffer->SetBounceEnable(Enable);
	}
	void SetBounceResistance(float Resist)
	{
		m_CBuffer->SetBounceResist(Resist);
	}
	void SetMaterial(CMaterial* Material)
	{
		m_Material = Material;
	}
	void SetSpawnTimeMax(float SpawnTime)
	{
		m_SpawnTimeMax = SpawnTime;
	}

	void SetSpawnEnable(unsigned int Count)
	{
		m_CBuffer->SetSpawnEnable(Count);
	}

	void SetStartMin(const Vector3& StartMin)
	{
		m_CBuffer->SetStartMin(StartMin);
	}

	void SetStartMax(const Vector3& StartMax)
	{
		m_CBuffer->SetStartMax(StartMax);
	}

	void SetRotationAngle(const Vector3& Angle)
	{
		m_CBuffer->SetRotationAngle(Angle);
	}

	void SetSpawnCountMax(unsigned int Count);

	void SetScaleMin(const Vector3& ScaleMin)
	{
		m_CBuffer->SetScaleMin(ScaleMin);
	}

	void SetScaleMax(const Vector3& ScaleMax)
	{
		m_CBuffer->SetScaleMax(ScaleMax);
	}

	void SetLifeTimeMin(float Min)
	{
		m_CBuffer->SetLifeTimeMin(Min);
	}

	void SetLifeTimeMax(float Max)
	{
		m_CBuffer->SetLifeTimeMax(Max);
	}

	void SetColorMin(const Vector4& ColorMin)
	{
		m_CBuffer->SetColorMin(ColorMin);
	}

	void SetColorMin(float r, float g, float b, float a)
	{
		m_CBuffer->SetColorMin(r, g, b, a);
	}

	void SetColorMax(const Vector4& ColorMax)
	{
		m_CBuffer->SetColorMax(ColorMax);
	}

	void SetColorMax(float r, float g, float b, float a)
	{
		m_CBuffer->SetColorMax(r, g, b, a);
	}

	void SetSpeedMin(float SpeedMin)
	{
		m_CBuffer->SetSpeedMin(SpeedMin);
	}

	void SetSpeedMax(float SpeedMax)
	{
		m_CBuffer->SetSpeedMax(SpeedMax);
	}

	void SetMove(bool Move)
	{
		m_CBuffer->SetMove(Move);
	}

	void SetIsRandomMoveDir(bool Random)
	{
		m_CBuffer->SetIsRandomMoveDir(Random);
	}

	void SetGravity(bool Gravity)
	{
		m_CBuffer->SetGravity(Gravity);
	}

	void SetMoveDir(const Vector3& MoveDir)
	{
		m_CBuffer->SetMoveDir(MoveDir);
	}

	// void SetApplyRandom(bool Enable)
	// {
	// 	m_CBuffer->SetApplyRandom(Enable);
	// }

	void Set2D(bool Is2D)
	{
		m_2D = Is2D;
		m_CBuffer->Set2D(Is2D);
	}

	void SetMoveAngle(const Vector3& MoveAngle)
	{
		m_CBuffer->SetMoveAngle(MoveAngle);
	}
};
