// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/os/windows.h"
#include "gpu/gpu.h"

GJ_SUPPRESS_WARNING_PUSH
GJ_SUPPRESS_WARNINGS
GJ_MSVC_SUPPRESS_WARNING(4820)
GJ_MSVC_SUPPRESS_WARNING(4265)
GJ_MSVC_SUPPRESS_WARNING(4365)
GJ_MSVC_SUPPRESS_WARNING(4514)
GJ_MSVC_SUPPRESS_WARNING(5264)
GJ_MSVC_SUPPRESS_WARNING(5204)
GJ_MSVC_SUPPRESS_WARNING(5220)
GJ_MSVC_SUPPRESS_WARNING(5220)
GJ_MSVC_SUPPRESS_WARNING(4986)

// Windows Runtime Library. Needed for Microsoft::WRL::ComPtr<> template class.
#include <wrl.h>
using Microsoft::WRL::ComPtr;

#include <d3d12.h>
#include <d3d12sdklayers.h>
#include <d3dcompiler.h>
#include <dxgi1_6.h>
#undef interface

GJ_SUPPRESS_WARNING_POP

GJ_GPU_NAMESPACE_BEGIN

// From DXSampleHelper.h
// Source: https://github.com/Microsoft/DirectX-Graphics-Samples
GJ_ALWAYS_INLINE void throw_if_failed(HRESULT hr) {
	if (FAILED(hr)) {
		GJ_ASSERT(false, "D3D12 Crash");
	}
}

GJ_GPU_NAMESPACE_END