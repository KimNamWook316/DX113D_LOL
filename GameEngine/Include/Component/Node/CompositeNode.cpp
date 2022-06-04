
#include "CompositeNode.h"
#include "../BehaviorTree.h"

CCompositeNode::CCompositeNode()
{
	SetTypeID(typeid(CCompositeNode).hash_code());
}

CCompositeNode::CCompositeNode(const CCompositeNode& Node)	:
	CNode(Node)
{
}

CCompositeNode::~CCompositeNode()
{

}

bool CCompositeNode::Save(FILE* File)
{
	CNode::Save(File);

	size_t Count = m_ChildrenList.size();
	fwrite(&Count, sizeof(size_t), 1, File);

	auto iter = m_ChildrenList.begin();
	auto iterEnd = m_ChildrenList.end();

	for (; iter != iterEnd; ++iter)
	{
		size_t ChildTypeID = (*iter)->GetTypeID();
		fwrite(&ChildTypeID, sizeof(size_t), 1, File);
		(*iter)->Save(File);
	}

	return true;
}

bool CCompositeNode::Load(FILE* File)
{
	CNode::Load(File);

	size_t Count = -1;
	fread(&Count, sizeof(size_t), 1, File);

	for (size_t i = 0; i < Count; ++i)
	{
		size_t ChildTypeID = -1;
		fread(&ChildTypeID, sizeof(size_t), 1, File);
		CNode* Child = m_Owner->LoadNode(this, ChildTypeID);
		Child->Load(File);
		AddChild(Child);
	}

	return true;
}

