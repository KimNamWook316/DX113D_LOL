#pragma once

#include "Component/SceneComponent.h"

class CBuildingComponent :
    public CSceneComponent
{
    friend class CGameObject;

protected:
    CBuildingComponent();
    CBuildingComponent(const CBuildingComponent& com);
    virtual ~CBuildingComponent();

private:
    LoLLine m_BuildingLine;
    int m_TowerOrder; // 포탑인 경우 몇차 포탑인지
    Building_Type m_BuildingType;

public:
    LoLLine GetBuildingLine()  const
    {
        return m_BuildingLine;
    }

    int GetTowerOrder() const
    {
        return m_TowerOrder;
    }

    Building_Type GetBuildingType() const
    {
        return m_BuildingType;
    }

    void SetBuildingLine(LoLLine Line)
    {
        m_BuildingLine = Line;
    }

    void SetTowerOrder(int Order)
    {
        m_TowerOrder = Order;
    }

    void SetBuildingType(Building_Type Type)
    {
        m_BuildingType = Type;
    }

public:
    virtual bool Save(FILE* File);
    virtual bool Load(FILE* File);
    virtual bool SaveOnly(FILE* File) override;
    virtual bool LoadOnly(FILE* File) override;
};

