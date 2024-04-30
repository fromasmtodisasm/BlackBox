#include "pch.hpp"
#include "xml.h"
//#include "xml.h"


// Constructor from tinyxml2 node

CIXMLDOMNode::CIXMLDOMNode(tinyxml2::XMLNode* node)
	: m_Node(node->ToElement())
	, m_nRef(0)
{
	m_ntNodeType = XDOM::NODE_ELEMENT;
	m_pChildNodes = new CXMLDOMNodeList;
	m_pChildNodes->AddRef();
	tinyxml2::XMLNode* pNode = m_Node->FirstChildElement();
	while (pNode)
	{
		m_pChildNodes->AddNode(new CIXMLDOMNode(pNode));
		pNode = pNode->NextSibling();
	}

	m_sName = m_Node->Name();
	auto text = m_Node->GetText();
	m_sText = text ? text : "";

	for (auto attr = m_Node->FirstAttribute(); attr; attr = attr->Next())
	{
		m_pChildNodes->AddNode(new CIXMLDOMNode(attr));
	}
}

CIXMLDOMNode::CIXMLDOMNode(const tinyxml2::XMLAttribute* attribute)
{
	m_ntNodeType = XDOM::NODE_ATTRIBUTE;
	m_pChildNodes = new CXMLDOMNodeList;
	m_pChildNodes->AddRef();
	m_sName = attribute->Name();
	m_sText = attribute->Value();
}

CIXMLDOMNode::~CIXMLDOMNode()
{
	m_pChildNodes->Release();
}

int CIXMLDOMNode::AddRef()
{
	return ++m_nRef;
}

void CIXMLDOMNode::Release()
{
	if (--m_nRef <= 0)
		delete this;
}

XDOM::_DOMNodeType CIXMLDOMNode::getNodeType()
{
	return m_ntNodeType;
}

const char* CIXMLDOMNode::getText()
{
	return m_sText.c_str();
}

const char* CIXMLDOMNode::getName()
{
	return m_sName.c_str();
}

XDOM::IXMLDOMNodeList* CIXMLDOMNode::getChildNodes()
{
	return m_pChildNodes;
}

void CIXMLDOMNode::setText(const char* sText)
{
	NOT_IMPLEMENTED;
}

void CIXMLDOMNode::setName(const char* sName)
{
}

bool CIXMLDOMNode::hasChildNodes()
{
	return m_pChildNodes->length() ? true : false;
}

bool CIXMLDOMNode::appendChild(IXMLDOMNode* pNode)
{
	NOT_IMPLEMENTED_V;
}

XDOM::IXMLDOMNode* CIXMLDOMNode::getAttribute(const char* sName)
{
	for (const auto it : m_pChildNodes->m_lstNodes)
	{
		if (it->getNodeType() == XDOM::NODE_ATTRIBUTE)
		{
			if (strcmp(it->getName(), sName) == 0)
				return it;
		}
	}
	return nullptr;
}

XDOM::IXMLDOMNodeList* CIXMLDOMNode::getElementsByTagName(const char* sName)
{
	XDOM::IXMLDOMNode* pNode;
	CXMLDOMNodeList* pNodeList;
	pNodeList = new CXMLDOMNodeList;
	m_pChildNodes->reset();
	while (pNode = m_pChildNodes->nextNode())
	{
#if defined(LINUX)
		if ((compareTextFileStrings(pNode->getName(), sName) == 0) && (pNode->getNodeType() == XDOM::NODE_ELEMENT))
#else
		if ((strcmp(pNode->getName(), sName) == 0) && (pNode->getNodeType() == XDOM::NODE_ELEMENT))
#endif
			pNodeList->AddNode(pNode);
	}
	if (pNodeList->length() == 0)
	{
		pNodeList->Release();
		return NULL;
	}

	return pNodeList;

}

// Inherited via IXMLDOMDocument

CXMLDocument::CXMLDocument()
{
	//m_nRef = 0;
	m_pChildNodes = new CXMLDOMNodeList;
	m_pChildNodes->AddRef();

}
CXMLDocument::~CXMLDocument()
{
	m_pChildNodes->Release();
}


int CXMLDocument::AddRef()
{
	return 0;
}

void CXMLDocument::Release()
{

}

XDOM::_DOMNodeType CXMLDocument::getNodeType()
{
	return XDOM::_DOMNodeType();
}

const char* CXMLDocument::getText()
{
	NOT_IMPLEMENTED_V;
	return nullptr;
}

const char* CXMLDocument::getName()
{
	NOT_IMPLEMENTED_V;
	return nullptr;
}

