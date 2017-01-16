#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <vector>

#include "engine.h"

#include "GL/glew.h"
#include "GL/freeglut.h"

#include <chrono>



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
mat4 rotation, inverseRotation;

qtrn quat = qtrn::qFromAngleAxis(0, vec4(0, 0, 1, 0));

SceneGraph* scene;
SceneNode* water;
WaterFrameBuffers* wfbos;
Camera *mainCamera, *upCamera, *downCamera;


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
	Shader *waterShader = new WaterShader("WaterVerticeShader.glsl", "WaterFragmentShader.glsl");
	ShaderManager::Instance()->AddShader("waterShader", waterShader);

	//simpleCubeShader
	Shader *cubeShader = new CubeShader("CubeVerticeShader.glsl", "CubeFragmentShader.glsl");
	ShaderManager::Instance()->AddShader("cubeShader", cubeShader);

	//skybox shader
	Shader *skyboxShader = new SkyboxShader("SkyboxVerticeShader.glsl", "SkyboxFragmentShader.glsl");
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
	faces.push_back("Box_Right.bmp");
	faces.push_back("Box_Left.bmp");
	faces.push_back("Box_Top.bmp");
	faces.push_back("Box_Bottom.bmp");
	faces.push_back("Box_Back.bmp");
	faces.push_back("Box_Front.bmp");

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

	Mesh* quadMesh = new Mesh(std::string("quad.obj"));
	MeshManager::Instance()->AddMesh("quad", quadMesh);
	checkOpenGLError("ERROR: Could not create meshes.");
}
void destroyMeshes() {
	MeshManager::Instance()->Destroy();
}

void createCameras() {
	mainCamera = new Camera(vec4());
	mainCamera->setViewMatrix(matFactory::Translate3(0, 0, -CameraDistance));
	mainCamera->setProjMatrix(matFactory::PerspectiveProjection(60, (float)WinX / WinY, 0.1f, 50));

	upCamera = new Camera(vec4(0, -1, 0, 0.5));
	upCamera->setViewMatrix(matFactory::Translate3(0, 0, -CameraDistance));
	upCamera->setProjMatrix(matFactory::PerspectiveProjection(60, (float)WinX / WinY, 0.1f, 50));

	downCamera = new Camera(vec4(0, 1, 0, 0.5));
	downCamera->setViewMatrix(matFactory::Translate3(0, 0, -CameraDistance));
	downCamera->setProjMatrix(matFactory::PerspectiveProjection(60, (float)WinX / WinY, 0.1f, 50));
}
void destroyCameras() {
	delete(mainCamera);
	delete(upCamera);
	delete(downCamera);
}

/////////////////////////////////////////////////////////////////////// SCENE
void createScene() {
	wfbos = new WaterFrameBuffers();
	scene = new SceneGraph(mainCamera, ShaderManager::Instance()->GetShader("waterShader"));

	SceneNode *root, *cube, *cube2, *cube3, *skybox;
	Texture * skyboxTexture;

	root = new SceneNode();
	root->setMatrix(matFactory::Identity4());
	scene->setRoot(root);
	skybox = new SkyboxSceneNode();
	skybox->setMatrix(matFactory::Scale3(100, 100, 100));
	vector<const GLchar*> faces = { "Box_Left.bmp" , "Box_Right.bmp" ,"Box_Top.bmp" , "Box_Bottom.bmp" , "Box_Back.bmp" ,"Box_Front.bmp" };

	skyboxTexture = new SkyboxTexture(faces);
	skybox->setTexture(skyboxTexture);
	skybox->setShader(ShaderManager::Instance()->GetShader("skyboxShader"));
	skybox->setMesh(MeshManager::Instance()->GetMesh("quad"));
	skybox->setColor(vec3(1, 0, 0));
	root->addNode(skybox);

	cube = new SceneNode();
	cube->setMatrix(matFactory::Scale3(2, 2, 2) * matFactory::Translate3(2,2,-2));
	cube->setColor(vec3(1, 0, 0));
	cube->setShader(ShaderManager::Instance()->GetShader("cubeShader"));
	cube->setMesh(MeshManager::Instance()->GetMesh("cube"));
	cube->setTexture(TextureManager::Instance()->GetTexture("dog"));
	root->addNode(cube);

	cube2 = new SceneNode();
	cube2->setMatrix(matFactory::Scale3(2, 2, 2) *matFactory::Translate3(-2, 2, -2));
	cube2->setColor(vec3(0, 1, 0));
	cube2->setShader(ShaderManager::Instance()->GetShader("cubeShader"));
	cube2->setMesh(MeshManager::Instance()->GetMesh("cube"));
	cube2->setTexture(TextureManager::Instance()->GetTexture("cat"));
	root->addNode(cube2);

	cube3 = new SceneNode();
	cube3->setMatrix(matFactory::Scale3(10, 2, 10) *matFactory::Translate3(0, -2, 0));
	cube3->setColor(vec3(0, 0, 1));
	cube3->setShader(ShaderManager::Instance()->GetShader("cubeShader"));
	cube3->setMesh(MeshManager::Instance()->GetMesh("cube"));
	cube3->setTexture(TextureManager::Instance()->GetTexture("cat"));
	root->addNode(cube3);

	


	water = new WaterSceneNode();
	water->setMatrix(matFactory::Scale3(10, 0, 10));
	water->setColor(vec3(1, 1, 0));
	water->setShader(ShaderManager::Instance()->GetShader("waterShader"));
	water->setMesh(MeshManager::Instance()->GetMesh("cube"));
	water->setReflectionTexture(wfbos->getReflectionTexture());
	water->setRefractionTexture(wfbos->getRefractionTexture());
	water->setDepthTexture(wfbos->getRefractionDepthTexture());
	

	checkOpenGLError("ERROR: Could not build scene.");

}

