#include <iostream>
#include <algorithm>

#include "seat/seat.h"

/*-------------------------------------------------*/
/* Just imagine we don't have access to this class */
class Entity {
public:
	int health = 100;
	virtual void Damage(int amount) {
		if (health <= 0)
			return;

		health = std::max(health - amount, 0);
		std::cout << health << std::endl;
	}
};
/*-------------------------------------------------*/

typedef void(__thiscall *Entity__Damage)(void*, int);
Entity__Damage Entity__Damage__o;
void hookedDamage(void* thisptr, int amount) {
	amount = 0;
	Entity__Damage__o(thisptr, amount);
}

int main() {
	Entity* pEntity = new Entity();

	VMT* vmt = VMTM::Load(pEntity);
	Entity__Damage__o = (Entity__Damage)vmt->Hook(0, hookedDamage);

	pEntity->Damage(1000);

	// pEntity->health remains 100 because the hook sets the `amount` parameter to 0 before the damage function runs.

	return 0;
}
