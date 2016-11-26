#ifndef Mesh_H
#define Mesh_H

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Vector3.h"
#include "Vector2.h"

#include "GL/glew.h"
#include "GL/freeglut.h"

class Mesh {
public:
	std::vector <Vector3> vertexData;
	std::vector <Vector2> texcoordData;
	std::vector <Vector3> normalData;

	std::vector <unsigned int> vertexIdx, texcoordIdx, normalIdx;
	
	GLint start = 0;
	GLsizei size = 0;

	Mesh(){}

	void parseVertex(std::stringstream& sin)
	{
		Vector3 v;
		sin >> v;
		vertexData.push_back(v);
	}

	void parseTexcoord(std::stringstream& sin)
	{
		Vector2 t;
		sin >> t;
		texcoordData.push_back(t);
	}

	void parseNormal(std::stringstream& sin)
	{
		Vector3 n;
		sin >> n;
		normalData.push_back(n);
	}

	void parseFace(std::stringstream& sin)
	{
		std::string token;
		for (int i = 0; i < 3; i++) {
			std::getline(sin, token, '/');
			if (token.size() > 0) vertexIdx.push_back(std::stoi(token));
			std::getline(sin, token, '/');
			if (token.size() > 0) texcoordIdx.push_back(std::stoi(token));
			std::getline(sin, token, ' ');
			if (token.size() > 0) normalIdx.push_back(std::stoi(token));
		}
	}

	void parseLine(std::stringstream& sin)
	{
		std::string s;
		sin >> s;
		if (s.compare("v") == 0) parseVertex(sin);
		else if (s.compare("vt") == 0) parseTexcoord(sin);
		else if (s.compare("vn") == 0) parseNormal(sin);
		else if (s.compare("f") == 0) parseFace(sin);
	}

	void loadMeshData(std::string& filename, MyShader* ms)
	{
		std::ifstream ifile(filename);
		while (ifile.good()) {
			std::string line;
			std::getline(ifile, line);
			parseLine(std::stringstream(line));
		}
		ms->TexcoordsLoaded = (texcoordIdx.size() > 0);
		ms->NormalsLoaded = (normalIdx.size() > 0);
	}

	void processMeshData(MyShader *ms)
	{
		for (unsigned int i = 0; i < vertexIdx.size(); i++) {
			unsigned int vi = vertexIdx[i];
			Vector3 v = vertexData[vi - 1];
			ms->Vertices.push_back(v);
			if (ms->TexcoordsLoaded) {
				unsigned int ti = texcoordIdx[i];
				Vector2 t = texcoordData[ti - 1];
				ms->Texcoords.push_back(t);
			}
			if (ms->NormalsLoaded) {
				unsigned int ni = normalIdx[i];
				Vector3 n = normalData[ni - 1];
				ms->Normals.push_back(n);
			}
		}
	}

	void freeMeshData()
	{
		vertexData.clear();
		texcoordData.clear();
		normalData.clear();
		vertexIdx.clear();
		texcoordIdx.clear();
		normalIdx.clear();
	}

	const void createMesh(std::string& filename, MyShader * ms)
	{
		start = ms->Vertices.size();
		loadMeshData(filename, ms);
		processMeshData(ms);
		freeMeshData();
		size = ms->Vertices.size() - start;
	}

	void draw() {
		glDrawArrays(GL_TRIANGLES, start, (GLsizei)size);
	}
};


#endif