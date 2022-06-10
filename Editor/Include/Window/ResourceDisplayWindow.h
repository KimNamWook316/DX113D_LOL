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
private :
	// 현재 Project 의 Resource Manager 에 Load 된 목록들을 보는 Window
	// - 여기에서, Drag 해서, Material Window 같은 곳에 Drop 하면, 해당 위치에 파일 정보가
	// - 세팅되는 원리로 코드를 작성할 것이다.
	// Texture
	class CIMGUIWidgetList* m_TextureList;
	// Shader
	class CIMGUIWidgetList* m_ShaderList;
	// Material
	class CIMGUIWidgetList* m_MaterialList;
private :
	std::vector<class CIMGUIWidgetList*> m_vecResourceTapList;
public :
	virtual bool Init() override;
public  :
	void GatherLoadedTextureResources();
	void GatherLoadedMaterialResources();
};

