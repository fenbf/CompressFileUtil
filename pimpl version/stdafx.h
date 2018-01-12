// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

// TODO: reference additional headers your program requires here

#define _HAS_AUTO_PTR_ETC	1 // hack to compile boost with std-c++17... https://stackoverflow.com/questions/39363720/compiling-boost-with-msvc2015-with-stdclatest-or-c17-n4190

#include <memory>
#include <vector>
#include <string>
#include <optional>
#include <iostream>
#include <fstream>
#include <map>
#include <filesystem>

#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>

// some common types:
using StringVector = std::vector<std::string>;
