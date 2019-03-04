#include "Object.hpp"
#include <fstream>
#include <cctype>

using std::fstream;
using std::ifstream;
using std::ofstream;
using std::regex;
using std::cmatch;


/*

std::ifstream in;

in.open(filename, std::ifstream::in);
if (in.fail()) return;
std::string line;
while (!in.eof()) {
	std::getline(in, line);
	std::istringstream iss(line.c_str());
	char trash;
	if (!line.compare(0, 2, "v ")) {
		iss >> trash;
		Vec3f v;
		for (int i = 0; i < 3; i++) iss >> v.raw[i];
		verts_.push_back(v);
	}
	else if (!line.compare(0, 2, "f ")) {
		std::vector<int> f;
		int itrash, idx;
		iss >> trash;
		while (iss >> idx >> trash >> itrash >> trash >> itrash) {
			idx--; // in wavefront obj all indices start at 1, not zero
			f.push_back(idx);
		}
		faces_.push_back(f);
	}
}
std::cerr << "# v# " << verts_.size() << " f# " << faces_.size() << std::endl;


*/

#if _LOAD_OBJ_ == 1



static struct OBJNode {
	UniType val;
	
	enum {
//		TVERT,
//		TPOLY,
//		TCOORD,
//		TNORMAL,
		TFUN,
		TARG
		// ...,
	}type;

};

struct Loader : public IGeometry{
private:
//	string vert("v");
//	string poly("f");
//	string normal("vn");
//	string group("g");
//	string mtres("mtllib");
//	string obj("o");
//	string tcoord("vt");
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
						n.val.sval = curf.c_str();
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
							n.val.sval = curf.c_str();
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
						n.val.sval = curf.c_str();
						n.type = OBJNode::TFUN;
						n.val.t = TSTR;
						out.push_back(n);
						curf = "";
					}
					curnum += it;
				}


			}
			i++;
		}
	}
	virtual bool Parse(string str) {
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
			switch (it.Type) {
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
		
		// cmatch res;
		// regex r;

		ifstream file(filename);

		if (!file.is_open()) { return false; }

		while (!file.eof()) {
			string str;
			file.getline(str);
			Parse(str);
		}

	}
};

#endif

Object * Object::Load(string path)
{
	Object *obj;
	

	
	return obj;
}
