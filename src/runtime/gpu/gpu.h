// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/shared.h"

// Begin the gpu namespace
#define GJ_GPU_NAMESPACE_BEGIN                                                                                         \
	GJ_NAMESPACE_BEGIN                                                                                                 \
	namespace gpu {

// End the gpu namespace
#define GJ_GPU_NAMESPACE_END                                                                                           \
	}                                                                                                                  \
	GJ_NAMESPACE_END

#define GJ_GPU_DEBUG 1

GJ_GPU_NAMESPACE_BEGIN

template <typename T>
using Shared = AtomicShared<T>;

template <typename T>
using SharedFromThis = AtomicSharedFromThis<T>;

// Forward declarations
class IBuffer;
class IDevice;
class IGraphicsCommandList;
class IGraphicsCommandRecorder;
class IGraphicsPipeline;
class IPixelShader;
class IRenderPassCommandRecorder;
class ISwapchain;
class ITexture;
class IVertexShader;

struct GraphicsPipelineDefinition;
struct InputParameter;

enum class BlendFactor : u8;
enum class BlendOp : u8;
enum class BufferUsage : u8;
enum class CompareOp : u8;
enum class CullMode : u8;
enum class DrawMode : u8;
enum class Format : u16;
enum class Heap : u8;
enum class Layout : u8;
enum class ShaderPrimitive : u8;
enum class TextureUsage : u8;

GJ_GPU_NAMESPACE_END
