///////////////////////////////////////////////////////////////////////
//
//  Loading OBJ mesh from external file
//
//	Final individual assignment:
//	1.	Create classes: Scene, Camera and Mesh (that loads a
//		mesh from a Wavefront OBJ file) and build a small
//		scenegraph of your tangram scene (you may create more 
//		classes if needed).
//	2.	Create a ground object and couple the tangram figure
//		to the ground. Press keys to move the ground about: 
//		the tangram figure must follow the ground.
//	3.	Animate between closed puzzle (initial square) and 
//		tangram figure by pressing a key.
//	4.	Spherical camera interaction through mouse. It should
//		be possible to interact while animation is playing.
//
//	Team assignment:
//	Pick your team (3 elements from one same lab) for the team
//	assignment you will be working until the end of the semester,
//	and register it online.
//
// (c) 2013-16 by Carlos Martinho
//
///////////////////////////////////////////////////////////////////////


#include <iostream>
#include <sstream>
#include <string>

#include "GL/glew.h"
#include "GL/freeglut.h"

#include "MatrixFactory.h"
#include "MatrixStack.h"
#include "MyShader.h"
#include "Qtrn.h"
#include "Vector2.h"
#include "Camera.h"
#include "Mesh.h"

#define CAPTION "OBJ_loader"

#define DEGREES_TO_RADIANS 0.01745329251994329547
#define RADIANS_TO_DEGREES 57.29577951308232185913

#define ANIMATION_SPEED 0.01

int WinX = 640, WinY = 480;
int WindowHandle = 1;
unsigned int FrameCount = 0;

GLuint VaoId;
GLuint VboVertices, VboTexcoords, VboNormals;

MyShader ms = MyShader();
MatrixFactory mf;
std::vector<Camera> cams;
Mesh square = Mesh();
Mesh triangle = Mesh();
Mesh parallelogram = Mesh();
Mesh scenefloor = Mesh();

MatrixStack ModelMatrixStack = MatrixStack();

int camera = 0;
int rotate = 0;
int lock = 0;
float anpoint = 0;
int animated = 0;

Qtrn q = qFromAngleAxis(0.0f, Vector4(1, 0, 0, 1));
Matrix4x4 scaleMatrix = mf.identity4();

int oldX, oldY;
float theta = 0, phi = 0;



/////////////////////////////////////////////////////////////////////// ERRORS

bool isOpenGLError() {
	bool isError = false;
	GLenum errCode;
	const GLubyte *errString;
	while ((errCode = glGetError()) != GL_NO_ERROR) {
		isError = true;
		errString = gluErrorString(errCode);
		std::cerr << "OpenGL ERROR [" << errString << "]." << std::endl;
	}
	return isError;
}

void checkOpenGLError(std::string error)
{
	if(isOpenGLError()) {
		std::cerr << error << std::endl;
		exit(EXIT_FAILURE);
	}
}

/////////////////////////////////////////////////////////////////////// VAOs & VBOs

