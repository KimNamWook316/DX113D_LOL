#pragma once

#include "Particle.h"

class CParticleManager
{
	friend class CResourceManager;

private:
	CParticleManager();
	~CParticleManager();

private:
	std::unordered_map<std::string, CSharedPtr<CParticle>>	m_mapParticle;

public :
	const std::unordered_map<std::string, CSharedPtr<CParticle>>& GetMapParticle() const
	{
		return m_mapParticle;
	}
public:
	bool Init();
	bool CreateParticle(const std::string& Name);
	CParticle* FindParticle(const std::string& Name);
	void ReleaseParticle(const std::string& Name);
	void AddParticle(CParticle* Particle);

	template <typename T>
	T* CreateParticleEmpty()
	{
		T* Particle = new T;

		// Particle->SetConstantBuffer(m_CBuffer);

		return Particle;
	}
};

