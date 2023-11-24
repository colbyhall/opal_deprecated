// Copyright Colby Hall. All Rights Reserved.

#include "game/query.h"
#include "game/world.h"

OP_GAME_NAMESPACE_BEGIN

Query& Query::read(ComponentType component) {
	m_reads.push(component);
	return *this;
}

Query& Query::write(ComponentType component) {
	m_writes.push(component);
	return *this;
}

void Query::execute(World& world, FunctionRef<void(Query::View&)> callback) {
	// Gather all archetypes that match the query
	Vector<u32> archetypes;
	archetypes.reserve(world.m_archetypes.len());
	for (u32 index = 0; index < world.m_archetypes.len(); ++index) {
		bool has_read = true;
		for (auto read : m_reads) {
			if (!world.m_archetypes[index].supports(read)) {
				has_read = false;
				break;
			}
		}
		if (!has_read) continue;

		bool has_write = true;
		for (auto write : m_writes) {
			if (!world.m_archetypes[index].supports(write)) {
				has_write = false;
				break;
			}
		}

		if (has_write) {
			archetypes.push(index);
		}
	}

	for (auto archetype_index : archetypes) {
		auto& archetype = world.m_archetypes[archetype_index];
		for (u32 index = 0; index < archetype.total_slots(); ++index) {
			if (archetype.is_slot_used(index)) {
				auto view = View(m_reads, m_writes, archetype, index);
				callback(view);
			}
		}
	}
}

OP_GAME_NAMESPACE_END