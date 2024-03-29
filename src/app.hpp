#ifndef _HB_APP
#define _HB_APP

#include <array>
#include <cstdint>
#include <vector>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace HB {

struct AppInfo {
    uint32_t width;
    uint32_t height;
    char const* name;
    char const* version;
};

class App {
public:
    void run();
    App(AppInfo);
    ~App();

private:
    static uint32_t const MAX_FRAMES_IN_FLIGHT = 2;
    std::vector<char const*> const m_validation_layers = {
        "VK_LAYER_KHRONOS_validation"
    };
#ifdef NDEBUG
    bool const m_enable_validation_layers = false;
    static VkDebugUtilsMessageSeverityFlagBitsEXT const LOG_LEVEL = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
#else
    bool const m_enable_validation_layers = true;
    static VkDebugUtilsMessageSeverityFlagBitsEXT const LOG_LEVEL = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
#endif
    std::vector<char const*> m_instance_extensions = {};
    std::vector<char const*> const m_device_extensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };
    AppInfo m_app_info;
    GLFWwindow* m_window;
    VkInstance m_instance;
    VkDebugUtilsMessengerEXT m_debug_messenger;
    VkSurfaceKHR m_surface;
    VkPhysicalDevice m_physical_device = VK_NULL_HANDLE;
    VkDevice m_device;
    VkQueue m_graphics_queue;
    VkQueue m_present_queue;
    VkSwapchainKHR m_swap_chain;
    std::vector<VkImage> m_swap_chain_images;
    VkFormat m_swap_chain_image_format;
    VkExtent2D m_swap_chain_extent;
    std::vector<VkImageView> m_swap_chain_image_views;
    VkPipelineLayout m_pipeline_layout;
    VkRenderPass m_render_pass;
    VkPipeline m_graphics_pipeline;
    std::vector<VkFramebuffer> m_swap_chain_framebuffers;
    VkCommandPool m_command_pool;
    // https://vulkan-tutorial.com/Drawing_a_triangle/Drawing/Frames_in_flight
    // these need to be vectors and be resized in their respective create functions if we want
    // to change max frames in flight (double/triple-buffering) on the fly
    std::array<VkCommandBuffer, MAX_FRAMES_IN_FLIGHT> m_command_buffers;
    std::array<VkSemaphore, MAX_FRAMES_IN_FLIGHT> m_image_available_semaphores;
    std::array<VkSemaphore, MAX_FRAMES_IN_FLIGHT> m_render_finished_semaphores;
    std::array<VkFence, MAX_FRAMES_IN_FLIGHT> m_in_flight_fences;
    bool m_framebuffer_resized = false;
    uint32_t m_current_frame = 0;
    VkBuffer m_vertex_buffer;
    VkDeviceMemory m_vertex_buffer_memory;

    struct QueueFamilyIndices;
    struct SwapChainSupportDetails;

    void destruct_swap_chain();
    bool check_validation_layer_support() const;
    void init_window();
    static void framebuffer_resize_callback(GLFWwindow*, int, int);
    void set_required_instance_extensions();
    bool check_instance_extension_support() const;
    void create_instance();
    static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
        VkDebugUtilsMessageSeverityFlagBitsEXT,
        VkDebugUtilsMessageTypeFlagsEXT,
        VkDebugUtilsMessengerCallbackDataEXT const*,
        void*);
    void populate_debug_messenger_create_info(VkDebugUtilsMessengerCreateInfoEXT&);
    void setup_debug_messenger();
    void create_surface();
    void init_vulkan();
    void pick_physical_device();
    QueueFamilyIndices find_queue_families(VkPhysicalDevice const&) const;
    bool check_device_extension_support(VkPhysicalDevice const&) const;
    SwapChainSupportDetails query_swap_chain_support(VkPhysicalDevice const&) const;
    VkSurfaceFormatKHR choose_swap_surface_format(std::vector<VkSurfaceFormatKHR> const&) const;
    VkPresentModeKHR choose_swap_present_mode(std::vector<VkPresentModeKHR> const&) const;
    VkExtent2D choose_swap_extent(VkSurfaceCapabilitiesKHR const&) const;
    bool is_device_suitable(VkPhysicalDevice const&) const;
    void create_logical_device();
    void create_swap_chain();
    void create_image_views();
    VkShaderModule create_shader_module(std::vector<char> const&) const;
    void create_render_pass();
    void create_graphics_pipeline();
    void create_framebuffers();
    void recreate_swap_chain();
    void create_command_pool();
    uint32_t find_memory_type(uint32_t const, VkMemoryPropertyFlags const&) const;
    void create_vertex_buffer();
    void create_command_buffers();
    void record_command_buffer(VkCommandBuffer, uint32_t const);
    void create_sync_objects();
    void draw_frame();
    void loop();
};

}

#endif
