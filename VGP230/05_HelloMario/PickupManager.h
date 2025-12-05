#pragma once

#include "Pickup.h"

class PickupManager : public Entity
{
public:
	~PickupManager() override;
	static PickupManager* Get();

	// Entity override
	void Load() override;
	void Update(float deltaTime) override;
	void Render() override;
	void Unload() override;

	// Pickup Manager functions
	void SpawnPickup(int count);
private:
	PickupManager();
	static PickupManager* mInstance;

	std::vector<Pickup*> mPickups;
	int mNextAvailableIndex;
};
