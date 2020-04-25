#pragma once
#include <string>
#include <vector>
#include "cxx_scope.h"

namespace cxx
{
	struct file_buffer
	{
		std::vector<char> buffer;

		file_buffer() = default;

		file_buffer(const file_buffer&  o) : buffer(o.buffer) { }

		file_buffer(const file_buffer &&o) : buffer(std::move(o.buffer)) { }

		file_buffer& operator=(const file_buffer&  o) { buffer = o.buffer; return *this; }

		file_buffer& operator=(const file_buffer &&o) { buffer = std::move(o.buffer); return *this; }

		char* buffer_ptr() { return& (buffer[0]); }

		const char* buffer_ptr() const { return& (buffer[0]); }

		operator bool() const { return buffer.size() > 0; }

		size_t length() const { return buffer.size(); }
	};

	bool read_file(const std::string& path, file_buffer& buffer)
	{
		FILE* f = fopen(path.c_str(), "rb");
		if (f != nullptr)
		{
            ON_EXIT
            {
                fclose(f);
            };

			fseek(f, 0, SEEK_END);
			size_t file_length = ftell(f);


			if (file_length > 0)
			{
				fseek(f, 0, SEEK_SET);
				buffer.buffer.resize(file_length);
				file_length = static_cast<uint32_t>(fread(buffer.buffer_ptr(), 1, file_length, f));
				if (file_length != buffer.length())
				{
					buffer.buffer.resize(file_length);
				}
				return true;
			}
			
		}
		return false;
	}
}
