/**************************************************************************/
/* vulkan_ghi.cpp                                                         */
/**************************************************************************/
/*                         This file is part of:                          */
/*                                Knoodle                                 */
/*                        https://knoodlegraph.org                        */
/**************************************************************************/
/* Copyright (c) 2025 Knoodle contributors (vide AUTHORS.md)              */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#include "vulkan_ghi/vulkan_ghi.hpp"
#include "ghi/ghi_manager.hpp"
#include "log/log.hpp"
#include "os/os.hpp"

#include <array>
#include <cassert>
#include <set>

namespace kn {
constexpr std::array<const char*, 1> validation_layers{"VK_LAYER_KHRONOS_validation"};

std::vector<const char*> get_required_extensions(const GHIDesc* desc) {
  std::vector<const char*> extensions;
  if (desc->enable_validation_layers) {
    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  }
  return extensions;
}

VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback([[maybe_unused]] VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                              [[maybe_unused]] VkDebugUtilsMessageTypeFlagsEXT messageType,
                                              const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                              [[maybe_unused]] void* pUserData) {
  if (pCallbackData) {
    KN_LOG(LogVulkan, Error, pCallbackData->pMessage);
  }
  return VK_FALSE;
}

bool is_supported() {
  KN_LOG(LogVulkan, Info, "Checking Vulkan GHI support");
  auto vulkan_lib = std::unique_ptr<os::Library>(os::load_library("vulkan-1"));
  if (!vulkan_lib->is_loaded()) {
    return false;
  }

  auto _vkCreateInstance = vulkan_lib->get_function<PFN_vkCreateInstance>("vkCreateInstance");
  auto _vkDestroyInstance = vulkan_lib->get_function<PFN_vkDestroyInstance>("vkDestroyInstance");
  if (!_vkCreateInstance || !_vkDestroyInstance) {
    return false;
  }

  VkInstanceCreateInfo create_info = {};
  create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  create_info.pApplicationInfo = nullptr;
  create_info.enabledLayerCount = static_cast<uint32_t>(validation_layers.size());
  create_info.ppEnabledLayerNames = validation_layers.data();

  VkInstance instance = VK_NULL_HANDLE;
  if (_vkCreateInstance(&create_info, nullptr, &instance) != VK_SUCCESS) {
    return false;
  }

  _vkDestroyInstance(instance, nullptr);

  return true;
}

IGHI* create_ghi() {
  return new VulkanGHI;
}

bool VulkanGHI::initialize(const GHIDesc* desc) {
  assert(desc);

  KN_LOG(LogVulkan, Info, "Initializing Vulkan GHI");

  if (!create_instance(desc))
    return false;

  if (!setup_debug_messenger(desc))
    return false;

  if (!setup_physical_device(desc))
    return false;

  return true;
}

void VulkanGHI::shutdown() {}

bool VulkanGHI::create_instance(const GHIDesc* desc) {
  KN_LOG(LogVulkan, Info, "Creating Vulkan instance");

  if (desc->enable_validation_layers && !check_validation_layer_support()) {
    KN_LOG(LogVulkan, Error, "Validation layers requested, but not available!");
    return false;
  }

  VkApplicationInfo app_info{};
  app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  app_info.pApplicationName = "Knoodle";
  app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  app_info.pEngineName = "Knoodle";
  app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  app_info.apiVersion = VK_API_VERSION_1_0;

  VkInstanceCreateInfo create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  create_info.pApplicationInfo = &app_info;

  auto extensions = get_required_extensions(desc);
  create_info.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
  create_info.ppEnabledExtensionNames = extensions.data();

  VkDebugUtilsMessengerCreateInfoEXT debug_create_info{};
  if (desc->enable_validation_layers) {
    create_info.enabledLayerCount = static_cast<uint32_t>(validation_layers.size());
    create_info.ppEnabledLayerNames = validation_layers.data();
    debug_create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debug_create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    debug_create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                                    VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                    VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    debug_create_info.pfnUserCallback = debug_callback;
    create_info.pNext = &debug_create_info;
  } else {
    create_info.enabledLayerCount = 0;
    create_info.ppEnabledLayerNames = nullptr;
  }

  if (VkResult result = vkCreateInstance(&create_info, nullptr, &_instance); result != VK_SUCCESS) {
    KN_LOG(LogVulkan, Error, "Failed to create Vulkan instance. Error: {}", result);
    return false;
  }

  return true;
}

bool VulkanGHI::check_validation_layer_support() {
  uint32_t layer_count;
  vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

  std::vector<VkLayerProperties> available_layers(layer_count);
  vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

  for (const char* layer_name : validation_layers) {
    bool layer_found = false;
    for (const auto& layer_properties : available_layers) {
      if (strcmp(layer_name, layer_properties.layerName) == 0) {
        layer_found = true;
        break;
      }
    }
    if (!layer_found) {
      return false;
    }
  }

  return true;
}

bool VulkanGHI::setup_debug_messenger(const GHIDesc* desc) {
  if (!desc->enable_validation_layers)
    return true;

  VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  createInfo.pfnUserCallback = debug_callback;

  auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(_instance, "vkCreateDebugUtilsMessengerEXT");
  if (func != nullptr) {
    return func(_instance, &createInfo, nullptr, &_debugMessenger) == VK_SUCCESS;
  }

  return false;
}

bool VulkanGHI::setup_physical_device(const GHIDesc*) {
  uint32_t device_count = 0;
  vkEnumeratePhysicalDevices(_instance, &device_count, nullptr);

  if (device_count == 0) {
    KN_LOG(LogVulkan, Error, "Failed to find GPUs with Vulkan support");
    return false;
  }

  std::vector<VkPhysicalDevice> devices(device_count);
  vkEnumeratePhysicalDevices(_instance, &device_count, devices.data());

  for (const auto& device : devices) {
    VkPhysicalDeviceProperties device_properties;
    vkGetPhysicalDeviceProperties(device, &device_properties);
    if (device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
      _physicalDevice = device;
      break;
    }
  }

  if (_physicalDevice == VK_NULL_HANDLE) {
    KN_LOG(LogVulkan, Error, "Failed to find a suitable GPU");
    return false;
  }

  KN_LOG(LogVulkan, Info, "Found a suitable GPU. Device: {}", device_properties.deviceName);

  return true;
}

bool VulkanGHI::setup_logical_device(const GHIDesc* desc) {
  // Find queue families
  uint32_t queue_family_count = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(_physicalDevice, &queue_family_count, nullptr);

  std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
  vkGetPhysicalDeviceQueueFamilyProperties(_physicalDevice, &queue_family_count, queue_families.data());

  _graphicsQueueFamilyIndex = -1;
  int32_t compute_family_index = -1;
  for (uint32_t i = 0; i < queue_family_count; ++i) {
    if (queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      _graphicsQueueFamilyIndex = i;
    }
    if (queue_families[i].queueFlags & VK_QUEUE_COMPUTE_BIT) {
      compute_family_index = i;
    }
  }

  if (_graphicsQueueFamilyIndex == -1 || compute_family_index == -1) {
    KN_LOG(LogVulkan, Error, "Failed to find queue families");
    return false;
  }

  // Create logical device
  std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
  std::set<int32_t> unique_queue_families = {_graphicsQueueFamilyIndex, compute_family_index};
  float queue_priority = 1.0f;

  for (int32_t queue_family : unique_queue_families) {
    VkDeviceQueueCreateInfo queue_create_info{};
    queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_info.queueFamilyIndex = queue_family;
    queue_create_info.queueCount = 1;
    queue_create_info.pQueuePriorities = &queue_priority;
    queue_create_infos.push_back(queue_create_info);
  }

  VkPhysicalDeviceFeatures device_features{};

  if (desc->enable_sample_anisotropy)
    device_features.samplerAnisotropy = VK_TRUE;

  if (desc->use_shader_float_64)
    device_features.shaderFloat64 = VK_TRUE;

  VkDeviceCreateInfo create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  create_info.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size());
  create_info.pQueueCreateInfos = queue_create_infos.data();
  create_info.pEnabledFeatures = &device_features;
  create_info.enabledExtensionCount = 0;
  create_info.ppEnabledExtensionNames = nullptr;
  create_info.enabledLayerCount = static_cast<uint32_t>(validation_layers.size());
  create_info.ppEnabledLayerNames = validation_layers.data();

