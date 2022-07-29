#pragma once

#include "Component\ColliderBox3D.h"

class CCrackedBlockCollider :
    public CColliderBox3D
{
    friend class CGameObject;

protected:
    CCrackedBlockCollider();
    CCrackedBlockCollider(const CCrackedBlockCollider& com);
    virtual ~CCrackedBlockCollider();

public:
    virtual void Start() override;
    virtual void Update(float DeltaTime) override;

private:
    bool m_Reverse;
    float m_Timer;
    float m_ColorChangeTimer;
    class CStaticMeshComponent* m_Mesh;
};

