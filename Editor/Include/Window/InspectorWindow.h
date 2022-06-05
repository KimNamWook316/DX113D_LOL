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
private :
    // 중복하여, Scene Component Widget 이 추가됨을 방지하기 위한 로직
    bool FindSceneComponent(class CSceneComponent* Com);
private:
    class CGameObjectWidget* m_ObjWidget;
};

