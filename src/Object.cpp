#include "Object.hpp"
#include <fstream>
#include <cctype>

using namespace std;

#if 0

struct OBJNode {
	UniType val;
	
	enum {
		TFUN,
		TARG
	}type;

};

struct Loader : public IGeometry{
private:

public:
	Loader(string filename) {	
		Load(filename);
	}
	virtual const vector<OBJNode> Lex(string str) {
		size_t i = 0;
		bool isnum = false;
		string curnum = "";
		string curf = "";
		vector<OBJNode> out;
		for (auto it : str) {
			if (!isnum) { curnum = ""; }

			if (it == ' ' || it == '\t' || it == '\n') { 
				
					if (isnum) { 
						OBJNode n;
						UniType v;
						// v.dval = curnum.stof
						v.dval = std::stod(curnum);
						v.t = TDVAL;
						n.val = v;
						n.type = OBJNode::TARG;

						out.push_back(n); 
						curnum = "";
						isnum = false;
					}
					else {
						if (curf != "") {
							OBJNode n;
							UniType v;
							n.val.sval = const_cast<char*>( curf.c_str() );
							n.type = OBJNode::TFUN;
							n.val.t = TSTR;
							out.push_back(n);
							curf = "";
						}
					}
					continue; 
				}
			if (isalpha(it)) { // vp 0000.0000
					isnum = false;
					switch (it) {
					case 'v':
						curf += it;
						break;
					case 'm':
						curf += it;
						break;
					case 'o': case 'g': case 'f': {
							OBJNode n;
							UniType v;
							curf = it;
							n.val.sval = const_cast<char*>(curf.c_str());
							n.type = OBJNode::TFUN;
							n.val.t = TSTR;
							out.push_back(n);
							curf = "";
							break;
						}
					}
					//if (isnum) {

					//}
					//else {
					//}
			}
			else {
				if (curf != "") {
					OBJNode n;
					UniType v;
					n.val.sval = const_cast<char*>(curf.c_str());;
					n.type = OBJNode::TFUN;
					n.val.t = TSTR;
					out.push_back(n);
					curf = "";
				}
				curnum += it;
			}
			
			i++;
		}
	}
	bool Parse(string str) {
		/*if (std::regex_match(str, r, poly)) {

		}
		else if (std::regex_match(str, r, normal)) {

		}
		else if (std::regex_match(str, r, group)) {

		}
		else if (std::regex_match(str, r, mtres)) {

		}
		else if (std::regex_match(str, r, obj)) {

		}
		else if (std::regex_match(str, r, tcoord)) {

		}
		else if (std::regex_match(str, r, vert)) {

		}*/

		vector<OBJNode> obj = Lex(str);

		for (auto it : obj) {
			switch (it.type) {
			case OBJNode::TFUN:
				break;
			case OBJNode::TARG:
				break;
			default:
				break;
			}
		}
	}
	virtual bool Load(string filename) {
		
		ifstream file(filename);

		if (!file.is_open()) { return false; }

		while (!file.eof()) {
			string str;
			std::getline(file,str);
			Parse(str);
		}

	}
};

#endif

Object::Object() 
{

}

void Object::draw() {}

void Object::move() {}

void Object::rotate() {}

Object * Object::Load(string path)
{
	Object *obj;
	

	
	return obj;
}
