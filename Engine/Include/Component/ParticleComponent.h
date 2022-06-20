#pragma once

#include "SceneComponent.h"
#include "../Resource/Particle/Particle.h"
#include "../Resource/Mesh/Mesh.h"
#include "../Resource/Shader/ParticleUpdateShader.h"
#include "../Resource/Shader/ParticleRenderShader.h"
#include "../Resource/Shader/ParticleConstantBuffer.h"
#include "../Resource/Material/Material.h"

class CParticleComponent :
    public CSceneComponent
{
	friend class CGameObject;

protected:
	CParticleComponent();
	CParticleComponent(const CParticleComponent& com);
	virtual ~CParticleComponent();

protected:
	CSharedPtr<CParticle>					m_Particle;
	std::vector<class CStructuredBuffer*>	m_vecStructuredBuffer;
	CSharedPtr<CMesh>						m_Mesh;
	CSharedPtr<CMaterial>					m_Material;
	CSharedPtr<CParticleUpdateShader>		m_UpdateShader;
	CParticleConstantBuffer*				m_CBuffer;
	ParticleInfo							m_Info;
	ParticleInfoShared						m_InfoShared;
	float									m_SpawnTime;
	float									m_SpawnTimeMax;
	bool  m_BillBoardEffect;
	std::string m_ParticleClassFileName;
public:
	void SetParticle(const std::string& Name);
	void SetParticle(CParticle* Particle);
	void SetSpawnTime(float Time);
public :
	CParticleConstantBuffer* GetCBuffer() const
	{
		return m_CBuffer;
	}
	CParticle* GetParticle() const
	{
		return m_Particle;
	}
public :
	void SetParticleClassFileName(const std::string& ParticleFileName)
	{
		m_ParticleClassFileName = ParticleFileName;
	}
	void SetMaterial(class CMaterial* Material)
	{
		m_Material = Material;
	}
	void SetBillBoardEffect(bool Enable)
	{
		m_BillBoardEffect = Enable;
	}
private :
	void ApplyBillBoardEffect();

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void PrevRender();
	virtual void Render();
	virtual void RenderParticleEffectEditor();
	virtual void PostRender();
	virtual CParticleComponent* Clone();
public :
	virtual bool Save(FILE* File);
	virtual bool Load(FILE* File);
private :
	virtual bool SaveOnly(FILE* File);
	virtual bool LoadOnly(FILE* File);
};

