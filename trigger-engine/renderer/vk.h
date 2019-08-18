#pragma once
#include "renderer.h"


#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN

#define VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../lib/vk/include/vulkan/vulkan.h"
#include <imgui.h>
#include "../core/imgui/imgui_impl_glfw.h"
#include "../core/imgui/imgui_impl_vulkan.h"
#include <stdio.h>
#include <stdlib.h>
#include <list>

#include <stdexcept>
#include <optional>
#include <set>


static int WIDTH = 800;
static int HEIGHT = 600;

//that layers is need add more sub-party
const static std::vector<const char*> validation_layers = 
{ 
	"VK_LAYER_KHRONOS_validation",
};

const std::vector<const char*> deviceExtensions = 
{
	VK_KHR_SWAPCHAIN_EXTENSION_NAME,
};

#ifdef NDEBUG
const static bool enable_validation_layer = false;
#else
const static bool enable_validation_layer = true;
#endif

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

//#define IMGUI_UNLIMITED_FRAME_RATE
#ifdef _DEBUG
#define IMGUI_VULKAN_DEBUG_REPORT
#endif

#ifdef IMGUI_VULKAN_DEBUG_REPORT
static VKAPI_ATTR VkBool32 VKAPI_CALL debug_report(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage, void* pUserData)
{
    (void)flags; (void)object; (void)location; (void)messageCode; (void)pUserData; (void)pLayerPrefix; // Unused arguments
    fprintf(stderr, "[vulkan] ObjectType: %i\nMessage: %s\n\n", objectType, pMessage);
    return VK_FALSE;
}
#endif // IMGUI_VULKAN_DEBUG_REPORT

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
	std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

	return VK_FALSE;
}



static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

namespace trigger 
{
    namespace rend
    {
		struct QueueFamilyIndices 
		{
			std::optional<uint32_t> graphicsFamily;
			std::optional<uint32_t> presentFamily;

			bool is_complete() 
			{
				return graphicsFamily.has_value() && presentFamily.has_value();
			}
		};

		struct SwapChainSupportDetails 
		{
			VkSurfaceCapabilitiesKHR capabilities;
			std::vector<VkSurfaceFormatKHR> formats;
			std::vector<VkPresentModeKHR> presentModes;
		};


        class vk : public trigger::rend::renderer
        {
        private:
            virtual int init() override;
            virtual void set_up() override;

            virtual void draw() override;
            virtual int rendering() override;
            void draw_editors(VkDevice device, ImGui_ImplVulkanH_Window *wd);

			//Vulkan Func
			VkResult create_debug_util_messenger_EXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* info, const VkAllocationCallbacks* allocator, VkDebugUtilsMessengerEXT* messenger)
			{
				auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
				if (func != nullptr)
				{
					return func(instance, info, allocator, messenger);
				}
				else
				{
					return VK_ERROR_EXTENSION_NOT_PRESENT;
				}
			}

			void destroy_debug_messenger_EXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
			{
				auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
				if (func != nullptr) 
				{
					func(instance, debugMessenger, pAllocator);
				}
			}

			void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
			void setupDebugMessenger();
			void createSurface();
			void pickPhysicalDevice();
			void createLogicalDevice();
			bool isDeviceSuitable(VkPhysicalDevice device);

			void createCommandPool();
			QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
			std::vector<const char*> getRequiredExtensions();
			bool checkValidationLayerSupport();
			void createSemaphores();
			void createSyncObjects();
			bool checkDeviceExtensionSupport(VkPhysicalDevice device);
			SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
			VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
			VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
			VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
			static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

        public:
			const int MAX_FRAMES_IN_FLIGHT = 2;
			static std::map < std::string, VkPipelineShaderStageCreateInfo > SHADER_STAGES;
			GLFWwindow* window;
			VkInstance instance;
			VkDebugUtilsMessengerEXT debugMessenger;
			VkSurfaceKHR surface;

			VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
			VkDevice device;

			VkQueue graphicsQueue;
			VkQueue presentQueue;
			VkQueue imguiQueue;
			VkSwapchainKHR swapChain = VK_NULL_HANDLE;
			std::vector<VkImage> swapChainImages;
			VkFormat swapChainImageFormat;
			VkExtent2D swapChainExtent;
			VkDescriptorSetLayout descriptorSetLayout;
			VkPipelineLayout pipelineLayout;
			VkRenderPass renderPass;
			VkPipeline graphicsPipeline;
			VkCommandPool commandPool;
			VkDescriptorPool descriptorPool;
			std::vector<VkDescriptorSet> descriptorSets;

