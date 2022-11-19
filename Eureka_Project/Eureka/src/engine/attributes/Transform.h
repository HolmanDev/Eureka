#pragma once

class Transform
{
private:
	glm::vec3 m_Scale;
	glm::quat m_Rotation;
	glm::vec3 m_Position;
public:
	Transform();
	~Transform();

	const glm::mat4 GetMatrix();

	void SetScale(glm::vec3 scale);
	void SetRotation(glm::quat rotation);
	void Rotate(glm::quat rotation);
	void RotateEuler(float angle, glm::vec3 dir);
	void RotateEuler2(float angle, glm::vec3 dir);
	void SetPosition(glm::vec3 position);

	inline const glm::vec3 GetScale() const { return m_Scale; }
	inline const glm::quat GetRotation() const { return m_Rotation; }
	inline const glm::vec3 GetPosition() const { return m_Position; }
	const glm::vec3 GetRight() const;
	const glm::vec3 WorldRightToLocal();
	const glm::vec3 GetUp() const;
	const glm::vec3 WorldUpToLocal();
	const glm::vec3 GetForward() const;
};