  if (vkCreateDevice(_physicalDevice, &create_info, nullptr, &_device) != VK_SUCCESS) {
    KN_LOG(LogVulkan, Error, "Failed to create logical device");
    return false;
  }

  vkGetDeviceQueue(_device, _graphicsQueueFamilyIndex, 0, &_graphicsQueue);
  vkGetDeviceQueue(_device, compute_family_index, 0, &_computeQueue);

  return true;
}

bool VulkanGHI::setup_command_pool(const GHIDesc*) {
  VkCommandPoolCreateInfo pool_info{};
  pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  pool_info.queueFamilyIndex = _graphicsQueueFamilyIndex;
  pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
  if (vkCreateCommandPool(_device, &pool_info, nullptr, &_commandPool) != VK_SUCCESS) {
    KN_LOG(LogVulkan, Error, "Failed to create command pool");
    return false;
  }
  return true;
}

bool VulkanGHI::setup_command_buffer(const GHIDesc*) {
  _command_buffers.resize(1);

  VkCommandBufferAllocateInfo alloc_info{};
  alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  alloc_info.commandPool = _commandPool;
  alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  alloc_info.commandBufferCount = static_cast<uint32_t>(_command_buffers.size());

  if (vkAllocateCommandBuffers(_device, &alloc_info, _command_buffers.data()) != VK_SUCCESS) {
    KN_LOG(LogVulkan, Error, "Failed to allocate command buffers");
    return false;
  }

  return true;
}

}  // namespace kn
