#include "Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>


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
	return glm::lookAt(mEye, mViewDirection, mUpVector);
}

void Camera::MoveForward(float speed) {
	mEye.z -= speed;

}
void Camera::MoveBackward(float speed) {
	mEye.z += speed;
}
void Camera::MoveLeft(float speed) {

}
void Camera::MoveRight(float speed) {

}