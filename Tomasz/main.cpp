///////////////////////////////////////////////////////////////////////
//
// Using quaternions to rotate in 3D.
//
// Assignment: 1. Create a class for Quaternions.
//             2. Create a scene with a camera rotating around an 
//                object at the origin and pointing towards it. 
//                Do NOT use "gluLookAt", use rotation matrices!
//             3. Gimbal lock mode ON: use Translation + Rotation 
//                matrices (e.g. Euler angles, Rodrigues’ formula). 
//             4. Gimbal lock mode OFF: use Quaternions to produce a 
//                transformation matrix to avoid gimbal lock.
//             5. Switch between modes by pressing the 'g' key.
//
// (c) 2013-16 by Carlos Martinho
//
///////////////////////////////////////////////////////////////////////

#define _USE_MATH_DEFINES 
#include <cmath>

#include "stdafx.h"
#include "shader.h"
#include "error.h"
#include "vertex.h"
#include "matrixFactory.h"
#include "quaternion.h"
#include <algorithm> 

#define VERTICES 0
#define VIEWING_DISTANCE_MIN  3.0

using namespace error;
using namespace transformations;


int WinX = 640, WinY = 640;
int WindowHandle = 0;
unsigned int FrameCount = 0;
GLuint VaoId, VboId[2];

vector3d target = *new vector3d(0, 0, 0);

static BOOL firstMouse = TRUE;
static BOOL isProjection = TRUE;
static BOOL g_bButton1Down = FALSE;
static BOOL g_bButton2Down = FALSE;
static BOOL isQuaterion = FALSE;

static GLfloat g_fViewDistance = 10 * VIEWING_DISTANCE_MIN;
static int g_yClick = 0;
static int g_xClick = 0;
int oldX=0,oldY=0;
/////////////////////////////////////////////////////////////////////// VAOs & VBOs

const vector4d Parallelogram[] = 
{
	*new vector4d(0.0f, 0.0f, 0.1f, 1.0f),
	*new vector4d(0.3f, 0.0f, 0.1f, 1.0f),
	*new vector4d(0.6f, 0.3f, 0.1f, 1.0f),
	*new vector4d(0.3f, 0.3f, 0.1f, 1.0f),

	*new vector4d(0.0f, 0.0f, -0.1f, 1.0f),
	*new vector4d(0.3f, 0.0f, -0.1f, 1.0f),
	*new vector4d(0.6f, 0.3f, -0.1f, 1.0f),
	*new vector4d(0.3f, 0.3f, -0.1f, 1.0f)
};

const vector4d RightTriangle[] =
{
	*new vector4d(0.0f, 0.0f, 0.1f, 1.0f),
	*new vector4d(0.3f, 0.0f, 0.1f, 1.0f),
	*new vector4d(0.0f, 0.3f, 0.1f, 1.0f),
	*new vector4d(0.0f, 0.0f, -0.1f, 1.0f),
	*new vector4d(0.3f, 0.0f, -0.1f, 1.0f),
	*new vector4d(0.0f, 0.3f, -0.1f, 1.0f)
};

const vector4d Square[] =
{
	*new vector4d(0.0f, 0.0f, 0.1f, 1.0f), 
	*new vector4d(0.3f, 0.0f, 0.1f, 1.0f), 
	*new vector4d(0.3f, 0.3f, 0.1f, 1.0f), 
	*new vector4d(0.0f, 0.3f, 0.1f, 1.0f),
	*new vector4d(0.0f, 0.0f, -0.1f, 1.0f),
	*new vector4d(0.3f, 0.0f, -0.1f, 1.0f),
	*new vector4d(0.3f, 0.3f, -0.1f, 1.0f),
	*new vector4d(0.0f, 0.3f, -0.1f, 1.0f)
};

