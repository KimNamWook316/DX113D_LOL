#pragma once

#include "GameObject/GameObject.h"
#include "Component/NavMeshComponent.h"

class CNavMeshObject :
    public CGameObject
{
    friend class CScene;

protected:
    CNavMeshObject();
    CNavMeshObject(const CNavMeshObject& obj);
    virtual ~CNavMeshObject();

private:
    CSharedPtr<CNavMeshComponent>    m_NavMeshComponent;

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CNavMeshObject* Clone();
};

