// Copyright Colby Hall. All Rights Reserved.

#include "core/os/memory.h"
#include "doctest/doctest.h"

OP_TEST_BEGIN
OP_MSVC_SUPPRESS_WARNING(5045)

TEST_CASE("op::core::memory") {
	SUBCASE("op::core::malloc, op::core::realloc, op::core::free, op::core::set") {
		// All allocation functions require the use of the Layout struct. This
		// stores size and alignment requirements
		const auto count = 45;
		const auto layout = core::Layout::array<int>(count);
		CHECK(layout.size == sizeof(int) * count);
		CHECK(layout.alignment == alignof(int));

		// core::malloc is the general purpose allocator of the core library.
		auto original = core::malloc(layout);

		// Malloc does not initialize the memory to any value.
		auto casted = original.as<int>();
		core::set(casted, 0, sizeof(int) * count);
		bool success = true;
		for (usize index = 0; index < count; ++index) {
			if (casted[index] != 0) {
				success = false;
			}
		}
		CHECK(success);

		// Realloc requires the same Layout structure as malloc
		const auto old_count = count;
		const auto old_layout = layout;
		const auto new_count = 75;
		const auto new_layout = core::Layout::array<int>(new_count);
		CHECK(new_layout.size == sizeof(int) * new_count);
		CHECK(new_layout.alignment == alignof(int));
		auto reallocated = core::realloc(original, old_layout, new_layout);

		// Ensure that the value stored in memory is not changed when realloced
		auto new_casted = reallocated.as<int>();
		bool new_success = true;
		for (usize index = 0; index < old_count; ++index) {
			if (new_casted[index] != 0) {
				new_success = false;
			}
		}
		CHECK(new_success);

		// Free only requires the pointer
		core::free(reallocated);
	}

	SUBCASE("op::core::copy") {
		const auto count = 8;
		const auto layout = core::Layout::array<int>(count);
		CHECK(layout.size == sizeof(int) * count);
		CHECK(layout.alignment == alignof(int));

		const int values[] = {
			1, 2, 3, 4, 5, 6, 7, 8,
		};

		auto a = core::malloc(layout);
		// Copy does not use an intermediate buffer so overlapping is possible
		core::copy(a, values, sizeof(int) * count);

		auto casted = a.as<int>();
		bool success = true;
		for (usize index = 0; index < count; ++index) {
			if (casted[index] != static_cast<int>(index) + 1) {
				success = false;
			}
		}
		CHECK(success);

		core::free(a);
	}

	SUBCASE("op::core::move") {
		const auto count = 8;
		const auto layout = core::Layout::array<int>(count);
		CHECK(layout.size == sizeof(int) * count);
		CHECK(layout.alignment == alignof(int));

		const int values[] = {
			1, 2, 3, 4, 5, 6, 7, 8,
		};

		auto a = core::malloc(layout);
		// Move uses an intermediate buffer so overlapping is possible
		core::move(a, values, sizeof(int) * count);

		auto casted = a.as<int>();
		bool success = true;
		for (usize index = 0; index < count; ++index) {
			if (casted[index] != static_cast<int>(index) + 1) {
				success = false;
			}
		}
		CHECK(success);

		core::free(a);
	}
}

OP_TEST_END
