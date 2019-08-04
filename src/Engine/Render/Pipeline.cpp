#include <BlackBox/Render/Pipeline.hpp>
#include <BlackBox/Render/OpenglDebug.hpp>
#include <BlackBox/Resources/MaterialManager.hpp>
#include <BlackBox/IGeometry.hpp>
#include <BlackBox/Object.hpp>

Pipeline *Pipeline::m_instance = nullptr;

Pipeline* Pipeline::instance()
{
  if (m_instance == nullptr)
  {
    m_instance = new Pipeline();
  }
  return m_instance;
}

void Pipeline::bindProgram(std::shared_ptr<CShaderProgram> program)
{
	shader = program;
}

void Pipeline::bindProgram(const char* name)
{
	auto mat = MaterialManager::instance()->getMaterial(name);
	if (mat != nullptr)
	{
		if (mat->program != nullptr)
			shader = mat->program;
		else
			shader = ShaderManager::instance()->getDefaultProgram();
	}
	shader->use();
}

void BoundingBox::draw()
{
	// Cube 1x1x1, centered on origin
	GLfloat vertices[] = {
		-0.5, -0.5, -0.5, 1.0,
		 0.5, -0.5, -0.5, 1.0,
		 0.5,  0.5, -0.5, 1.0,
		-0.5,  0.5, -0.5, 1.0,
		-0.5, -0.5,  0.5, 1.0,
		 0.5, -0.5,  0.5, 1.0,
		 0.5,  0.5,  0.5, 1.0,
		-0.5,  0.5,  0.5, 1.0,
	};
	GLuint vao_vertices, vbo_vertices;
	glCheck(glGenVertexArrays(1, &vao_vertices));
	glCheck(glBindVertexArray(vao_vertices));

	glCheck(glGenBuffers(1, &vbo_vertices));
	glCheck(glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices));
	glCheck(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));
	glCheck(glBindBuffer(GL_ARRAY_BUFFER, 0));

	GLushort elements[] = {
		0, 1, 2, 3,
		4, 5, 6, 7,
		0, 4, 1, 5, 2, 6, 3, 7
	};
	GLuint ibo_elements;
	glCheck(glGenBuffers(1, &ibo_elements));
	glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_elements));
	glCheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW));
	glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

	glm::vec3 size = glm::vec3(max.x - min.x, max.y - min.y, max.z - min.z);
	glm::vec3 center = glm::vec3((min.x + max.x) / 2, (min.y + max.y) / 2, (min.z + max.z) / 2);
	glm::mat4 transform = glm::translate(glm::mat4(1), center) * glm::scale(glm::mat4(1), size );

	/* Apply object's transformation matrix */
	//glm::mat4 m = mesh->object2world * transform;
	Pipeline::instance()->model = Pipeline::instance()->object->getTransform() * transform;
	Pipeline::instance()->shader->setup();

	glCheck(glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices));
	glCheck(glEnableVertexAttribArray(0));
	glCheck(glVertexAttribPointer(
		0,  // attribute
		4,                  // number of elements per vertex, here (x,y,z,w)
		GL_FLOAT,           // the type of each element
		GL_FALSE,           // take our values as-is
		0,                  // no extra data between each position
		0                   // offset of first element
	));

	glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_elements));
	glCheck(glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, 0));
	glCheck(glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, (GLvoid*)(4 * sizeof(GLushort))));
	glCheck(glDrawElements(GL_LINES, 8, GL_UNSIGNED_SHORT, (GLvoid*)(8 * sizeof(GLushort))));
	glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

	glCheck(glDisableVertexAttribArray(0));
	glCheck(glBindBuffer(GL_ARRAY_BUFFER, 0));

	glCheck(glDeleteBuffers(1, &vbo_vertices));
	glCheck(glDeleteBuffers(1, &ibo_elements));
	glCheck(glDeleteVertexArrays(1, &vao_vertices));
}
