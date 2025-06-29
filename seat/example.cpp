#include <iostream>
#include "seat/seat.h"

class Entity {
public:
	int health = 100;
	virtual void Damage(int amount) {
		if (health > 0)
			health -= amount;
		else
			health = 0;
		std::cout << health << std::endl;
	}

	virtual void Respawn() {
		health = 100;
	}
};

class Player : public Entity {};



typedef void(__thiscall *Player__Damage)(void*, int);
Player__Damage Player__Damage__o;
void hookedDamage(void* thisptr, int amount) {
	amount = 0;
	Player__Damage__o(thisptr, amount);
}

int main() {
	Player* pPlayer = new Player();

	Seat seat;

	Player__Damage__o = (Player__Damage)seat.Hook(pPlayer, 0, (void*)hookedDamage);

	pPlayer->Damage(25);

	return 0;
}
