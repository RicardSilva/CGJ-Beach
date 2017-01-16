#include "vec.h"
#include "GL/glew.h"
#include "GL/freeglut.h"
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include "Shader.h"
#include "mat.h"
#include <cstdio>
#include <cmath>

namespace engine{

	typedef struct {
		GLfloat x, y, z;
	} Vertex;

	class MeshGenerator{

	public:
		int width;
		int height;
		float* vertices = 0;
		int* indices = 0;

		MeshGenerator(int w, int h){
			width = w;
			height = h;
			getVertices(w, h);
			getIndices(w, h);
		};

		int getVerticesCount(int width, int height) {
			return width * height * 3;
		}

		int getIndicesCount(int width, int height) {
			return (width*height) + (width - 1)*(height - 2);
		}

		float* getVertices(int width, int height) {
			if (vertices) return vertices;

			vertices = new float[getVerticesCount(width, height)];
			int i = 0;

			for (int row = 0; row < height; row++) {
				for (int col = 0; col < width; col++) {
					double c = (double)col;
					vertices[i++] = (float)col;
					vertices[i++] = (rand(*new vec2(col, row)) + atan((c - 50) / 10) * 5);
					vertices[i++] = (float)row;
				}
			}

			return vertices;
		}

		int* getIndices(int width, int height) {
			if (indices) return indices;

			indices = new int[getIndicesCount(width, height)];
			int i = 0;

			for (int row = 0; row < height - 1; row++) {
				if ((row & 1) == 0) { // even rows
					for (int col = 0; col < width; col++) {
						indices[i++] = col + row * width + 1;
						indices[i++] = col + (row + 1) * width + 1;
					}
				}
				else { // odd rows
					for (int col = width - 1; col > 0; col--) {
						indices[i++] = col + (row + 1) * width + 1;
						indices[i++] = col - 1 + +row * width + 1;
					}
				}
			}
			if ((height & 1) && height > 2) {
				indices[i++] = (height - 1) * width + 1;
			}

			return indices;
		}

		float getHeight(vec2 v){
			int i = 0;
			for (int i = 0; i < getVerticesCount(width, height); i++) {
				if (vertices[i] ==v.x){
					if (vertices[i + 2] == v.y)
						return vertices[i+1];
				}
			}
			return 0;

		}


		vec3 calculateNormal(Vertex vertex){
			// read neightbor heights using an arbitrary small offset
			float x0 = vertex.x;
			float y0 = vertex.z;

			float ax = (width / (width - 1));
			float ay = (height / (height - 1));

			vec3 exNormal = *new vec3();
			vec3 up = *new vec3(x0, y0 + ay, getHeight(*new vec2(x0,y0+ay)));
			vec3 upright = *new vec3(x0 + ax, y0 + ay, getHeight(*new vec2(x0 + ax, y0 + ay)));
			vec3 right = *new vec3(x0 + ax, y0, getHeight(*new vec2(x0 + ax, y0)));
			vec3 down = *new vec3(x0, y0 - ay, getHeight(*new vec2(x0, y0 - ay)));
			vec3 downleft = *new vec3(x0 - ax, y0 - ay, getHeight(*new vec2(x0 - ax, y0 - ay)));
			vec3 left = *new vec3(x0 - ax, y0, getHeight(*new vec2(x0 - ax, y0)));

			vec3 N1 = CrossProduct(up, left);

			vec3 N2 = CrossProduct(upright, up);
			vec3 N3 = CrossProduct(right,upright);
			vec3 N4 = CrossProduct(down,right);
			vec3 N5 = CrossProduct(downleft,down);
			vec3 N6 = CrossProduct(left,downleft);

			vec3 N = N1 + N2 + N3 + N4;

			return N.Normalized();
		}

		float rand(vec2 c){
			return (sin(DotProduct(c, vec2(12.9898, 78.233))) * 43758.5453) - floor(sin(DotProduct(c, vec2(12.9898, 78.233))) * 43758.5453);
		}

	};
}