// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "gpu/gpu.h"

OP_SUPPRESS_WARNING_PUSH
OP_SUPPRESS_WARNINGS
OP_MSVC_SUPPRESS_WARNING(4820)
OP_MSVC_SUPPRESS_WARNING(4265)
OP_MSVC_SUPPRESS_WARNING(4365)
OP_MSVC_SUPPRESS_WARNING(4514)
OP_MSVC_SUPPRESS_WARNING(5264)
OP_MSVC_SUPPRESS_WARNING(5204)
OP_MSVC_SUPPRESS_WARNING(5220)
OP_MSVC_SUPPRESS_WARNING(5220)
OP_MSVC_SUPPRESS_WARNING(4986)

OP_MSVC_SUPPRESS_WARNING(5039) // 10.0.19041.0\um\winbase.h(7679)
OP_MSVC_SUPPRESS_WARNING(4668)

#define UNICODE
#define WIN32_MEAN_AND_LEAN
#include <Windows.h>

// Windows Runtime Library. Needed for Microsoft::WRL::ComPtr<> template class.
#include <wrl.h>
using Microsoft::WRL::ComPtr;

#include <d3d12.h>
#include <d3d12sdklayers.h>
#include <d3dcompiler.h>
#include <dxgi1_6.h>
#undef min
#undef max

OP_SUPPRESS_WARNING_POP

OP_GPU_NAMESPACE_BEGIN

// From DXSampleHelper.h
// Source: https://github.com/Microsoft/DirectX-Graphics-Samples
OP_ALWAYS_INLINE void throw_if_failed(HRESULT hr) {
	if (FAILED(hr)) {
		OP_PANIC("D3D12 Crash");
	}
}

OP_GPU_NAMESPACE_END
