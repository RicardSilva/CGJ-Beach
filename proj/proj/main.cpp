#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <vector>

#include "engine.h"

#include "GL/glew.h"
#include "GL/freeglut.h"


#define CAPTION "Loading World"
using namespace engine;

//int WinX = 640, WinY = 480;
int WindowHandle = 0;
unsigned int FrameCount = 0;

float CameraDistance = 20;


int oldX, oldY;
bool rotated = false;

bool mouseLeftDown;
bool mouseRightDown;
float mouseX, mouseY;
float prevX, prevY;
float key_yaw, key_pitch;
mat4 rotation;

qtrn quat = qtrn::qFromAngleAxis(0, vec4(0, 0, 1, 0));

SceneGraph* scene;
SceneNode* water;
WaterFrameBuffers* wfbos;


//////////////////////////////////////////////////////////////////// LIGHT

struct Light {
	vec3 position;
	vec3 intensities;
	float attenuation;
	float ambientCoefficient; 
};
///// MATERIAL PARSING

void extractMTLdata(string fp, string* materials, float diffuses[][3], float speculars[][3])
{
	// Counters
	int m = 0;
	int d = 0;
	int s = 0;

	// Open MTL file
	ifstream inMTL;
	inMTL.open(fp);
	if (!inMTL.good())
	{
		cout << "ERROR OPENING MTL FILE" << endl;
		exit(1);
	}

	// Read MTL file
	while (!inMTL.eof())
	{
		string line;
		getline(inMTL, line);
		string type = line.substr(0, 2);

		// Names
		if (type.compare("ne") == 0)
		{
			// 1
			// Extract token
			string l = "newmtl ";
			materials[m] = line.substr(l.size());
			m++;
		}

		// 2
		// Diffuses
		else if (type.compare("Kd") == 0)
		{
			// Implementation challenge!
		}

		// 3
		// Speculars
		else if (type.compare("Ks") == 0)
		{
			// Implementation challenge!
		}
	}

	// Close MTL file
	inMTL.close();
}


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
		getchar();
		exit(EXIT_FAILURE);
	}
}

void createShaders()
{
	//waterShader
	Shader *waterShader = new WaterShader();
	waterShader->Init("WaterVerticeShader.glsl", "WaterFragmentShader.glsl");
	ShaderManager::Instance()->AddShader("waterShader", waterShader);

	//simpleCubeShader
	Shader *cubeShader = new CubeShader();
	cubeShader->Init("CubeVerticeShader.glsl", "CubeFragmentShader.glsl");
	ShaderManager::Instance()->AddShader("cubeShader", cubeShader);

	checkOpenGLError("ERROR: Could not create shaders.");
}
void destroyShaders()
{
	ShaderManager::Instance()->Destroy();
	checkOpenGLError("ERROR: Could not destroy shaders.");
}

void createTextures() {
	Texture *dogTexture = new Texture("sample.png");
	TextureManager::Instance()->AddTexture("dog", dogTexture);
	Texture *catTexture = new Texture("csample.png");
	TextureManager::Instance()->AddTexture("cat", catTexture);

	vector<const GLchar*> faces;
	faces.push_back("right.jpg");
	faces.push_back("left.jpg");
	faces.push_back("top.jpg");
	faces.push_back("bottom.jpg");
	faces.push_back("back.jpg");
	faces.push_back("front.jpg");

	Texture *skyboxTexture = new SkyboxTexture(faces);
	TextureManager::Instance()->AddTexture("skybox", skyboxTexture);

	checkOpenGLError("ERROR: Could not create textures.");
}
void destroyTextures()
{
	TextureManager::Instance()->Destroy();
	checkOpenGLError("ERROR: Could not destroy textures.");
}

void createMeshes() {
	Mesh* cubeMesh = new Mesh(std::string("cube.obj"));
	MeshManager::Instance()->AddMesh("cube", cubeMesh);
	checkOpenGLError("ERROR: Could not create meshes.");
}
void destroyMeshes() {
	MeshManager::Instance()->Destroy();
}


