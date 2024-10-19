#ifndef CAMERA_HPP
#define CAMERA_HPP
#include <glm/glm.hpp>
#include "../Shaders/Shader.hpp"
class Camera {
	public:	

		///Def Const
		Camera();


		void SetProjectionMatrix(float fovy, float aspect, float near, float far);
		/// La matriz de la vista que se genera y se devuelve.
		glm::mat4 GetViewMatrix() const;

		glm::mat4 GetProjectionMatrix() const;

		void ProjectionMatrix(Shader& shader, const char* uniform);

		void MouseLook(int mouseX, int mouseY);

		void MoveForward(float speed);
		void MoveBackward(float speed);
		void MoveLeft(float speed);
		void MoveRight(float speed);
		void MoveUp(float speed);
		void MoveDown(float speed);

	private:
		glm::mat4 mProjectionMatrix;

		glm::vec3 mEye;
		glm::vec3 mViewDirection;
		glm::vec3 mUpVector;

		glm::vec2 mOldMousePosition;
		
};

#endif