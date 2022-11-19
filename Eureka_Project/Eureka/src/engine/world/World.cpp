#include "pch.h"
#include "World.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "engine/core/Time.h"
#include "engine/core/WindowInfo.h"
#include "engine/events/EventManager.h"
#include "engine/concepts/AttributeManager.h"
#include "engine/renderer/Framebuffer.h"
#include "engine/concepts/AssetManager.h"
#include "engine/attributes/Physics.h"
#include "game/Spin.h"
#include "game/inventions/Invention.h"
#include "game/inventions/parts/Parts.h"
#include "game/player/player.h"
#include "game/GameManager.h"
#include "game/ui/UIManager.h"
#include "game/inventions/InventionManager.h"

World::World()
{
}

World::~World()
{
}

void World::Start()
{
}

void World::Update()
{
}

void World::Init()
{
	GameManager::SetWorld(this);
	UIManager::Init();

	std::shared_ptr<GameObject> player = std::make_shared<GameObject>("Player");
	AddGameObject(player);
	player->GetTransform().SetPosition(glm::vec3(0.0f, 0.0f, 10.0f));
	std::shared_ptr<Player> playerAttribute = std::make_shared<Player>();
	player->AddAttribute(playerAttribute);
	EventManager::AddSubscriber(playerAttribute);
	playerAttribute->Init();
	SetCamera(playerAttribute->GetCamera());

	std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>("Invention");
	AddGameObject(gameObject);
	std::shared_ptr<Invention> invention = std::make_shared<Invention>();
	int invention_key = InventionManager::AddInvention(invention);
	invention->SetKey(invention_key);

	gameObject->AddAttribute(invention);
	EventManager::AddSubscriber(invention);
	invention->SetPlayer(playerAttribute.get());
	std::shared_ptr<Part> part1 = std::make_shared<pStructure>();
	invention->AddPart(part1);
	invention->GenerateMesh(); //! Create file loading for materials
	invention->ApplyMesh();
	invention->GenerateColliders();
	invention->ApplyColliders();
	std::shared_ptr<Physics> physics = std::make_shared<Physics>(gameObject->GetTransform());
	gameObject->AddAttribute(physics);
	EventManager::AddSubscriber(physics);
	//invention->Run();
	//gameObject->AddAttribute<Spin>();

	std::shared_ptr<UIElement> blockTypeUI = std::make_shared<UIElement>();
	blockTypeUI->SetName("blockTypeUI");
	AddUIElement(blockTypeUI);
	blockTypeUI->SetPosition(glm::vec2(0.025f, 0.025f));
	blockTypeUI->SetSize(glm::vec2(0.1f, 0.1f));
	std::shared_ptr<Shader> shader = AssetManager::GetShader("UI");
	std::shared_ptr<Material> material = std::make_shared<Material>(shader);
	material->SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	Texture texture("res/textures/StructureIcon.png", { GL_LINEAR, GL_CLAMP_TO_EDGE });
	blockTypeUI->SetMaterial(material);
	material->SetTexture(&texture);
	blockTypeUI->Invalidate();
	//blockTypeUI->SetTexture(texture);
}

void World::Close()
{
	m_GameObjects.Clear();
	m_UIElements.Clear();
	InventionManager::Close();
}

std::vector<std::shared_ptr<GameObject>> World::GetGameObjects()
{
	return m_GameObjects.GetElements();
}

std::vector<std::shared_ptr<UIElement>> World::GetUIElements()
{
	return m_UIElements.GetElements();
}

void World::SetCamera(std::shared_ptr<Camera> camera)
{
	m_Camera = camera;
}

void World::AddGameObject(std::shared_ptr<GameObject> gameObject)
{
	unsigned int id = m_GameObjects.Add(gameObject);
	gameObject->SetID(id);
	AttributeManager::AddGameObject(gameObject);
}

void World::RemoveGameObject(std::shared_ptr<GameObject> gameObject)
{
	m_GameObjects.RemoveAt(gameObject->GetID());
}

