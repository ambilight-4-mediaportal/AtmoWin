#include "stdafx.h"
#include "ObjectModel.h"
#include "MemberDescriptor.h"

IMPLEMENT_FACTORY(MyConfiguration, configuration)
IMPLEMENT_FACTORY(MySection, section)
IMPLEMENT_FACTORY(MySetting ,	setting)

int g_verify0ref = 0;

void MyConfiguration::MapXMLTagsToMembers()
{
	// All of the <Section>'s under <Configuration> goes into this list
	MapMember(&m_lstMyObjects, MySection::GetStaticTag());
}

void MySetting::MapXMLTagsToMembers()
{
  MapAttribute(&m_strName,"name");
  MapAttribute(&m_strValue,"value");
}

void MySection::MapXMLTagsToMembers()
{
  MapAttribute(&m_strSectionName, "name");
  MapMember(&m_lstMySettings, MySetting::GetStaticTag());
}
