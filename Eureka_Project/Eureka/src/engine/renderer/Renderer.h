#pragma once

#include "engine/glCore.h"

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "engine/renderer/RenderCommand.h"
#include "engine/world/GameObject.h"
#include "engine/world/Camera.h"
#include "Framebuffer.h"
#include "engine/ui/UIElement.h"

class SSAO;

//! Put inside renderer class?
struct Model
{
    std::shared_ptr<Material> Material;
    std::shared_ptr<VertexArray> VertexArray;
    glm::mat4 Transform;
};

struct UI
{
    unsigned int Framebuffer;
    glm::vec2 Pos;
    glm::vec2 Size;
    glm::vec2 TextureSize;
};

//! Make everything static!
class Renderer 
{
private:
    struct SceneData
    {
        glm::mat4 vpMatrix;
    };

    static std::unique_ptr<SceneData> s_SceneData;
    static std::vector<Model> s_RenderQueue;
    static std::vector<UI> s_UIRenderQueue;
    static std::unique_ptr<SSAO> s_SSAO;
    static bool s_VisibleColliders;
public:
    static void Init();
    static void Close();
    static void Render();
    static void RenderUI();
    static void BeginScene(Camera& camera);
    static void EndScene();
    static void Submit(const std::shared_ptr<Material>& material, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));
    static void SubmitUI(const unsigned int framebuffer, const glm::vec2 pos, const glm::vec2 size, glm::vec2 textureSize); // Add rotation

    static bool VisibleCollidersEnabled() { return s_VisibleColliders; }
    static void EnableVisibleColliders();
    static void DisableVisibleColliders();

    static const SSAO& GetSSAO() { return *s_SSAO; }
    static void SetSSAO(std::unique_ptr<SSAO> ssao);
};