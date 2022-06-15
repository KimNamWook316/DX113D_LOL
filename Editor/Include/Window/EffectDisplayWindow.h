#pragma once
#include "IMGUIWindow.h"

class CEffectDisplayWindow :
    public CIMGUIWindow
{
    friend class CEditorManager;
public:
    CEffectDisplayWindow();
    ~CEffectDisplayWindow();
public:
    virtual bool Init() override;
private :
    class CIMGUIImage* m_ParticleRenderTarget;
};