void destroyScene() {
	delete(scene);
}

void drawScene()
{
	//render reflection
	wfbos->bindReflectionFrameBuffer();
	scene->setCamera(downCamera);
	scene->draw();

	//render refraction
	wfbos->bindRefractionFrameBuffer();
	scene->setCamera(upCamera);
	scene->draw();

	//render to screen
	wfbos->unbindCurrentFrameBuffer();
	scene->setCamera(mainCamera);
	scene->draw();

	//render water
	water->draw(matFactory::Identity4());

	checkOpenGLError("ERROR: Could not draw scene.");
}


/////////////////////////////////////////////////////////////////////// CALLBACKS

void cleanup()
{
	destroyMeshes();
	destroyTextures();
	destroyShaders();
	destroyCameras();
	destroyScene();	
}

void display()
{
	++FrameCount;
	
	wfbos->bindReflectionFrameBuffer();
	//glClearColor(0.0f, 0.8f, 1.0f, 1.0f);
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

void screenshot(const std::string& filename, int x, int y)
{// get the image data
	long imageSize = x * y * 3;
	unsigned char *data = new unsigned char[imageSize];
	glReadPixels(0, 0, x, y, GL_BGR, GL_UNSIGNED_BYTE, data);// split x and y sizes into bytes
	int xa = x % 256;
	int xb = (x - xa) / 256; int ya = y % 256;
	int yb = (y - ya) / 256;//assemble the header
	unsigned char header[18] = { 0,0,2,0,0,0,0,0,0,0,0,0,(char)xa,(char)xb,(char)ya,(char)yb,24,0 };
	// write header and data to file
	fstream File(filename, ios::out | ios::binary);
	File.write(reinterpret_cast<char *>(header), sizeof(char) * 18);
	File.write(reinterpret_cast<char *>(data), sizeof(char)*imageSize);
	File.close();

	delete[] data;
	data = NULL;
}

void screenshotPNG(const std::string& filename) {
	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);
	SOIL_save_screenshot(filename.c_str(), SOIL_SAVE_TYPE_BMP,0,0, w, h);
}

void myKeydown(unsigned char key, int x, int y) {
	key = tolower(key);
	switch (key) {
	case('p') :
		unsigned long milliseconds_since_epoch =
		std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		screenshotPNG(std::string("../../printscreens/") + std::to_string(milliseconds_since_epoch) + std::string(".bmp"));
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

	//scene->getCamera()->setViewMatrix(matFactory::Translate3(0, 0, -CameraDistance) * rotation);
	mainCamera->setViewMatrix(matFactory::Translate3(0, 0, -CameraDistance) * rotation);
	upCamera->setViewMatrix(matFactory::Translate3(0, 0, -CameraDistance) * rotation);
	downCamera->setViewMatrix(matFactory::Translate3(0, 0, -CameraDistance) * inverseRotation);
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

		mat4 rotateInverseY = matFactory::Rotate3(vec3(1, 0, 0), -key_pitch);
		inverseRotation = rotateInverseY * rotateX;
		
		//scene->getCamera()->setViewMatrix(matFactory::Translate3(0, 0, -CameraDistance) * rotation);
		mainCamera->setViewMatrix(matFactory::Translate3(0, 0, -CameraDistance) * rotation);
		upCamera->setViewMatrix(matFactory::Translate3(0, 0, -CameraDistance) * rotation);
		downCamera->setViewMatrix(matFactory::Translate3(0, 0, -CameraDistance) * inverseRotation);
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
	createCameras();
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