XDOM::IXMLDOMNodeList* CXMLDocument::getChildNodes()
{
	//return new CXMLDOMNodeList(m_Document.FirstChild());
	return m_pChildNodes;
}

void CXMLDocument::setText(const char* sText)
{
}

void CXMLDocument::setName(const char* sName)
{
}

bool CXMLDocument::hasChildNodes()
{
	return !m_Document.NoChildren();
}

bool CXMLDocument::appendChild(IXMLDOMNode* pNode)
{
	return false;
}

XDOM::IXMLDOMNode* CXMLDocument::getAttribute(const XMLCHAR* sName)
{
	return nullptr;
}

XDOM::IXMLDOMNodeList* CXMLDocument::getElementsByTagName(const XMLCHAR* sName)
{
	//return new CXMLDOMNodeList(m_Document.FirstChildElement(sName));
	XDOM::IXMLDOMNode* pNode;
	CXMLDOMNodeList* pNodeList;
	pNodeList = new CXMLDOMNodeList;
	m_pChildNodes->reset();
	while ((pNode = m_pChildNodes->nextNode()) != NULL)
	{
#if defined(LINUX)
		if (compareTextFileStrings(pNode->getName(), sName) == 0)
#else
		if (strcmp(pNode->getName(), sName) == 0)
#endif
			pNodeList->AddNode(pNode);
	}
	//	if(pNodeList->length()==0)
	//		return NULL;
	return pNodeList;

}

bool CXMLDocument::load(const XMLCHAR* sSource)
{
	//m_sErrorString = "";
	//_XMLDOMParserImpl xml(this);
	std::vector<unsigned char> buf;
	FILE* file = GetISystem()->GetIPak()->FOpen(sSource, "rb");
	if (file) {
		GetISystem()->GetIPak()->FSeek(file, 0, SEEK_END);
		int fileSize = GetISystem()->GetIPak()->FTell(file);
		GetISystem()->GetIPak()->FSeek(file, 0, SEEK_SET);
		buf.resize(fileSize);
		GetISystem()->GetIPak()->FRead(&*buf.begin(), fileSize, 1, file);
		GetISystem()->GetIPak()->FClose(file);
		//calculate the checksum of the file
		//that's used by to verify that a clien has the same level data
#if 0
		m_wCheckSum = 0;
		for (std::vector<unsigned char>::iterator itr = buf.begin(); itr != buf.end(); ++itr)
		{
			//m_wCheckSum += (*itr);
		}

		if (!xml.parse(buf, m_sErrorString))
#endif
		auto result = m_Document.Parse((char*)buf.data(), buf.size()) == tinyxml2::XML_SUCCESS;
		if (!result)
		{
			CryLog("XMLDOM : ", m_Document.ErrorStr());
			return false;
		}
		tinyxml2::XMLNode* pRoot = m_Document.RootElement();
		if (pRoot)
		{
			tinyxml2::XMLNode* pNode = pRoot->FirstChild();
			while (pNode)
			{
				m_pChildNodes->AddNode(new CIXMLDOMNode(pNode));
				pNode = pNode->NextSibling();
			}
		}
		return true;
	}
	else {
		return false;
	}
}

bool CXMLDocument::loadXML(const XMLCHAR* szString)
{
	return m_Document.Parse(szString) == tinyxml2::XML_SUCCESS;
}

XDOM::IXMLDOMNode* CXMLDocument::getRootNode()
{
	return nullptr;
}

XDOM::IXMLDOMNode* CXMLDocument::createNode(XDOM::_DOMNodeType Type, const char* name)
{
	return nullptr;
}

const XMLCHAR* CXMLDocument::getErrorString()
{
	return m_Document.ErrorStr();
}

unsigned short CXMLDocument::getCheckSum()
{
	return 0;
}

//IXMLDOMBase

int CXMLDOMNodeList::AddRef() { return ++m_nRef; }

void CXMLDOMNodeList::Release() { if (--m_nRef <= 0) delete this; }

CXMLDOMNodeList::CXMLDOMNodeList()
{
	m_nRef = 0;
	m_itor = m_lstNodes.begin();
}

void CXMLDOMNodeList::AddNode(XDOM::IXMLDOMNode* pNode)
{
	m_lstNodes.push_back(pNode);
	pNode->AddRef();
}

size_t CXMLDOMNodeList::length()
{
	return m_lstNodes.size();
}

void CXMLDOMNodeList::reset()
{
	m_itor = m_lstNodes.begin();
}

XDOM::IXMLDOMNode* CXMLDOMNodeList::nextNode()
{
	XDOM::IXMLDOMNode* pTemp;
	if (m_itor == m_lstNodes.end())
		return NULL;
	pTemp = (*m_itor);
	++m_itor;
	return pTemp;
}
