#pragma once 


class VMT {
public:
	VMT(void* instance);
	~VMT();

	/* Replace a function in the vtable with yours */
	template <typename T>
	void* Hook(unsigned int index, T* function) {
		m_vtable[index] = function;
		return m_vtable_o[index];
	}

	/* The instance */
	void* m_instance;

	/* The instance's original vtable pointer */
	void** m_vtable_o;
	/* Our vtable */
	void** m_vtable;
};