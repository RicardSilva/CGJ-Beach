#pragma once

#include "vec.h"
#include "GL/glew.h"
#include "GL/freeglut.h"
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;
namespace engine {
	class Material
	{
		vec3 Ka;
		vec3 Kd;
		vec3 Ks;
		GLfloat Ns;
		GLfloat d;
		GLfloat Tr;
		GLfloat Ni;
		vec3 Ke;
		vec3 Tf;
		GLint illum;

	public:

		vec3 getKs(){
			return Ks;
		}

		vec3 getKd(){
			return Kd;
		}

		GLfloat getNs(){
			return Ns;
		}

		Material(std::string& filename)
		{
			loadMaterialData(filename);
		}

		void parseNs(std::stringstream& sin)
		{
			sin >> Ns;
		}

		void parseNi(std::stringstream& sin)
		{
			sin >> Ni;
		}

		void parseD(std::stringstream& sin)
		{
			sin >> d;
		}

		void parseTr(std::stringstream& sin)
		{
			sin >> Tr;
		}

		void parseTf(std::stringstream& sin)
		{
			vec3 v;
			sin >> v.x >> v.y >> v.z;
			Tf = v;
		}

		void parseIllum(std::stringstream& sin)
		{
			sin >> illum;
		}

		void parseKa(std::stringstream& sin)
		{
			vec3 v;
			sin >> v.x >> v.y >> v.z;
			Ka = v;
		}

		void parseKd(std::stringstream& sin)
		{
			vec3 v;
			sin >> v.x >> v.y >> v.z;
			Kd = v;
		}

		void parseKs(std::stringstream& sin)
		{
			vec3 v;
			sin >> v.x >> v.y >> v.z;
			Ks = v;
		}

		void parseKe(std::stringstream& sin)
		{
			vec3 v;
			sin >> v.x >> v.y >> v.z;
			Ke = v;
		}


		void parseLine(std::stringstream& sin)
		{
			std::string s;
			sin >> s;
			if (s.compare("Ns") == 0) parseNs(sin);
			else if (s.compare("Ni") == 0) parseNi(sin);
			else if (s.compare("d") == 0) parseD(sin);
			else if (s.compare("Tr") == 0) parseTr(sin);
			else if (s.compare("Tf") == 0) parseTf(sin);
			else if (s.compare("illum") == 0) parseIllum(sin);
			else if (s.compare("Ka") == 0) parseKa(sin);
			else if (s.compare("Kd") == 0) parseKd(sin);
			else if (s.compare("Ks") == 0) parseKs(sin);
			else if (s.compare("Ke") == 0) parseKe(sin);
		}

		void loadMaterialData(std::string& filename)
		{
			std::ifstream ifile(filename);
			while (ifile.good()) {
				std::string line;
				std::getline(ifile, line);
				parseLine(std::stringstream(line));
			}
		}
	};

}