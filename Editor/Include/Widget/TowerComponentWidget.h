#pragma once
#include "SceneComponentWidget.h"

class CTowerComponentWidget :
    public CSceneComponentWidget
{
public:
	CTowerComponentWidget();
	~CTowerComponentWidget();

public:
	virtual bool Init() override;

public:
	virtual void SetSceneComponent(class CSceneComponent* Com) override;

private:
	int m_TowerOrder;
	LoLLine m_TowerLine;
	class CIMGUIInputInt* m_TowerOrderInput;
	class CIMGUIRadioButton* m_TowerLineRadioButton;

private:
	void OnChangeTowerOrder(int Order);
	void OnChangeTowerLine(const char* Label, bool Check);
};

