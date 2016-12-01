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
#include <fstream>
#include <sstream>
#include <string>

#include <vector>

#include "mat.h"
#include "Mesh.h"
#include "Shader.h"
#include "engine.h"

#include "GL/glew.h"
#include "GL/freeglut.h"

#define CAPTION "Loading World"
using namespace engine;

int WinX = 640, WinY = 480;
int WindowHandle = 0;
unsigned int FrameCount = 0;

float CameraDistance = 3;

float k = 0;
bool animate = false;
int oldX, oldY;
bool rotated = false;

bool mouseLeftDown;
bool mouseRightDown;
float mouseX, mouseY;
float prevX, prevY;
float key_yaw, key_pitch;

qtrn quat = qtrn::qFromAngleAxis(0, vec4(0, 1, 0, 0));

SceneGraph* scene;
Shader* shader;

ShaderManager *ShaderManager::_instance = nullptr;
SceneGraphManager *SceneGraphManager::_instance = nullptr;
MeshManager *MeshManager::_instance = nullptr;

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
	if (isOpenGLError()) {
		std::cerr << error << std::endl;
		exit(EXIT_FAILURE);
	}
}

void createShaderProgram(std::string& vs_file, std::string& fs_file)
{

	shader = new Shader();
	shader->LoadFromFile(GL_VERTEX_SHADER, vs_file);
	shader->LoadFromFile(GL_FRAGMENT_SHADER, fs_file);

	shader->CreateProgram();

	shader->BindAttributeLocation(VERTICES, "inPosition");
	//if (mesh->areTexcoordsLoaded())
		shader->BindAttributeLocation(TEXCOORDS, "inTexcoord");
	//if (mesh->areNormalsLoaded())
		shader->BindAttributeLocation(NORMALS, "inNormal");

	shader->LinkProgram();

	//ModelMatrix_UId = shader->GetUniformLocation("ModelMatrix");
	//UboId = shader->GetUniformBlockIndex("SharedMatrices");
	ShaderManager::instance()->addShader("shader", shader);

	checkOpenGLError("ERROR: Could not create shaders.");
}

void destroyShaderProgram()
{
    shader->DeleteProgram();
	checkOpenGLError("ERROR: Could not destroy shaders.");
	delete shader;
}


/////////////////////////////////////////////////////////////////////// SCENE
void createScene() {
	Camera *camera = new Camera();
	camera->setViewMatrix(matFactory::Translate3(0,0,-CameraDistance) );
	camera->setProjMatrix(matFactory::PerspectiveProjection(60, (float)WinX/WinY, 0.1, 15));
	scene = new SceneGraph(camera, shader);
	checkOpenGLError("ERROR: Could not build scene.");

	

	Mesh* cubeMesh = new Mesh(std::string("cube.obj"));

	SceneNode *root, *cube;

	root = new SceneNode();
	root->setMatrix(matFactory::Identity4());
	root->setShader(shader);
	root->setColor(vec3(0, 0, 0));
	scene->setRoot(root);

	cube = new SceneNode();
	cube->setMatrix(matFactory::Translate3(-0.5, -0.5, 0));
	cube->setShader(shader);
	cube->setMesh(cubeMesh);
	cube->setColor(vec3(1,0,0));
	root->addNode(cube);



	

}

void destroyScene() {
	
}

void drawScene()
{
	scene->draw();
	checkOpenGLError("ERROR: Could not draw scene.");
}



/////////////////////////////////////////////////////////////////////// CALLBACKS

void cleanup()
{
	destroyShaderProgram();
	destroyScene();	
}

void display()
{
	++FrameCount;
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
void update() {
	
}
void timer(int value)
{
	std::ostringstream oss;
	oss << CAPTION << ": " << FrameCount << " FPS @ (" << WinX << "x" << WinY << ")";
	std::string s = oss.str();
	glutSetWindow(WindowHandle);
	glutSetWindowTitle(s.c_str());
	FrameCount = 0;
	update();
	glutTimerFunc(1000, timer, 0);
}

void myKeydown(unsigned char key, int x, int y) {
	key = tolower(key);
	switch (key) {
	case('g') :
		animate = !animate;
		break;
	}

}

void mouseWheel(int button, int dir, int x, int y)
{
	if (dir > 0) {
		CameraDistance -= 1;
	}
	else {
		CameraDistance += 1;
	}

	mat4 rotate = qGLMatrix(quat);

	scene->getCamera()->setViewMatrix(matFactory::Translate3(0, 0, -CameraDistance) * rotate);
}

void OnMouseDown(int button, int state, int x, int y) {
	rotated = false;
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		oldX = x;
		oldY = y;
		rotated = true;
	}

}

void OnMouseMove(int x, int y) {
	if (rotated) {

		key_yaw = 0.5f * (x - oldX);
		key_pitch = 0.5f * (y - oldY);

		oldX = x;
		oldY = y;


		qtrn rotateX = qtrn::qFromAngleAxis(-key_pitch, vec4(1, 0, 0, 1));
		qtrn rotateY = qtrn::qFromAngleAxis(-key_yaw, vec4(0, 1, 0, 1));
		qtrn newQquat = rotateY * rotateX;

		key_pitch = key_yaw = 0;

		quat = quat * newQquat;
		mat4 rotate = qGLMatrix(quat);

		scene->getCamera()->setViewMatrix (matFactory::Translate3(0, 0, -CameraDistance) * rotate);

	}
}

void SpecialInput(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		scene->root->setMatrix(matFactory::Translate3(0, 0.1, 0) * scene->root->getMatrix());
		break;
	case GLUT_KEY_DOWN:
		scene->root->setMatrix(matFactory::Translate3(0, -0.1, 0) * scene->root->getMatrix());
		break;
	case GLUT_KEY_LEFT:
		scene->root->setMatrix(matFactory::Translate3(-0.1, 0, 0) * scene->root->getMatrix());
		break;
	case GLUT_KEY_RIGHT:
		scene->root->setMatrix(matFactory::Translate3(0.1, 0, 0) * scene->root->getMatrix());
		break;
	}
}

/////////////////////////////////////////////////////////////////////// SETUP

void setupCallbacks()
{
	glutCloseFunc(cleanup);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);

	glutKeyboardFunc(myKeydown);
	glutMouseFunc(OnMouseDown);
	glutMotionFunc(OnMouseMove);
	glutMouseWheelFunc(mouseWheel);
	glutSpecialFunc(SpecialInput);
	glutTimerFunc(0, timer, 0);
}

void setupOpenGL() {
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
	WindowHandle = glutCreateWindow(CAPTION);
	if (WindowHandle < 1) {
		std::cerr << "ERROR: Could not create a new rendering window." << std::endl;
		exit(EXIT_FAILURE);
	}
}

void init(int argc, char* argv[])
{
	setupGLUT(argc, argv);
	setupGLEW();
	setupOpenGL();

	createShaderProgram(std::string("VerticeShader.glsl"),
		std::string("FragmentShader.glsl"));
	createScene();
	setupCallbacks();
}

int main(int argc, char* argv[])
{
	init(argc, argv);
	glutMainLoop();
	exit(EXIT_SUCCESS);
}

///////////////////////////////////////////////////////////////////////
