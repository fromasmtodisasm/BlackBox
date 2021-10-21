#include <tinyxml2.h>

#include <BlackBox\System\ISystem.hpp>

class CXMLDocument;
class CIXMLDOMNode;
class CXMLDOMNodeList;


class CIXMLDOMNode :
        public _reference_target_t,
        public XDOM::IXMLDOMNode
{
    // IXMLDOMBase interface
public:
    virtual int AddRef() override
    {

    }
    virtual void Release() override
    {

    }

    // IXMLDOMNode interface
public:
    virtual XDOM::_DOMNodeType getNodeType() override
    {

    }
    virtual const char *getText() override
    {

    }
    virtual const char *getName() override
    {

    }
    virtual XDOM::IXMLDOMNodeList *getChildNodes() override
    {

    }
    virtual void setText(const char *sText) override
    {

    }
    virtual void setName(const char *sName) override
    {

    }
    virtual bool hasChildNodes() override
    {

    }
    virtual bool appendChild(IXMLDOMNode *pNode) override
    {

    }
    virtual XDOM::IXMLDOMNode *getAttribute(const char *sName) override
    {

    }
    virtual XDOM::IXMLDOMNodeList *getElementsByTagName(const char *sName) override
    {

    }

};

class CXMLDOMNodeList :
        public _reference_target_t,
        public XDOM::IXMLDOMNodeList
{


    // IXMLDOMBase interface
public:
    virtual int AddRef() override
    {
		return ++m_nRefCounter;
    }
    virtual void Release() override
    {
		++m_nRefCounter;
    }
public:
    CXMLDOMNodeList(tinyxml2::XMLNode* first) : m_NodeList(first){

    }

    // IXMLDOMNodeList interface
public:
    virtual size_t length() override
    {
		return 0;
    }
    virtual void reset() override
    {

    }
    virtual XDOM::IXMLDOMNode *nextNode() override
    {
		//return new CXMLDOMNodeList(m_NodeList->NextSibling());
		return nullptr; 
    }

    tinyxml2::XMLNode* m_NodeList;
};

class CXMLDocument : public XDOM::IXMLDOMDocument
{
  public:


// Inherited via IXMLDOMDocument
	virtual int AddRef() override
	{
		return 0;
	}
	virtual void Release() override
	{
	}
	virtual XDOM::_DOMNodeType getNodeType() override
	{
		return XDOM::_DOMNodeType();
	}
	virtual const char*		   getText() override
	{
		return nullptr;
	}
	virtual const char* getName() override
	{
		return nullptr;
	}
	virtual XDOM::IXMLDOMNodeList* getChildNodes() override
	{
        return new CXMLDOMNodeList(m_Document.FirstChild());
	}
	virtual void setText(const char* sText) override
	{
	}
	virtual void setName(const char* sName) override
	{
	}
	virtual bool hasChildNodes() override
	{
		return !m_Document.NoChildren();
	}
	virtual bool appendChild(IXMLDOMNode* pNode) override
	{
		return false;
	}
	virtual XDOM::IXMLDOMNode* getAttribute(const XMLCHAR* sName) override
	{
		return nullptr;
	}
	virtual XDOM::IXMLDOMNodeList* getElementsByTagName(const XMLCHAR* sName) override
	{
		return new CXMLDOMNodeList(m_Document.FirstChildElement(sName));
	}
	virtual bool load(const XMLCHAR* sSourceFile) override
	{
		return m_Document.LoadFile(sSourceFile) == tinyxml2::XML_SUCCESS;
	}
	virtual bool loadXML(const XMLCHAR* szString) override
	{
		return m_Document.Parse(szString) == tinyxml2::XML_SUCCESS;
	}
	virtual XDOM::IXMLDOMNode* getRootNode() override
	{
		return nullptr;
	}
	virtual XDOM::IXMLDOMNode* createNode(XDOM::_DOMNodeType Type, const char* name) override
	{
		return nullptr;
	}
	virtual const XMLCHAR* getErrorString() override
	{
		return m_Document.ErrorStr();
	}
	virtual unsigned short getCheckSum() override
	{
		return 0;
	}


	tinyxml2::XMLDocument m_Document;
};
