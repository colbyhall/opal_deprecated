// Copyright Colby Hall. All Rights Reserved.

#pragma once

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

GJ_MSVC_SUPPRESS_WARNING(5039) // 10.0.19041.0\um\winbase.h(7679)
GJ_MSVC_SUPPRESS_WARNING(4668)

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
#undef interface
#undef min
#undef max

GJ_SUPPRESS_WARNING_POP

GJ_GPU_NAMESPACE_BEGIN

// From DXSampleHelper.h
// Source: https://github.com/Microsoft/DirectX-Graphics-Samples
GJ_ALWAYS_INLINE void throw_if_failed(HRESULT hr) {
	if (FAILED(hr)) {
		GJ_PANIC("D3D12 Crash");
	}
}

GJ_GPU_NAMESPACE_END
