#pragma once
#include "SceneComponentWidget.h"

class CParticleComponentWidget :
    public CSceneComponentWidget
{
public:
    CParticleComponentWidget();
    virtual ~CParticleComponentWidget();


private:
    class CIMGUITextInput* m_MeshName;
    class CIMGUIButton* m_LoadMeshButton;

    class CIMGUIComboBox* m_MaterialSlotCombo;
    class CIMGUIColor3* m_BaseColorEdit;
    class CIMGUIColor3* m_AmbientColorEdit;
    class CIMGUIColor3* m_SpecularColorEdit;
    class CIMGUIInputFloat* m_SpecluarPowerEdit;
    class CIMGUIColor3* m_EmissiveColorEdit;
    class CIMGUICheckBox* m_TransparencyEdit;
    class CIMGUISliderFloat* m_OpacityEdit;
};

