#pragma once

#include "IMGUIWindow.h"

class CInspectorWindow :
    public CIMGUIWindow
{
public:
    CInspectorWindow();
    virtual ~CInspectorWindow();

public:
    virtual bool Init() override;

public:
    void OnSelectGameObject(class CGameObject* Obj);
    void OnClearGameObject();

private:
    class CGameObjectWidget* m_ObjWidget;
};

