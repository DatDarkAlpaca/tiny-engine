#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace tiny
{
	class ICamera
	{
	public:
		virtual ~ICamera() = default;

	public:
		glm::mat4 getView() const { return view; }

		glm::mat4 getProjection() const { return projection; }

	public:
		glm::vec3 position = glm::vec3(0.f);
		glm::vec3 target = glm::vec3(0.f);
		glm::mat4 view = glm::mat4(1.0f), projection = glm::mat4(1.0f);
	};

	class OrthographicCamera : public ICamera
	{
	public:
		OrthographicCamera() = default;

	public:
		void updateView()
		{
			view = glm::lookAt(position, position + target, glm::vec3(0.0f, 1.0f, 0.0f));
		}

		void updateProjection(float left, float right, float bottom, float top, float zNear = -1.0f, float zFar = 1.0f)
		{
			projection = glm::ortho(left, right, bottom, top, zNear, zFar);
		}
	};
}