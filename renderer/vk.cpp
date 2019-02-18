#include "vk.h"

using namespace trigger::renderer;

void vk::init()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API , GLFW_NO_API);
    GLFWwindow* window = glfwCreateWindow(800, 600, "Vulkan", nullptr, nullptr);
    uint extension_count = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);
    std::cout << extension_count << std::endl;
    glm::mat4 mat;
    glm::vec4 vec;
    auto test = mat * vec;
    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return;
}

void vk::set_up()
{

}

void vk::resize()
{

}

int vk::rendering()
{
    return 0;
}