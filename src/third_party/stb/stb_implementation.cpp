// Copyright Colby Hall. All Rights Reserved.

#include "stb/stb.h"

OP_SUPPRESS_WARNING_PUSH
OP_STB_SUPRESS_WARNINGS

#define STBI_ASSERT(x) OP_ASSERT(x)
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STBRP_ASSERT(x) OP_ASSERT(x)
#define STB_RECT_PACK_IMPLEMENTATION
#include "stb_rect_pack.h"

#define STBTT_assert(x) OP_ASSERT(x)
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

OP_SUPPRESS_WARNING_POP