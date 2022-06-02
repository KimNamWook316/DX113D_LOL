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
		m_Component = Object->GetRootComponent();
	}

	void SetComponent(class CSceneComponent* Component)
	{
		m_Component = Component;
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

	class CSceneComponent* m_Component;

	bool m_Using;
};

