#include "MyShader.h"

std::string MyShader::read(const std::string& filename) {
	std::ifstream ifile(filename);
	std::string shader_string;
	while (ifile.good()) {
		std::string line;
		std::getline(ifile, line);
		shader_string += line + "\n";
	}
	return shader_string;
}

void MyShader::checkLinkage(const GLuint program_id) {
	GLint linked;
	glGetProgramiv(program_id, GL_LINK_STATUS, &linked);
	if (linked == GL_FALSE) {
		GLint length;
		glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &length);
		GLchar* const log = new char[length];
		glGetProgramInfoLog(program_id, length, &length, log);
		std::cerr << "[LINK] " << std::endl << log << std::endl;
		delete log;
	}
}

const GLuint MyShader::addShader(const GLuint program_id, const GLenum shader_type, const std::string& filename)
{
	const GLuint shader_id = glCreateShader(shader_type);
	const std::string scode = read(filename);
	const GLchar* code = scode.c_str();
	glShaderSource(shader_id, 1, &code, 0);
	glCompileShader(shader_id);
	checkCompilation(shader_id, filename);
	glAttachShader(program_id, shader_id);
	return shader_id;
}

void MyShader::createShaderProgram(std::string& vs_file, std::string& fs_file)
{
	ProgramId = glCreateProgram();

	VertexShaderId = addShader(ProgramId, GL_VERTEX_SHADER, vs_file);
	FragmentShaderId = addShader(ProgramId, GL_FRAGMENT_SHADER, fs_file);

	glBindAttribLocation(ProgramId, VERTICES, "inPosition");
	if (TexcoordsLoaded)
		glBindAttribLocation(ProgramId, TEXCOORDS, "inTexcoord");
	if (NormalsLoaded)
		glBindAttribLocation(ProgramId, NORMALS, "inNormal");

	glLinkProgram(ProgramId);
	checkLinkage(ProgramId);

	ModelMatrix_UId = glGetUniformLocation(ProgramId, "ModelMatrix");
	ViewMatrix_UId = glGetUniformLocation(ProgramId, "ViewMatrix");
	ProjectionMatrix_UId = glGetUniformLocation(ProgramId, "ProjectionMatrix");

	checkOpenGLError("ERROR: Could not create shaders.");
}

void MyShader::destroyShaderProgram()
{
	glUseProgram(0);
	glDetachShader(ProgramId, VertexShaderId);
	glDetachShader(ProgramId, FragmentShaderId);

	glDeleteShader(FragmentShaderId);
	glDeleteShader(VertexShaderId);
	glDeleteProgram(ProgramId);

	checkOpenGLError("ERROR: Could not destroy shaders.");
}