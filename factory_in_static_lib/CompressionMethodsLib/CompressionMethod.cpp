#include "stdafx.h"
#include "CompressionMethod.h"

//#include <Poco/File.h>
//#include <Poco/Path.h>
//#include <Poco/Zip/Compress.h>
//#include <Poco/TemporaryFile.h>
//
//#include "bzlib.h"
//
//#include "Utils.h"

/////////////////////////////////////////////////////////////////////////////////////////
// CompressionMethodFactory

std::map<std::string, CompressionMethodInfo> CompressionMethodFactory::s_map;

bool CompressionMethodFactory::Register(const std::string name, const CompressionMethodInfo& info)
{
	if (auto it = s_map.find(name); it == s_map.end())
	{
		s_map[name] = info;
		std::cout << name << " registered\n";
		return true;
	}
	return false;
}

std::unique_ptr<ICompressionMethod> CompressionMethodFactory::Create(const std::string& name)
{
	if (auto it = s_map.find(name); it != s_map.end())
		return it->second.m_CreateFunc();

	return nullptr;
}

//std::map<std::string, CompressionMethodInfo>& CompressionMethodFactory::GetMap()
//{
//	static std::map<std::string, CompressionMethodInfo> s_methods;
//	return s_methods;
//}



