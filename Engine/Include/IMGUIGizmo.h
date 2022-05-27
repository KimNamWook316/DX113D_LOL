#pragma once

#include "IMGUIWidget.h"
#include "ImGuizmo.h"
#include "GameObject\GameObject.h"

class CIMGUIGizmo :
	public CIMGUIWidget
{
public:
	CIMGUIGizmo();
	~CIMGUIGizmo();

public:
	virtual bool Init() override;
	virtual void Render() override;

public:
	void SetGameObject(class CGameObject* Object)
	{
		m_Object = Object;
	}

	void SetOperationMode(ImGuizmo::OPERATION Operation)
	{
		m_Operation = Operation;
	}

	void SetMode(ImGuizmo::MODE Mode)
	{
		m_Mode = Mode;
	}

private:
	Matrix m_matWorld;

	ImGuizmo::OPERATION m_Operation;
	ImGuizmo::MODE m_Mode;
	class CGameObject* m_Object;

	bool m_Using;
};

