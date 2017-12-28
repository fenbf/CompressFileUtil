// CompressFileUtil.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "FileCompressor.h"


// https://www.codeproject.com/Articles/256158/Learning-Poco-Zip-files
// https://theboostcpplibraries.com/boost.program_options

// https://justcheckingonall.wordpress.com/2008/01/21/bzip2-gotta-love-it/

// tar before bz2? https://www.codeproject.com/Articles/1585/CFileTar-pack-and-unpack-file-archives

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
		
		if (vm.count("out"))
			params.m_output = vm["out"].as<std::string>();
		
		if (vm.count("files"))
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

void RunCompressor(const InputParams& params) noexcept
{
	try
	{
		FileCompressor compressor{ params.m_output, params.m_files };
		compressor.Compress();
	}
	catch (const std::exception& ex)
	{
		std::cerr << "Error: " << ex.what() << '\n';
	}
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

		RunCompressor(params);
	}
	else
	{
		ShowHelp(desc);
	}
}

