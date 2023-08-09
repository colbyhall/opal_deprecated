// Copyright Colby Hall. All Rights Reserved.

#include "core/containers/map.h"
#include "doctest/doctest.h"

GJ_TEST_BEGIN

TEST_CASE("gj::core::Map") {
	SUBCASE("Map::reserve") {
		Map<int, int> map;
		map.reserve(128);
		CHECK(map.cap() >= 128);
	}

	SUBCASE("Map::insert") {
		Map<int, int> map;
		map.insert(100, 200);
		CHECK(map.len() == 1);
	}

	SUBCASE("Map::remove") {
		Map<int, int> map;
		map.insert(100, 200);
		REQUIRE(map.len() == 1);
		const auto value = map.remove(100);
		REQUIRE(value.is_set());
		const auto x = value.unwrap();
		CHECK(x == 200);
	}

	SUBCASE("Map::retain") {
		Map<int, int> map;
		map.insert(100, 200);
		map.insert(99, 100);
		map.insert(1209312, 50);
		map.insert(12342, 25);
		REQUIRE(map.len() == 4);
		map.retain([](const auto&, const auto& value) { return value > 50; });
		CHECK(map.len() == 2);
	}

	SUBCASE("Map::find, Map::find_mut") {
		Map<int, int> map;
		map.insert(100, 200);
		map.insert(99, 100);
		map.insert(1209312, 50);
		map.insert(12342, 25);
		REQUIRE(map.len() == 4);
		const auto a = map.find(99).unwrap();
		CHECK(a == 100);
		const auto b = map.find_mut(1209312).unwrap();
		CHECK(b == 50);
	}

	SUBCASE("Map::iter, Map::iter_mut") {
		Map<int, int> map;
		map.insert(100, 200);
		map.insert(99, 100);
		map.insert(1209312, 50);
		map.insert(12342, 25);
		REQUIRE(map.len() == 4);

		int accum = 0;
		for (auto it = map.iter(); it; ++it) {
			accum += it.value();
		}
		CHECK(accum == 375);
	}
}

GJ_TEST_END
