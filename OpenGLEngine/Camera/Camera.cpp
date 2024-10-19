#include "Camera.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <iostream>
#include <glm/gtc/type_ptr.hpp>

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
glm::mat4 Camera::GetProjectionMatrix() const {
	return mProjectionMatrix;
}
void Camera::SetProjectionMatrix(float fovy,float aspect,float near,float far) {

	// Initializes matrices since otherwise they will be the null matrix
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	// Makes camera look in the right direction from the right position
	view = glm::lookAt(mEye, mEye + mViewDirection, mUpVector);
	// Adds perspective to the scene
	projection = glm::perspective(fovy, aspect, near, far);

	// Sets new camera matrix
	mProjectionMatrix = projection * view;
}
void Camera::ProjectionMatrix(Shader& shader, const char* uniform)
{
	// Exports camera matrix
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(mProjectionMatrix));
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

	glm::vec3 right = glm::cross(mViewDirection, mUpVector);

	mViewDirection = glm::rotate(mViewDirection, glm::radians(mouseDelta.x), mUpVector);
	mViewDirection = glm::rotate(mViewDirection, glm::radians(mouseDelta.y), right);


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