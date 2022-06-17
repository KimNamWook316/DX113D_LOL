#pragma once

#include "IMGUIBeginMenu.h"

class CDataMenu :
    public CIMGUIBeginMenu
{
public:
    CDataMenu();
    virtual ~CDataMenu();

public:
    virtual bool Init() override;

private:
    void OnToggleCollisionProfileToggle();

private:
    class CIMGUIMenuItem* m_CollisionProfileToggle;
};

