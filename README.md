<h1 align="center">VMTM</h1>

<p align="center"><em>a vmt swapping library</em></p>


<h2 align="center">usage</h2>

Example:

```cpp
typedef void(__thiscall *Entity__Damage)(void*, int);
Entity__Damage Entity__Damage__o;

void hookedDamage(void* thisptr, int amount) {
    amount = 0;
    Entity__Damage__o(thisptr, amount);
}

int main() {
    // pEntity = heap allocated object of which you want to hook a function.

    // Setting up the hooking system
    VMT* vmt = VMTM::Load(pEntity);

    // Hooking @ index 0
    Entity__Damage__o = (Entity__Damage)vmt->Hook(0, hookedDamage);

    return 0;
}
```
Check [this](https://github.com/aglaeaaaa/seat/blob/master/example.cpp) out if youre still confused.
