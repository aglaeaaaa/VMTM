#pragma once

namespace seat {
	void* Hook(void* instance, unsigned int index, void* function);
	bool Unhook(void* instance, void** replacementVtable, void** originalVtable);
}