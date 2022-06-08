#include "Common.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Camera.hpp"
#include "Skybox.hpp"
#include "Light.hpp"
#include "Model.hpp"
#include "Shader.hpp"
#include "Program.hpp"
#include "PickingTechnique.hpp"
#include "PickingTexture.hpp"

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 1024
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

using std::cout;
using std::endl;

struct ModelTransform {
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
};

struct Color { float r, g, b, a; };

Color background{ 0.f, 0.f, 0.f, 0.f };

struct Material {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
};

Camera camera(glm::vec3(0.0f, 0.0f, -8.0f));

bool wireframeMode = false;

void updatePolygonMode() { wireframeMode ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); }

void onResize(GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); }

void onScroll(GLFWwindow* window, double x, double y) { camera.ChangeFOV(y); }

int m_clicked_object_id = -1;

ModelTransform modelsTransform[4] = {
	{
		glm::vec3(0.0f, 0.0f, 1.5f),	// position
		glm::vec3(0.0f, 0.0f, 0.0f),	// rotation
		glm::vec3(.8f, .8f, .8f)		// scale
	},
	{
		glm::vec3(1.5f, 0.0f, 1.5f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.05f, 0.05f, 0.05f)
	},
	{
		glm::vec3(-1.5f, 0.0f, 1.5f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.3f, 0.3f, 0.3f)
	},
	{
		glm::vec3(0.0f, -0.1f, 0.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, 1.0f)
	} };

bool isSolved = false;

void solve() { // Model models[4] = { backpack, helmet, skull, watermelon };
	if (modelsTransform[0].rotation.x == 5.0f && modelsTransform[0].rotation.y == -0.15f)
	{
		if (modelsTransform[1].rotation.x == 5.0f && modelsTransform[1].rotation.y == -0.1f)
		{
			if (modelsTransform[2].rotation.x == -3.0f && modelsTransform[2].rotation.y == 0.2f)
			{
				isSolved = true;
				std::cout << "Поздравляю, Вы открыли загадочный ящик! В качестве награды Вы получаете Арбуз от Алима!" << std::endl;
			}
		}
	}
}

bool postprocessing = true;
bool grayscale = false;
bool blur = false;
bool negative = false;
bool fisheye = false;
bool bloom = false;

void onKeyAction(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_SPACE: // Wireframe mode
			wireframeMode = !wireframeMode;
			updatePolygonMode();
			break;
		case GLFW_KEY_C: // Change cursor mode
			glfwSetInputMode(window, GLFW_CURSOR, glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
			glfwSetCursorPos(window, WINDOW_WIDTH/2, WINDOW_HEIGHT/2);
			break;
		case GLFW_KEY_Z: // Free camera movement on Y-axis
			camera.noClip = !camera.noClip;
			break;
		case GLFW_KEY_R: // Reset models rotation
			for (int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(modelsTransform) - 1; i++) modelsTransform[i].rotation = glm::vec3(0.0f, 0.0f, 0.0f);
			isSolved = false;
			break;
		case GLFW_KEY_O: // Print objectives
			std::cout << std::endl << "Задачи для открытия загадочного ящика (последовательность выполнения не важна): " << std::endl;
			std::cout << "\t1. Провращай рюкзак 3 раза влево, затем 5 раз вверх." << std::endl;
			std::cout << "\t2. Провращай череп 4 раза вправо, затем 3 раза вниз." << std::endl;
			std::cout << "\t3. Провращай шлем 5 раз вверх, затем 2 раза влево." << std::endl;
			std::cout << "Для вращения объектов сначало их надо выбрать, кликнув по ним в режиме выбора (кнопка C)." << std::endl;
			std::cout << "Выбранный объект будет подсвечен. Вращайте объекты влево / вправо и вверх / вниз с помощью стрелок." << std::endl;
			std::cout << "Если Вы ошибётесь в последовательности действий, Вы можете вернуться к начальному положению всех объектов, нажав на кнопку R." << std::endl;
			break;
		case GLFW_KEY_P:
			postprocessing = !postprocessing;
			break;

		// Postprocessing effects
		case GLFW_KEY_1:
			grayscale = !grayscale;
			break;
		case GLFW_KEY_2:
			blur = !blur;
			break;
		case GLFW_KEY_3:
			negative = !negative;
			break;
		case GLFW_KEY_4:
			fisheye = !fisheye;
			break;
		case GLFW_KEY_5:
			bloom = !bloom;
			break;
		}

		// Objects rotating
		if ((glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL) && (m_clicked_object_id != -1)) {
			switch (key) {
			case GLFW_KEY_LEFT:
				modelsTransform[m_clicked_object_id].rotation.y -= 0.05f;
				break;
			case GLFW_KEY_RIGHT:
				modelsTransform[m_clicked_object_id].rotation.y += 0.05f;
				break;
			case GLFW_KEY_UP:
				modelsTransform[m_clicked_object_id].rotation.x += 1.0f;
				break;
			case GLFW_KEY_DOWN:
				modelsTransform[m_clicked_object_id].rotation.x -= 1.0f;
				break;
			}
		}
	}
}

