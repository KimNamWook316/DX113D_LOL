#pragma once
#include "ConstantBufferBase.h"
#include "../../ParticleInfo.h"

class CParticleTempValConstantBuffer :
    public CConstantBufferBase
{
public:
	CParticleTempValConstantBuffer();
	CParticleTempValConstantBuffer(const CParticleTempValConstantBuffer& Buffer);
	virtual ~CParticleTempValConstantBuffer();

protected:
	ParticleTempValCBuffer	m_BufferData;

public:
	virtual bool Init();
	virtual void UpdateCBuffer();
	virtual CParticleTempValConstantBuffer* Clone();

public :
	void SetCommonParticleComponentWorldPos(const Vector3& WorldPos)
	{
		m_BufferData.CommonParticleComponentWorldPos = WorldPos;
	}
	void SetCommonWorldScale(const Vector3& Scale)
	{
		m_BufferData.CommonWorldScale = Scale;
	}
	void SetResetParticleSharedInfoSumSpawnCnt(bool Enable)
	{
		m_BufferData.ResetParticleSharedInfoSumSpawnCnt = Enable ? 1 : 0;
	}
};

