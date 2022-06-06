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
    void OnCreateSceneComponent(class CSceneComponent* Com);
    void OnCreateObjectComponent(class CObjectComponent* Com);
    void OnClearGameObject();
    void OnDeleteSceneComponent(class CSceneComponent* Com);
    void OnDeleteGameObject();

private:
    class CGameObjectWidget* m_ObjWidget;
};

