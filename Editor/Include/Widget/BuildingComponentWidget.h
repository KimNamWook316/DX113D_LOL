#pragma once
#include "SceneComponentWidget.h"

class CBuildingComponentWidget :
    public CSceneComponentWidget
{
public:
	CBuildingComponentWidget();
	~CBuildingComponentWidget();

public:
	virtual bool Init() override;

public:
	virtual void SetSceneComponent(class CSceneComponent* Com) override;

private:
	int m_TowerOrder;
	LoLLine m_BuildingLine;
	Building_Type m_BuildingType;

	class CIMGUIInputInt* m_TowerOrderInput;
	class CIMGUIRadioButton* m_BuildingLineRadioButton;
	class CIMGUIRadioButton* m_BuildingTypeRadioButton;


private:
	void OnChangeTowerOrder(int Order);
	void OnChangeBuildingLine(const char* Label, bool Check);
	void OnChangeBuildingType(const char* Label, bool Check);
};

