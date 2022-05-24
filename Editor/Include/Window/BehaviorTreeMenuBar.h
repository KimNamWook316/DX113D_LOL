#pragma once
#include "IMGUIWindow.h"
class CBehaviorTreeMenuBar :
    public CIMGUIWindow
{
public:
	CBehaviorTreeMenuBar();
	virtual ~CBehaviorTreeMenuBar();

private:
	class CIMGUIPopUpModal* m_NodeEditorModal;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
};

