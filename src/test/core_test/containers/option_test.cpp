// Copyright Colby Hall. All Rights Reserved.

#include "core/containers/option.h"
#include "doctest/doctest.h"

SF_TEST_BEGIN

class NonTriviallyCopyable {
public:
	NonTriviallyCopyable() = default;
	NonTriviallyCopyable(const NonTriviallyCopyable&) = delete;
	NonTriviallyCopyable& operator=(const NonTriviallyCopyable&) = delete;
	NonTriviallyCopyable(NonTriviallyCopyable&&) noexcept {}
	NonTriviallyCopyable& operator=(NonTriviallyCopyable&&) noexcept { return *this; }

	int m_x = 0;
};

TEST_CASE("sf::core::Option") {
	SUBCASE("non_trivially copyable") {
		Option<NonTriviallyCopyable> foo = sf::nullopt;
		CHECK(!foo.is_set());

		// Non trivially copyable can not be copied. To unwrap without mutating
		// option must use as_ref methods
		// auto copy = foo;
		// CHECK(!copy.is_set());

		// Non trivially copyable will reset after being unwrapped
		foo = NonTriviallyCopyable{};
		CHECK(foo.is_set());
		CHECK(foo.unwrap().m_x == 0);
		CHECK(!foo.is_set());
	}

	SUBCASE("trivially copyable") {
		Option<int> foo = sf::nullopt;
		CHECK(!foo.is_set());

		// Trivially copyable can be copied
		auto copy = foo;
		CHECK(!copy.is_set());

		// Trivially copyable are not reset when unwrapped. Due to this no
		// as_ref method is available
		copy = 45;
		CHECK(copy.is_set());
		CHECK(copy.unwrap() == 45);
		CHECK(copy.is_set());
	}

	SUBCASE("reference") {
		Option<int&> foo = sf::nullopt;
		CHECK(!foo.is_set());

		// reference options can be copied
		auto copy = foo;
		CHECK(!copy.is_set());

		// options are not reset when unwrapped. Due to this no
		// as_ref method is available
		int bar = 45;
		copy = bar;
		CHECK(copy.is_set());
		CHECK(copy.unwrap() == 45);
		CHECK(copy.is_set());
	}
}

SF_TEST_END
