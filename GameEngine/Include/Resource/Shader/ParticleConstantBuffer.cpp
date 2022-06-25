
#include "ParticleConstantBuffer.h"
#include "ConstantBuffer.h"

CParticleConstantBuffer::CParticleConstantBuffer() :
	m_BufferData{}
{
	m_BufferData.SpawnCountMax = 100;
	m_BufferData.ColorMin = Vector4(1.f, 1.f, 1.f, 1.f);
	m_BufferData.ColorMax = Vector4(1.f, 1.f, 1.f, 1.f);
	m_BufferData.LifeTimeMin = 0.5f;
	m_BufferData.LifeTimeMax = 1.f;
	m_BufferData.ScaleMin = Vector3(30.f, 30.f, 1.f);
	m_BufferData.ScaleMax = Vector3(30.f, 30.f, 1.f);
	m_BufferData.SpeedMin = 1.f;
	m_BufferData.SpeedMax = 3.f;
	m_BufferData.StartMin = Vector3(-10.f, -10.f, 0.f);
	m_BufferData.StartMax = Vector3(10.f, 10.f, 0.f);

	m_BufferData.ParticleBounce = 0;
	m_BufferData.ParticleBounceResistance = 0.98f;

	m_BufferData.IsGenerateRing = 0;
	m_BufferData.GenerateRadius = 20.f;
	m_BufferData.IsLoopGenerateRing = 0;

	m_BufferData.AlphaMin = 1.f;
	m_BufferData.AlphaMax = 1.f;

	m_BufferData.IsGenerateCircle = 0;

	m_BufferData.IsGenerateTorch = 0;

	m_BufferData.IsMoveDirRandom = 0;
}

CParticleConstantBuffer::CParticleConstantBuffer(const CParticleConstantBuffer& Buffer) :
	CConstantBufferBase(Buffer)
{
	m_BufferData = Buffer.m_BufferData;
}

CParticleConstantBuffer::~CParticleConstantBuffer()
{
}

bool CParticleConstantBuffer::Init()
{
	SetConstantBuffer("ParticleCBuffer");

	return true;
}

void CParticleConstantBuffer::UpdateCBuffer()
{
	m_Buffer->UpdateBuffer(&m_BufferData);
}

CParticleConstantBuffer* CParticleConstantBuffer::Clone()
{
	return new CParticleConstantBuffer(*this);
}

void CParticleConstantBuffer::Save(FILE* File)
{
	// CRef::Save(File);
	fwrite(&m_BufferData, sizeof(ParticleCBuffer), 1, File);
}

void CParticleConstantBuffer::Load(FILE* File)
{
	// CRef::Load(File);

	fread(&m_BufferData, sizeof(ParticleCBuffer), 1, File);
}
