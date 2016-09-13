#pragma once
// Std. Includes
#include <vector>

// GL Includes
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	SPACE,
	DOWN
};
// Default camera values
const GLfloat YAW = -90.0f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 3.0f;
const GLfloat SENSITIVTY = 0.25f;
const GLfloat ZOOM = 45.0f;

class Camera {
public:
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	GLfloat Yaw;
	GLfloat Pitch;

	GLfloat MoveSpeed;
	GLfloat MouseSens;
	GLfloat Zoom;

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		GLfloat yaw = YAW, GLfloat pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), 
		MoveSpeed(SPEED), MouseSens(SENSITIVTY), Zoom(ZOOM)
	{
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	Camera(GLfloat posX, GLfloat posY, 
		GLfloat posZ, GLfloat upX, 
		GLfloat upY, GLfloat upZ,
		GLfloat yaw, GLfloat pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MoveSpeed(SPEED), 
		MouseSens(SENSITIVTY), Zoom(ZOOM)
	{
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}

	void ProcessKey(Camera_Movement dre, GLfloat deltaTime){
		GLfloat velocity = MoveSpeed*deltaTime;
		if (dre == FORWARD)
			Position += Front * velocity;
		if (dre == BACKWARD)
			Position -= Front * velocity;
		if (dre == LEFT)
			Position -= Right * velocity;
		if (dre == RIGHT)
			Position += Right * velocity;
		if (dre == SPACE)
			Position += WorldUp * velocity;
		if (dre == DOWN)
			Position -= WorldUp * velocity;

	}
	// y:Locked
	void ProcessKey1(Camera_Movement dre, GLfloat deltaTime) {
		GLfloat velocity = MoveSpeed*deltaTime;
		if (dre == FORWARD)
			Position += glm::vec3( Front.x,0,Front.z) * velocity;
		if (dre == BACKWARD)
			Position -= glm::vec3( Front.x, 0, Front.z) * velocity;
		if (dre == LEFT)
			Position -= Right * velocity;
		if (dre == RIGHT)
			Position += Right * velocity;
		if (dre == SPACE)
			Position += WorldUp * velocity;
		if (dre == DOWN)
			Position -= WorldUp * velocity;
	}

	void ProcessMouseMove(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true)
	{
		xoffset *= MouseSens;
		yoffset *= MouseSens;

		Yaw += xoffset;
		Pitch += yoffset;

		if (constrainPitch)
		{
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		}

		updateCameraVectors();
	}
	void ProcessMouseScroll(GLfloat yoffset)
	{
		if (Zoom >= 1.0f && Zoom <= 60.0f)
			Zoom -= yoffset;
		if (Zoom <= 1.0f)
			Zoom = 1.0f;
		if (Zoom >= 60.0f)
			Zoom = 60.0f;
	}
private:
	void updateCameraVectors()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw));
		Front = glm::normalize(front);

		Right = glm::normalize(glm::cross(Front, WorldUp)); 
		Up = glm::normalize(glm::cross(Right,Front));
	}
};