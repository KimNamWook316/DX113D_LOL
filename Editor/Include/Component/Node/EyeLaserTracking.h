#pragma once

#include "Component/Node/ActionNode.h"

class CEyeLaserTracking :
    public CActionNode
{
    friend class CGameBehaviorTree;

public:
    CEyeLaserTracking();
    CEyeLaserTracking(const CEyeLaserTracking& Node);
    virtual ~CEyeLaserTracking();

protected:
    virtual NodeResult OnStart(float DeltaTime);
    virtual NodeResult OnUpdate(float DeltaTime);
    virtual NodeResult OnEnd(float DeltaTime);

private:
    class CGameObject* m_Player;

public:
    virtual NodeResult Invoke(float DeltaTime) override;
};