void inputHandle(GLFWwindow* window, double dt) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
	{
		cout << "CAMERA POSITION (x, y, z): " << camera.Position.x << " " << camera.Position.y << " " << camera.Position.z << endl;
		cout << "CAMERA YAW & PITCH: " << camera.Yaw << " " << camera.Pitch << endl;
	}
	uint32_t dir = 0;
	if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS) dir |= CAM_DOWN;
	if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS) dir |= CAM_UP;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) dir |= CAM_FORWARD;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) dir |= CAM_BACKWARD;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) dir |= CAM_LEFT;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) dir |= CAM_RIGHT;
	
	if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
		double newx = 0.f, newy = 0.f;
		glfwGetCursorPos(window, &newx, &newy); // Позиция курсора в оконных координатах
		static double x = newx, y = newy;
		double xoffset = newx - x;
		double yoffset = newy - y;
		x = newx;
		y = newy;

		camera.Move(dir, dt);
		camera.Rotate(xoffset, -yoffset);
	}
}

struct {
	bool isPressed = false;
	int x;
	int y;
} m_leftMouseButton;

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mode)
{
	if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL) {
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		if (button == GLFW_MOUSE_BUTTON_LEFT) {
			m_leftMouseButton.isPressed = (action == GLFW_PRESS);
			m_leftMouseButton.x = x;
			m_leftMouseButton.y = y;
			//cout << "Cursor pos: " << m_leftMouseButton.x << ", " << m_leftMouseButton.y << endl;
		}
	}
}

Light* flashLight, * redLamp, * blueLamp, * sunLight;

