#pragma once

#include "IMGUIWindow.h"

class CObjectHierarchyWindow :
	public CIMGUIWindow
{
public:
	CObjectHierarchyWindow();
	virtual ~CObjectHierarchyWindow();

private:
	std::vector<class CIMGUITree*>	m_vecObjectTree;
	class CIMGUIButton* m_ObjectCreatePopUpButton;
	class CObjectCreateModal* m_ObjectCreateModal;
	class CIMGUIButton* m_ObjectDeleteButton;

	class CIMGUITree* m_Root;
	class CIMGUITree* m_SelectNode;
	class CIMGUITree* m_DragSrc;
	class CIMGUITree* m_DragDest;

public:
	class CIMGUITree* GetRoot()	const;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

public:
	// 오브젝트 생성 팝업창 띄워주는 콜백
	void OnCreateObjectPopUp();
	void OnDeleteObject();

	// 2번째 인자에 지금 선택되어 있는 노드를 m_SelectNode에 넣어준는 함수
	// 위에서 아래로 탐색하므로 1번째 인자에는 루트 노드를 넣어줘야함
	//void FindSelectNode(class CIMGUITree* RootNode);
	CIMGUITree* GetSelectNode()	const;
	void OnSetSelectNode(CIMGUITree* Tree);
	void OnDragDropSrc(class CIMGUITree* SrcTree);
	void OnDragDropDest(class CIMGUITree* DestTree, const std::string& ParentName, const std::string& NewChildName);
};


