#pragma once
#include "Component\ColliderSphere.h"

class CArrowCollisionFireCollider :
    public CColliderSphere
{
    friend class CGameObject;

protected:
    CArrowCollisionFireCollider();
    CArrowCollisionFireCollider(const CArrowCollisionFireCollider& com);
    virtual ~CArrowCollisionFireCollider();

public:
    virtual void Start() override;
    virtual void Update(float DeltaTime) override;

public:
    void OnCollidePlayerArrow(const CollisionResult& Result);

};

