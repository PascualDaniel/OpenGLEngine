#include "Camera.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <iostream>

Camera::Camera() {
	//Se asume que esta en el origen
	mEye		   = glm::vec3(0.0f, 0.0f, 0.0f);
	//Se asume que se mira hacia el mundo
	// se sigue -z porque sino se miraria detras de la camara
	mViewDirection = glm::vec3(0.0f, 0.0f, -1.0f);
	//Se asume que se empieza en un plano perfecto
	mUpVector	   = glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::mat4 Camera::GetViewMatrix() const {
	return glm::lookAt(mEye,mEye + mViewDirection, mUpVector);
}

void Camera::MouseLook(int mouseX, int mouseY) {
	std::cout << "Mouser: " << mouseX << std::endl;
	

	glm::vec2 currentMouse = glm::vec2(mouseX, mouseY);


	static bool firstLook = true;
	if (firstLook) {
		mOldMousePosition = currentMouse;
		firstLook = false;
	}
	
	float sensitivity = 0.1f; // Adjust this value to your liking
	glm::vec2 mouseDelta = sensitivity * (mOldMousePosition - currentMouse);;

	mViewDirection = glm::rotate(mViewDirection, glm::radians(mouseDelta.x), mUpVector);

	mOldMousePosition = currentMouse;
}

void Camera::MoveForward(float speed) {
	mEye += (mViewDirection*speed);

}
void Camera::MoveBackward(float speed) {
	mEye -= (mViewDirection * speed);
}
void Camera::MoveLeft(float speed) {
	glm::vec3 left = glm::cross(mUpVector, mViewDirection);
	mEye += left * speed;
}
void Camera::MoveRight(float speed) {
	glm::vec3 right = glm::cross(mViewDirection, mUpVector);
	mEye += right * speed;
}
void Camera::MoveUp(float speed) {
	mEye += mUpVector * speed;
}
void Camera::MoveDown(float speed) {
	mEye -= mUpVector * speed;
}