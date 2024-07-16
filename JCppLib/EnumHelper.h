#pragma once

#include <string>
#include <functional>
#include <algorithm>

struct CompilerStr
{
	const char* buffer;
	int size;
	constexpr CompilerStr() :
		buffer(nullptr), size(0)
	{}
	constexpr void FillData(const char* buffer, int size)
	{
		this->buffer = buffer;
		this->size = size;
	}
	std::string ToString() const
	{
		if(buffer == nullptr || size == 0)
		{
			return "";
		}
		return std::string(buffer, size);
	}
};

template<int N>
struct CompilerStrs
{
	CompilerStr values[N];
	constexpr CompilerStrs() {}
	std::string GetStr(int index) const
	{
		if(index < 0 || index >= N)
		{
			return "";
		}
		return values[index].ToString();
	}
};

constexpr int CompilerStrlen(const char* buffer)
{
	int i = 0;
	for(; buffer[i]; ++i) {}
	return i;
}

template<int N>
constexpr CompilerStrs<N> CompilerSplitEnumNames(const char* buffer)
{
	CompilerStrs<N> res;
	int index = 0;
	int offset = -1;
	for(int i = 0; index < N && buffer[i]; ++i)
	{
	if(buffer[i] == ',')
		{
			if(offset >= 0 && offset < i)
			{
				res.values[index++].FillData(buffer + offset, i - offset);
			}
			offset = -1;
		}
		else if(offset == -1 && buffer[i] != ' ')
		{
			offset = i;
		}
	}
	if(index < N && offset != -1)
	{
		int len = CompilerStrlen(buffer);
		if(offset >= 0 && offset < len)
		{
			res.values[index++].FillData(buffer + offset, len - offset);
		}
	}
	return res;
}

constexpr int CompilerGetEnumNamesNum(const char* buffer)
{
	int num = 0;
	for(int i = 0; buffer[i]; ++i)
	{
		if(buffer[i] == ',' && buffer[i + 1] != 0)
		{
			++num;
		}
	}
	return num + 1;
}

/*
ENUM_DEFINE(EType, JNI, ClassNotFound, MethodNotFound);
ENUM_NAMES_OF_EType.GetStr(static_cast<int>(m_t));
*/

#define ENUM_DEFINE(type, ...) enum type { __VA_ARGS__ }; \
	constexpr int ENUM_SIZE_OF_##type = CompilerGetEnumNamesNum(#__VA_ARGS__); \
	constexpr char ENUM_ORIGIN_NAMES_OF_##type[] = #__VA_ARGS__; \
	constexpr CompilerStrs<ENUM_SIZE_OF_##type> ENUM_NAMES_OF_##type = CompilerSplitEnumNames<ENUM_SIZE_OF_##type>(ENUM_ORIGIN_NAMES_OF_##type);
