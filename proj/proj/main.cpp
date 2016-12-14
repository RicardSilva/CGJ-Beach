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

int WinX = 640, WinY = 480;
int WindowHandle = 0;
unsigned int FrameCount = 0;

float CameraDistance = 5;


int oldX, oldY;
bool rotated = false;

bool mouseLeftDown;
bool mouseRightDown;
float mouseX, mouseY;
float prevX, prevY;
float key_yaw, key_pitch;

qtrn quat = qtrn::qFromAngleAxis(0, vec4(0, 0, 1, 0));

SceneGraph* scene;
Shader* shader, *skyboxShader;
//Texture* texture;
//Texture* skyboxTexture;
GLuint UBO_BP=0;
GLint UboID;

//ShaderManager *ShaderManager::_instance = nullptr;


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
	//cubeShader
	Shader *cubeShader = new Shader();
	cubeShader->LoadFromFile(GL_VERTEX_SHADER, "VerticeShader.glsl");
	cubeShader->LoadFromFile(GL_FRAGMENT_SHADER, "FragmentShader.glsl");

	cubeShader->CreateProgram();

	cubeShader->BindAttributeLocation(VERTICES, "inPosition");
	cubeShader->BindAttributeLocation(TEXCOORDS, "inTexcoord");
	cubeShader->BindAttributeLocation(NORMALS, "inNormal");

	cubeShader->LinkProgram();

	//ModelMatrix_UId = shader->GetUniformLocation("ModelMatrix");
	
	ShaderManager::Instance()->AddShader("cubeShader", cubeShader);

	skyboxShader = new Shader();
	skyboxShader->LoadFromFile(GL_VERTEX_SHADER, "VerticeShaderSkybox.glsl");
	skyboxShader->LoadFromFile(GL_FRAGMENT_SHADER, "FragmentShaderSkybox.glsl");

	skyboxShader->CreateProgram();

	skyboxShader->BindAttributeLocation(VERTICES, "inPosition");
	skyboxShader->BindAttributeLocation(TEXCOORDS, "inTexcoord");
	skyboxShader->BindAttributeLocation(NORMALS, "inNormal");



	skyboxShader->LinkProgram();
	ShaderManager::Instance()->AddShader("skyboxShader", skyboxShader);

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
	Camera *camera = new Camera();
	camera->setViewMatrix(matFactory::Translate3(0,0,-CameraDistance) );
	camera->setProjMatrix(matFactory::PerspectiveProjection(60, (float)WinX/WinY, 0.1f, 15));
	scene = new SceneGraph(camera, ShaderManager::Instance()->GetShader("cubeShader"));

	SceneNode *root, *cube, *cube2, *skybox;

	root = new SceneNode();
	root->setMatrix(matFactory::Identity4());
	root->setShader(ShaderManager::Instance()->GetShader("cubeShader"));
	root->setColor(vec3(0, 0, 0));
	scene->setRoot(root);

	cube = new SceneNode();
	cube->setMatrix(matFactory::Translate3(0, 0, 0));

	cube->setTexture(TextureManager::Instance()->GetTexture("dog"));
	cube->setShader(ShaderManager::Instance()->GetShader("cubeShader"));
	cube->setMesh(MeshManager::Instance()->GetMesh("cube"));
	cube->setColor(vec3(1,0,0));
	root->addNode(cube);

	skybox = new SceneNode();
	skybox->setMatrix(matFactory::Scale3(3,3,3));	
	skybox->setSkybox(TextureManager::Instance()->GetTexture("skybox"));
	skybox->setShaderSkybox(ShaderManager::Instance()->GetShader("skyboxShader"));
	skybox->setMesh(MeshManager::Instance()->GetMesh("cube"));
	skybox->setColor(vec3(1, 0, 0));
	root->addNode(skybox);


	checkOpenGLError("ERROR: Could not build scene.");

}

void destroyScene() {
	delete(scene);
}

void drawScene()
{
	scene->draw();
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
		scene->root->setMatrix(matFactory::Translate3(0, 0.1f, 0) * scene->root->getMatrix());
		break;
	case GLUT_KEY_DOWN:
		scene->root->setMatrix(matFactory::Translate3(0, -0.1f, 0) * scene->root->getMatrix());
		break;
	case GLUT_KEY_LEFT:
		scene->root->setMatrix(matFactory::Translate3(-0.1f, 0, 0) * scene->root->getMatrix());
		break;
	case GLUT_KEY_RIGHT:
		scene->root->setMatrix(matFactory::Translate3(0.1f, 0, 0) * scene->root->getMatrix());
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
