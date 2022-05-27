#pragma once

#include "../Core.h"
#include "../../Window/Window.h"

#include "Utils/DebugUtilsMessenger.h"
#include "Utils/Extensions.h"
#include "Utils/QueueFamilyIndices.h"
#include "Utils/PhysicalDevice.h"
#include "Utils/SwapChainSupportDetails.h"

// std lib headers
#include <array>

#if ENABLE_VALIDATION_LAYERS == 1
	#define VALIDATION_LAYERS_ENABLED true
#else
    #define VALIDATION_LAYERS_ENABLED false
#endif

namespace SnekVk {

	class VulkanDevice {
		/**
		 * Tracks whether Vulkan is using validation layers, and whether we should include
		 * them on instantiation. Vulkan does not do any error validation by default, and
		 * will typically fail silently if no validation layers are active. We must enable
		 * validation layers manualy if we wish to have any errors or suggestions enabled.  
		 **/
		static const bool enableValidationLayers = VALIDATION_LAYERS_ENABLED;

		public:
		
		// 'Structors

		VulkanDevice(SnekVk::Window *window);
		VulkanDevice();

		~VulkanDevice();

		// Deleting move and copy constructors
		VulkanDevice(const VulkanDevice &) = delete;
		VulkanDevice& operator=(const VulkanDevice &) = delete;
		VulkanDevice(VulkanDevice &&) = delete;
		VulkanDevice& operator=(VulkanDevice &&) = delete;

		static VulkanDevice* GetDeviceInstance() { return vulkanDeviceInstance; }

		void SetWindow(Window* window);
		
		/**
		 * Returns a copy of the command pool held by the device. 
		 * Command pools are objects that command buffer memory is typically allocated from. 
		 * Ensures that memory is not dynamically allocated every time we need a new command buffer. 
		 **/
		VkCommandPool GetCommandPool() { return commandPool; }

		/**
		 * Returns a copy of the logical device held by the VulkanDevice. 
		 * Logical devices represent an interface to a piece of hardware (typically the GPU).
		 * Logical devices allow us to allocate memory and transition memory between the GPU 
		 * and CPU.
		 **/
		VkDevice Device() { return device; }

		/**
		 * Returns a copy of the window surface held by the VulkanDevice. 
		 * A window surface represents the actual window we wish to draw to. Vulkan can't 
		 * draw anything to a screen without an actual window to send vertex data to. 
		 **/
		VkSurfaceKHR Surface() { return surface; }

		/**
		 * Returns a copy of the graphics queue held by the VulkanDevice. 
		 * A graphics queue is a queue where all command buffers which require a rendering step 
		 * are initially sent. This queue takes a command buffer and reads the rendering data 
		 * stored within. There can be multiple graphics queue (typically for multi-threaded)
		 * draw calls. 
		 **/
		VkQueue GraphicsQueue() { return graphicsQueue; }

		/**
		 * Returns a copy of the present queue held by the VulkanDevice. 
		 * A present queue takes rendering data and provides it to the window surface. Present 
		 * queues are needed if data is to actually make it to the window at all.  
		 **/
		VkQueue PresentQueue() { return presentQueue; }

		size_t GetDeviceAlignment() { return properties.limits.minUniformBufferOffsetAlignment; }

		/**
		 * Returns a struct containing all relevant swapChain support information. 
		 **/
		SwapChainSupportDetails::SwapChainSupportDetails GetSwapChainSupport() { return SwapChainSupportDetails::QuerySupport(physicalDevice, surface); }
		
		/**
		 * Returns a bitmask value representing the memory typre required to allocate GPU memory. 
		 * Different rendering operations require different memory types. These are typically
		 * differentiated using a bitmask. This functions throws an exception if the required 
		 * memory type is unavailable.
		 * @param typeFilter - The bitmask for the required memory type
		 * @param properties - The available device memory properties. Used to query for available memory. 
		 **/
		uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

		/**
		 * Returns a struct containing the indices for our graphics and present queues. 
		 * Queues are stored in an array, each of which are differentiated by a specific index. 
		 * These indices act as identifiers which allow Vulkan to know which queue to submit something to.
		 **/
		QueueFamilyIndices::QueueFamilyIndices FindPhysicalQueueFamilies() { return QueueFamilyIndices::FindQueueFamilies(physicalDevice, surface); }

		/**
		 * Returns a valid Vulkan image format from a list of formats. Returns the first format that's 
		 * found to be valid.
		 * @param candidates - an array of acceptable formats.
		 * @param formatCount - the number of elements in the candidates array. 
		 * @param tiling - the ideal image tiling required for the given format. 
		 * @param features - a bitmask of required format features. Each format has different features associated
		 * 					with it. This ensures that we only get the features we want. 
		 **/
		VkFormat FindSupportedFormat(
			const VkFormat* candidates, size_t formatCount, VkImageTiling tiling, VkFormatFeatureFlags features);

		// Buffer Helper Functions

		/** 
		 * Prepares a command buffer for writing.
		 * @returns - a command buffer that's been activated.
		 **/
		VkCommandBuffer BeginSingleTimeCommands();

