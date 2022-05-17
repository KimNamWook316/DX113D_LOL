#pragma once

#include "IMGUIWindow.h"

class CObjectComponentWindow :
    public CIMGUIWindow
{
public:
	CObjectComponentWindow();
	virtual ~CObjectComponentWindow();

private:
	class CIMGUIButton*					m_ComponentCreatePopUpButton;
	class CObjectComponentCreateModal*	m_ComponentCreateModal;
	class CIMGUIListBox*				m_ComponentListBox;


public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

public:
	// Component 생성 팝업창 띄워주는 콜백
	void OnCreateComponentPopUp();
	std::string GetComponentNameInput()	const;

public:
	// List에서 Select된 Component 선형 탐색
	//void FindSelectComponent(class CIMGUITree* RootNode);
};