int main() {
	system("chcp 65001");

#pragma region WINDOW INITIALIZATION
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Window", NULL, NULL);
	if (window == NULL) {
		cout << "Couldnt create window" << endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
		throw std::runtime_error("Cant init GLEW");

	glfwSetFramebufferSizeCallback(window, onResize);
	glfwSetScrollCallback(window, onScroll);
	glfwSetKeyCallback(window, onKeyAction);
	glfwSetMouseButtonCallback(window, MouseButtonCallback);

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glEnable(GL_DEPTH_TEST);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	updatePolygonMode();
	glEnable(GL_CULL_FACE); // Включить срезание граней
	glFrontFace(GL_CCW); // Определение передних полигонов; cw - clockwise - по часовой; ccw (по умолчанию) - counter clockwise - против часовой
#pragma endregion

	int box_width, box_height, channels;
	unsigned char* data = stbi_load("images\\arbuzman.png", &box_width, &box_height, &channels, 0);

	const int verts = 36;

	float cube[] = {
	//position			normal					texture				color
	// LEFT SIDE
	-1.0f,-1.0f,-1.0f,	-1.0f,  0.0f,  0.0f,	0.0f, 1.0f,		0.0f, 1.0f, 0.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,	-1.0f,  0.0f,  0.0f,	1.0f, 1.0f,		0.0f, 1.0f, 0.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,	-1.0f,  0.0f,  0.0f,	1.0f, 0.0f,		0.0f, 1.0f, 0.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,	-1.0f,  0.0f,  0.0f,	0.0f, 1.0f,		0.0f, 1.0f, 0.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,	-1.0f,  0.0f,  0.0f,	1.0f, 0.0f,		0.0f, 1.0f, 0.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,		0.0f, 1.0f, 0.0f, 1.0f,
	// BACK SIDE																			
	1.0f, 1.0f,-1.0f,	0.0f,  0.0f, -1.0f, 	0.0f, 0.0f,		1.0f, 0.0f, 0.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,	0.0f,  0.0f, -1.0f, 	1.0f, 1.0f,		1.0f, 0.0f, 0.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,	0.0f,  0.0f, -1.0f, 	1.0f, 0.0f,		1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 1.0f,-1.0f,	0.0f,  0.0f, -1.0f,		0.0f, 0.0f,		1.0f, 0.0f, 0.0f, 1.0f,
	1.0f,-1.0f,-1.0f,	0.0f,  0.0f, -1.0f,		0.0f, 1.0f,		1.0f, 0.0f, 0.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,	0.0f,  0.0f, -1.0f,		1.0f, 1.0f,		1.0f, 0.0f, 0.0f, 1.0f,
	// DOWN SIDE																			
	1.0f,-1.0f, 1.0f,	0.0f, -1.0f,  0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,	0.0f, -1.0f,  0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 1.0f, 1.0f,
	1.0f,-1.0f,-1.0f,	0.0f, -1.0f,  0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,	0.0f, -1.0f,  0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,	0.0f, -1.0f,  0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,	0.0f, -1.0f,  0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 1.0f, 1.0f,
	// FRONT SIDE																			
	-1.0f, 1.0f, 1.0f,	0.0f,  0.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,	0.0f,  0.0f, 1.0f,		0.0f, 1.0f,		0.0f, 0.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,	0.0f,  0.0f, 1.0f,		1.0f, 1.0f,		0.0f, 0.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,	0.0f,  0.0f, 1.0f,		1.0f, 0.0f,		0.0f, 0.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,	0.0f,  0.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,	0.0f,  0.0f, 1.0f,		1.0f, 1.0f,		0.0f, 0.0f, 1.0f, 1.0f,
	// RIGHT SIDE																			
	1.0f, 1.0f, 1.0f,	1.0f,  0.0f,  0.0f,		0.0f, 0.0f,		1.0f, 0.0f, 0.0f, 1.0f,
	1.0f,-1.0f,-1.0f,	1.0f,  0.0f,  0.0f,		1.0f, 1.0f,		1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 1.0f,-1.0f,	1.0f,  0.0f,  0.0f,		1.0f, 0.0f,		1.0f, 0.0f, 0.0f, 1.0f,
	1.0f,-1.0f,-1.0f,	1.0f,  0.0f,  0.0f,		1.0f, 1.0f,		1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 1.0f,	1.0f,  0.0f,  0.0f,		0.0f, 0.0f,		1.0f, 0.0f, 0.0f, 1.0f,
	1.0f,-1.0f, 1.0f,	1.0f,  0.0f,  0.0f,		0.0f, 1.0f,		1.0f, 0.0f, 0.0f, 1.0f,
	// UP SIDE	(dont reverse texture y-coords)												
	1.0f, 1.0f, 1.0f,	0.0f,  1.0f,  0.0f,		1.0f, 0.0f,		0.0f, 1.0f, 0.0f, 1.0f,
	1.0f, 1.0f,-1.0f,	0.0f,  1.0f,  0.0f,		1.0f, 1.0f,		0.0f, 1.0f, 0.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,	0.0f,  1.0f,  0.0f,		0.0f, 1.0f,		0.0f, 1.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 1.0f,	0.0f,  1.0f,  0.0f,		1.0f, 0.0f,		0.0f, 1.0f, 0.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,	0.0f,  1.0f,  0.0f,		0.0f, 1.0f,		0.0f, 1.0f, 0.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,	0.0f,  1.0f,  0.0f,		0.0f, 0.0f,		0.0f, 1.0f, 0.0f, 1.0f
	};

	Material cubeMaterials[3] = {
		{
			glm::vec3(0.25, 0.20725, 0.20725),
			glm::vec3(1, 0.829, 0.829),
			glm::vec3(0.296648, 0.296648, 0.296648),
			12.f
		}, // Pearl
		{
			glm::vec3(0.25, 0.25, 0.25),
			glm::vec3(0.4, 0.4, 0.4),
			glm::vec3(0.774597, 0.774597, 0.774597),
			77.f
		}, // Chrome
		{
			glm::vec3(0.1745, 0.01175, 0.01175),
			glm::vec3(0.61424, 0.04136, 0.04136),
			glm::vec3(0.727811, 0.626959, 0.626959),
			77.f
		}, // Ruby
	};

	ModelTransform boxTransform = {
		glm::vec3(0.0f, 0.0f, 0.0f),	// position
		glm::vec3(0.0f, 0.0f, 0.0f),	// rotation
		glm::vec3(0.2f, 0.2f, 0.2f)		// scale
	};

#pragma region BUFFERS INITIALIZATION
	unsigned int box_texture;
	glGenTextures(1, &box_texture);

	glBindTexture(GL_TEXTURE_2D, box_texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	if (channels == 3) glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, box_width, box_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	else glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, box_width, box_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);

	// Vertex Buffer Object, Vertex Array Object
	unsigned int VBO_polygon, VAO_polygon;
	glGenBuffers(1, &VBO_polygon); // Генерируем буфер, в котором будут вершины
	glGenVertexArrays(1, &VAO_polygon); // Генерируем вертексный массив

	glBindVertexArray(VAO_polygon);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_polygon); // Привязываем буфер к таргету

	// GL_STREAM_DRAW - данные записываются один раз, редко используются ГП
	// GL_STATIC_DRAW - данные записываются один раз, используются часто
	// GL_DYNAMIC_DRAW - данные могут перезаписываться
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW); // Загружает в видеопамять данные из ОЗУ

	// Как читать данные из массива
	// 1 параметр - номер атрибута из шейдера (location = ?)
	// 2 - количество данных на 1 вершину (не менее 1 и не более 4)
	// 3 - тип данных
	// 4 - должны ли быть данные нормализованными 
	// 5 - шаг (через сколько байт начинается следующая область для второй вершины)
	// 6 - указатель для адреса смещения
	
	// position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	// color
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);

	Skybox skybox;

	float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
	// positions   // texCoords
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,
	 1.0f,  1.0f,  1.0f, 1.0f
	};

	// screen quad VAO
	unsigned int quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	// framebuffer
	unsigned int framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	// color attachment texture
	unsigned int textureColorbuffer;
	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
	// renderbuffer object
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WINDOW_WIDTH, WINDOW_HEIGHT); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

