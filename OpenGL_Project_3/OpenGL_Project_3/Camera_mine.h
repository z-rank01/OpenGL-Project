#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <iostream>
#include <vector>

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

const float yaw = -90.0f;
const float pitch = 0.0f;
const float speed = 2.5f;
const float sensitive = 0.1f;
const float zoom = 45.0f;

class Camera
{
public:
	glm::vec3 cameraPos;
	glm::vec3 cameraWorldUp;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
	glm::vec3 cameraRight;

	float Yaw;
	float Pitch;

	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float YAW = yaw, float PITCH = pitch) : cameraFront(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(speed), MouseSensitivity(sensitive), Zoom(zoom)
	{
		cameraPos = position;
		cameraWorldUp = up;
		Yaw = YAW;
		Pitch = PITCH;
		Update();
	}

	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float YAW = yaw, float PITCH = pitch) : cameraFront(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(speed), MouseSensitivity(sensitive), Zoom(zoom)
	{
		cameraPos = glm::vec3(posX, posY, posZ);
		cameraWorldUp = glm::vec3(upX, upY, upZ);
		Yaw = YAW;
		Pitch = PITCH;
		Update();
	}
	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	}

	void ProcessKeyboard(Camera_Movement direction, float deltatime)
	{
		float velocity = MovementSpeed * deltatime;
		if (direction == FORWARD)
		{
			cameraPos += cameraFront * velocity;
		}
		if (direction == BACKWARD)
		{
			cameraPos -= cameraFront * velocity;
		}
		if (direction == LEFT)
		{
			cameraPos -= cameraRight * velocity;
		}
		if (direction == RIGHT)
		{
			cameraPos += cameraRight * velocity;
		}
	}

	void ProcessMouseMovement(float x, float y, GLboolean constrain = true)
	{
		x *= MouseSensitivity;
		y *= MouseSensitivity;

		Yaw += x;
		Pitch += y;

		if (constrain)
		{
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		}

		Update();
	}

	void ProcessMouseScroll(float y)
	{
		Zoom -= y;
		if (Zoom < 1.0f)
		{
			Zoom = 1.0f;
		}
		if (Zoom > 45.0f)
		{
			Zoom = 45.0f;
		}
	}

private:
	void Update()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

		cameraFront = glm::normalize(front);
		cameraRight = glm::normalize(glm::cross(cameraFront, cameraWorldUp));
		cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
	}
};

