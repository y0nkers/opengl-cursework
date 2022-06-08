#ifndef PICKING_TECHNIQUE_HPP
#define PICKING_TECHNIQUE_HPP

#include "Program.hpp"

class PickingTechnique : public Program
{
public:

	PickingTechnique() {};

	void Init() {
		m_WVPLocation = getUniformId("gWVP");
		m_objectIndexLocation = getUniformId("gObjectIndex");
		m_drawIndexLocation = getUniformId("gDrawIndex");
	}

	void SetWVP(const glm::mat4& WVP) { glUniformMatrix4fv(m_WVPLocation, 1, GL_FALSE, glm::value_ptr(WVP)); }
	void DrawStartCB(unsigned int DrawIndex) { glUniform1ui(m_drawIndexLocation, DrawIndex); }
	void SetObjectIndex(unsigned int ObjectIndex) { glUniform1ui(m_objectIndexLocation, ObjectIndex); }

private:
	GLuint m_WVPLocation = 0;
	GLuint m_drawIndexLocation = 0;
	GLuint m_objectIndexLocation = 0;
};

#endif