const GLfloat colorGreen[] = { 0.0f, 1.0f, 0.0f, 1.0f };
const GLfloat colorGreenWall[] = { 0.1f, 0.8f, 0.1f, 1.0f };
const GLfloat colorGreenWall2[] = { 0.05f, 0.9f, 0.05f, 1.0f };
const GLfloat colorGreenWall3[] = { 0.15f, 0.7f, 0.15f, 1.0f };
const GLfloat colorRed[] = { 1.0f, 0.0f, 0.0f, 1.0f };
const GLfloat colorRedWall[] = { 0.8f, 0.1f, 0.1f, 1.0f };
const GLfloat colorRedWall2[] = { 0.9f, 0.05f, 0.05f, 1.0f };
const GLfloat colorRedWall3[] = { 0.7f, 0.15f, 0.15f, 1.0f };
const GLfloat colorBlue[] = { 0.0f, 0.0f, 1.0f, 1.0f };
const GLfloat colorBlueWall[] = { 0.1f, 0.1f, 0.8f, 1.0f };
const GLfloat colorBlueWall2[] = { 0.05f, 0.05f, 0.9f, 1.0f };
const GLfloat colorBlueWall3[] = { 0.15f, 0.15f, 0.7f, 1.0f };
const GLfloat colorYellow[] = { 1.0f, 1.0f, 0.0f, 1.0f };
const GLfloat colorYellowWall[] = { 0.9f, 0.9f, 0.2f, 1.0f };
const GLfloat colorYellowWall2[] = { 0.95f, 0.95f, 0.1f, 1.0f };
const GLfloat colorYellowWall3[] = { 0.85f, 0.85f, 0.3f, 1.0f };
const GLfloat colorMagenta[] = { 1.0f, 0.0f, 1.0f, 1.0f };
const GLfloat colorMagentaWall[] = { 0.9f, 0.2f, 0.9f, 1.0f };
const GLfloat colorMagentaWall2[] = { 0.95f, 0.1f, 0.95f, 1.0f };
const GLfloat colorMagentaWall3[] = { 0.85f, 0.3f, 0.85f, 1.0f };
const GLfloat colorCyan[] = { 0.0f, 1.0f, 1.0f, 1.0f };
const GLfloat colorCyanWall[] = { 0.2f, 0.9f, 0.9f, 1.0f };
const GLfloat colorCyanWall2[] = { 0.1f, 0.95f, 0.95f, 1.0f };
const GLfloat colorCyanWall3[] = { 0.3f, 0.85f, 0.85f, 1.0f };
const GLfloat colorWhite[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat colorWhiteWall[] = { 0.9f, 0.9f, 0.9f, 1.0f };
const GLfloat colorWhiteWall2[] = { 0.95f, 0.95f, 0.95f, 1.0f };



const GLubyte IndicesTriangle[] =
{
	// top part
	0,1,2,
	// down part
	5,4,3,
	// front wall 
	0,3,1, 1,3,4,
	// back wall
	5,1,4, 5,2,1,
	// third wall
	0,2,3, 3,2,5

};

const GLubyte IndicesSquare[] =
{
	// top
	0,1,2, 2,3,0,
	// bottom
	4,6,5, 4,7,6,
	// front wall
	0,4,1, 5,1,4,
	// back wall
	7,3,2, 7,2,6,
	// left wall
	7,4,0, 7,0,3,
	// right wall
	6,2,1, 6,1,5
};

const GLubyte IndicesParallelogram[] =
{
	// top
	0,1,2, 2,3,0,
	// bottom
	4,6,5, 4,7,6,
	// front wall
	0,4,1, 5,1,4,
	// back wall
	7,3,2, 7,2,6,
	// left wall
	7,4,0, 7,0,3,
	// right wall
	6,2,1, 6,1,5
};

//matrix4d ViewMatrix = matrixFactory::createViewMatrix(*new vector3d(5, 5, 5 ), *new vector3d(0, 0, 0), *new vector3d(0, 1, 0));
matrix4d rotX = matrixFactory::createRotationMat4(0, 1, 0, 0);
matrix4d rotY = matrixFactory::createRotationMat4(0, 0, 1, 0);
matrix4d rot = rotX*rotY;
matrix4d trans = matrixFactory::createTranslationMat4(0, 0, -5.0f).T();

vector4d xaxis = *new vector4d(1, 0, 0, 1);
vector4d yaxis = *new vector4d(0, 1, 0, 1);
vector4d zaxis = *new vector4d(0, 0, 1, 1);

quaternion qu = qu.qFromAngleAxis(0,zaxis);

matrix4d ViewMatrix = rot*trans;
matrix4d ProjectionMatrix = matrixFactory::createProjectionMatrix(g_fViewDistance, 640 / 480, 0.1f, 100.0f);

void createBufferObjects()
{
	glGenVertexArrays(3, &VaoId);

	glBindVertexArray(VaoId);
	{
		glGenBuffers(3, VboId);

		glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(RightTriangle), RightTriangle, GL_STATIC_DRAW);
		glEnableVertexAttribArray(VERTICES);
		glVertexAttribPointer(VERTICES, 4, GL_FLOAT, GL_FALSE, sizeof(vector4d), 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VboId[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(IndicesTriangle), IndicesTriangle, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, VboId[2]);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(matrix4d) * 2, 0, GL_STREAM_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, shader::UBO_BP, VboId[2]);
	}
	glBindVertexArray(VaoId+1);
	{
		glGenBuffers(3, VboId);

		glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Square), Square, GL_STATIC_DRAW);
		glEnableVertexAttribArray(VERTICES);
		glVertexAttribPointer(VERTICES, 4, GL_FLOAT, GL_FALSE, sizeof(vector4d), 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VboId[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(IndicesSquare), IndicesSquare, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, VboId[2]);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(matrix4d) * 2, 0, GL_STREAM_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, shader::UBO_BP, VboId[2]);
	}
	glBindVertexArray(VaoId + 2);
	{
		glGenBuffers(3, VboId);

		glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Parallelogram), Parallelogram, GL_STATIC_DRAW);
		glEnableVertexAttribArray(VERTICES);
		glVertexAttribPointer(VERTICES, 4, GL_FLOAT, GL_FALSE, sizeof(vector4d), 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VboId[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(IndicesParallelogram), IndicesParallelogram, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, VboId[2]);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(matrix4d) * 2, 0, GL_STREAM_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, shader::UBO_BP, VboId[2]);
	}
	std::cout << VaoId <<std::endl;

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	checkOpenGLError("ERROR: Could not create VAOs and VBOs.");
}

