#include "souistd.h"
#include "core/SObjectFactory.h"
#include "res.mgr/SObjDefAttr.h"

namespace SOUI
{

SObjectFactoryMgr::SObjectFactoryMgr(void)
{
    m_pFunOnKeyRemoved=OnFactoryRemoved;
    //AddStandardWindowFactory();
}

//************************************
// Method:    RegisterFactory,注册APP自定义的窗口类
// Access:    public
// Returns:   bool
// Qualifier:
// Parameter: SObjectFactory * pWndFactory:窗口工厂指针
// Parameter: bool bReplace:强制替换原有工厂标志
//************************************
bool SObjectFactoryMgr::RegisterFactory(SObjectFactory & objFactory, bool bReplace)
{
	if (HasKey(objFactory.GetObjectInfo()))
	{
		if (!bReplace) return false;
		RemoveKeyObject(objFactory.GetObjectInfo());
	}
	AddKeyObject(objFactory.GetObjectInfo(), objFactory.Clone());
	return true;
}

void SObjectFactoryMgr::AddStandardFactory()
{

}


void SObjectFactoryMgr::OnFactoryRemoved( const SObjectFactoryPtr & obj )
{
    delete obj;
}


//************************************
// Method:    UnregisterFactor,反注册APP自定义的窗口类
// Access:    public
// Returns:   bool
// Qualifier:
// Parameter: SWindowFactory * pWndFactory
//************************************

inline bool SObjectFactoryMgr::UnregisterFactory(const SObjectInfo & objInfo)
{
	return  RemoveKeyObject(objInfo);
}

IObject * SObjectFactoryMgr::CreateObject(const SObjectInfo & objInfo) const
{
    if(!HasKey(objInfo))
    {
        SLOGFMTW(L"Warning: no ojbect %s of type:%d in SOUI!!", (LPCWSTR)objInfo.mName, objInfo.mType);
        return NULL;
    }
    IObject * pRet = GetKeyObject(objInfo)->NewObject();
    SASSERT(pRet);
    SetSwndDefAttr(pRet);
    return pRet;
}


SObjectInfo SObjectFactoryMgr::BaseObjectInfoFromObjectInfo(const SObjectInfo & objInfo)
{
	if (!HasKey(objInfo))
	{
		return SObjectInfo();
	}
	
	SObjectInfo ret = GetKeyObject(objInfo)->GetObjectInfo();

	if (ret == objInfo)
		ret = SObjectInfo();
	return ret;
}


void SObjectFactoryMgr::SetSwndDefAttr(IObject * pObject) const
{
    LPCWSTR pszClassName = pObject->GetObjectClass();
    
	if (pObject->GetClassType() != Window) return;

    //检索并设置类的默认属性
    pugi::xml_node defAttr = GETCSS(pszClassName);
    if(defAttr)
    {
        //优先处理"class"属性
        pugi::xml_attribute attrClass=defAttr.attribute(L"class");
        if(attrClass)
        {
            attrClass.set_userdata(1);
			pObject->SetAttribute(attrClass.name(), attrClass.value(), TRUE);
        }
        for (pugi::xml_attribute attr = defAttr.first_attribute(); attr; attr = attr.next_attribute())
        {
            if(attr.get_userdata()) continue;
			pObject->SetAttribute(attr.name(), attr.value(), TRUE);
        }
        if(attrClass)
        {
            attrClass.set_userdata(0);
        }
    }
}

}//namesspace SOUI