#pragma endregion

	Program screen_program;
	screen_program.create();
	screen_program.attachShader(Shader::createVertexShader("shaders/postprocess.vert"));
	screen_program.attachShader(Shader::createFragmentShader("shaders/postprocess.frag"));
	screen_program.link();

	Program model_program;
	model_program.create();
	model_program.attachShader(Shader::createVertexShader("shaders/model.vert"));
	model_program.attachShader(Shader::createTessellationControlShader("shaders/model_tc.glsl"));
	model_program.attachShader(Shader::createTessellationEvaluationShader("shaders/model_te.glsl"));
	model_program.attachShader(Shader::createFragmentShader("shaders/model.frag"));
	model_program.link();

	Program light_program;
	light_program.create();
	light_program.attachShader(Shader::createVertexShader("shaders/light.vert"));
	light_program.attachShader(Shader::createFragmentShader("shaders/light.frag"));
	light_program.link();

	Program polygon_program;
	polygon_program.create();
	polygon_program.attachShader(Shader::createVertexShader("shaders/basic.vert"));
	polygon_program.attachShader(Shader::createFragmentShader("shaders/basic.frag"));
	polygon_program.link();

	skybox.getTexture().load({ "textures/room/posx.png", "textures/room/negx.png", "textures/room/posy.png", "textures/room/negy.png", "textures/room/posz.png", "textures/room/negz.png" });

	Program skybox_program;
	skybox_program.create();
	skybox_program.attachShader(Shader::createVertexShader("shaders/room.vert"));
	skybox_program.attachShader(Shader::createFragmentShader("shaders/room.frag"));
	skybox_program.link();

	// PICKING
	PickingTechnique pickingTechnique;
	pickingTechnique.create();
	pickingTechnique.attachShader(Shader::createVertexShader("shaders/picking.vert"));
	pickingTechnique.attachShader(Shader::createFragmentShader("shaders/picking.frag"));
	pickingTechnique.link();
	pickingTechnique.Init();

	PickingTexture pickingTexture;
	pickingTexture.Init(WINDOW_WIDTH, WINDOW_HEIGHT);

	Model helmet("models/helmet/helmet.obj", true);
	Model backpack("models/backpack/backpack.obj", false);
	Model skull("models/skull/forest_skull.obj", true);
	Model watermelon("models/watermelon/watermelon.obj", true);
	Model models[4] = { backpack, helmet, skull, watermelon };
	
	double oldTime = glfwGetTime(), newTime, deltaTime;

