// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/function.h"
#include "core/containers/result.h"
#include "core/containers/string.h"

SF_CORE_NAMESPACE_BEGIN

class File final : NonCopyable {
public:
	enum class Flags : u32 { Read, Write, Create };
	enum class Error : u32 { NotFound, InUse };
	static Result<File, Error> open(const StringView& path, Flags flags);

	SF_NO_DISCARD usize size() const;
	SF_NO_DISCARD usize cursor() const { return m_cursor; }

	enum class Seek : u32 { Begin, Current, End };
	usize seek(Seek method, isize distance);
	SF_ALWAYS_INLINE void rewind() { seek(Seek::Begin, 0); }
	void set_eof();

	usize read(Slice<u8> buffer);
	void write(Slice<const u8> buffer);

	SF_ALWAYS_INLINE File(File&& move) noexcept : m_handle(move.m_handle), m_flags(move.m_flags), m_cursor(0) {
		move.m_handle = nullptr;
	}
	SF_ALWAYS_INLINE File& operator=(File&& move) noexcept {
		auto to_destroy = sf::move(*this);
		SF_UNUSED(to_destroy);

		m_handle = move.m_handle;
		m_flags = move.m_flags;
		move.m_handle = nullptr;
		return *this;
	}
	~File();

private:
	SF_ALWAYS_INLINE File(void* handle, Flags flags) : m_handle(handle), m_flags(flags), m_cursor(0) {}

	void* m_handle;
	Flags m_flags;
	usize m_cursor;
};
SF_ENUM_CLASS_BITFIELD(File::Flags)

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

SF_CORE_NAMESPACE_END

// Export to sf namespace
SF_NAMESPACE_BEGIN
using core::File;
using core::read_to_string;

using core::cwd;
using core::DirectoryItem;
using core::read_dir;
using core::read_dir_recursive;
SF_NAMESPACE_END