void destroyBufferObjects()
{
	glBindVertexArray(VaoId);
	glDisableVertexAttribArray(VERTICES);
	glDeleteBuffers(3, VboId);
	glDeleteVertexArrays(1, &VaoId);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	checkOpenGLError("ERROR: Could not destroy VAOs and VBOs.");
}


void drawScene()
{
	glBindBuffer(GL_UNIFORM_BUFFER, VboId[2]);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(matrix4d), ViewMatrix.getM());
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(matrix4d), sizeof(matrix4d), ProjectionMatrix.getM());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	checkOpenGLError("ERROR: Could not draw scene.1");

	glBindVertexArray(VaoId);
	glUseProgram(shader::ProgramId);

	
	// small triangle nr 1 (left ear)
	glUniform4fv(shader::UniformColorId, 1, colorBlue);
	glUniformMatrix4fv(shader::UniformModelMatrixId, 1, GL_TRUE, (T(-0.49, 0.16, 0)*R(120, 0, 0, 1)).getM());
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (GLvoid*)0);
	glUniform4fv(shader::UniformColorId, 1, colorBlueWall);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (GLvoid*)6); 
	glUniform4fv(shader::UniformColorId, 1, colorBlueWall2);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (GLvoid*)12);
	glUniform4fv(shader::UniformColorId, 1, colorBlueWall3);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (GLvoid*)18);
	
	// small triangle nr 2 (right ear)
	glUniform4fv(shader::UniformColorId, 1, colorRed);
	glUniformMatrix4fv(shader::UniformModelMatrixId, 1, GL_TRUE, (T(-0.49 , 0.16, 0)*R(-60, 0, 0, 1)).getM());
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (GLvoid*)0);
	glUniform4fv(shader::UniformColorId, 1, colorRedWall);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (GLvoid*)6);
	glUniform4fv(shader::UniformColorId, 1, colorRedWall2);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (GLvoid*)12);
	glUniform4fv(shader::UniformColorId, 1, colorRedWall3);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (GLvoid*)18);

	// big triangle nr 1 (body, back leg)
	glUniform4fv(shader::UniformColorId, 1, colorGreen);
	glUniformMatrix4fv(shader::UniformModelMatrixId, 1, GL_TRUE, (T(0.60, -0.25, 0)*R(155, 0, 0, 1)*S(2, 2, 1.1)).getM());
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (GLvoid*)0);
	glUniform4fv(shader::UniformColorId, 1, colorGreenWall);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (GLvoid*)6);
	glUniform4fv(shader::UniformColorId, 1, colorGreenWall2);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (GLvoid*)12);
	glUniform4fv(shader::UniformColorId, 1, colorGreenWall3);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (GLvoid*)18);

	// big triangle nr 2 (body, front leg)
	glUniform4fv(shader::UniformColorId, 1, colorCyan);
	glUniformMatrix4fv(shader::UniformModelMatrixId, 1, GL_TRUE, (T(-0.5, -0.2, 0)*R(-70, 0, 0, 1)*S(2, 2, 1.2)).getM());
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (GLvoid*)0);
	glUniform4fv(shader::UniformColorId, 1, colorCyanWall);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (GLvoid*)6);
	glUniform4fv(shader::UniformColorId, 1, colorCyanWall2);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (GLvoid*)12);
	glUniform4fv(shader::UniformColorId, 1, colorCyanWall3);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (GLvoid*)18);

	// medium triangle (body)
	glUniform4fv(shader::UniformColorId, 1, colorMagenta);
	glUniformMatrix4fv(shader::UniformModelMatrixId, 1, GL_TRUE, (T(0.2,-0.4,0)*R(110,0,0,1)*S(sqrtf(2), sqrtf(2), 1.3)).getM());
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (GLvoid*)0);
	glUniform4fv(shader::UniformColorId, 1, colorMagentaWall);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (GLvoid*)6);
	glUniform4fv(shader::UniformColorId, 1, colorMagentaWall2);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (GLvoid*)12);
	glUniform4fv(shader::UniformColorId, 1, colorMagentaWall3);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (GLvoid*)18);
	// square (head)
	glBindVertexArray(VaoId+1);
	glUniform4fv(shader::UniformColorId, 1, colorYellow);
	glUniformMatrix4fv(shader::UniformModelMatrixId, 1, GL_TRUE, (T(-0.6, -0.25, 0)*R(30, 0, 0, 1)*S(1, 1, 1.4)).getM());
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_BYTE, (GLvoid*)0);
	glUniform4fv(shader::UniformColorId, 1, colorYellowWall);
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_BYTE, (GLvoid*)12);
	glUniform4fv(shader::UniformColorId, 1, colorYellowWall2);
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_BYTE, (GLvoid*)24);

	// parallelogram (tail)
	glBindVertexArray(VaoId + 2);
	glUniform4fv(shader::UniformColorId, 1, colorWhite);
	glUniformMatrix4fv(shader::UniformModelMatrixId, 1, GL_TRUE, (T(0.6, -0.25, 0)*R(60, 0, 0, 1)*S(1, 1, 1.5)).getM());
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_BYTE, (GLvoid*)0);
	glUniform4fv(shader::UniformColorId, 1, colorWhiteWall);
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_BYTE, (GLvoid*)12);
	glUniform4fv(shader::UniformColorId, 1, colorWhiteWall2);
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_BYTE, (GLvoid*)24);

	glUseProgram(0);
	glBindVertexArray(0);

	checkOpenGLError("ERROR: Could not draw scene.");
}
///// KEYBOARD AND MOUSE ////
void MouseButton(int button, int state, int x, int y)
{
//	 Respond to mouse button presses.
	// If button1 pressed, mark this state so we know in motion function.
	if (button == GLUT_LEFT_BUTTON && state==GLUT_DOWN)
	{
		oldX = g_xClick;
		oldY = g_yClick;
		g_bButton2Down = FALSE;
		g_bButton1Down = TRUE;
		g_yClick = y;
		g_xClick = x;
	}
	else if(button == GLUT_RIGHT_BUTTON&& state == GLUT_DOWN){
		g_bButton1Down = FALSE;
		g_bButton2Down = TRUE;
		g_yClick = y;
		g_xClick = x;
	}
	else {
		g_bButton1Down = FALSE;
		firstMouse = TRUE;
		g_bButton2Down = FALSE;
	}

}
void MouseWheel(int wheel, int direction, int x, int y) {
	trans = matrixFactory::createTranslationMat4(0, 0, -direction).T()*trans;
	ViewMatrix = rot*trans;
}