/////////////////////////////////////////////////////////////////////// SCENE
void createScene() {
	wfbos = new WaterFrameBuffers();
	Camera *camera = new Camera();
	camera->setViewMatrix(matFactory::Translate3(0,0,-CameraDistance) );
	camera->setProjMatrix(matFactory::PerspectiveProjection(60, (float)WinX/WinY, 0.1f, 50));
	scene = new SceneGraph(camera, ShaderManager::Instance()->GetShader("waterShader"));

	SceneNode *root, *cube, *cube2, *cube3;

	root = new SceneNode();
	root->setMatrix(matFactory::Identity4());
	scene->setRoot(root);

	cube = new SceneNode();
	cube->setMatrix(matFactory::Translate3(2,2,0));
	cube->setColor(vec3(1, 0, 0));
	cube->setShader(ShaderManager::Instance()->GetShader("cubeShader"));
	cube->setMesh(MeshManager::Instance()->GetMesh("cube"));
	cube->setTexture(TextureManager::Instance()->GetTexture("dog"));
	root->addNode(cube);

	cube2 = new SceneNode();
	cube2->setMatrix(matFactory::Translate3(-2, 2, 0));
	cube2->setColor(vec3(0, 1, 0));
	cube2->setShader(ShaderManager::Instance()->GetShader("cubeShader"));
	cube2->setMesh(MeshManager::Instance()->GetMesh("cube"));
	cube2->setTexture(TextureManager::Instance()->GetTexture("cat"));
	root->addNode(cube2);

	cube3 = new SceneNode();
	cube3->setMatrix(matFactory::Translate3(0, -2, 0));
	cube3->setColor(vec3(0, 0, 1));
	cube3->setShader(ShaderManager::Instance()->GetShader("cubeShader"));
	cube3->setMesh(MeshManager::Instance()->GetMesh("cube"));
	cube3->setTexture(TextureManager::Instance()->GetTexture("cat"));
	root->addNode(cube3);

	water = new SceneNode();
	water->setMatrix(matFactory::Scale3(10, 0, 10));
	water->setColor(vec3(1, 1, 0));
	water->setShader(ShaderManager::Instance()->GetShader("waterShader"));
	water->setMesh(MeshManager::Instance()->GetMesh("cube"));

	
	water->setTexture(new Texture(wfbos->getReflectionTexture()));
	

	checkOpenGLError("ERROR: Could not build scene.");

}

void destroyScene() {
	delete(scene);
}

void drawScene()
{
	//render reflection
	wfbos->bindReflectionFrameBuffer();
	//scene->getCamera()->setViewMatrix(matFactory::Translate3(0, 0, -CameraDistance));
	scene->draw(vec4(0,1,0,0));

	//render refractions
	wfbos->bindRefractionFrameBuffer();
	//scene->getCamera()->setViewMatrix(matFactory::Translate3(0, 0, -CameraDistance));
	scene->draw(vec4(0,-1,0,0));

	//render to screen
	wfbos->unbindCurrentFrameBuffer();
	//scene->getCamera()->setViewMatrix(matFactory::Translate3(0, 0, -CameraDistance));
	scene->draw(vec4());
	water->draw(matFactory::Identity4());

	checkOpenGLError("ERROR: Could not draw scene.");
}


/////////////////////////////////////////////////////////////////////// CALLBACKS

void cleanup()
{
	destroyMeshes();
	destroyTextures();
	destroyShaders();
	destroyScene();	
}

void display()
{
	++FrameCount;
	
	wfbos->bindReflectionFrameBuffer();
	glClearColor(0.0f, 0.8f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	wfbos->bindRefractionFrameBuffer();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	wfbos->unbindCurrentFrameBuffer();
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
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
		
		break;
	}
}

//void mouseWheel(int button, int dir, int x, int y)
//{
//	if (dir > 0) {
//		CameraDistance -= 1;
//	}
//	else {
//		CameraDistance += 1;
//	}
//
//	mat4 rotate = qGLMatrix(quat);
//
//	scene->getCamera()->setViewMatrix(matFactory::Translate3(0, 0, -CameraDistance) * rotate);
//}
//
//void OnMouseDown(int button, int state, int x, int y) {
//	rotated = false;
//	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
//		oldX = x;
//		oldY = y;
//		rotated = true;
//	}
//}
//
//void OnMouseMove(int x, int y) {
//	if (rotated) {
//
//		key_yaw = 0.5f * (x - oldX);
//		key_pitch = 0.5f * (y - oldY);
//
//		oldX = x;
//		oldY = y;
//
//		qtrn rotateX = qtrn::qFromAngleAxis(-key_pitch, vec4(1, 0, 0, 1));
//		qtrn rotateY = qtrn::qFromAngleAxis(-key_yaw, vec4(0, 1, 0, 1));
//		qtrn newQquat = rotateY * rotateX;
//
//		key_pitch = key_yaw = 0;
//
//		quat = quat * newQquat;
//		mat4 rotate = qGLMatrix(quat);
//
//		scene->getCamera()->setViewMatrix (matFactory::Translate3(0, 0, -CameraDistance) * rotate);
//	}
//}

void mouseWheel(int button, int dir, int x, int y)
{
	if (dir > 0) {
		CameraDistance -= 1;
	}
	else {
		CameraDistance += 1;
	}

	scene->getCamera()->setViewMatrix(matFactory::Translate3(0, 0, -CameraDistance) * rotation);
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

		key_yaw += 0.5f * (x - oldX);
		if (key_yaw > 360)
			key_yaw -= 360;
		key_pitch += 0.5f * (y - oldY);
		if (key_pitch > 360)
			key_pitch -= 360;

		oldX = x;
		oldY = y;

		mat4 rotateX = matFactory::Rotate3(vec3(0,1,0), key_yaw);
		mat4 rotateY = matFactory::Rotate3(vec3(1,0,0), key_pitch);
		rotation = rotateY * rotateX;
		
		scene->getCamera()->setViewMatrix(matFactory::Translate3(0, 0, -CameraDistance) * rotation);
	}
}

void SpecialInput(int key, int x, int y){}
	

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
	glEnable(GL_CLIP_DISTANCE0);
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

	createShaders();
	createMeshes();
	createTextures();
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
