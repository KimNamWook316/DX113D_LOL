#pragma once

#include "IMGUIWidget.h"

class CIMGUIGrid :
    public CIMGUIWidget
{
	friend class CIMGUIWindow;

private:
	CIMGUIGrid();
	~CIMGUIGrid();

public:
	virtual bool Init() override;
	virtual void Render() override;

private:
	Matrix m_matIdentity;
};