void MouseMotion(int x, int y)
{
	if (g_bButton1Down)
	{
		if (firstMouse)
		{
			oldX = x;
			oldY = y;
			firstMouse = false;
		}

		float xoffset = x-oldX;
		float yoffset = y-oldY;

		if (!isQuaterion) {
			rotX = rotX*matrixFactory::createRotationMat4(yoffset, 1, 0, 0);
			rotY = rotY*matrixFactory::createRotationMat4(xoffset, 0, 1, 0);
			rot = rotY*rotX;
		}
		else {

			qu = qu*qu.qFromAngleAxis(xoffset, yaxis)*qu.qFromAngleAxis(yoffset, xaxis);
			qu.qGLMatrix(qu, rot);
			ViewMatrix = trans*rot;

		}
		oldX = x;
		oldY = y;

		ViewMatrix = rot*trans;

	}
}
void changePerspectiveOrthographic()
{
	if (isProjection == FALSE) {
		ProjectionMatrix = matrixFactory::createProjectionMatrix(g_fViewDistance, 640 / 480, 0.1f, 100);
		isProjection = TRUE;
	}
	else {
		ProjectionMatrix = matrixFactory::createOrthographicMatrix(-2, 2, 2, -2, 0.1f, 100);
		isProjection = FALSE;
	}
}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:             // ESCAPE key
		exit(0);
		break;
	case 'p':
		changePerspectiveOrthographic();
		break;
	case 'z':
		ProjectionMatrix = matrixFactory::createProjectionMatrix(g_fViewDistance++, 640 / 480, 0.1f, 100);
		break;
	case 'x':
		ProjectionMatrix = matrixFactory::createProjectionMatrix(g_fViewDistance--, 640 / 480, 0.1f, 100);
		break;
	case 'g':
		isQuaterion = !isQuaterion;
		std::cout << "Gimbal lock mode: "; 
		isQuaterion ? std::cout << "OFF" : std::cout << "ON";
		std::cout << std::endl;
		break;
	}
}

