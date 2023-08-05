// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/os/windows.h"
#include "gpu/gpu.h"

EU_SUPPRESS_WARNING_PUSH
EU_SUPPRESS_WARNINGS
EU_MSVC_SUPPRESS_WARNING(4820)
EU_MSVC_SUPPRESS_WARNING(4265)
EU_MSVC_SUPPRESS_WARNING(4365)
EU_MSVC_SUPPRESS_WARNING(4514)
EU_MSVC_SUPPRESS_WARNING(5264)
EU_MSVC_SUPPRESS_WARNING(5204)
EU_MSVC_SUPPRESS_WARNING(5220)
EU_MSVC_SUPPRESS_WARNING(5220)
EU_MSVC_SUPPRESS_WARNING(4986)

// Windows Runtime Library. Needed for Microsoft::WRL::ComPtr<> template class.
#include <wrl.h>
using Microsoft::WRL::ComPtr;

#include <d3d12.h>
#include <d3d12sdklayers.h>
#include <d3dcompiler.h>
#include <dxgi1_6.h>
#undef interface

EU_SUPPRESS_WARNING_POP

EU_GPU_NAMESPACE_BEGIN

// From DXSampleHelper.h
// Source: https://github.com/Microsoft/DirectX-Graphics-Samples
EU_ALWAYS_INLINE void throw_if_failed(HRESULT hr) {
	if (FAILED(hr)) {
		EU_ASSERT(false, "D3D12 Crash");
	}
}

EU_GPU_NAMESPACE_END