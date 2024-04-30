#include <tinyxml2.h>

#include <BlackBox/System/ISystem.hpp>

class CXMLDocument;
class CIXMLDOMNode;
class CXMLDOMNodeList;

class CIXMLDOMNode :
    public _reference_target_t,
    public XDOM::IXMLDOMNode
{
private:
	using xml_string = string;
	tinyxml2::XMLElement* m_Node;
public:
	xml_string m_sText;
	xml_string m_sName;

	XDOM::_DOMNodeType m_ntNodeType;
	CXMLDOMNodeList* m_pChildNodes;
private:
	int m_nRef;

public:
	// Constructor from tinyxml2 node
	CIXMLDOMNode(tinyxml2::XMLNode* node);
	CIXMLDOMNode(const tinyxml2::XMLAttribute* attribute);
	~CIXMLDOMNode();

	// IXMLDOMBase interface
public:
	virtual int AddRef() override;
	virtual void Release() override;

	// IXMLDOMNode interface
public:
	virtual XDOM::_DOMNodeType getNodeType() override;
	virtual const char* getText() override;
	virtual const char* getName() override;
	virtual XDOM::IXMLDOMNodeList* getChildNodes() override;
	virtual void setText(const char* sText) override;
	virtual void setName(const char* sName) override;
	virtual bool hasChildNodes() override;
	virtual bool appendChild(IXMLDOMNode* pNode) override;
	virtual XDOM::IXMLDOMNode* getAttribute(const char* sName) override;
	virtual XDOM::IXMLDOMNodeList* getElementsByTagName(const char* sName) override;
};

class CXMLDOMNodeList :
    public _reference_target_t,
    public XDOM::IXMLDOMNodeList
{
	// IXMLDOMBase interface
public:
	//IXMLDOMBase
	int AddRef();
	void Release();;


public:
	CXMLDOMNodeList();

	void AddNode(XDOM::IXMLDOMNode* pNode);

	// IXMLDOMNodeList interface
public:
	virtual size_t length() override;
	void reset() override;

	XDOM::IXMLDOMNode* nextNode() override;


	//tinyxml2::XMLNode* m_NodeList;
	typedef std::list<XDOM::IXMLDOMNode*> DOMNodeList;
	typedef DOMNodeList::iterator DOMNodeListItor;

	DOMNodeList m_lstNodes;
	DOMNodeListItor m_itor;
	int m_nRef;
};

class CXMLDocument : public XDOM::IXMLDOMDocument
{
	CXMLDOMNodeList* m_pChildNodes;
	
public:
	CXMLDocument();
	~CXMLDocument();
	// Inherited via IXMLDOMDocument
	virtual int AddRef() override;
	virtual void Release() override;
	virtual XDOM::_DOMNodeType getNodeType() override;
	virtual const char* getText() override;
	virtual const char* getName() override;
	virtual XDOM::IXMLDOMNodeList* getChildNodes() override;
	virtual void setText(const char* sText) override;
	virtual void setName(const char* sName) override;
	virtual bool hasChildNodes() override;
	virtual bool appendChild(IXMLDOMNode* pNode) override;
	virtual XDOM::IXMLDOMNode* getAttribute(const XMLCHAR* sName) override;
	virtual XDOM::IXMLDOMNodeList* getElementsByTagName(const XMLCHAR* sName) override;
	virtual bool load(const XMLCHAR* sSourceFile) override;
	virtual bool loadXML(const XMLCHAR* szString) override;
	virtual XDOM::IXMLDOMNode* getRootNode() override;
	virtual XDOM::IXMLDOMNode* createNode(XDOM::_DOMNodeType Type, const char* name) override;
	virtual const XMLCHAR* getErrorString() override;
	virtual unsigned short getCheckSum() override;

	tinyxml2::XMLDocument m_Document;
};
