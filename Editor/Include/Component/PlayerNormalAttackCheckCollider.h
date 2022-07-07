#pragma once

#include "Component/ColliderBox3D.h"

class CPlayerNormalAttackCheckCollider :
    public CColliderBox3D
{
    friend class CGameObject;

protected:
    CPlayerNormalAttackCheckCollider();
    CPlayerNormalAttackCheckCollider(const CPlayerNormalAttackCheckCollider& com);
    virtual ~CPlayerNormalAttackCheckCollider();

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void PrevRender();
    virtual void Render();
    virtual void PostRender();
    virtual CPlayerNormalAttackCheckCollider* Clone();
    virtual bool Save(FILE* File);
    virtual bool Load(FILE* File);
    virtual bool Collision(CColliderComponent* Dest);
    virtual bool CollisionMouse(const Vector2& MousePos);

public:
    void AttackSuccess(const CollisionResult& Result);
};

