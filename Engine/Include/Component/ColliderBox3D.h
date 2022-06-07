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
    virtual void SetOffset(const Vector3& Offset)
    {
        CColliderComponent::SetOffset(Offset);
        m_Info.Min += Offset;
        m_Info.Max += Offset;
    }

    virtual void SetOffset(float x, float y, float z)
    {
        CColliderComponent::SetOffset(x, y, z);
        m_Info.Min += Vector3(x, y, z);
        m_Info.Max += Vector3(x, y, z);
    }

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

    Vector3 GetExtent() const
    {
        Vector3 Extent;

        Extent.x = m_Info.AxisLen[0];
        Extent.y = m_Info.AxisLen[1];
        Extent.z = m_Info.AxisLen[2];

        return Extent;
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
    virtual bool Save(FILE* File);
    virtual bool Load(FILE* File);
    virtual bool Collision(CColliderComponent* Dest);
    virtual bool CollisionMouse(const Vector2& MousePos);
};

