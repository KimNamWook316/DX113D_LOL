#pragma once
#include "IMGUIBeginMenu.h"

class CResourceMenu :
    public CIMGUIBeginMenu
{
public:
    CResourceMenu();
    virtual ~CResourceMenu();
private :
    class CIMGUIMenuItem* m_ParticleEditorToggle;
    class CIMGUIMenuItem* m_AnimationEditorToggle;
    class CIMGUIMenuItem* m_MaterialEditorToggle;
public:
    virtual bool Init() override;
    virtual void Render() override;
private:
    void OnToggleParticleEditorCallback();
};

