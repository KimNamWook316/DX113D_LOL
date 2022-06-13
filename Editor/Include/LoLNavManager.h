#pragma once

#include "Scene/Navigation3DManager.h"

class CLoLNavManager :
    public CNavigation3DManager
{
    friend class CScene;

private:
    CLoLNavManager();
    ~CLoLNavManager();


public:
    void Start();
    bool Init();
    void Update(float DeltaTime);
};

