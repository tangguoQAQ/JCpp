#pragma once

#include "JClass.h"
#include <memory>
#include <iostream>

namespace Java
{
	/**
	 * @param className ��������·��, �� "java.lang.String"
	 * @throws Java::Exception::JniException
	 */
	JClass::JClass(ConstString className) noexcept(false) : name(className), self(*GetClassAndCache(className)) {}

	JClass::~JClass() noexcept
	{
		self.reset();
		GetClassAndCache((name + "?").c_str());
	}

	bool JClass::operator==(const JClass& another) const noexcept
	{
		return *self == *another.self;
	}

	bool JClass::operator!=(const JClass& another) const noexcept
	{
		return !(*this == another);
	}

	std::ostream& Java::operator<<(std::ostream& os, const JClass& jc)
	{
		return os << std::string("JClass@") + jc.name + ";";
	}

	::jclass JClass::Ptr() const noexcept
	{
		return *self;
	}

	ConstString JClass::Name() const noexcept
	{
		return name.c_str();
	}
}
