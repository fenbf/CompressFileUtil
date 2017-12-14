// CompressFileUtil.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#define _HAS_AUTO_PTR_ETC	1 // hack to compile boost with std-c++17... https://stackoverflow.com/questions/39363720/compiling-boost-with-msvc2015-with-stdclatest-or-c17-n4190

#include <boost/program_options.hpp>

#include <Poco/File.h>
#include <Poco/Path.h>
#include <Poco/Zip/Compress.h>
#include <iostream>
#include <fstream>
#include <string>
#include <optional>

// https://www.codeproject.com/Articles/256158/Learning-Poco-Zip-files
// https://theboostcpplibraries.com/boost.program_options

using namespace boost::program_options;

struct InputParams
{
	std::vector<std::string> m_files;
	std::string m_output;
	bool m_showHelp{ false };
};

options_description PrepareOptionDescription() noexcept
{
	options_description desc{ "Options" };
	desc.add_options()
		("help,h", "Help screen")
		("out,o", value<std::string>()->default_value("out.zip"))
		("files,f", value<std::vector<std::string>>()->multitoken(), "File to add");

	return desc;
}

std::optional<InputParams> ParseCommandLine(int argc, char* argv[], const options_description& optDescription) noexcept
{
	try
	{
		variables_map vm;
		store(parse_command_line(argc, argv, optDescription), vm);
		notify(vm);

		InputParams params;

		params.m_showHelp = vm.count("help");
		params.m_output = vm["out"].as<std::string>();
		params.m_files = vm["files"].as<std::vector<std::string>>();

		return params;
	}
	catch (const error &ex)
	{
		std::cerr << ex.what() << '\n';
	}

	return std::nullopt;
}

void ShowHelp(const options_description& desc) noexcept
{
	std::cout << desc << '\n';
}

void RunApp(const InputParams& params) noexcept
{
	for (auto& fn : params.m_files)
		std::cout << "input: " << fn << "\n";

	std::cout << "ouput: " << params.m_output << "\n";

	//auto strFile = vm["file"].as<std::string>();
	//	std::cout << "Filename: " << strFile << '\n';

	//	std::ofstream out("out.zip", std::ios::binary);
	//	Poco::Zip::Compress c(out, /*seekable output*/true);
	//	//for (int i = 2; i < argc; i++)
	//	{
	//		Poco::File f(strFile);
	//		if (f.exists())
	//		{
	//			Poco::Path p(f.path());
	//			if (f.isDirectory())
	//			{
	//				std::cout << "Ignore directory " << f.path() << std::endl;
	//			}
	//			else if (f.isFile())
	//			{
	//				std::cout << "Add file " << f.path() << std::endl;
	//				c.addFile(p, p.getFileName());
	//			}
	//		}
	//	}
	//	c.close();
	//	out.close();
}

int main(int argc, char* argv[])
{
	auto desc = PrepareOptionDescription();
	auto inputParams = ParseCommandLine(argc, argv, desc);

	if (inputParams.has_value())
	{
		auto params = inputParams.value();

		if (params.m_showHelp)
			ShowHelp(desc);

		RunApp(params);
	}
	else
	{
		ShowHelp(desc);
	}
}

