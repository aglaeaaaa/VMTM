#include "seat.h"
std::vector<VMT*> VMTM::vmts;

VMT* VMTM::Load(void* instance) {
	VMT* vmt = new VMT(instance);
	vmts.push_back(vmt);

	return vmt;
}

void VMTM::Unload() {
	for (VMT* vmt : vmts) {
		delete vmt;
	}
	vmts.clear();
}
