#pragma once
#include "ObjectComponentWidget.h"

class CMinionAIComponentWidget :
    public CObjectComponentWidget
{
public:
    CMinionAIComponentWidget();
    virtual ~CMinionAIComponentWidget();

public:
    virtual bool Init() override;

public:
    // 반드시 Init 이후 이 함수 호출해야 함
    virtual void SetObjectComponent(class CObjectComponent* Com);

private:
    class CIMGUITree* m_RootTree;
    class CIMGUIComboBox* m_MinionTypeCombo;

private:
    Minion_Type m_MinionType;

public:
    void OnChangeMinionType(int Index, const char* Label);
};

