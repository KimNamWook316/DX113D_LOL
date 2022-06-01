#pragma once
#include "GameObject\GameObject.h"
class CSample3DParticle :
    public CGameObject
{
    friend class CScene;

protected:
    CSample3DParticle();
    CSample3DParticle(const CSample3DParticle& obj);
    virtual ~CSample3DParticle();

private:
    CSharedPtr<class CParticleComponent>    m_Particle;

public:
    virtual bool Init();
};

