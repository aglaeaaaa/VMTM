#include "vmt.h"
#include <vector>

#include <Windows.h>



/* https://open.spotify.com/track/73t0jlbyewOLofTwn13kA1?si=a45ca6e4f72b4213 */
/* https://open.spotify.com/track/26ynMPvUZBTax5YNXYcf6e?si=337fbb24f89d46bf */
VMT::VMT(void* instance) {
	/* Store the instance for later */
	m_instance = instance;
	/* Store the original vtable */
	m_vtable_o = *(void***)instance;

	std::vector<void*> arrFunctions;
	int i = 0;
	while (true) {
		void* pFunction = m_vtable_o[i];
		if (!pFunction) break;

		MEMORY_BASIC_INFORMATION mbi = {};
		if (!VirtualQuery(pFunction, &mbi, sizeof(mbi))) break;

		if (!(mbi.Protect & (PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY))) break;

		arrFunctions.push_back(pFunction);
		i++;
	}

	/* Fill our new vtable with the old functions */
	m_vtable = new void* [arrFunctions.size()];
	for (int i = 0; i < arrFunctions.size(); i++) {
		m_vtable[i] = arrFunctions[i];
	}

	/* Set object's vtable to our new one */
	*(void***)instance = m_vtable;
}

VMT::~VMT() {
	*(void***)m_instance = m_vtable_o;
	delete[](m_vtable);
}
