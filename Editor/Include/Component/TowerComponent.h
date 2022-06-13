#pragma once

#include "Component/SceneComponent.h"

class CTowerComponent :
    public CSceneComponent
{
    friend class CGameObject;

protected:
    CTowerComponent();
    CTowerComponent(const CTowerComponent& com);
    virtual ~CTowerComponent();

private:
    LoLLine m_TowerLine;
    int m_TowerOrder; // 몇차 포탑인지

public:
    LoLLine GetTowerLine()  const
    {
        return m_TowerLine;
    }

    int GetTowerOrder() const
    {
        return m_TowerOrder;
    }

    void SetTowerLine(LoLLine Line)
    {
        m_TowerLine = Line;
    }

    void SetTowerOrder(int Order)
    {
        m_TowerOrder = Order;
    }

public:
    virtual bool Save(FILE* File);
    virtual bool Load(FILE* File);
    virtual bool SaveOnly(FILE* File) override;
    virtual bool LoadOnly(FILE* File) override;
};

