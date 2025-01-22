/**************************************************************************/
/**************************************************************************/
/* vulkan_ghi.hpp                                                         */
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

#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include "ghi_interface.hpp"
#include "vulkan-ghi_api.hpp"

#ifndef KN_VULKAN_DEBUG
#define KN_VULKAN_DEBUG 0
#endif

namespace kn {

/**
 * @brief Checks if the GHI is supported on the current platform.
 */
extern "C" KN_VULKAN_API bool is_supported();

/**
 * @brief Creates a new GHI instance.
 */
extern "C" KN_VULKAN_API IGHI* create_ghi();

class VulkanGHI : public IGHI {
 public:
  KN_VULKAN_API bool initialize(const GHIDesc* desc) override;
  KN_VULKAN_API void shutdown() override;

 private:
  bool create_instance(const GHIDesc* desc);
  bool check_validation_layer_support();
  bool setup_debug_messenger(const GHIDesc* desc);
  bool setup_physical_device(const GHIDesc* desc);
  bool setup_logical_device(const GHIDesc* desc);
  bool setup_command_pool(const GHIDesc* desc);

 private:
  VkInstance _instance{VK_NULL_HANDLE};
  VkDebugUtilsMessengerEXT _debugMessenger{VK_NULL_HANDLE};
  VkPhysicalDevice _physicalDevice{VK_NULL_HANDLE};
  VkSampleCountFlagBits _msaaSamples{VK_SAMPLE_COUNT_1_BIT};
  VkDevice _device{VK_NULL_HANDLE};

  VkQueue _graphicsQueue{VK_NULL_HANDLE};
  VkQueue _computeQueue{VK_NULL_HANDLE};

  int32_t _graphicsQueueFamilyIndex;
  VkCommandPool _commandPool{VK_NULL_HANDLE};
};

}  // namespace kn
