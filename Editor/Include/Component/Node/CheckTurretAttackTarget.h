#pragma once
#include "Component/Node/ConditionNode.h"

class CCheckTurretAttackTarget :
    public CConditionNode
{
    friend class CGameBehaviorTree;

public:
    CCheckTurretAttackTarget();
    CCheckTurretAttackTarget(const CCheckTurretAttackTarget& Node);
    virtual ~CCheckTurretAttackTarget();

public:
    virtual NodeResult OnStart(float DeltaTime);
    virtual NodeResult OnUpdate(float DeltaTime);
    virtual NodeResult OnEnd(float DeltaTime);

public:
    virtual NodeResult Invoke(float DeltaTime);

private:
    std::list<CGameObject*> m_TargetChampionList; // 포탑 사거리 내에 존재하는 상대방(챔피언, 미니언 포함)
    std::list<CGameObject*> m_FriendChampionList; // 포탑 사거리 내에 존재하는 아군 챔피언

private:
    // 우선순위 1 : 아군 챔피언을 공격하는 상대방 챔피언
    // 우선순위 2 : 상대방 미니언
    // 우선순위 3 : 상대방 챔피언
    CGameObject* FindPriorAttackTarget();
    CGameObject* CheckPriorAttackCandidate();
};

