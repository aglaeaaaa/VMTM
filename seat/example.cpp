#include <iostream>
#include <algorithm>

#include "seat/seat.h"

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

	Entity__Damage__o = (Entity__Damage)seat::Hook(pEntity, 0, (void*)hookedDamage);

	std::cout << "We've hooked the Entity::Damage function and set the damage parameter to 0 before calling original" << std::endl;
	pEntity->Damage(1000);

	return 0;
}

