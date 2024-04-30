#ifndef CAMERA_HPP
#define CAMERA_HPP
#include <glm/glm.hpp>
class Camera {
	public:	

		///Def Const
		Camera();

		/// La matriz de la vista que se genera y se devuelve.
		glm::mat4 GetViewMatrix() const;

		void MoveForward(float speed);
		void MoveBackward(float speed);
		void MoveLeft(float speed);
		void MoveRight(float speed);

	private:
		glm::vec3 mEye;
		glm::vec3 mViewDirection;
		glm::vec3 mUpVector;
		
};

#endif