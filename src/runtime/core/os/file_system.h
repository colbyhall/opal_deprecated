// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/bitflag.h"
#include "core/containers/function.h"
#include "core/containers/result.h"
#include "core/containers/string.h"

EU_CORE_NAMESPACE_BEGIN

class File final : NonCopyable {
public:
	enum class Flags : u32 { Read, Write, Create };
	enum class Error : u32 { NotFound, InUse };
	static Result<File, Error> open(const StringView& path,
									BitFlag<Flags> flags);

	EU_NO_DISCARD usize size() const;
	EU_NO_DISCARD usize cursor() const { return m_cursor; }

	enum class Seek : u32 { Begin, Current, End };
	usize seek(Seek method, isize distance);
	EU_ALWAYS_INLINE void rewind() { seek(Seek::Begin, 0); }
	void set_eof();

	usize read(Slice<u8> buffer);
	void write(Slice<const u8> buffer);

	EU_ALWAYS_INLINE File(File&& move) noexcept :
		m_handle(move.m_handle), m_flags(move.m_flags), m_cursor(0) {
		move.m_handle = nullptr;
	}
	EU_ALWAYS_INLINE File& operator=(File&& move) noexcept {
		auto to_destroy = eu::move(*this);
		EU_UNUSED(to_destroy);

		m_handle = move.m_handle;
		m_flags = move.m_flags;
		move.m_handle = nullptr;
		return *this;
	}
	~File();

private:
	EU_ALWAYS_INLINE File(void* handle, BitFlag<Flags> flags) :
		m_handle(handle), m_flags(flags), m_cursor(0) {}

	void* m_handle;
	BitFlag<Flags> m_flags;
	usize m_cursor;
};

Result<String, File::Error> read_to_string(const StringView& path);

struct DirectoryItem {
	enum class Type : u32 { File, Directory, Unknown };
	struct Meta {
		u64 creation_time;
		u64 last_access_time;
		u64 last_write_time;

		usize size;
		bool read_only;
	};

	StringView path;
	Type type;
	Meta meta;
};

using ReadDirFunction = FunctionRef<bool(const DirectoryItem&)>;

void read_dir(StringView path, ReadDirFunction function);
void read_dir_recursive(StringView path, ReadDirFunction function);

String cwd();

EU_CORE_NAMESPACE_END

// Export to eu namespace
EU_NAMESPACE_BEGIN
using core::File;
using core::read_to_string;

using core::cwd;
using core::DirectoryItem;
using core::read_dir;
using core::read_dir_recursive;
EU_NAMESPACE_END