void World::RemoveGameObjectAt(unsigned int id)
{
	m_GameObjects.RemoveAt(id);
}

void World::AddUIElement(std::shared_ptr<UIElement> uiElement)
{
	unsigned int id = m_UIElements.Add(uiElement);
	uiElement->SetID(id);
}

void World::RemoveUIElement(std::shared_ptr<UIElement> uiElement)
{
	m_UIElements.RemoveAt(uiElement->GetID());
}

void World::RemoveUIElementAt(unsigned int id)
{
	m_UIElements.RemoveAt(id);
}

std::shared_ptr<UIElement> World::GetUIElement(std::string name)
{
	std::vector<std::shared_ptr<UIElement>> uiElements = GetUIElements();
	for (std::shared_ptr<UIElement> uiElement : uiElements)
	{
		if (uiElement->GetName() == name)
		{
			return uiElement;
		}
	}
	return nullptr;
}

//! Dynamically create shader and effects. The templates that decide the effects should hold all the uniform data.
void World::Render()
{
	Renderer::BeginScene(*m_Camera);
	for (std::shared_ptr<GameObject> gameObject : GetGameObjects())
	{
		std::shared_ptr<Mesh> mesh = gameObject->GetAttribute<Mesh>();
		if (mesh != nullptr) {
			std::shared_ptr<Material> material = mesh->GetMaterial();
			Renderer::Submit(material, mesh->GetVertexArray(), gameObject->GetModelMatrix()); //! Fix vertex array here
		}

	}
	Renderer::Render();
	if (Renderer::VisibleCollidersEnabled())
		RenderColliders();

	for (std::shared_ptr<UIElement> uiElement : GetUIElements())
	{
		const Texture& texture = uiElement->GetMaterial()->GetTexture();
		Renderer::SubmitUI(uiElement->GetFramebuffer().GetID(), uiElement->GetPosition(), uiElement->GetSize(), 
			glm::vec2(texture.GetWidth(), texture.GetHeight()));
	}
	Renderer::RenderUI();

	UIManager::Run();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	Renderer::EndScene();
}

void World::RenderColliders()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	std::shared_ptr<Shader> shader = AssetManager::GetShader("ReplacementShader");
	VertexBufferLayout layout;
	layout.Push<float>(3);
	shader->Bind();
	shader->SetUniformMat4f("u_ViewProj", m_Camera->GetProjectionMatrix() * m_Camera->GetViewMatrix());
	shader->SetUniform4f("u_Color", 0.3f, 1.0f, 0.4f, 1.0f);
	for (std::shared_ptr<GameObject> gameObject : GetGameObjects())
	{
		std::vector<std::shared_ptr<MeshCollider>> meshColliders = gameObject->GetAttributes<MeshCollider>();
		for (std::shared_ptr<MeshCollider> meshCollider : meshColliders)
		{
			std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>(
				meshCollider->GetVertices(), 
				meshCollider->GetVertexCount() * 3 * sizeof(float)
			);
			std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>(
				meshCollider->GetIndices(), 
				meshCollider->GetIndexCount()
			);
			std::shared_ptr<VertexArray> vao = std::make_shared<VertexArray>();
			vao->ClearVertexBuffers();
			vao->SetLayout(layout);
			vao->AddVertexBuffer(vertexBuffer);
			vao->SetIndexBuffer(indexBuffer);
			glm::mat4 matrix = glm::translate(gameObject->GetModelMatrix(), meshCollider->center);
			shader->SetUniformMat4f("u_Transform", matrix);
			RenderCommand::Draw(vao);
		}
	}
	shader->Unbind();
}

void World::LogGameObjects()
{
	int i = 0;
	for (std::shared_ptr<GameObject> worldGameObject : m_GameObjects.GetElements())
	{
		if (worldGameObject != nullptr) { //! Can prob remove this
			std::cout << i << ": " << worldGameObject->GetID() << ", " << worldGameObject->GetName() << std::endl;
		}
		i++;
	}
}