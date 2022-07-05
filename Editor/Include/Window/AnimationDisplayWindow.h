#pragma once
#include "IMGUIWindow.h"

class CAnimationDisplayWindow :
    public CIMGUIWindow
{
    friend class CEditorManager;
public:
    CAnimationDisplayWindow();
    ~CAnimationDisplayWindow();
public:
    virtual bool Init() override;
	// Animation Render Target
private:
    class CIMGUIImage* m_AnimationRenderTarget;

};

