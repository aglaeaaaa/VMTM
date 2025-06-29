#pragma once

#include <vector>
#include "vmt/vmt.h"


static class VMTM {
public:
	static VMT* Load(void* instance);
	static void Unload();

	static std::vector<VMT*> vmts;
};
