#include "Units.h"

//Each team is a object of class Team
//Batch everything into 1 vertice vector
//unit move by changing the vertices
//vertices of two team and board are handled independently, then get merged together
//managed through UnitPos(basically a miniature of vertices): x,y, Numth in the corresponding vertices, UnitStride
//

constexpr auto TOP_SIDE = -1;
constexpr auto BOTTOM_SIDE = 1;

std::vector<float> EndSrc_vertices;
std::string result("");
bool Ending = false;

//FPS counter
double prevTime = 0.0f;
double crntTime = 0.0f;
double timeDiff;
unsigned int counter = 0;

void FPScounter(GLFWwindow* window)
{
	crntTime = glfwGetTime();
	timeDiff = crntTime - prevTime;
	counter++;
	if (timeDiff >= 1.0 / 30.0)
	{
		std::string FPS = std::to_string((1.0 / timeDiff) * counter);
		std::string ms = std::to_string((timeDiff / counter) * 1000);
		std::string newTitle = "Things - " + FPS + "FPS / " + ms + "ms";
		glfwSetWindowTitle(window, newTitle.c_str());
		prevTime = crntTime;
		counter = 0;
	}
}

std::vector<GLuint> units_Indices =
{
		0, 1, 2, 0, 2, 3,		// board

		4, 5, 6, 4, 6, 7,		// 1,1
		8, 9, 10, 8, 10, 11,	// 3,1
		12, 13, 14, 12, 14, 15, // 5,1
		16, 17, 18, 16, 18, 19, // 7,1

		20, 21, 22, 20, 22, 23, // 2,2
		24, 25, 26, 24, 26, 27, // 4,2
		28, 29, 30, 28, 30, 31, // 6,2
		32, 33, 34, 32, 34, 35,	// 8,2

		36, 37, 38, 36, 38, 39,	// 1,3
		40, 41, 42, 40, 42, 43,	// 3,3
		44, 45, 46, 44, 46, 47,	// 5,3
		48, 49, 50, 48, 50, 51,	// 7,3
		//////////////////////
		52, 53, 54, 52, 54, 55,	// 2,8
		56, 57, 58, 56, 58, 59,	// 4,8
		60, 61, 62, 60, 62, 63,	// 6,8
		64, 65, 66, 64, 66, 67,	// 8,8

		68, 69, 70, 68, 70, 71,	// 1,7
		72, 73, 74, 72, 74, 75,	// 3,7
		76, 77, 78, 76, 78, 79,	// 5,7
		80, 81, 82, 80, 82, 83,	// 7,7

		84, 85, 86, 84, 86, 87,	// 2,6
		88, 89, 90, 88, 90, 91,	// 4,6
		92, 93, 94, 92, 94, 95,	// 6,6
		96, 97, 98, 96, 98, 99,	// 8,6

		//100, 101, 102, 100, 102, 103
};
std::vector<GLuint> EndSrc_indices =
{
	100, 101, 102, 100, 102, 103
};
int ClickCount = 0;

