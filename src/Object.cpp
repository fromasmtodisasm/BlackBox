#include "Object.hpp"
#include <fstream>

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

struct Loader : public IGeometry{
private:
	regex vert("^v");
	regex poly("^f");
	regex normal("^vn");
	regex group("^g");
	regex mtres("^mtllib");
	regex obj("^o");
	regex tcoord("^vt");
public:
	Loader(string filename) {	
		Load(filename);
	}
	virtual bool Exec(string str) {
		if (std::regex_match(str, r, poly)) {
			
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
			exec(str);
		}

	}
};

#endif

Object * Object::Load(string path)
{
	Object *obj;
	

	
	return obj;
}
