#pragma once

#include "../../Ref.h"
#include "../Material/Material.h"
#include "../Shader/ParticleUpdateShader.h"
#include "../Shader/ParticleConstantBuffer.h"

class CParticle :
	public CRef
{
	friend class CParticleManager;

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
	float									m_SpawnTime;
	bool									m_2D;
	int										m_SpawnCountMax;

public:
	CMaterial* CloneMaterial()	const
	{
		return m_Material->Clone();
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

	float GetSpawnTime()	const
	{
		return m_SpawnTime;
	}

public:
	bool Init();
	virtual bool Save(FILE* File) override;
	virtual bool Load(FILE* File) override;
public:
	void AddStructuredBuffer(const std::string& Name, unsigned int Size, unsigned int Count,
		int Register, bool Dynamic = false,
		int StructuredBufferShaderType = (int)Buffer_Shader_Type::Compute);
	bool ResizeBuffer(const std::string& Name, unsigned int Size, unsigned int Count,
		int Register, bool Dynamic = false,
		int StructuredBufferShaderType = (int)Buffer_Shader_Type::Compute);
	void CloneStructuredBuffer(std::vector<CStructuredBuffer*>& vecBuffer);

public:
	CParticleConstantBuffer* GetCBuffer()	const
	{
		return m_CBuffer;
	}

	int GetApplyRandom() const
	{
		return m_CBuffer->GetApplyRandom();
	}

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

	const Vector3& GetRotationAngle()
	{
		return m_CBuffer->GetRotationAngle();
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

	const Vector3& GetMoveAngle()
	{
		return m_CBuffer->GetMoveAngle();
	}
public:
	void SetMaterial(CMaterial* Material)
	{
		m_Material = Material;
	}

	void SetSpawnTime(float SpawnTime)
	{
		m_SpawnTime = SpawnTime;
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

	void SetColorMax(const Vector4& ColorMax)
	{
		m_CBuffer->SetColorMax(ColorMax);
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

	void SetGravity(bool Gravity)
	{
		m_CBuffer->SetGravity(Gravity);
	}

	void SetMoveDir(const Vector3& MoveDir)
	{
		m_CBuffer->SetMoveDir(MoveDir);
	}

	void SetApplyRandom(bool Enable)
	{
		m_CBuffer->SetApplyRandom(Enable);
	}

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

