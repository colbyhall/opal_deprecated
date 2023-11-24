// Copyright Colby Hall. All Rights Reserved.

#include "core/containers/function.h"
#include "core/containers/vector.h"
#include "game/archetype.h"

OP_GAME_NAMESPACE_BEGIN

class World;

class Query {
public:
	explicit Query() = default;

	Query& read(ComponentType component);
	Query& write(ComponentType component);

	class View {
	public:
		explicit View(
			Slice<const ComponentType> reads,
			Slice<const ComponentType> writes,
			Archetype& archetype,
			u32 slot
		)
			: m_reads(reads)
			, m_writes(writes)
			, m_archetype(archetype)
			, slot(slot) {}

		template <typename T>
		T const& read() const {
			Option<T const&> result = nullopt;
			for (auto& component : m_reads) {
				if (component == T::type()) {
					result = m_archetype.read<T>(slot);
					break;
				}
			}
			return result.unwrap();
		}

		template <typename T>
		T& write() {
			Option<T&> result = nullopt;
			for (auto& component : m_writes) {
				if (component == T::type()) {
					result = m_archetype.write<T>(slot);
					break;
				}
			}
			return result.unwrap();
		}

	private:
		Slice<const ComponentType> m_reads;
		Slice<const ComponentType> m_writes;

		Archetype& m_archetype;
		u32 slot;
	};
	void execute(World& world, FunctionRef<void(View&)> callback);

private:
	Vector<ComponentType> m_reads;
	Vector<ComponentType> m_writes;
};

OP_GAME_NAMESPACE_END