/////////////////////////////////////////////////////////////////////// CALLBACKS

void cleanup()
{
	shader::destroyShaderProgram();
	destroyBufferObjects();
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

/////////////////////////////////////////////////////////////////////// SETUP

void setupCallbacks() 
{
	glutCloseFunc(cleanup);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutTimerFunc(0,timer,0);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(MouseButton);
	glutMouseWheelFunc(MouseWheel);
	glutMotionFunc(MouseMotion);	

}

void checkOpenGLInfo()
{
	const GLubyte *renderer = glGetString(GL_RENDERER);
	const GLubyte *vendor = glGetString(GL_VENDOR);
	const GLubyte *version = glGetString(GL_VERSION);
	const GLubyte *glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
	std::cerr << "OpenGL Renderer: " << renderer << " (" << vendor << ")" << std::endl;
	std::cerr << "OpenGL version " << version << std::endl;
	std::cerr << "GLSL version " << glslVersion << std::endl;
}

void setupOpenGL()
{
	checkOpenGLInfo();
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

void setupGLEW() 
{
	glewExperimental = GL_TRUE;
	GLenum result = glewInit() ; 
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

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	
	glutInitWindowSize(WinX, WinY);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	WindowHandle = glutCreateWindow(CAPTION);
	if(WindowHandle < 1) {
		std::cerr << "ERROR: Could not create a new rendering window." << std::endl;
		exit(EXIT_FAILURE);
	}
}

void init(int argc, char* argv[])
{
	setupGLUT(argc, argv);
	setupGLEW();
	setupOpenGL();
	shader::createShaderProgramFromFiles("Shaders/vertexShader.txt","Shaders/fragmentShader.txt");
	createBufferObjects();
	setupCallbacks();
}

int main(int argc, char* argv[])
{
	init(argc, argv);
	glutMainLoop();
	exit(EXIT_SUCCESS);
}
//
/////////////////////////////////////////////////////////////////////////