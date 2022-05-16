#pragma once

#include "IMGUIWindow.h"

class CComponentHierarchyWindow :
    public CIMGUIWindow
{
public:
	CComponentHierarchyWindow();
	virtual ~CComponentHierarchyWindow();

private:
	std::vector<class CIMGUITree*>	m_vecComponentTree;
	class CIMGUIButton*				m_ComponentCreatePopUpButton;
	class CComponentCreateModal*		m_ComponentCreateModal;

	class CIMGUIButton*				m_ComponentDelButton;
	class CIMGUIButton*				m_ComponentDelHierarchyButton;
	class CIMGUITree*				m_Root;
	class CIMGUITree*				m_SelectNode;

public:
	class CIMGUITree* GetRoot()	const;
	class CIMGUITree* GetSelectNode()	const
	{
		return m_SelectNode;
	}

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

public:
	// Component 생성 팝업창 띄워주는 콜백
	void OnCreateComponentPopUp();
	// Delete Button 누르면 호출되는 콜백
	void OnDeleteComponent();
	// Delete Hierarchy
	void OnDeleteHierarchyComponent();

public:
	// 2번째 인자에 지금 선택되어 있는 노드를 넣어주는 함수
	// 위에서 아래로 탐색하므로 1번째 인자에는 루트 노드를 넣어줘야함
	void FindSelectNode(class CIMGUITree* RootNode);
};

