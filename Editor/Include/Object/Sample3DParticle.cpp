
#include "Sample3DParticle.h"
#include "Component/ParticleComponent.h"

CSample3DParticle::CSample3DParticle()
{
	SetTypeID<CSample3DParticle>();
}

CSample3DParticle::CSample3DParticle(const CSample3DParticle& obj) :
	CGameObject(obj)
{
	m_Particle = (CParticleComponent*)FindComponent("Particle");
}

CSample3DParticle::~CSample3DParticle()
{
}

bool CSample3DParticle::Init()
{
	m_Particle = CreateComponent<CParticleComponent>("Particle");

	SetRootComponent(m_Particle);

	m_Particle->SetRelativePos(200.f, 50.f, 0.f);

	m_Particle->SetParticle("Bubble");

	SetLifeSpan(5.f);
	//m_Particle->SetSpawnTime(0.1f);

	return true;
}