void createBufferObjects()
{
	glGenVertexArrays(1, &VaoId);
	glBindVertexArray(VaoId);
	{
		glGenBuffers(1, &VboVertices);
		glBindBuffer(GL_ARRAY_BUFFER, VboVertices);
		glBufferData(GL_ARRAY_BUFFER, ms.Vertices.size() * sizeof(Vector3), &ms.Vertices[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(VERTICES);
		glVertexAttribPointer(VERTICES, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0);
	}
	if (ms.TexcoordsLoaded)
	{
		glGenBuffers(1, &VboTexcoords);
		glBindBuffer(GL_ARRAY_BUFFER, VboTexcoords);
		glBufferData(GL_ARRAY_BUFFER, ms.Texcoords.size() * sizeof(Vector2), &ms.Texcoords[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(TEXCOORDS);
		glVertexAttribPointer(TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2), 0);
	}
	if (ms.NormalsLoaded)
	{
		glGenBuffers(1, &VboNormals);
		glBindBuffer(GL_ARRAY_BUFFER, VboNormals);
		glBufferData(GL_ARRAY_BUFFER, ms.Normals.size() * sizeof(Vector3), &ms.Normals[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(NORMALS);
		glVertexAttribPointer(NORMALS, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0);
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	checkOpenGLError("ERROR: Could not create VAOs and VBOs.");
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
	glBindVertexArray(0);

	checkOpenGLError("ERROR: Could not destroy VAOs and VBOs.");
}

/////////////////////////////////////////////////////////////////////// SCENE

void drawScene()
{	
	q = qNormalize(q);
	Matrix4x4 Rot = mf.create4fromQuat(q._x, q._y, q._z, q._t);
	Matrix4x4 Trans = mf.createTranslation4(0,0,-5);
	Matrix4x4 Scale = scaleMatrix;
	Matrix4x4 ViewMatrix3 = Trans * Rot * Scale;

	glBindVertexArray(VaoId);
	glUseProgram(ms.ProgramId);

	if (lock)
		cams[camera].activateViewMatrix(ms.ViewMatrix_UId);
	else
		glUniformMatrix4fv(ms.ViewMatrix_UId, 1, GL_FALSE, mf.createGLfrom4(ViewMatrix3));

	cams[camera].activateProjMatrix(ms.ProjectionMatrix_UId);

	ModelMatrixStack.pushMatrix();

		//Floor
		glUniformMatrix4fv(ms.ModelMatrix_UId, 1, GL_FALSE, mf.createGLfrom4(ModelMatrixStack.getModelMatrix()));
		scenefloor.draw();

		Vector3 axis = Vector3(0, 0, 1);
		Vector3 xaxis = Vector3(1, 0, 0);
	
		//Big triangles

		ModelMatrixStack.pushMatrix();

			//Lower Body
			ModelMatrixStack.translate(Vector3(-0.37f, 0.27f, 0.0f)*anpoint);
			ModelMatrixStack.rotate(-45 * anpoint, axis);

			ModelMatrixStack.translate(Vector3(0.275f, 0.25f, 0.0f));
			ModelMatrixStack.rotate(180, axis);
			ModelMatrixStack.scale(Vector3(sqrt(2) / 2, sqrt(2) / 2, 1));

			glUniformMatrix4fv(ms.ModelMatrix_UId, 1, GL_FALSE, mf.createGLfrom4(ModelMatrixStack.getModelMatrix()));
			triangle.draw();

		ModelMatrixStack.popMatrix();

		ModelMatrixStack.pushMatrix();

			//Upper Body
			ModelMatrixStack.translate(Vector3(-0.225f, 1.0f, 0.0f)*anpoint);	
			ModelMatrixStack.rotate(180 * anpoint, axis);


			ModelMatrixStack.translate(Vector3(-0.225f, 0.75f, 0.0f));
			ModelMatrixStack.rotate(-135, axis);
			ModelMatrixStack.scale(Vector3(sqrt(2) / 2, sqrt(2) / 2, 1));
	
			glUniformMatrix4fv(ms.ModelMatrix_UId, 1, GL_FALSE, mf.createGLfrom4(ModelMatrixStack.getModelMatrix()));
			triangle.draw();

		ModelMatrixStack.popMatrix();

		ModelMatrixStack.pushMatrix();

			//Medium triangle
			ModelMatrixStack.translate(Vector3(0.24, -0.525f, 0.0f)*anpoint);
			ModelMatrixStack.rotate(90 * anpoint, axis);

			ModelMatrixStack.translate(Vector3(0.275f, -0.25f, 0.0f));
			ModelMatrixStack.rotate(0, axis);
			ModelMatrixStack.scale(Vector3(0.5f, 0.5f, 1));

			glUniformMatrix4fv(ms.ModelMatrix_UId, 1, GL_FALSE, mf.createGLfrom4(ModelMatrixStack.getModelMatrix()));
			triangle.draw();

		ModelMatrixStack.popMatrix();

		ModelMatrixStack.pushMatrix();

			//Small triangles
			//Foot
			ModelMatrixStack.translate(Vector3(-0.4f, 0.325f, 0.0f)*anpoint);

			ModelMatrixStack.translate(Vector3(0.15f, -0.325f, 0.0f));
			ModelMatrixStack.rotate(-135, axis);
			ModelMatrixStack.scale(Vector3(sqrt(2) / 4, sqrt(2) / 4, 1));

			glUniformMatrix4fv(ms.ModelMatrix_UId, 1, GL_FALSE, mf.createGLfrom4(ModelMatrixStack.getModelMatrix()));
			triangle.draw();

		ModelMatrixStack.popMatrix();

		ModelMatrixStack.pushMatrix();

			//Beak
			ModelMatrixStack.translate(Vector3(-0.725f, 0.5f, 0.0f)*anpoint);
			ModelMatrixStack.rotate(180 * anpoint, axis);

			ModelMatrixStack.translate(Vector3(-0.725f, 0.25f, 0.0f));
			ModelMatrixStack.rotate(135, axis);
			ModelMatrixStack.scale(Vector3(sqrt(2) / 4, sqrt(2) / 4, 1));

			glUniformMatrix4fv(ms.ModelMatrix_UId, 1, GL_FALSE, mf.createGLfrom4(ModelMatrixStack.getModelMatrix()));
			triangle.draw();

		ModelMatrixStack.popMatrix();

		ModelMatrixStack.pushMatrix();

			//Square
			ModelMatrixStack.translate(Vector3(0.725f, -0.5f, 0.0f)*anpoint);

			ModelMatrixStack.translate(Vector3(-0.475f, 0.5f, 0.0f));
			ModelMatrixStack.rotate(135, axis);
			ModelMatrixStack.scale(Vector3(sqrt(2) / 4, sqrt(2) / 4, 1));
	
			glUniformMatrix4fv(ms.ModelMatrix_UId, 1, GL_FALSE, mf.createGLfrom4(ModelMatrixStack.getModelMatrix()));
			square.draw();

		ModelMatrixStack.popMatrix();

		ModelMatrixStack.pushMatrix();

			//Parallelogram
			ModelMatrixStack.translate(Vector3(0.225f, -0.375f, 0.2f) * anpoint);
			ModelMatrixStack.rotate(45 * anpoint, axis);
			ModelMatrixStack.rotate(-180 * anpoint, xaxis);

			ModelMatrixStack.translate(Vector3(0.275f, -0.25f, 0.0f));
			ModelMatrixStack.rotate(-45, axis);
			ModelMatrixStack.scale(Vector3(0.5f, 0.5f, 1));

			glUniformMatrix4fv(ms.ModelMatrix_UId, 1, GL_FALSE, mf.createGLfrom4(ModelMatrixStack.getModelMatrix()));
			parallelogram.draw();

		ModelMatrixStack.popMatrix();

	ModelMatrixStack.popMatrix();

	glUseProgram(0);
	glBindVertexArray(0);

	checkOpenGLError("ERROR: Could not draw scene.");
}

/////////////////////////////////////////////////////////////////////// CALLBACKS

void keyPressed(unsigned char key, int x, int y) {
	switch (key) {
	case 'P':
	case 'p':
		camera ^= 1;
		break;

	case 'G':
	case 'g':
		lock ^= 1;
		break;

	case 'L':
	case 'l':
		animated ^= 1;
		break;

	case 'W':
	case 'w':
		ModelMatrixStack.translate(Vector3(0.0f, 0.05f, 0.0f));
		break;

	case 'A':
	case 'a':
		ModelMatrixStack.translate(Vector3(-0.05f, 0.0f, 0.0f));
		break;

	case 'S':
	case 's':
		ModelMatrixStack.translate(Vector3(0.0f, -0.05f, 0.0f));
		break;

	case 'D':
	case 'd':
		ModelMatrixStack.translate(Vector3(0.05f, 0.0f, 0.0f));
		break;

	case 'R':
	case 'r':
		ModelMatrixStack.translate(Vector3(0.0f, 0.0f, -0.05f));
		break;

	case 'F':
	case 'f':
		ModelMatrixStack.translate(Vector3(0.0f, 0.0f, 0.05f));
		break;
	}
}

void cleanup()
{
	ms.destroyShaderProgram();
	destroyBufferObjects();
}

void display()
{
	++FrameCount;
	if (animated == 1 && anpoint < 1) {
		anpoint += ANIMATION_SPEED;
		if (anpoint > 1) anpoint = 1;
	}
	else if(animated == 0 && anpoint > 0) {
		anpoint -= ANIMATION_SPEED;
		if (anpoint < 0) anpoint = 0;
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawScene();
	glutSwapBuffers();
}

void idle()
{
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	WinX = w;
	WinY = h;
	glViewport(0, 0, WinX, WinY);
}

void timer(int value)
{
	std::ostringstream oss;
	oss << CAPTION << ": " << FrameCount << " FPS @ (" << WinX << "x" << WinY << ")";
	std::string s = oss.str();
	glutSetWindow(WindowHandle);
	glutSetWindowTitle(s.c_str());
	FrameCount = 0;
	glutTimerFunc(1000, timer, 0);
}

void click(int button, int state, int x, int y) {
	if (button == 0) {//Left Mouse
		oldX = x;
		oldY = y;
		rotate ^= 1;
	}
	else if (button == 3) {//Scroll Up
		Matrix4x4 scale = mf.createScale4(1.01, 1.01, 1.01);

		cams[0].ViewMatrix = cams[0].ViewMatrix * scale;
		cams[1].ViewMatrix = cams[1].ViewMatrix * scale;
		scaleMatrix = scaleMatrix * scale;
	}
	else if (button == 4) {//Scroll Down
		Matrix4x4 scale = mf.createScale4(1/1.01, 1/1.01, 1/1.01);

		cams[0].ViewMatrix = cams[0].ViewMatrix * scale;
		cams[1].ViewMatrix = cams[1].ViewMatrix * scale;
		scaleMatrix = scaleMatrix * scale;
	}
}

void drag(int x, int y) {

	if (rotate) {
		theta = (x - oldX)*0.01f;
		phi = (y - oldY)*0.01f;

		oldX = x;
		oldY = y;

		Matrix4x4 rot = mf.createRotation(Vector3(1, 0, 0), phi * RADIANS_TO_DEGREES) * mf.createRotation(Vector3(0, 1, 0), theta * RADIANS_TO_DEGREES);

		Qtrn qt1 = qFromAngleAxis(phi * RADIANS_TO_DEGREES, Vector4(-1, 0, 0, 1));
		Qtrn qt2 = qFromAngleAxis(theta * RADIANS_TO_DEGREES, Vector4(0, -1, 0, 1));

		q = qMultiply(q, qMultiply(qt1, qt2));

		cams[0].ViewMatrix = cams[camera].ViewMatrix * rot;
		cams[1].ViewMatrix = cams[camera].ViewMatrix * rot;
	}
}


/////////////////////////////////////////////////////////////////////// SETUP

void setupCallbacks() 
{
	glutCloseFunc(cleanup);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyPressed);
	glutMouseFunc(click);
	glutMotionFunc(drag);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutTimerFunc(0, timer, 0);
}

void setupOpenGL(){
	std::cerr << "CONTEXT: OpenGL v" << glGetString(GL_VERSION) << std::endl;
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glDepthRange(0.0, 1.0);
	glClearDepth(1.0);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
}

void setupGLEW() {
	glewExperimental = GL_TRUE;
	GLenum result = glewInit();
	if (result != GLEW_OK) {
		std::cerr << "ERROR glewInit: " << glewGetString(result) << std::endl;
		exit(EXIT_FAILURE);
	}
	GLenum err_code = glGetError();
}

void setupGLUT(int argc, char* argv[])
{
	glutInit(&argc, argv);

	glutInitContextVersion(3, 3);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	glutInitWindowSize(WinX, WinY);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	int window_handle = glutCreateWindow(CAPTION);
	if (window_handle < 1) {
		std::cerr << "ERROR: Could not create a new rendering window." << std::endl;
		exit(EXIT_FAILURE);
	}
}

void init(int argc, char* argv[])
{
	setupGLUT(argc, argv);
	setupGLEW();
	setupOpenGL();
	mf = MatrixFactory();
	ModelMatrixStack = MatrixStack(mf);
	Vector3 eye = Vector3(0, 0, 4);	
	Vector3 center = Vector3(0, 0, 0);
	Vector3 up = Vector3(0, 1, 0);
	cams.push_back(Camera(mf, eye, center, up, -2, 2, -2, 2, 1, 10));
	cams.push_back(Camera(mf, eye, center, up, 30, 640 / 480, 1, 10));

	triangle.createMesh(std::string("triangle.obj"), &ms);
	square.createMesh(std::string("square.obj"), &ms);
	parallelogram.createMesh(std::string("parallelogram.obj"), &ms);
	scenefloor.createMesh(std::string("floor.obj"), &ms);
	ms.createShaderProgram(std::string("cube_vs.glsl"),
		std::string("cube_fs.glsl"));
	createBufferObjects();
	setupCallbacks();
}

int main(int argc, char* argv[])
{
	init(argc, argv);
	glutMainLoop();
	exit(EXIT_SUCCESS);
}

///////////////////////////////////////////////////////////////////////