#pragma region LIGHT INITIALIZATION

	ModelTransform lightTransform = {
	glm::vec3(0.f, 0.f, 0.f),
	glm::vec3(0.f, 0.f, 0.f),
	glm::vec3(0.01, 0.01f, 0.01f) };

	std::vector<Light*> lights;
	int total_lights = 3;
	int active_lights = 0;

	redLamp = new Light("LampRed", true);
	redLamp->initLikePointLight(
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.1f, 0.1f, 0.1f),
		glm::vec3(1.0f, 0.2f, 0.2f),
		glm::vec3(1.0f, 0.2f, 0.2f),
		1.0f, 0.1f, 0.09f);
	lights.push_back(redLamp);

	blueLamp = new Light("LampBlue", true);
	blueLamp->initLikePointLight(
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.1f, 0.1f, 0.1f),
		glm::vec3(0.2f, 0.2f, 1.0f),
		glm::vec3(1.0f, 0.2f, 1.0f),
		1.0f, 0.1f, 0.09f);
	lights.push_back(blueLamp);

	sunLight = new Light("Sun", true);
	sunLight->initLikeDirectionalLight(
		glm::vec3(-1.0f, -1.0f, -1.0f),
		glm::vec3(0.1f, 0.1f, 0.1f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(0.0f, 0.0f, 0.0f));
	lights.push_back(sunLight);

	//flashLight = new Light("FlashLight", true);
	//flashLight->initLikeSpotLight(
	//	glm::vec3(0.0f, 0.0f, 0.0f),
	//	glm::vec3(0.0f, 0.0f, 0.0f),
	//	glm::radians(10.f),
	//	glm::vec3(0.0f, 0.0f, 0.0f),
	//	glm::vec3(0.7f, 0.7f, 0.6f),
	//	glm::vec3(0.8f, 0.8f, 0.6f),
	//	1.0f, 0.1f, 0.09f);
	//lights.push_back(flashLight);

#pragma endregion

	std::cout << "Эй! Хочешь узнать, что находится в том загадочном ящике? Для этого тебе нужно будет проделать шаги, указанные в записке." << std::endl;
	std::cout << "Для того, чтобы посмотреть записку, нажми английскую букву O." << std::endl;

	while (!glfwWindowShouldClose(window)) {
		newTime = glfwGetTime();
		deltaTime = newTime - oldTime;
		oldTime = newTime;
		inputHandle(window, deltaTime);

		if (!isSolved) solve();

		glm::mat4 p = camera.GetProjectionMatrix();
		glm::mat4 v = camera.GetViewMatrix();
		glm::mat4 pv = p * v;
		glm::mat4 model;

		//flashLight->position = camera.Position; // - camera.Up * 0.3f;
		//flashLight->direction = camera.Front;

		redLamp->position.x = -0.5f;
		redLamp->position.z = 0.1f * cos(newTime * 2);
		redLamp->position.y = 0.1f * sin(newTime * 2);

		blueLamp->position.x = 0.5f;
		blueLamp->position.z = 0.1f * cos(newTime * 2 + glm::pi<float>());
		blueLamp->position.y = 0.1f * sin(newTime * 2 + glm::pi<float>());

		glClearColor(background.r, background.g, background.b, background.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Picking Phase
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = camera.GetProjectionMatrix();
		if (m_leftMouseButton.isPressed) {
			pickingTexture.EnableWriting();

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			pickingTechnique.use();

			for (unsigned i = 0; i < (int)ARRAY_SIZE_IN_ELEMENTS(modelsTransform) - 1; i++) {
				model = glm::mat4(1.0f);
				model = glm::translate(model, modelsTransform[i].position);
				model = glm::scale(model, modelsTransform[i].scale);
				// Background is zero, the real objects start at 1
				pickingTechnique.SetObjectIndex(i + 1);
				glm::mat4 WVP = projection * view * model;
				pickingTechnique.SetWVP(WVP);
				models[i].Draw(pickingTechnique, true);
			}

			pickingTexture.DisableWriting();
		}

		// Render Phase
		if (postprocessing) glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		else glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glEnable(GL_DEPTH_TEST);
		glClearColor(background.r, background.g, background.b, background.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (m_leftMouseButton.isPressed) {
			PickingTexture::PixelInfo Pixel = pickingTexture.ReadPixel(m_leftMouseButton.x, WINDOW_HEIGHT - m_leftMouseButton.y - 1);
			if (Pixel.ObjectID != 0) {
				// Compensate for the SetObjectindex call in the picking phase
				m_clicked_object_id = Pixel.ObjectID - 1;
				assert(m_clicked_object_id < ARRAY_SIZE_IN_ELEMENTS(modelsTransform));
			}
			else m_clicked_object_id = -1;
		}

		modelsTransform[3].rotation.z = sin(glfwGetTime() * 2);
		modelsTransform[3].rotation.x = cos(glfwGetTime() * 8);

		for (unsigned i = 0; i < ARRAY_SIZE_IN_ELEMENTS(modelsTransform); i++) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, modelsTransform[i].position);
			model = glm::rotate(model, glm::radians(modelsTransform[i].rotation.x), glm::vec3(1.f, 0.f, 0.f));
			model = glm::rotate(model, (modelsTransform[i].rotation.y), glm::vec3(0.f, 1.f, 0.f));
			model = glm::rotate(model, glm::radians(modelsTransform[i].rotation.z), glm::vec3(0.f, 0.f, 1.f));
			model = glm::scale(model, modelsTransform[i].scale);

			model_program.use();
			model_program.setMat4("pv", pv);
			model_program.setMat4("model", model);
			model_program.setFloat("shininess", 64.0f);
			model_program.setVec3("WorldPosition", camera.Position);

			if (i == m_clicked_object_id) model_program.setVec4("COLOR", glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
			else model_program.setVec4("COLOR", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

			active_lights = 0;
			for (int i = 0; i < lights.size(); i++)
			{
				active_lights += lights[i]->putInShader(model_program, active_lights); // +1 либо +0
			}
			model_program.setInt("lights_count", active_lights);

			models[i].Draw(model_program, false);
		}

		// Drawing lamps
		light_program.use();
		light_program.setMat4("pv", pv);
		glBindVertexArray(VAO_polygon);

		// Red Lamp
		lightTransform.position = redLamp->position;
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightTransform.position);
		model = glm::scale(model, lightTransform.scale);
		light_program.setMat4("model", model);
		light_program.setVec3("lightColor", glm::vec3(1.0f, 0.2f, 0.2f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Blue Lamp
		lightTransform.position = blueLamp->position;
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightTransform.position);
		model = glm::scale(model, lightTransform.scale);
		light_program.setMat4("model", model);
		light_program.setVec3("lightColor", glm::vec3(0.2f, 0.2f, 1.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Mystery Box
		if (!isSolved) {
			polygon_program.use();
			polygon_program.setMat4("pv", pv);
			polygon_program.setBool("wireframeMode", wireframeMode);
			polygon_program.setVec3("viewPos", camera.Position);

			int active_lights = 0;
			for (int i = 0; i < total_lights; i++) {
				active_lights += lights[i]->putInShader(polygon_program, active_lights);
			}

			polygon_program.setInt("lightsCount", active_lights);

			model = glm::mat4(1.0f);
			model = glm::translate(model, boxTransform.position);
			model = glm::rotate(model, glm::radians(boxTransform.rotation.x), glm::vec3(1.f, 0.f, 0.f));
			model = glm::rotate(model, glm::radians(boxTransform.rotation.y), glm::vec3(0.f, 1.f, 0.f));
			model = glm::rotate(model, glm::radians(boxTransform.rotation.z), glm::vec3(0.f, 0.f, 1.f));
			model = glm::scale(model, boxTransform.scale);

			polygon_program.setMat4("model", model);

			polygon_program.setVec3("material.ambient", cubeMaterials[1].ambient);
			polygon_program.setVec3("material.diffuse", cubeMaterials[1].diffuse);
			polygon_program.setVec3("material.specular", cubeMaterials[1].specular);
			polygon_program.setFloat("material.shininess", cubeMaterials[1].shininess);

			glBindTexture(GL_TEXTURE_2D, box_texture);
			glBindVertexArray(VAO_polygon);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		skybox_program.use();
		glm::mat4 vp = camera.GetProjectionMatrix() * glm::mat4(glm::mat3(camera.GetViewMatrix()));
		skybox_program.setMat4("viewProject", vp);
		skybox_program.setBool("wireframeMode", wireframeMode);
		skybox.render(skybox_program);

		// now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
		if (postprocessing) {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
			glClearColor(background.r, background.g, background.b, background.a);
			glClear(GL_COLOR_BUFFER_BIT);
			glBindVertexArray(quadVAO);
			glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
			screen_program.use();
			screen_program.setInt("screenTexture", 0);
			screen_program.setBool("grayscale", grayscale);
			screen_program.setBool("blur", blur);
			screen_program.setBool("fisheye", fisheye);
			screen_program.setBool("negative", negative);
			screen_program.setBool("bloom", bloom);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		glfwSwapBuffers(window); // Смена буферов
		glfwPollEvents(); // Обработка сообщений от ОС (нажатие кнопок, изменить окно)
	}

	glfwTerminate();
	return 0;
}