		/** 
		 * Stops a command buffer from beign written to (after having been written to already).
		 * Submits the resulting command buffer to the graphics queue after all writing is 
		 * complete.
		 **/
		void EndSingleTimeCommands(VkCommandBuffer commandBuffer);

		/**
		 * Copies a buffer. This is generally used when copying the values within a buffer into
		 * another (such as when setting up a staging buffer between the CPU and GPU).
		 * 
		 * @param srcBuffer - the buffer being copied.
		 * @param dstBuffer - the buffer where the data is being copied into.
		 * @param size - the expected size of the resulting buffer.
		 **/
		void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

		/**
		 *  Copies the a buffer into an image (useful for textures).
		 * 
		 * 	@param buffer - the buffer being copied over
		 *  @param image - the image housing the resulting data
		 *  @param width - the size of the image
		 *  @param height - the height of the image
		 *  @param layerCount - the number of layers contained within the image (if any).
		 **/
		void CopyBufferToImage(
			VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount);

		/**
		 * Creates an image with memory allocation information
		 * 
		 * @param imageInfo - the creation info for the image.
		 * @param properties - the required memory properties.
		 * @param image - the image where memory will be allocated to.
		 * @param imageMemory - the image memory being allocated to.
		 **/
		void CreateImageWithInfo(
			const VkImageCreateInfo &imageInfo,
			VkMemoryPropertyFlags properties,
			VkImage &image,
			VkDeviceMemory &imageMemory);

		/**
		 * A struct containing all our GPU information (such as it's name) 
		 **/
		VkPhysicalDeviceProperties properties;

		private:
		/**
		 * Instantiates a Vulkan instance for the use of this renderer. 
		 * 
		 * A Vulkan instance pulls in relevant Vulkan code and synchronises that code with 
		 * the GPU for usage. 
		 * This function specifically runs the following operations:
		 * 1) Checks if validation layers are enabled. Ensures that validation layers are 
		 * 	  available for usage if any are specified.
		 * 2) Checks if all required extensions are available (including those from glfw).
		 * 3) Populates a debug callback for validation layer reporting (if enabled)
		 * 4) Creates a Vulkan instance and stores it in the 'instance' variable. 
		 **/
		void CreateInstance();

		/**
		 * Instantiates a DebugUtilsMessenger struct and stores it in the 'debugMessenger'
		 * instance variable. 
		 * 
		 * Since Vulkan does not automatically handle errors, it's necessary to manually
		 * configure it to do so if needed. A DebugUtilsMessenger stores a callback which
		 * which is then called when an error or warning is issued.
		 **/
		void SetupDebugMessenger();

		/**
		 * Queries the window for a vulkan-ready surface for rendering. The result is stored
		 * in the 'surface' instane variable. 
		 * 
		 * Vulkan is not always guaranteed to be compatible with every window system. 
		 * For example, glfw must be configured in a way that a surface is returned
		 * that vulkan can actually render to.
		 **/
		void CreateSurface();

		/** 
		 * Searches for and configures a physical device to be used for rendering. A 
		 * physical device refers to a piece of hardware (a GPU). This is then stored 
		 * in the 'physicalDevice' instance variable.
		 * 
		 * This function searches over all available devices, iterates over them, and 
		 * consequently returns the one that matches our criteria.
		 * The device in question MUST support both graphics and present queues, must 
		 * have our required present modes and formats, and finally must support the 
		 * 'samplerAnistropy' feature.
		 **/
		void PickPhysicalDevice();

		/** 
		 * Creates a logical device struct and stores it in the 'device' instance variable. 
		 * It also extracts the 'graphicsQueue' and 'presentQueue' Vulkan structs.
		 * 
		 * This function will create the relevant queues for rendering, create a logical 
		 * device using our physical device, and then extract the graphics and present queues.
		 **/
		void CreateLogicalDevice();

		/** 
		 * Creates a command pool for storing allocating command buffer memory without dynamic
		 * allocation. Stores the result in the 'commandPool' instance variable.
		 * 
		 * The command pool in question is build specifically for graphics operation and 
		 * therefore uses the graphics queue for allocation. Specifies that all command
		 * buffers allocated by this pool are short lived. Also specifies that all 
		 * buffers allocated by the pool can be reset. 
		 **/
		void CreateCommandPool();

		static void SetVulkanDeviceInstance(VulkanDevice* device) { vulkanDeviceInstance = device; }

		static VulkanDevice* vulkanDeviceInstance;

		VkInstance instance;
		VkDebugUtilsMessengerEXT debugMessenger;

		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

		Window* window {nullptr};
		VkCommandPool commandPool;

		VkDevice device;
		VkSurfaceKHR surface;
		VkQueue graphicsQueue;
		VkQueue presentQueue;

		/**
		 * An array storing all required validation layers (if enabled).
		 **/
		const std::array<const char*, 1> validationLayers = { "VK_LAYER_KHRONOS_validation" };

		/**
		 * An array storing all required extensions. All of these must be present for the renderer to start.
		 **/
		const std::array<const char*, 1> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	};

}  // namespace lve
