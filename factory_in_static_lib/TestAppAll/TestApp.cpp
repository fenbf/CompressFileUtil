#include "stdafx.h"
#include "CompressionMethod.h"
#include <cassert>

int main()
{
	auto pMethod = CompressionMethodFactory::Create("ZIP");
	assert(pMethod);
    return 0;
}

