#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Satellite.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <thread>
#include <windows.h>
#include <chrono>
#include <atomic>
#include <iomanip>
#include <sstream>



//Vertex Shader
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"uniform mat4 MVP;\n"
"void main()\n"
"{\n"
"	gl_Position = MVP * vec4(aPos, 1.0);\n"
"}\0";

//Fragment Shader from Circle
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(0.0f, 0.53f, 0.12f, 1.0f);\n"
"}\n\0";

//Fragment Shader from Round
const char* fragmentShaderSource2 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
"}\n\0";

//Fragment Shader from Satellite
const char* fragmentShaderSource3 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
"}\n\0";




std::atomic<bool> isRunning(true);

void hideCursor() {
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}


void ConsolLog(Satellite& sat) {
	HANDLE hBuffers[2];
	hBuffers[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	hBuffers[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

	CONSOLE_CURSOR_INFO cursorInfo;
	cursorInfo.dwSize = 100;
	cursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(hBuffers[0], &cursorInfo);
	SetConsoleCursorInfo(hBuffers[1], &cursorInfo);

	int currentBuffer = 0;

	while (isRunning) {
		sat.Update(1);
		
		std::stringstream ss;

		std::streambuf* oldCoutBuffer = std::cout.rdbuf();
		std::cout.rdbuf(ss.rdbuf());

		sat.ShowInfo();
		std::cout.rdbuf(oldCoutBuffer);

		std::string output = ss.str();
		DWORD bytesWritten = 0;

		COORD topLeft = { 0, 0 };
		FillConsoleOutputCharacterA(hBuffers[currentBuffer], ' ', 80 * 25, topLeft, &bytesWritten);
		WriteConsoleA(hBuffers[currentBuffer], output.c_str(), static_cast<DWORD>(output.size()), &bytesWritten, NULL);
		SetConsoleActiveScreenBuffer(hBuffers[currentBuffer]);
		currentBuffer = 1 - currentBuffer;

		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}
}

void Visualization(Satellite& sat) {

	const int height = 720;
	const int width = 1280;

	//Circle Settings
	const int numOfCircleSegment = 64;
	const double positionScale = 9000000; //  height of window = 10.000.000 meters
	float radius = static_cast<float>(Earth_Radius / positionScale);

	std::vector<float> circleVertices;

	//Center point of circle
	circleVertices.push_back(0.0f);
	circleVertices.push_back(0.0f);
	circleVertices.push_back(0.0f);

	//Generate poites on circle for trigonometry formuls
	for (int i = 0; i <= numOfCircleSegment; i++) {
		//Angle for the current step in radians
		float angle = i * 2.0f * static_cast<float>(PI) / static_cast<float>(numOfCircleSegment);
		//X
		circleVertices.push_back(cos(angle) * radius);
		//Y
		circleVertices.push_back(sin(angle) * radius);
		//Z (==0)
		circleVertices.push_back(0.0f);
	}

	//Rendering the satellite's orbit

	double period = sat.getOrbitalPeriod();
	Satellite gSat = sat;

	const int numOfRoundSegment = 300;
	double timeStep = period / static_cast<double>(numOfRoundSegment);

	std::vector<float> roundVertices;

	for (int i = 0; i < numOfRoundSegment; i++) {
		//Angle for the current step in radians
		float angle = i * 2.0f * static_cast<float>(PI) / static_cast<float>(numOfRoundSegment);
		//X
		roundVertices.push_back((float)(gSat.position[0] / positionScale));
		//Y
		roundVertices.push_back((float)(gSat.position[1] / positionScale));
		//Z (==0)
		roundVertices.push_back(0.0f);
		gSat.Update(timeStep);
	}




	std::thread th(ConsolLog, std::ref(sat));

	hideCursor();
	if (!glfwInit()) {
		isRunning = false;
		th.join();
		return;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(width, height, "Visualisation", NULL, NULL);
	if (window == NULL) {
		glfwTerminate();
		isRunning = false;
		th.join();
		return;
	}

	glfwMakeContextCurrent(window);
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to init GLAD" << "\n";
		isRunning = false;
		th.join();
		return;
	}

	glViewport(0, 0, width, height);


	//===== EARTH SHADER PROGRAM ====

	//Create Vertex Shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	//Create Fragment Shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glad_glCompileShader(fragmentShader);

	//Create Shader Program
	GLuint shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glLinkProgram(shaderProgram);
	glDeleteShader(fragmentShader);
	


	//===== ORBIT SHADER PROGRAM ====

	//Create Fragment Shader
	GLuint fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader2, 1, &fragmentShaderSource2, NULL);
	glad_glCompileShader(fragmentShader2);

	//Create Shader Program
	GLuint shaderProgram2 = glCreateProgram();

	glAttachShader(shaderProgram2, vertexShader);
	glAttachShader(shaderProgram2, fragmentShader2);
	glLinkProgram(shaderProgram2);

	glDeleteShader(fragmentShader2);


	//===== SATELLITE SHADER PROGRAM ====

	//Create Fragment Shader
	GLuint fragmentShader3 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader3, 1, &fragmentShaderSource3, NULL);
	glad_glCompileShader(fragmentShader3);

	//Create Shader Program
	GLuint shaderProgram3 = glCreateProgram();

	glAttachShader(shaderProgram3, vertexShader);
	glAttachShader(shaderProgram3, fragmentShader3);
	glLinkProgram(shaderProgram3);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader3);
	

	//Render circle
	GLuint VAO, VBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);


	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, circleVertices.size() * sizeof(float), circleVertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);



	//Render round
	GLuint VAO2, VBO2;

	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);

	glBindVertexArray(VAO2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, roundVertices.size() * sizeof(float), roundVertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	//Render point for satellite
	GLuint VAO3, VBO3;

	glGenVertexArrays(1, &VAO3);
	glGenBuffers(1, &VBO3);

	glBindVertexArray(VAO3);

	glBindBuffer(GL_ARRAY_BUFFER, VBO3);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float), NULL, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	Satellite startState = sat;

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Camera settings
		float aspect = (float)width / (float)height;
		int aspectLoc = glGetUniformLocation(shaderProgram, "aspect");
		glUniform1f(aspectLoc, aspect);


		glm::mat4 projection = glm::ortho(-2.0f, 2.0f, -2.0f / aspect, 2.0f / aspect, -10.0f, 10.0f);

		glm::mat4 view = glm::lookAt(
			glm::vec3(0.0f, -0.0f, 5.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f)
		);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 MVP = projection * view * model;

		//Render Earth
		glUseProgram(shaderProgram);
		int mvpLoc = glGetUniformLocation(shaderProgram, "MVP");
		glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(MVP));

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLE_FAN, 0, numOfCircleSegment + 2);


		//Render orbit
		double period = startState.getOrbitalPeriod();
		Satellite gSat = startState;

		std::vector<float> dynamicOrbitVertices;
		for (int i = 0; i < numOfRoundSegment; i++) {
			dynamicOrbitVertices.push_back((float)(gSat.position[0] / positionScale));
			dynamicOrbitVertices.push_back((float)(gSat.position[1] / positionScale));
			dynamicOrbitVertices.push_back((float)(gSat.position[2] / positionScale));
			gSat.Update(period / numOfRoundSegment);
		}

		glBindBuffer(GL_ARRAY_BUFFER, VBO2);
		glBufferData(GL_ARRAY_BUFFER, dynamicOrbitVertices.size() * sizeof(float), dynamicOrbitVertices.data(), GL_DYNAMIC_DRAW);

		glUseProgram(shaderProgram2);
		int mvpLoc2 = glGetUniformLocation(shaderProgram2, "MVP");
		glUniformMatrix4fv(mvpLoc2, 1, GL_FALSE, glm::value_ptr(MVP));

		int aspectLoc2 = glGetUniformLocation(shaderProgram2, "aspect");
		glUniform1f(aspectLoc2, aspect);

		glLineWidth(3.5f);
		glBindVertexArray(VAO2);
		glDrawArrays(GL_LINE_LOOP, 0, numOfRoundSegment);
		glLineWidth(1.0f);

		// Render satellite
		std::vector<float> satPosition = {
			(float)(sat.position[0] / positionScale),
			(float)(sat.position[1] / positionScale),
			(float)(sat.position[2] / positionScale)
		};

		glBindBuffer(GL_ARRAY_BUFFER, VBO3);
		glBufferData(GL_ARRAY_BUFFER, satPosition.size() * sizeof(float), satPosition.data(), GL_DYNAMIC_DRAW);

		glUseProgram(shaderProgram3);
		int mvpLoc3 = glGetUniformLocation(shaderProgram3, "MVP");
		glUniformMatrix4fv(mvpLoc3, 1, GL_FALSE, glm::value_ptr(MVP));
		glPointSize(16.0f);

		glBindVertexArray(VAO3);
		glDrawArrays(GL_POINTS, 0, 1);

		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);
	glDeleteProgram(shaderProgram2);
	glDeleteProgram(shaderProgram3);
	
	isRunning = false;
	if (th.joinable()) {
		th.join();
	}

	glfwTerminate();
}