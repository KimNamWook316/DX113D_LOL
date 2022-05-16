#pragma once

#include "IMGUIWindow.h"

class CSceneComponentHierarchyWindow :
    public CIMGUIWindow
{
public:
	CSceneComponentHierarchyWindow();
	virtual ~CSceneComponentHierarchyWindow();

private:
	std::vector<class CIMGUITree*>	m_vecComponentTree;
	class CIMGUIButton*				m_ComponentCreatePopUpButton;
	class CSceneComponentCreateModal*	m_ComponentCreateModal;
	class CIMGUIButton*					m_ComponentDeleteButton;

	class CIMGUITree* m_Root;
	class CIMGUITree* m_SelectNode;
	class CIMGUITree* m_DragSrc;
	class CIMGUITree* m_DragDest;

public:
	class CIMGUITree* GetRoot()	const;

	class CIMGUITree* GetDragSrc()	const;
	class CIMGUITree* GetDragDest()	const;

	void SetDragSrc(class CIMGUITree* DragSrc);
	void SetDragDest(class CIMGUITree* DragDest);

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

public:
	// Component 생성 팝업창 띄워주는 콜백
	void OnCreateComponentPopUp();
	// Component 생성 팝업창내에서 Create버튼 누르면 호출되는 콜백
	void OnCreateComponent();
	std::string GetComponentNameInput()	const;

public:
	// m_SelectNode에 지금 선택되어 있는 노드를 넣어주는 함수
	// 위에서 아래로 탐색하므로 1번째 인자에는 루트 노드를 넣어줘야함
	//void FindSelectNode(class CIMGUITree* RootNode);

	void OnDragDropSrc(class CIMGUITree* SrcTree);
	void OnDragDropDest(class CIMGUITree* DestTree, const std::string& ParentName, const std::string& NewChildName);
	
	// Object Hierarchy Window에서 새로운 Object를 선택할때마다 SceneComponentWindow에 목록을 갱신 
	void OnUpdateSceneComponetWindow(class CIMGUITree* SelectObjectNode);

	void OnSetSelectNode(CIMGUITree* Tree);
	void OnDeleteComponent();
};

