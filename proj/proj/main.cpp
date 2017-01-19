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

float CameraDistance = 10;


int oldX, oldY;
bool rotated = false;
float param = 0.0f;
float variableHeight;

bool mouseLeftDown;
bool mouseRightDown;
float mouseX, mouseY;
float prevX, prevY;
float key_yaw, key_pitch;
mat4 rotation, inverseRotation;

qtrn quat = qtrn::qFromAngleAxis(0, vec4(0, 0, 1, 0));

SceneGraph* scene;
SceneNode *water, *sand, *tree, *leafs, *tree2, *leafs2, *shrub, *shrubLeafs;
WaterFrameBuffers* wfbos;
Camera *mainCamera, *upCamera, *downCamera;


//////////////////////////////////////////////////////////////////// LIGHT

struct Light {
	vec3 position;
	vec3 intensities;
	float attenuation;
	float ambientCoefficient; 
};

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

	//sandShader
	Shader *sandShader = new CubeShader("VerticeShaderSand.glsl", "FragmentShaderSand.glsl");
	ShaderManager::Instance()->AddShader("sandShader", sandShader);
	//flatSandShader
	Shader *flatSandShader = new CubeShader("VerticeShaderSand.glsl", "FragmentShaderFlatSand.glsl");
	ShaderManager::Instance()->AddShader("flatSandShader", flatSandShader);

	//rockShader
	Shader *rockShader = new CubeShader("RockVerticeShader.glsl", "RockFragmentShader.glsl");
	ShaderManager::Instance()->AddShader("rockShader", rockShader);

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
	Texture *sandTexture = new Texture("sand2.jpg");
	TextureManager::Instance()->AddTexture("sand", sandTexture);

	Texture *trunkTexture = new Texture("palm_bark.png");
	TextureManager::Instance()->AddTexture("trunk", trunkTexture);
	Texture *leafsTexture = new Texture("palm_leafs.png");
	TextureManager::Instance()->AddTexture("leafs", leafsTexture);


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

	Mesh* rockMesh = new Mesh(std::string("rock2.obj"));
	MeshManager::Instance()->AddMesh("rock", rockMesh);

	Mesh* sandMesh = new Mesh(false);
	MeshManager::Instance()->AddMesh("sand", sandMesh);

	Mesh* sandMeshFlat = new Mesh(true);
	MeshManager::Instance()->AddMesh("sandFlat", sandMeshFlat);

	Mesh* quadMesh = new Mesh(std::string("quad.obj"));
	MeshManager::Instance()->AddMesh("quad", quadMesh);

	Mesh* treeMesh = new Mesh(std::string("palmTrunk.obj"));
	MeshManager::Instance()->AddMesh("tree", treeMesh);
	Mesh* leafsMesh = new Mesh(std::string("palmLeafs.obj"));
	MeshManager::Instance()->AddMesh("leafs", leafsMesh);

	Mesh* shrubMesh = new Mesh(std::string("shrubTrunk.obj"));
	MeshManager::Instance()->AddMesh("shrub", shrubMesh);
	Mesh* shrubLeafsMesh = new Mesh(std::string("shrubLeafs.obj"));
	MeshManager::Instance()->AddMesh("shrubLeafs", shrubLeafsMesh);

	checkOpenGLError("ERROR: Could not create meshes.");
}

void destroyMeshes() {
	MeshManager::Instance()->Destroy();
}

void createMaterials() {
	Material* cubeMaterial = new Material(std::string("cube.mtl"));
	MaterialManager::Instance()->AddMaterial("cube", cubeMaterial);
	
	Material* rockMaterial = new Material(std::string("rock2.mtl"));
	MaterialManager::Instance()->AddMaterial("rock", rockMaterial);

	Material* sandMaterial = new Material(std::string("sand.mtl"));
	MaterialManager::Instance()->AddMaterial("sand", sandMaterial);
	checkOpenGLError("ERROR: Could not create materials.");
}
void destroyMaterials() {
	MaterialManager::Instance()->Destroy();
}

