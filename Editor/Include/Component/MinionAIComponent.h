#pragma once

#include "Component/ObjectComponent.h"

class CMinionAIComponent :
    public CObjectComponent
{
    friend class CGameObject;

public:
    CMinionAIComponent();
    virtual ~CMinionAIComponent();
    CMinionAIComponent(const CMinionAIComponent& Com);

private:
    Minion_Type     m_MinionType;
    CGameObject*    m_Target;
    bool            m_OnFight; // 라인에서 상대방 미니언이나 챔피언과 전투중이다 -> 이때는 최종 목표 지점인 상대방 넥서스까지 가는 행위를 잠깐 멈추고 전투한다

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void PrevRender();
    virtual void Render();
    virtual void PostRender();
    virtual CMinionAIComponent* Clone();


public:
    void SetMinionType(const Minion_Type Type)
    {
        m_MinionType = Type;
    }

    const Minion_Type& GetMinionType()  const
    {
        return m_MinionType;
    }

    void SetOnFight(bool Fight)
    {
        m_OnFight = Fight;
    }



public:
    virtual bool Save(FILE* File);
    virtual bool Load(FILE* File);
    virtual bool SaveOnly(FILE* File) override;
    virtual bool LoadOnly(FILE* File) override;

public:
    void OnAttack(const CollisionResult& Result);
    void OnTrack(const CollisionResult& Result);
};

