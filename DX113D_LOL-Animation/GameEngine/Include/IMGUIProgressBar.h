#pragma once

#include "IMGUIWidget.h"

class CIMGUIProgressBar :
    public CIMGUIWidget
{
	friend class CIMGUIWindow;
	friend class CIMGUIChild;
	friend class CIMGUIPopUpModal;
	friend class CIMGUITree;
	friend class CIMGUIWidgetList;
	friend class CIMGUIInputFloat3;
	friend class CIMGUIMainMenuBar;
	friend class CIMGUIBeginMenu;

private:
	CIMGUIProgressBar();
	virtual ~CIMGUIProgressBar();

public:
	virtual bool Init() override;
	virtual void Render() override;

public:
	void SetPercent(const float& Percent)
	{
		m_Percent = Percent;
	}

private:
	float m_Percent;
};

