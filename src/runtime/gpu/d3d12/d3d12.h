// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "gpu/gpu.h"

SF_SUPPRESS_WARNING_PUSH
SF_SUPPRESS_WARNINGS
SF_MSVC_SUPPRESS_WARNING(4820)
SF_MSVC_SUPPRESS_WARNING(4265)
SF_MSVC_SUPPRESS_WARNING(4365)
SF_MSVC_SUPPRESS_WARNING(4514)
SF_MSVC_SUPPRESS_WARNING(5264)
SF_MSVC_SUPPRESS_WARNING(5204)
SF_MSVC_SUPPRESS_WARNING(5220)
SF_MSVC_SUPPRESS_WARNING(5220)
SF_MSVC_SUPPRESS_WARNING(4986)

SF_MSVC_SUPPRESS_WARNING(5039) // 10.0.19041.0\um\winbase.h(7679)
SF_MSVC_SUPPRESS_WARNING(4668)

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

SF_SUPPRESS_WARNING_POP

SF_GPU_NAMESPACE_BEGIN

// From DXSampleHelper.h
// Source: https://github.com/Microsoft/DirectX-Graphics-Samples
SF_ALWAYS_INLINE void throw_if_failed(HRESULT hr) {
	if (FAILED(hr)) {
		SF_PANIC("D3D12 Crash");
	}
}

SF_GPU_NAMESPACE_END