void createCameras() {
	mainCamera = new Camera(vec4());
	mainCamera->setViewMatrix(matFactory::Translate3(0, 0, -CameraDistance));
	mainCamera->setProjMatrix(matFactory::PerspectiveProjection(60, (float)WinX / WinY, 0.1f, 20));

	upCamera = new Camera(vec4(0, -1, 0, 0));
	upCamera->setViewMatrix(matFactory::Translate3(0, 0, -CameraDistance));
	upCamera->setProjMatrix(matFactory::PerspectiveProjection(60, (float)WinX / WinY, 0.1f, 20));

	downCamera = new Camera(vec4(0, 1, 0, 0));
	downCamera->setViewMatrix(matFactory::Translate3(0, 0, -CameraDistance));
	downCamera->setProjMatrix(matFactory::PerspectiveProjection(60, (float)WinX / WinY, 0.1f, 20));
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

	SceneNode *root, *root2, *cube, *cube2, *rock, *rock2, *rock3, *rock32, *rock33, *rock4, *sandFlat, *skybox;

	Texture * skyboxTexture;


	root = new SceneNode(false);
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


	rock = new SceneNode(false);
	rock->setMatrix(matFactory::Scale3(0.1, 0.1, 0.1) *matFactory::Translate3(2, 0, 40));
	rock->setShader(ShaderManager::Instance()->GetShader("rockShader"));
	rock->setMesh(MeshManager::Instance()->GetMesh("rock"));
	root->addNode(rock);

	rock2 = new SceneNode(false);
	rock2->setMatrix(matFactory::Scale3(0.1, 0.1, 0.1) *matFactory::Translate3(-42, -8, 2));
	rock2->setShader(ShaderManager::Instance()->GetShader("rockShader"));
	rock2->setMesh(MeshManager::Instance()->GetMesh("rock"));
	root->addNode(rock2);

	rock3 = new SceneNode(false);
	rock3->setMatrix(matFactory::Scale3(0.1, 0.1, 0.1) *matFactory::Translate3(20, 5, -25));
	rock3->setShader(ShaderManager::Instance()->GetShader("rockShader"));
	rock3->setMesh(MeshManager::Instance()->GetMesh("rock"));
	root->addNode(rock3);
	
	rock32 = new SceneNode(false);
	rock32->setMatrix(matFactory::Scale3(0.1, 0.1, 0.1) *matFactory::Translate3(22, 5, -25)*matFactory::Rotate3(*new vec3(1, 1, 1), -45));
	rock32->setShader(ShaderManager::Instance()->GetShader("rockShader"));
	rock32->setMesh(MeshManager::Instance()->GetMesh("rock"));
	root->addNode(rock32);

	rock33 = new SceneNode(false);
	rock33->setMatrix(matFactory::Scale3(0.1, 0.1, 0.1) *matFactory::Translate3(21, 5, -26)*matFactory::Rotate3(*new vec3(1, 1, 1), 90));
	rock33->setShader(ShaderManager::Instance()->GetShader("rockShader"));
	rock33->setMesh(MeshManager::Instance()->GetMesh("rock"));
	root->addNode(rock33);

	rock4 = new SceneNode(false);
	rock4->setMatrix(matFactory::Scale3(0.1, 0.1, 0.1) *matFactory::Translate3(18, 5, 23));
	rock4->setShader(ShaderManager::Instance()->GetShader("rockShader"));
	rock4->setMesh(MeshManager::Instance()->GetMesh("rock"));
	root->addNode(rock4);

	sand = new SandSceneNode();
	sand->setMatrix(matFactory::Scale3(0.1, 0.1, 0.1) *matFactory::Translate3(-50, -2, -50));
	sand->setShader(ShaderManager::Instance()->GetShader("sandShader"));
	sand->setMesh(MeshManager::Instance()->GetMesh("sand"));
	sand->setMaterial(MaterialManager::Instance()->GetMaterial("sand"));
	sand->setTexture(TextureManager::Instance()->GetTexture("sand"));
	root->addNode(sand);


	sandFlat = new FlatSandSceneNode();
	sandFlat->setMatrix(matFactory::Scale3(0.1, 0.1, 0.1) *matFactory::Translate3(-50, -2, -50));
	sandFlat->setShader(ShaderManager::Instance()->GetShader("flatSandShader"));
	sandFlat->setMesh(MeshManager::Instance()->GetMesh("sandFlat"));
	sandFlat->setMaterial(MaterialManager::Instance()->GetMaterial("sand"));
	sandFlat->setTexture(TextureManager::Instance()->GetTexture("sand"));
	root->addNode(sandFlat);

	tree = new SceneNode(true);
	tree->setMatrix(matFactory::Translate3(3, 0.5, -2) * matFactory::Scale3(0.05f, 0.05f, 0.05f));
	tree->setMesh(MeshManager::Instance()->GetMesh("tree"));
	tree->setShader(ShaderManager::Instance()->GetShader("cubeShader"));
	tree->setTexture(TextureManager::Instance()->GetTexture("trunk"));
	root->addNode(tree);

	leafs = new SceneNode(true);
	leafs->setMatrix(matFactory::Translate3(3, 0.5, -2) * matFactory::Scale3(0.05f, 0.05f, 0.05f));
	leafs->setMesh(MeshManager::Instance()->GetMesh("leafs"));
	leafs->setShader(ShaderManager::Instance()->GetShader("cubeShader"));
	leafs->setTexture(TextureManager::Instance()->GetTexture("leafs"));
	root->addNode(leafs);

	tree2 = new SceneNode(true);
	tree2->setMatrix(matFactory::Translate3(2, 0.5, 1) * matFactory::Scale3(0.04f, 0.04f, 0.04f));
	tree2->setMesh(MeshManager::Instance()->GetMesh("tree"));
	tree2->setShader(ShaderManager::Instance()->GetShader("cubeShader"));
	tree2->setTexture(TextureManager::Instance()->GetTexture("trunk"));
	root->addNode(tree2);

	leafs2 = new SceneNode(true);
	leafs2->setMatrix(matFactory::Translate3(2, 0.5, 1) * matFactory::Scale3(0.04f, 0.04f, 0.04f));
	leafs2->setMesh(MeshManager::Instance()->GetMesh("leafs"));
	leafs2->setShader(ShaderManager::Instance()->GetShader("cubeShader"));
	leafs2->setTexture(TextureManager::Instance()->GetTexture("leafs"));
	root->addNode(leafs2);

	shrub = new SceneNode(true);
	shrub->setMatrix(matFactory::Translate3(1, 0.25, 2) * matFactory::Scale3(0.04f, 0.04f, 0.04f));
	shrub->setMesh(MeshManager::Instance()->GetMesh("shrub"));
	shrub->setShader(ShaderManager::Instance()->GetShader("cubeShader"));
	shrub->setTexture(TextureManager::Instance()->GetTexture("trunk"));
	root->addNode(shrub);

	shrubLeafs = new SceneNode(true);
	shrubLeafs->setMatrix(matFactory::Translate3(1, 0.25, 2) * matFactory::Scale3(0.04f, 0.04f, 0.04f));
	shrubLeafs->setMesh(MeshManager::Instance()->GetMesh("shrubLeafs"));
	shrubLeafs->setShader(ShaderManager::Instance()->GetShader("cubeShader"));
	shrubLeafs->setColor(vec3(1.0f, 0.0f, 0.0f));
	shrubLeafs->setTexture(TextureManager::Instance()->GetTexture("leafs"));
	root->addNode(shrubLeafs);

	water = new WaterSceneNode();
	water->setMatrix(matFactory::Scale3(10, 0.1, 20));
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
	glViewport(0, 0, WinX, WinY);
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
	glViewport(0, 0, WinX, WinY);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawScene();
	glutSwapBuffers();
}

