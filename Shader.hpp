#ifndef SHADER_HPP
#define SHADER_HPP 1

#include "Common.hpp"

class Shader
{
	unsigned shaderId;
	GLenum type;

	std::string getSource(const std::string& fileName) const;
	std::string getCompileMessageErrorAndClear() const;
	Shader(const std::string& fileName, GLenum t);
public:
	~Shader() { glDeleteProgram(shaderId); };
	void use() { glUseProgram(shaderId); };

	static Shader createVertexShader(const std::string& fileName) { return Shader(fileName, GL_VERTEX_SHADER); }
	static Shader createFragmentShader(const std::string& fileName) { return Shader(fileName, GL_FRAGMENT_SHADER); }
	static Shader createTessellationControlShader(const std::string& fileName) { return Shader(fileName, GL_TESS_CONTROL_SHADER); }
	static Shader createTessellationEvaluationShader(const std::string& fileName) { return Shader(fileName, GL_TESS_EVALUATION_SHADER); }

	void clear() const { glDeleteShader(shaderId); }

	unsigned getId() const { return shaderId; }
	GLenum getType() const { return type; }
};
#endif
