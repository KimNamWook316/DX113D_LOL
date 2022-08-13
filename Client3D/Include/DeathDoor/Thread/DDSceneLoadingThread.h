#pragma once

#include "Thread.h"

class CDDSceneLoadingThread :
    public CThread
{
    friend class CDDSceneMode;

protected:
    CDDSceneLoadingThread();
    virtual ~CDDSceneLoadingThread();

protected:
    virtual void Run() override;

public:
    void Load(const std::string& SceneFileName, class CDDSceneMode* Owner);

private:
    class CDDSceneMode* m_Owner;
    std::string m_NextSceneFileName;
};

