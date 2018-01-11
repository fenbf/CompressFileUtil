// CompressFileUtil.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "FileCompressor.h"

struct InputParams
{
	StringVector m_files;
	std::string m_output;
};

std::optional<InputParams> ParseCommandLine(int argc, char* argv[]) noexcept
{
	if (argc > 2)
	{
		InputParams params;
		params.m_output = argv[1];
		for (int i = 2; i < argc; ++i)
			params.m_files.push_back(argv[i]);

		return params;
	}

	return std::nullopt;
}

void ShowHelp() noexcept
{
	std::cout << "compressor.exe outputFileName file1 file2 file3...\n";
}

void RunCompressor(const InputParams& params) noexcept
{
	try
	{
		FileCompressor compressor;
		compressor.Compress(params.m_files, params.m_output);
	}
	catch (const std::exception& ex)
	{
		std::cerr << "Error: " << ex.what() << '\n';
	}
}

int main(int argc, char* argv[])
{
	auto inputParams = ParseCommandLine(argc, argv);

	if (inputParams.has_value())
	{
		auto params = inputParams.value();

		RunCompressor(params);
	}
	else
	{
		ShowHelp();
	}
}

