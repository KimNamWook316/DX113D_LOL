#pragma once
#include "GameObject\GameObject.h"
#include "Component/StaticMeshComponent.h"

class CMapObj :
    public CGameObject
{
    friend class CScene;

protected:
    CMapObj();
    CMapObj(const CMapObj& obj);
    virtual ~CMapObj();

private:
    CSharedPtr<CStaticMeshComponent>    m_Mesh;

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CMapObj* Clone();
};

