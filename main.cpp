#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
// Map Libs
#include <SOIL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLFW/glfw3.h>
#include "shader.h"
#include "camera.h"
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void Do_Movement();

const GLuint WIDTH = 800, HEIGHT = 600;

// Camera

bool keys[1024];

bool R_click = false;


GLfloat lastX;
GLfloat lastY;

Camera camera(glm::vec3(0.0f, 0.0f, 4.0f));
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

//light attribute
glm::vec3 lightPos(1.5f, 1.5f, 0.f);

int main()
{
	glfwInit();

	// Set all the required options for GLFW
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL_test", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create window";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	// viewport dimensions
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	glEnable(GL_DEPTH_TEST);
	// build and compile shader
	// shader
	Shader lightingObjShader("shaders/lightingVer.vs","shaders/lightingObj.frag");
	Shader lampShader("shaders/lampVer.vs", "shaders/lamp.frag");

	// Set up vertex data (and buffer(s)) and attribute pointers
	/*
	GLfloat vertices[] = {
		-0.5f, -0.5f, -0.5f, 
		0.5f, -0.5f, -0.5f,  
		0.5f,  0.5f, -0.5f,  
		0.5f,  0.5f, -0.5f, 
		-0.5f,  0.5f, -0.5f,  
		-0.5f, -0.5f, -0.5f,  

		-0.5f, -0.5f,  0.5f,  
		0.5f, -0.5f,  0.5f,  
		0.5f,  0.5f,  0.5f,  
		0.5f,  0.5f,  0.5f,  
		-0.5f,  0.5f,  0.5f,  
		-0.5f, -0.5f,  0.5f, 

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f, 
		-0.5f, -0.5f, -0.5f, 
		-0.5f, -0.5f, -0.5f, 
		-0.5f, -0.5f,  0.5f, 
		-0.5f,  0.5f,  0.5f,

		0.5f,  0.5f,  0.5f, 
		0.5f,  0.5f, -0.5f,  
		0.5f, -0.5f, -0.5f,  
		0.5f, -0.5f, -0.5f,  
		0.5f, -0.5f,  0.5f,  
		0.5f,  0.5f,  0.5f,  

		-0.5f, -0.5f, -0.5f, 
		0.5f, -0.5f, -0.5f, 
		0.5f, -0.5f,  0.5f,  
		0.5f, -0.5f,  0.5f,  
		-0.5f, -0.5f,  0.5f,  
		-0.5f, -0.5f, -0.5f, 

		-0.5f,  0.5f, -0.5f, 
		0.5f,  0.5f, -0.5f,  
		0.5f,  0.5f,  0.5f,  
		0.5f,  0.5f,  0.5f,  
		-0.5f,  0.5f,  0.5f, 
		-0.5f,  0.5f, -0.5f
	};
	*/

	

	GLfloat vertices[] = {
		// Positions          // Normals           // Texture Coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};

	
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};
	
	GLuint VBO, containerVAO;
	glGenVertexArrays(1, &containerVAO);
	glGenBuffers(1, &VBO);

	//glGenBuffers(1, &EBO);

	glBindVertexArray(containerVAO);
	// 2. 把顶点数组复制到缓冲中供OpenGL使用
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	/*
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	*/
	// 3. 设置顶点属性指针
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
	 //4. 解绑VAO
	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)


	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);


	//load texture
	GLuint diffuseMap,specularMap,emissionMap;
	glGenTextures(1, &diffuseMap);
	glGenTextures(1, &specularMap);
	glGenTextures(1, &emissionMap);
	int width1, height1;
	unsigned char* image;
	// diffuse map
	image = SOIL_load_image("container2.png", &width1, &height1, 0 ,SOIL_LOAD_RGB);
	glBindTexture(GL_TEXTURE_2D, diffuseMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width1, height1, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
	//specular map 
	image = SOIL_load_image("container2_specular.png", &width1, &height1, 0, SOIL_LOAD_RGB);
	glBindTexture(GL_TEXTURE_2D, specularMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width1, height1, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	lightingObjShader.Use();
	glUniform1i(glGetUniformLocation(lightingObjShader.Program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(lightingObjShader.Program, "material.specular"), 1);
	//game loop
	while (!glfwWindowShouldClose(window))
	{
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//events check
		glfwPollEvents();
		Do_Movement();
		//Render
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		GLfloat Cur_time = glfwGetTime();
		//lightPos = glm::vec3( (GLfloat) 1.8*cos(Cur_time), (GLfloat) 1.8*sin(Cur_time),0.f);

		//
		lightingObjShader.Use();
		//GLint objectColorLoc = glGetUniformLocation(lightingObjShader.Program, "objectColor");
		//GLint lightColorLoc = glGetUniformLocation(lightingObjShader.Program, "lightColor");
		GLint lightPosLoc = glGetUniformLocation(lightingObjShader.Program, "light.position");
		GLint lightDirLoc = glGetUniformLocation(lightingObjShader.Program, "light.direction");
		GLint viewPosLoc = glGetUniformLocation(lightingObjShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.Position.x, camera.Position.y, camera.Position.z);
		glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(lightDirLoc, -0.2f, -1.0f, -0.3f);
		//glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f);
		//glUniform3f(lightColorLoc, 1.0f, 0.5f, 1.0f);

		// material struct 
		//glUniform3f(glGetUniformLocation(lightingObjShader.Program, "material.ambient"), 0.0f, 1.f, 0.06f);
		//glUniform3f(glGetUniformLocation(lightingObjShader.Program, "material.diffuse"), 0.0f, 0.50980392f, 0.50980392f);
		glUniform3f(glGetUniformLocation(lightingObjShader.Program, "material.specular"), 0.5f, 0.5f, 0.5f);
		glUniform1f(glGetUniformLocation(lightingObjShader.Program, "material.shininess"), 64.0f);

		//light struct
		glUniform3f(glGetUniformLocation(lightingObjShader.Program, "light.ambient"), 0.2f, 0.2f, 0.2f);
		glUniform3f(glGetUniformLocation(lightingObjShader.Program, "light.diffuse"), 0.5f, 0.5f, 0.5f);// 让我们把这个光调暗一点，这样会看起来更自然
		glUniform3f(glGetUniformLocation(lightingObjShader.Program, "light.specular"), 1.0f, 1.0f, 1.0f);

		// Create transformations
		
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;
		//model = glm::rotate(model, (GLfloat)Cur_time * 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
		view = camera.GetViewMatrix();
		//model = glm::rotate(model, glm::radians(30.f), glm::vec3(0.f, 1.f, 0.f));
		projection = glm::perspective((float)glm::radians(camera.Zoom), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
		// Get their uniform location
		GLint modelLoc = glGetUniformLocation(lightingObjShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingObjShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingObjShader.Program, "projection");
		// Pass them to the shaders
	//	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		// Note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		//Bind diffuse map
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		// specular map
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);

		// Draw
		
		glBindVertexArray(containerVAO);
		for (int i = 0;i < 10;i++)
		{
			model = glm::mat4();
			model = glm::translate(model, cubePositions[i]);
			model = glm::rotate(model, (GLfloat)(Cur_time * (i+1)/10), glm::vec3(1.f ,  1.f, (i+1) % 2));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);

		/*

		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");
		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4();
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		*/
		//swap the screen buffers
		glfwSwapBuffers(window);

	}
	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &containerVAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}

// key callback function
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void Do_Movement()
{
	// Camera controls
	// ProcessKey1() locks y 
	if (keys[GLFW_KEY_W])
		camera.ProcessKey(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera.ProcessKey(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera.ProcessKey(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera.ProcessKey(RIGHT, deltaTime);
	if (keys[GLFW_KEY_SPACE])
		camera.ProcessKey(SPACE, deltaTime);
	if (keys[GLFW_KEY_V])
		camera.ProcessKey(DOWN, deltaTime);

}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (R_click)
	{


		GLfloat xoffset = xpos - lastX;
		GLfloat yoffset = lastY - ypos;

		lastX = xpos;
		lastY = ypos;

		camera.ProcessMouseMove(xoffset, yoffset);
	}
	else {
		lastX = xpos;
		lastY = ypos;
	}

}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
		R_click = true;
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
		R_click = false;
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{

	camera.ProcessMouseScroll(yoffset);
}