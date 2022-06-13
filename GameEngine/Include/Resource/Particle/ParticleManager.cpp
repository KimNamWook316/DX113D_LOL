
#include "ParticleManager.h"

CParticleManager::CParticleManager()
{
}

CParticleManager::~CParticleManager()
{
	auto iter = m_mapParticle.begin();
	auto iterEnd = m_mapParticle.end();

	for (; iter != iterEnd;)
	{
		m_mapParticle.erase(iter);
	}

	m_mapParticle.empty();
}

bool CParticleManager::Init()
{
	return true;
}

bool CParticleManager::CreateParticle(const std::string& Name)
{
	CParticle* Particle = new CParticle;

	Particle->SetName(Name);

	if (!Particle->Init())
	{
		SAFE_RELEASE(Particle);
		return false;
	}

	m_mapParticle.insert(std::make_pair(Name, Particle));

	return true;
}

CParticle* CParticleManager::FindParticle(const std::string& Name)
{
	auto	iter = m_mapParticle.find(Name);

	if (iter == m_mapParticle.end())
		return nullptr;

	return iter->second;
}

void CParticleManager::ReleaseParticle(const std::string& Name)
{
	auto	iter = m_mapParticle.find(Name);

	if (iter != m_mapParticle.end())
	{
		if (iter->second->GetRefCount() == 1)
			m_mapParticle.erase(iter);
	}
}

void CParticleManager::AddParticle(CParticle* Particle)
{
	CParticle* FoundParticle = FindParticle(Particle->GetName());
	
	if (FoundParticle)
		return;

	m_mapParticle.insert(std::make_pair(Particle->GetName(), Particle));
}
