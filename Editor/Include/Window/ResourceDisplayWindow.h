#pragma once
#include "IMGUIWindow.h"

class CResourceDisplayWindow :
    public CIMGUIWindow
{
	friend class CEditorManager;
	// friend class CAnimationInstanceConvertThread;
public:
	CResourceDisplayWindow();
	~CResourceDisplayWindow();
};

