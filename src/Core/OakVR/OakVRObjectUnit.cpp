#include "OakVRObjectUnit.h"

#include "Log/Log.h"


namespace oakvr
{
	ObjectSharedPointer OakVRObjectUnit::CreateObject(const StringId &objId)
	{
		return std::make_shared<Object>(objId);
	}

	auto OakVRObjectUnit::AddObjectToGraph(ObjectSharedPointer pObject, const StringId &parentId) -> void
	{
		m_objectMap[pObject->GetId()] = pObject;
		auto pCurrentParent = pObject->GetParent();
		
		if (parentId != pCurrentParent->GetId())
		{
			// we must also add the object as a child of the parent
			auto pParent = FindObject(parentId);
			
			if (pParent)
			{
				pParent->AddChild(pObject);
			}
			else
			{
				Log::Error("Parent id [%s] was not found", parentId.c_str());
			}

			pObject->SetParent(pParent);
		}
	}

	auto OakVRObjectUnit::RemoveObjectFromGraph(const StringId &id) -> void
	{
		auto it = m_objectMap.find(id);
		if (it != m_objectMap.end())
		{
			if (id != "root")
			{
				auto pObject = it->second;
				pObject->GetParent()->RemoveChild(pObject);
			}
			m_objectMap.erase(it);
		}
	}

	ObjectSharedPointer OakVRObjectUnit::FindObject(const StringId &objectId)
	{
		auto it = m_objectMap.find(objectId);
		if (it != std::end(m_objectMap))
		{
			return it->second;
		}
		return nullptr;
	}

	auto OakVRObjectUnit::TransferObject(const StringId &id, const StringId &newParentId) -> void
	{
		if (id == "root")
			return;
		
		auto pParent = FindObject(newParentId);
		if (pParent)
		{
			auto pObj = FindObject(id);
			if (pObj)
			{
				pObj->GetParent()->RemoveChild(pObj);
				pObj->SetParent(pParent);
				pParent->AddChild(pObj);
				
			}
			
		}
	}
}