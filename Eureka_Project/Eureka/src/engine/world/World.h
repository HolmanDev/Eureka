#pragma once

#include "GameObject.h"
#include "engine/ui/UIElement.h"
#include "Camera.h"
#include "engine/concepts/PackedVector.h"
#include "engine/renderer/Renderer.h"

class World : public EventSubscriber
{
private:
	packedVector<std::shared_ptr<GameObject>> m_GameObjects;
	packedVector<std::shared_ptr<UIElement>> m_UIElements;
	std::shared_ptr<Camera> m_Camera;
public:
	World();
	~World();

	void Start() override;
	void Update() override;

	void Init();
	void Close();

	std::vector<std::shared_ptr<GameObject>> GetGameObjects();
	std::vector<std::shared_ptr<UIElement>> GetUIElements();
	inline const std::shared_ptr<Camera> GetCamera() const { return m_Camera; }

	void SetCamera(std::shared_ptr<Camera> camera);

	// void CreateGameObject(/*file reference*/);
	void AddGameObject(std::shared_ptr<GameObject> gameObject);
	void RemoveGameObject(std::shared_ptr<GameObject> gameObject);
	void RemoveGameObjectAt(unsigned int id);
	void AddUIElement(std::shared_ptr<UIElement> uiElement);
	void RemoveUIElement(std::shared_ptr<UIElement> uiElement);
	void RemoveUIElementAt(unsigned int id);
	std::shared_ptr<UIElement> GetUIElement(std::string name);

	void Render();
	void RenderColliders();

	void LogGameObjects();
	//void LogUIElements();
};