void idle()
{
	
}

void reshape(int w, int h)
{
	WinX = w;
	WinY = h;
	glViewport(0, 0, WinX, WinY);
}
void update() {
	variableHeight = ((sin(param*PI / 180)) - 1) / 8;

	water->setMatrix(matFactory::Translate3(0, variableHeight, 0) *matFactory::Scale3(10, 0.01, 10));

	upCamera->setClippingPlane(vec4(0, -1, 0, variableHeight+0.2));
	downCamera->setClippingPlane(vec4(0, 1, 0, -variableHeight));

	upCamera->setViewMatrix(matFactory::Translate3(0, variableHeight, -CameraDistance) * rotation);
	downCamera->setViewMatrix(matFactory::Translate3(0, -variableHeight, -CameraDistance) * inverseRotation);

	param+=0.5;
	if (param > 360.0f) param -= 360.0f;

}
void timer(int value)
{
	std::ostringstream oss;
	oss << CAPTION << ": " << FrameCount * 60 << " FPS @ (" << WinX << "x" << WinY << ")";
	std::string s = oss.str();
	glutSetWindow(WindowHandle);
	glutSetWindowTitle(s.c_str());
	FrameCount = 0;
	update();
	glutPostRedisplay();
	glutTimerFunc(16.666, timer, 0);
}

void screenshotTGA(const std::string& filename)
{
	int x = glutGet(GLUT_WINDOW_WIDTH);
	int y = glutGet(GLUT_WINDOW_HEIGHT);
	// get the image data
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
void screenshotBMP(const std::string& filename) {
	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);
	SOIL_save_screenshot(filename.c_str(), SOIL_SAVE_TYPE_BMP,0,0, w, h);
}

void myKeydown(unsigned char key, int x, int y) {
	key = tolower(key);
	if (key == 'b') {
		unsigned long milliseconds_since_epoch =
			std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		screenshotBMP(std::string("../../printscreens/") + std::to_string(milliseconds_since_epoch) + std::string(".bmp"));
	}
	else if (key == 't') {
		unsigned long milliseconds_since_epoch =
			std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		screenshotTGA(std::string("../../printscreens/") + std::to_string(milliseconds_since_epoch) + std::string(".tga"));
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
		upCamera->setViewMatrix(matFactory::Translate3(0, variableHeight, -CameraDistance) * rotation);
		downCamera->setViewMatrix(matFactory::Translate3(0, variableHeight, -CameraDistance) * inverseRotation);
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
	createMaterials();
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
