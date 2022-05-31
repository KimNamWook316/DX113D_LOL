#pragma once

#include "GameObject\GameObject.h"
#include "Component/LandScape.h"

class CLandScapeObj :
    public CGameObject
{
    friend class CScene;

protected:
    CLandScapeObj();
    CLandScapeObj(const CLandScapeObj& obj);
    virtual ~CLandScapeObj();

private:
    CSharedPtr<CLandScape>    m_LandScape;

    int m_FrameCount;

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CLandScapeObj* Clone();
};