			std::vector<VkImageView> swapChainImageViews;
			std::vector<VkFramebuffer> swapChainFramebuffers;
			std::vector<VkCommandBuffer> commandBuffers;

			std::vector<VkSemaphore> imageAvailableSemaphores;
			std::vector<VkSemaphore> renderFinishedSemaphores;
			size_t currentFrame = 0;
			std::vector<VkFence> inFlightFences;
			bool framebufferResized = false;

			std::vector<VkBuffer> vertexBuffers;
			std::vector<VkDeviceMemory> vertexBufferMemorys;
			std::vector<VkBuffer> indexBuffers;
			std::vector<VkDeviceMemory> indexBufferMemorys;

			std::vector<VkBuffer> uniformBuffers;
			std::vector<VkDeviceMemory> uniformBuffersMemory;

			//mesh render
			std::map<std::string, mesh*> mesh_map;
			std::map<hash_id, ::renderer*> mesh_renderers;
			//Texture
			VkImage textureImage;
			VkDeviceMemory textureImageMemory;
			VkImageView textureImageView;
			VkSampler textureSampler;
			//depth
			VkImage depthImage;
			VkDeviceMemory depthImageMemory;
			VkImageView depthImageView;
			//mipmap
			uint32_t mipLevels;

			//msaa
			VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;
			VkImage colorImage;
			VkDeviceMemory colorImageMemory;
			VkImageView colorImageView;

			//Camera
			vec3 cam_pos{2.0f, 2.0f, 2.0f}; //w,a,s,d
			vec3 cam_dir; // roll, pitch, yam
			float horizontalAngle = 3.14f;
			float verticalAngle = 0.0f;
			float speed = 0.5f; // every 3 unitsl
			float mouseSpeed = 0.1f;
			glm::vec3 up{0.0f, 0.0f, 1.0f};
			glm::vec3 right;
			glm::vec3 direction{ 0.0f, 0.0f, 0.0f };


			std::vector<vertex> vertices;
			std::vector<uint32_t> indices;

			void cleanupSwapChain();
			void createSwapChain();
			void createImageViews();
			void createRenderPass();
			void createGraphicsPipeline();
			void createFramebuffers();
			void createCommandBuffers();
			void createVertexBuffer(const std::vector<vertex> vertices);
			void createIndexBuffer(const std::vector<uint32_t> indices);
			void createDescriptorSetLayout();
			void createUniformBuffers();
			void updateUniformBuffer(uint32_t currentImage);
			void createDescriptorPool();
			void createDescriptorSets();
			void createTextureImageView();
			VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);
			void createTextureSampler();
			void createDepthResources();
			VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
			VkFormat findDepthFormat();
			void createImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
			mesh loadModel(trigger::core::file file);
			void loadModel();
			void createTextureImage();
			bool hasStencilComponent(VkFormat format);
			void generateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);
			void createColorResources();
			VkSampleCountFlagBits getMaxUsableSampleCount();

			VkCommandBuffer beginSingleTimeCommands();
			void endSingleTimeCommands(VkCommandBuffer commandBuffer);
			void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
			void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

			void add_mesh(std::string name,  mesh *data);
			void add_mesh(std::string name, trigger::core::file file);


			void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
			void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
			uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
			void vk_clean();

			virtual void resize() override;

			vk(int w, int h, bool edit, trigger::core::engine* engine) : renderer(w, h, edit, engine)
			{
				this->init();
			}

			virtual ~vk()
			{
				cleanupSwapChain();


				for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
					vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
					vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
					vkDestroyFence(device, inFlightFences[i], nullptr);
				}

				vkDestroyCommandPool(device, commandPool, nullptr);
				for (auto framebuffer : swapChainFramebuffers) 
				{
					vkDestroyFramebuffer(device, framebuffer, nullptr);
				}

				vkDestroyPipeline(device, graphicsPipeline, nullptr);
				vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
				vkDestroyRenderPass(device, renderPass, nullptr);
				for (auto imageView : swapChainImageViews)
				{
					vkDestroyImageView(device, imageView, nullptr);
				}
				vkDestroySwapchainKHR(device, swapChain, nullptr);
				vkDestroyDevice(device, nullptr);

				if (enable_validation_layer) 
				{
					destroy_debug_messenger_EXT(instance, debugMessenger, nullptr);
				}

				vkDestroySurfaceKHR(instance, surface, nullptr);
				vkDestroyInstance(instance, nullptr);

				glfwDestroyWindow(window);

				glfwTerminate();
			}

        };
    }
}


