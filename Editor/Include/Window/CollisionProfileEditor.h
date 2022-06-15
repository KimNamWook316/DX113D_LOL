#pragma once

#include "IMGUIWindow.h"

class CCollisionProfileEditor :
    public CIMGUIWindow
{
public:
    CCollisionProfileEditor();
    virtual ~CCollisionProfileEditor();

public:
    virtual bool Init() override;

private:
    void Refresh();

private:
    void OnSelectList(int Idx, const char* Label);
    void OnClickCreate();
    void OnCheckInteraction(int Idx, bool Check);
    void OnClickSave();
    void OnClickLoad(); 
    void OnClickDelete();

private:
    class CIMGUIListBox* m_ProfileList;
    class CIMGUIComboBox* m_ChannelList;
    class CIMGUITextInput* m_ProfileNameInput;
    class CIMGUIButton* m_CreateProfileButton;
    class CIMGUITextInput* m_ProfileName;
    class CIMGUITextInput* m_ProfileChannel;
    class CIMGUICheckBox* m_InteractionEdit;
    class CIMGUIButton* m_SaveButton;
    class CIMGUIButton* m_LoadButton;
    class CIMGUIButton* m_DeleteButton;
};

