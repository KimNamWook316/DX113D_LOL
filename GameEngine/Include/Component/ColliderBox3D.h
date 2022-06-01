#pragma once
#include "ColliderComponent.h"

class CColliderBox3D :
    public CColliderComponent
{
    friend class CGameObject;

protected:
    CColliderBox3D();
    CColliderBox3D(const CColliderBox3D& com);
    virtual ~CColliderBox3D();

protected:
    Box3DInfo   m_Info;

public:
    Box3DInfo GetInfo() const
    {
        return m_Info;
    }

    void SetExtent(float X, float Y, float Z)
    {
        m_Info.AxisLen[0] = X;
        m_Info.AxisLen[1] = Y;
        m_Info.AxisLen[2] = Z;

        SetWorldScale(m_Info.AxisLen[0] * 2.f, m_Info.AxisLen[1] * 2.f, m_Info.AxisLen[2] * 2.f);
    }


public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void PrevRender();
    virtual void Render();
    virtual void PostRender();
    virtual CColliderBox3D* Clone();
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);
    virtual bool Collision(CColliderComponent* Dest);
    virtual bool CollisionMouse(const Vector2& MousePos);
};

