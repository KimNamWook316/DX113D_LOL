#pragma once

#include "Widget/Widget.h"

class CDDMouseWidget :
    public CWidget
{
    friend class CWidgetWindow;

protected:
    CDDMouseWidget();
    virtual ~CDDMouseWidget();

public:
	virtual void Render() override;
};

