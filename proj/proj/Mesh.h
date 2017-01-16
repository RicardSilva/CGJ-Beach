#pragma once
#include "GL/glew.h"
#include "GL/freeglut.h"
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include "Shader.h"
#include "mat.h"
#include "MeshGenerator.h"

typedef struct {
	GLfloat x, y, z;
} Vertex;

typedef struct {
	GLfloat u, v;
} Texcoord;

typedef struct {
	GLfloat nx, ny, nz;
} Normal;

namespace engine {

	class Mesh {

	public:

		bool TexcoordsLoaded;
		bool NormalsLoaded;

		std::vector <Vertex> Vertices, vertexData;
		std::vector <Texcoord> Texcoords, texcoordData;
		std::vector <Normal> Normals, normalData;

		std::vector <unsigned int> vertexIdx, texcoordIdx, normalIdx;

		#define VERTICES 0
		#define TEXCOORDS 1
		#define NORMALS 2

		GLuint VaoId, VboId[3];
		GLuint VboSharedMatrices, VboVertices, VboTexcoords, VboNormals;

		GLint UboId;

	public:

		bool isSand;
		bool isFlat;

		Mesh(bool flat){
			isFlat = flat;
			isSand = true;
			GenerateRandomNoiseMesh(100, 100);
			processMeshData();
			freeMeshData();
			createBufferObjects();
		}

		Mesh(std::string& filename)
		{
			isSand = false;
			loadMeshData(filename);
			processMeshData();
			freeMeshData();
			createBufferObjects();
			
		}

		virtual ~Mesh() {
			destroyBufferObjects();
		}

		void GenerateRandomNoiseMesh(int width, int height){

			MeshGenerator meshGen = *new MeshGenerator(width, height, isFlat);
			for (int i = 0; i < meshGen.getVerticesCount(width, height); i = i + 3){
				Vertex v;
				v.x = meshGen.vertices[i];
				v.y = meshGen.vertices[i + 1];
				v.z = meshGen.vertices[i + 2];
				vertexData.push_back(v);
			}

			for (int i = 0; i < meshGen.getIndicesCount(height, height); i++){
				vertexIdx.push_back(meshGen.indices[i]);
			}
			int k = 0;
			for (int i = 0; i < vertexIdx.size(); i++) {
				Vertex v = vertexData[vertexIdx[i]];
				vec3 normal = meshGen.calculateNormal(v);
				Normal n = *new Normal();
				n.nx = normal.x;
				n.ny = normal.y;
				n.nz = normal.z;
				normalData.push_back(n);
				Texcoord t = *new Texcoord();
				t.u = i;
				t.v = i;
				texcoordData.push_back(t);
				normalIdx.push_back(i + 1);
				texcoordIdx.push_back(vertexIdx[i]);
			}

			TexcoordsLoaded = true;
			NormalsLoaded = true;
		}


		void parseVertex(std::stringstream& sin)
		{
			Vertex v;
			sin >> v.x >> v.y >> v.z;
			vertexData.push_back(v);
		}

		void parseTexcoord(std::stringstream& sin)
		{
			Texcoord t;
			sin >> t.u >> t.v;
			texcoordData.push_back(t);
		}

		void parseNormal(std::stringstream& sin)
		{
			Normal n;
			sin >> n.nx >> n.ny >> n.nz;
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

		void loadMeshData(std::string& filename)
		{
			std::ifstream ifile(filename);
			while (ifile.good()) {
				std::string line;
				std::getline(ifile, line);
				parseLine(std::stringstream(line));
			}
			TexcoordsLoaded = (texcoordIdx.size() > 0);
			NormalsLoaded = (normalIdx.size() > 0);
		}

		void processMeshData()
		{
			for (unsigned int i = 0; i < vertexIdx.size(); i++) {
				unsigned int vi = vertexIdx[i];
				Vertex v = vertexData[vi - 1];
				Vertices.push_back(v);
				if (TexcoordsLoaded) {
					unsigned int ti = texcoordIdx[i];
					Texcoord t = texcoordData[ti - 1];
					Texcoords.push_back(t);
				}
				if (NormalsLoaded) {
					unsigned int ni = normalIdx[i];
					Normal n = normalData[ni - 1];
					Normals.push_back(n);
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

		void createBufferObjects()
		{
			glGenVertexArrays(1, &VaoId);

			glBindVertexArray(VaoId);
			{
				glGenBuffers(1, &VboVertices);
				glBindBuffer(GL_ARRAY_BUFFER, VboVertices);
				glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), &Vertices[0], GL_STATIC_DRAW);
				glEnableVertexAttribArray(VERTICES);
				glVertexAttribPointer(VERTICES, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

			}
			if (TexcoordsLoaded)
			{
				glGenBuffers(1, &VboTexcoords);
				glBindBuffer(GL_ARRAY_BUFFER, VboTexcoords);
				glBufferData(GL_ARRAY_BUFFER, Texcoords.size() * sizeof(Texcoord), &Texcoords[0], GL_STATIC_DRAW);
				glEnableVertexAttribArray(TEXCOORDS);
				glVertexAttribPointer(TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof(Texcoord), 0);
			}
			if (NormalsLoaded)
			{
				glGenBuffers(1, &VboNormals);
				glBindBuffer(GL_ARRAY_BUFFER, VboNormals);
				glBufferData(GL_ARRAY_BUFFER, Normals.size() * sizeof(Normal), &Normals[0], GL_STATIC_DRAW);
				glEnableVertexAttribArray(NORMALS);
				glVertexAttribPointer(NORMALS, 3, GL_FLOAT, GL_FALSE, sizeof(Normal), 0);
			}
			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}

		void destroyBufferObjects()
		{
			glBindVertexArray(VaoId);
			glDisableVertexAttribArray(VERTICES);
			glDisableVertexAttribArray(TEXCOORDS);
			glDisableVertexAttribArray(NORMALS);
			glDeleteBuffers(1, &VboVertices);
			glDeleteBuffers(1, &VboTexcoords);
			glDeleteBuffers(1, &VboNormals);
			glDeleteVertexArrays(1, &VaoId);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
			glBindVertexArray(0);
		}

		void draw() {
			glBindVertexArray(VaoId);
			if (isSand)
				glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)Vertices.size());
			else
				glDrawArrays(GL_TRIANGLES, 0, (GLsizei)Vertices.size());
			glBindVertexArray(0);
		}
		
	};
}