#pragma once
#include "IMGUIPopUpModal.h"

class CParticlePopUpScreen :
    public CIMGUIPopUpModal
{
public:
	CParticlePopUpScreen();
	virtual ~CParticlePopUpScreen();

private:
	//class CIMGUIPopUpModal* m_ObjectCreatePopUp;
	class CIMGUIImage* m_ParticleRenderTarget;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

};