bool firstClick = true;
int main()
{
	glfwInit();

	// version of OpenGL
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	//CORE profile
	//only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Checkers", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	gladLoadGL();

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize OpenGL context" << std::endl;
		return -1;
	}
	glfwSwapInterval(1);
	glViewport(0, 0, 900, 900);
	glClearColor(1.0f, 0.93f, 0.80f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glm::mat4 layout_projection = glm::ortho(-50.0f, 850.0f, -50.0f, 850.0f, -1.0f, 1.0f);	

		///////////

	Shader unit_shaderProgram;
	unit_shaderProgram.CreateShader("Shaders/default_unit.vert", "Shaders/default_unit.frag");

	/*glUseProgram(unit_shaderProgram.ID);
	auto location = glGetUniformLocation(unit_shaderProgram.ID, "u_texture");
	int samplers[3] = { 0, 1, 2 };
	glUniform1iv(location, 3, samplers);*/

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	unsigned int unit_vbo;
	unsigned int unit_ebo;
	unsigned int unit_vao;
	unsigned int unit_texture0;	

	Team Yellow_Team;
	Yellow_Team.name = "YELLOW TEAM";
	Yellow_Team.side = BOTTOM_SIDE;
	Yellow_Team.UnitsPos =
	{
		1,1,0,1,		3,1,1,1,		5,1,2,1,		7,1,3,1,
		2,2,4,1,		4,2,5,1,		6,2,6,1,		8,2,7,1,
		1,3,8,1,		3,3,9,1,		5,3,10,1,		7,3,11,1
	};
	Yellow_Team.units_vertices = 
	{
	0.0f,	0.0f,	0.0f,		1.0f, 0.0f, 0.0f,	0.0f, 0.5f,
	100.0f, 0.0f,	0.0f,		1.0f, 0.0f, 0.0f,	0.5f, 0.5f,
	100.0f, 100.0f, 0.0f,	    1.0f, 0.0f, 0.0f,	0.5f, 1.0f,		 // 1,1
	0.0f,	100.0f, 0.0f,	    1.0f, 0.0f, 0.0f,	0.0f, 1.0f,

	200.0f,	0.0f,	0.0f,		1.0f, 0.0f, 0.0f,	0.0f, 0.5f,
	300.0f, 0.0f,	0.0f,		1.0f, 0.0f, 0.0f,	0.5f, 0.5f,
	300.0f, 100.0f, 0.0f,	    1.0f, 0.0f, 0.0f,	0.5f, 1.0f,		 // 3, 1
	200.0f,	100.0f, 0.0f,	    1.0f, 0.0f, 0.0f,	0.0f, 1.0f,

	400.0f,	0.0f,	0.0f,		1.0f, 0.0f, 0.0f,	0.0f, 0.5f,
	500.0f, 0.0f,	0.0f,		1.0f, 0.0f, 0.0f,	0.5f, 0.5f,
	500.0f, 100.0f, 0.0f,	    1.0f, 0.0f, 0.0f,	0.5f, 1.0f,		 // 5, 1
	400.0f,	100.0f, 0.0f,	    1.0f, 0.0f, 0.0f,	0.0f, 1.0f,

	600.0f,	0.0f,	0.0f,		1.0f, 0.0f, 0.0f,	0.0f, 0.5f,
	700.0f, 0.0f,	0.0f,		1.0f, 0.0f, 0.0f,	0.5f, 0.5f,
	700.0f, 100.0f, 0.0f,	    1.0f, 0.0f, 0.0f,	0.5f, 1.0f,		 // 7, 1
	600.0f,	100.0f, 0.0f,	    1.0f, 0.0f, 0.0f,	0.0f, 1.0f,

	100.0f,	100.0f,	0.0f,		1.0f, 0.0f, 0.0f,	0.0f, 0.5f,
	200.0f, 100.0f,	0.0f,		1.0f, 0.0f, 0.0f,	0.5f, 0.5f,
	200.0f, 200.0f, 0.0f,	    1.0f, 0.0f, 0.0f,	0.5f, 1.0f,		 // 2,2
	100.0f,	200.0f, 0.0f,	    1.0f, 0.0f, 0.0f,	0.0f, 1.0f,

	300.0f,	100.0f,	0.0f,		1.0f, 0.0f, 0.0f,	0.0f, 0.5f,
	400.0f, 100.0f,	0.0f,		1.0f, 0.0f, 0.0f,	0.5f, 0.5f,
	400.0f, 200.0f, 0.0f,	    1.0f, 0.0f, 0.0f,	0.5f, 1.0f,		 // 4,2
	300.0f,	200.0f, 0.0f,	    1.0f, 0.0f, 0.0f,	0.0f, 1.0f,

	500.0f,	100.0f,	0.0f,		1.0f, 0.0f, 0.0f,	0.0f, 0.5f,
	600.0f, 100.0f,	0.0f,		1.0f, 0.0f, 0.0f,	0.5f, 0.5f,
	600.0f, 200.0f, 0.0f,	    1.0f, 0.0f, 0.0f,	0.5f, 1.0f,		 // 6,2
	500.0f,	200.0f, 0.0f,	    1.0f, 0.0f, 0.0f,	0.0f, 1.0f,

	700.0f,	100.0f,	0.0f,		1.0f, 0.0f, 0.0f,	0.0f, 0.5f,
	800.0f, 100.0f,	0.0f,		1.0f, 0.0f, 0.0f,	0.5f, 0.5f,
	800.0f, 200.0f, 0.0f,	    1.0f, 0.0f, 0.0f,	0.5f, 1.0f,		 // 8,2
	700.0f,	200.0f, 0.0f,	    1.0f, 0.0f, 0.0f,	0.0f, 1.0f,

	0.0f,	200.0f,	0.0f,		1.0f, 0.0f, 0.0f,	0.0f, 0.5f,
	100.0f, 200.0f,	0.0f,		1.0f, 0.0f, 0.0f,	0.5f, 0.5f,
	100.0f, 300.0f, 0.0f,	    1.0f, 0.0f, 0.0f,	0.5f, 1.0f,		 // 1,3
	0.0f,	300.0f, 0.0f,	    1.0f, 0.0f, 0.0f,	0.0f, 1.0f,

	200.0f,	200.0f,	0.0f,		1.0f, 0.0f, 0.0f,	0.0f, 0.5f,
	300.0f, 200.0f,	0.0f,		1.0f, 0.0f, 0.0f,	0.5f, 0.5f,
	300.0f, 300.0f, 0.0f,	    1.0f, 0.0f, 0.0f,	0.5f, 1.0f,		 // 3,3
	200.0f,	300.0f, 0.0f,	    1.0f, 0.0f, 0.0f,	0.0f, 1.0f,

	400.0f,	200.0f,	0.0f,		1.0f, 0.0f, 0.0f,	0.0f, 0.5f,
	500.0f, 200.0f,	0.0f,		1.0f, 0.0f, 0.0f,	0.5f, 0.5f,
	500.0f, 300.0f, 0.0f,	    1.0f, 0.0f, 0.0f,	0.5f, 1.0f,		 // 5,3
	400.0f,	300.0f, 0.0f,	    1.0f, 0.0f, 0.0f,	0.0f, 1.0f,

	600.0f,	200.0f,	0.0f,		1.0f, 0.0f, 0.0f,	0.0f, 0.5f,
	700.0f, 200.0f,	0.0f,		1.0f, 0.0f, 0.0f,	0.5f, 0.5f,
	700.0f, 300.0f, 0.0f,	    1.0f, 0.0f, 0.0f,	0.5f, 1.0f,		 // 7,3
	600.0f,	300.0f, 0.0f,	    1.0f, 0.0f, 0.0f,	0.0f, 1.0f,
	};
	//
	Team Grey_Team;
	Grey_Team.name = "GREY TEAM";
	Grey_Team.side = TOP_SIDE;
	Grey_Team.UnitsPos =
	{
		2,8,0,1,		4,8,1,1,		6,8,2,1,		8,8,3,1,
		1,7,4,1,		3,7,5,1,		5,7,6,1,		7,7,7,1,
		2,6,8,1,		4,6,9,1,		6,6,10,1,		8,6,11,1
	};
	Grey_Team.units_vertices =
	{
		100.0f,	700.0f,	0.0f,		1.0f, 0.0f, 0.0f,	0.5f, 0.0f,
		200.0f, 700.0f,	0.0f,		1.0f, 0.0f, 0.0f,	1.0f, 0.0f,
		200.0f, 800.0f, 0.0f,	    1.0f, 0.0f, 0.0f,	1.0f, 0.5f,		 // 2,8
		100.0f,	800.0f, 0.0f,	    1.0f, 0.0f, 0.0f,	0.5f, 0.5f,

		300.0f,	700.0f,	0.0f,		1.0f, 0.0f, 0.0f,	0.5f, 0.0f,
		400.0f, 700.0f,	0.0f,		1.0f, 0.0f, 0.0f,	1.0f, 0.0f,
		400.0f, 800.0f, 0.0f,	    1.0f, 0.0f, 0.0f,	1.0f, 0.5f,		 // 4,8
		300.0f,	800.0f, 0.0f,	    1.0f, 0.0f, 0.0f,	0.5f, 0.5f,

		500.0f,	700.0f,	0.0f,		1.0f, 0.0f, 0.0f,	0.5f, 0.0f,
		600.0f, 700.0f,	0.0f,		1.0f, 0.0f, 0.0f,	1.0f, 0.0f,
		600.0f, 800.0f, 0.0f,	    1.0f, 0.0f, 0.0f,	1.0f, 0.5f,		 // 6,8
		500.0f,	800.0f, 0.0f,	    1.0f, 0.0f, 0.0f,	0.5f, 0.5f,

		700.0f,	700.0f,	0.0f,		1.0f, 0.0f, 0.0f,	0.5f, 0.0f,
		800.0f, 700.0f,	0.0f,		1.0f, 0.0f, 0.0f,	1.0f, 0.0f,
		800.0f, 800.0f, 0.0f,	    1.0f, 0.0f, 0.0f,	1.0f, 0.5f,		 // 8,8
		700.0f,	800.0f, 0.0f,	    1.0f, 0.0f, 0.0f,	0.5f, 0.5f,

		0.0f,	600.0f,	0.0f,		1.0f, 0.0f, 0.0f,	0.5f, 0.0f,
		100.0f, 600.0f,	0.0f,		1.0f, 0.0f, 0.0f,	1.0f, 0.0f,
		100.0f, 700.0f, 0.0f,	    1.0f, 0.0f, 0.0f,	1.0f, 0.5f,		 // 1,7
		0.0f,	700.0f, 0.0f,	    1.0f, 0.0f, 0.0f,	0.5f, 0.5f,

		200.0f,	600.0f,	0.0f,		1.0f, 0.0f, 0.0f,	0.5f, 0.0f,
		300.0f, 600.0f,	0.0f,		1.0f, 0.0f, 0.0f,	1.0f, 0.0f,
		300.0f, 700.0f, 0.0f,	    1.0f, 0.0f, 0.0f,	1.0f, 0.5f,		 // 3,7
		200.0f,	700.0f, 0.0f,	    1.0f, 0.0f, 0.0f,	0.5f, 0.5f,

		400.0f,	600.0f,	0.0f,		1.0f, 0.0f, 0.0f,	0.5f, 0.0f,
		500.0f, 600.0f,	0.0f,		1.0f, 0.0f, 0.0f,	1.0f, 0.0f,
		500.0f, 700.0f, 0.0f,	    1.0f, 0.0f, 0.0f,	1.0f, 0.5f,		 // 5,7
		400.0f,	700.0f, 0.0f,	    1.0f, 0.0f, 0.0f,	0.5f, 0.5f,

		600.0f,	600.0f,	0.0f,		1.0f, 0.0f, 0.0f,	0.5f, 0.0f,
		700.0f, 600.0f,	0.0f,		1.0f, 0.0f, 0.0f,	1.0f, 0.0f,
		700.0f, 700.0f, 0.0f,	    1.0f, 0.0f, 0.0f,	1.0f, 0.5f,		 // 7,7
		600.0f,	700.0f, 0.0f,	    1.0f, 0.0f, 0.0f,	0.5f, 0.5f,

		100.0f,	500.0f,	0.0f,		1.0f, 0.0f, 0.0f,	0.5f, 0.0f,
		200.0f, 500.0f,	0.0f,		1.0f, 0.0f, 0.0f,	1.0f, 0.0f,
		200.0f, 600.0f, 0.0f,	    1.0f, 0.0f, 0.0f,	1.0f, 0.5f,		 // 2,6
		100.0f,	600.0f, 0.0f,	    1.0f, 0.0f, 0.0f,	0.5f, 0.5f,

		300.0f,	500.0f,	0.0f,		1.0f, 0.0f, 0.0f,	0.5f, 0.0f,
		400.0f, 500.0f,	0.0f,		1.0f, 0.0f, 0.0f,	1.0f, 0.0f,
		400.0f, 600.0f, 0.0f,	    1.0f, 0.0f, 0.0f,	1.0f, 0.5f,		 // 4,6
		300.0f,	600.0f, 0.0f,	    1.0f, 0.0f, 0.0f,	0.5f, 0.5f,

		500.0f,	500.0f,	0.0f,		1.0f, 0.0f, 0.0f,	0.5f, 0.0f,
		600.0f, 500.0f,	0.0f,		1.0f, 0.0f, 0.0f,	1.0f, 0.0f,
		600.0f, 600.0f, 0.0f,	    1.0f, 0.0f, 0.0f,	1.0f, 0.5f,		 // 6,6
		500.0f,	600.0f, 0.0f,	    1.0f, 0.0f, 0.0f,	0.5f, 0.5f,

		700.0f,	500.0f,	0.0f,		1.0f, 0.0f, 0.0f,	0.5f, 0.0f,
		800.0f, 500.0f,	0.0f,		1.0f, 0.0f, 0.0f,	1.0f, 0.0f,
		800.0f, 600.0f, 0.0f,	    1.0f, 0.0f, 0.0f,	1.0f, 0.5f,		 // 8,6
		700.0f,	600.0f, 0.0f,	    1.0f, 0.0f, 0.0f,	0.5f, 0.5f,
	};

	Position currentPosY;
	Position destiPosY;
	Position currentPosG;
	Position destiPosG;

	while (!glfwWindowShouldClose(window))
	{
		FPScounter(window);

		glClearColor(1.0f, 0.93f, 0.80f, 1.0f);
		if (Ending)
		{
			units_Indices.insert(units_Indices.end(), EndSrc_indices.begin(), EndSrc_indices.end());
		}
		//setting up vao,ebo, vbo
		{
			glGenVertexArrays(1, &unit_vao);

			glBindVertexArray(unit_vao);

			glGenBuffers(1, &unit_vbo);
			glBindBuffer(GL_ARRAY_BUFFER, unit_vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 833, nullptr, GL_DYNAMIC_DRAW);

			glGenBuffers(1, &unit_ebo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, unit_ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * units_Indices.size(), units_Indices.data(), GL_STATIC_DRAW);

			glBindBuffer(GL_ARRAY_BUFFER, unit_vbo);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			//
			glBindBuffer(GL_ARRAY_BUFFER, unit_vbo);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			//
			glBindBuffer(GL_ARRAY_BUFFER, unit_vbo);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
			glEnableVertexAttribArray(2);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			//
			/*glBindBuffer(GL_ARRAY_BUFFER, unit_vbo);
			glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(8 * sizeof(float)));
			glEnableVertexAttribArray(3);
			glBindBuffer(GL_ARRAY_BUFFER, 0);*/

			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}
		////
		int widthImg, heightImg, numColCh;
		//image properties
		{
			stbi_set_flip_vertically_on_load(true);
			unsigned char* bytes = stbi_load("everything1.png", &widthImg, &heightImg, &numColCh, 0);

			glGenTextures(1, &unit_texture0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, unit_texture0);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_NEAREST);

			//// use GL_CLAMP_TO_BORDER
			//float flatColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
			//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatColor);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
			glGenerateMipmap(GL_TEXTURE_2D);
			stbi_image_free(bytes);
			glBindTexture(GL_TEXTURE_2D, 0);

			GLuint texUni = glGetUniformLocation(unit_shaderProgram.ID, "tex0");
			unit_shaderProgram.Activate();
			// Sets the value of the uniform
			glUniform1i(texUni, 0);
		}
		///////////
		Board design1;
		design1.background = {
			0.0f,	0.0f,	0.0f,		1.0f, 0.0f, 0.0f,	0.0f, 0.0f,
			800.0f, 0.0f,	0.0f,		1.0f, 0.0f, 0.0f,	0.5f, 0.0f,
			800.0f, 800.0f, 0.0f,	    1.0f, 0.0f, 0.0f,	0.5f, 0.5f,		//board
			0.0f,	800.0f, 0.0f,	    1.0f, 0.0f, 0.0f,	0.0f, 0.5f,
		};
		if (result == Yellow_Team.name)
		{
			EndSrc_vertices =
			{
				200.0f,	300.0f,	0.0f,		1.0f, 0.0f, 0.0f,	0.5f, 0.75f,
				600.0f, 300.0f,	0.0f,		1.0f, 0.0f, 0.0f,	1.0f, 0.75f,
				600.0f, 500.0f, 0.0f,	    1.0f, 0.0f, 0.0f,	1.0f, 1.0f,		//ending frame
				200.0f,	500.0f, 0.0f,	    1.0f, 0.0f, 0.0f,	0.5f, 1.0f,
			};
		}
		else if (result == Grey_Team.name)
		{
			EndSrc_vertices =
			{
				200.0f,	300.0f,	0.0f,		1.0f, 0.0f, 0.0f,	0.5f, 0.5f,
				600.0f, 300.0f,	0.0f,		1.0f, 0.0f, 0.0f,	1.0f, 0.5f,
				600.0f, 500.0f, 0.0f,	    1.0f, 0.0f, 0.0f,	1.0f, 0.75f,		//ending frame
				200.0f,	500.0f, 0.0f,	    1.0f, 0.0f, 0.0f,	0.5f, 0.75f,
			};
		}
		

		design1.background.insert(design1.background.end(), Yellow_Team.units_vertices.begin(), Yellow_Team.units_vertices.end());
		design1.background.insert(design1.background.end(), Grey_Team.units_vertices.begin(), Grey_Team.units_vertices.end());
		if (Ending)
		{
			design1.background.insert(design1.background.end(), EndSrc_vertices.begin(), EndSrc_vertices.end());
		}

		glBindBuffer(GL_ARRAY_BUFFER, unit_vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 833, design1.background.data());
		glClear(GL_COLOR_BUFFER_BIT);

		unit_shaderProgram.Activate();
		glUniformMatrix4fv(glGetUniformLocation(unit_shaderProgram.ID, "v_MVP"), 1, GL_FALSE, glm::value_ptr(layout_projection));

		glBindTexture(GL_TEXTURE_2D, unit_texture0);
		//glBindTexture(GL_TEXTURE_2D, unit_texture1);
		//glBindTexture(GL_TEXTURE_2D, unit_texture2);
		//
		glBindVertexArray(unit_vao);
		glDrawElements(GL_TRIANGLES, 6 * (1 + 4 * 3 * 2 + 1), GL_UNSIGNED_INT, 0);

		bool firstFrame = true;
		if (firstFrame)
		{
			glfwSwapBuffers(window);
			//glfwPollEvents();
			firstFrame = false;
		}
		/*if (Ending)
		{
			break;
		}*/
		if (!Ending)
		{
			switch (ClickCount)
			{
			case 0:
			{
				//std::cout << "Yellow team's turn" << std::endl;

				GetCursorPosition(window, currentPosY);
				break;
			}
			case 1:
			{
				GetCursorPosition(window, destiPosY);
				break;
			}
			case 2:
			{
				//yellow_team.units_vertices take 384 GLfloat
				Yellow_Team.Move(window, currentPosY, destiPosY, Grey_Team);
				break;
			}
			case 3:
			{
				//std::cout << "Grey team's turn" << std::endl;

				GetCursorPosition(window, currentPosG);
				break;
			}
			case 4:
			{
				GetCursorPosition(window, destiPosG);
				break;
			}
			case 5:
			{
				Grey_Team.Move(window, currentPosG, destiPosG, Yellow_Team);
				break;
			}
			case 6:
			{
				ClickCount = 0;
			}
			}

			if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
			{
				for (int i = 0; i < Yellow_Team.UnitsPos.size(); i = i + 4)
				{
					for (int j = 0; j < 4; j++)
					{
						std::cout << Yellow_Team.UnitsPos[i + j] << ",";
					}
					std::cout << std::endl;
				}
			}
			if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
			{
				for (int i = 0; i < Grey_Team.UnitsPos.size(); i = i + 4)
				{
					for (int j = 0; j < 4; j++)
					{
						std::cout << Grey_Team.UnitsPos[i + j] << ",";
					}
					std::cout << std::endl;
				}
			}
			if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
			{
				for (int i = 0; i < Grey_Team.UnitsPos.size(); i = i + 4)
				{
					Grey_Team.UnitsPos[i] = 0;
					Grey_Team.UnitsPos[i + 1] = 0;
				}
				std::cout << "Grey Eliminated!!" << std::endl;
			}
			
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			{
				for (int i = 0; i < Yellow_Team.UnitsPos.size(); i = i + 4)
				{
					Yellow_Team.UnitsPos[i] = 0;
					Yellow_Team.UnitsPos[i + 1] = 0;
				}
				std::cout << "Yellow Eliminated!!" << std::endl;
			}

			if (ClickCount > 6 || ClickCount < 0)
			{
				std::cerr << "Clickcount out of range :" << ClickCount << std::endl;
				break;
			}

			if (!firstFrame)
			{
				glfwWaitEvents();
			}
			//glFinish();

			result = CheckWin(Yellow_Team, Grey_Team);
			if (result != "")
			{
				std::cout << "Checking !!!" << std::endl;
				std::cout << result << " \nWins!!!!!!";
				Ending = true;
				break;
			}
			if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			{
				break;
			}
		}

		glDeleteVertexArrays(1, &unit_vao);
		glDeleteBuffers(1, &unit_vbo);
		glDeleteBuffers(1, &unit_ebo);
		glDeleteTextures(1, &unit_texture0);
		//if (Ending)
		//{
		//	glfwSwapBuffers(window);
		//}
		//int stateSRC = glfwGetKey(window, GLFW_KEY_ESCAPE);
		//if (stateSRC == GLFW_PRESS)
		//{
		//	glfwWaitEvents();
		//	glfwDestroyWindow(window);
		//	break;
		//}
	}
	unit_shaderProgram.Delete();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}