#include "Program.hpp"

Program::Program()
{
	amount = new unsigned;
	*amount = 1;
}

Program::Program(const Shader& vertex, const Shader& fragment)
{
	programId = glCreateProgram();
	glAttachShader(programId, vertex.getId());
	glAttachShader(programId, fragment.getId());
	link();
	vertex.clear();
	fragment.clear();
	amount = new unsigned;
	*amount = 1;
}

Program::Program(const std::string& vertFileName, const std::string& fragFileName)
{
	programId = glCreateProgram();
	Shader vertex = Shader::createVertexShader(vertFileName);
	Shader fragment = Shader::createFragmentShader(fragFileName);
	glAttachShader(programId, vertex.getId());
	glAttachShader(programId, fragment.getId());
	link();
	vertex.clear();
	fragment.clear();
	amount = new unsigned;
	*amount = 1;
}

Program::Program(const Program& program)
{
	swap(program);
}

Program& Program::operator=(const Program& program)
{
	clear();
	swap(program);
	return *this;
}

void Program::swap(const Program& program)
{
	programId = program.programId;
	amount = program.amount;
	*amount = *amount + 1;
}

void Program::clear()
{
	*amount = *amount - 1;
	if (*amount == 0)
	{
		delete amount;
		if (programId != 0)
			glDeleteProgram(programId);
	}
}

Program::~Program()
{
	clear();
}

std::string Program::getLinkMessageErrorAndClear() const
{
	int length;
	glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &length);
	char* message = new char[length];

	glGetProgramInfoLog(programId, length, &length, message);
	glDeleteProgram(programId);

	std::string finalMess = message;
	delete[] message;
	return finalMess;
}

void Program::create()
{
	if (programId != 0) throw std::runtime_error("Cant create exists program");
	programId = glCreateProgram();
}

void Program::attachShader(const Shader& shader) const
{
	if (programId == 0) throw std::runtime_error("Cant attach shader to empty program");
	glAttachShader(programId, shader.getId());
	shader.clear();
}

void Program::link() const
{
	if (programId == 0) throw std::runtime_error("Cant link empty program");
	glLinkProgram(programId);
	int success;
	glGetProgramiv(programId, GL_LINK_STATUS, &success);
	if (!success)
		throw std::runtime_error(getLinkMessageErrorAndClear());
}

unsigned Program::getUniformId(const std::string& name) const
{
	return glGetUniformLocation(programId, name.c_str());
}

void Program::setInt(const std::string& name, int i) const
{
	glUniform1i(getUniformId(name), i);
}

void Program::setBool(const std::string& name, bool b) const {
	glUniform1i(getUniformId(name), b ? 1 : 0);
}

void Program::setFloat(const std::string& name, float f) const
{
	glUniform1f(getUniformId(name), f);
}

void Program::setVec2(const std::string& name, const glm::vec2& vec) const
{
	glUniform2fv(getUniformId(name), 1, glm::value_ptr(vec));
}

void Program::setVec3(const std::string& name, const glm::vec3& vec) const
{
	glUniform3fv(getUniformId(name), 1, glm::value_ptr(vec));
}

void Program::setVec4(const std::string& name, const glm::vec4& vec) const
{
	glUniform4fv(getUniformId(name), 1, glm::value_ptr(vec));
}

void Program::setMat2(const std::string& name, const glm::mat2& mat) const
{
	glUniformMatrix2fv(getUniformId(name), 1, GL_FALSE, glm::value_ptr(mat));
}

void Program::setMat3(const std::string& name, const glm::mat3& mat) const
{
	glUniformMatrix3fv(getUniformId(name), 1, GL_FALSE, glm::value_ptr(mat));
}

void Program::setMat4(const std::string& name, const glm::mat4& mat) const
{
	glUniformMatrix4fv(getUniformId(name), 1, GL_FALSE, glm::value_ptr(mat));
}
