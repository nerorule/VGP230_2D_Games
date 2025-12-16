#include "Entity.h"
#include "Player.h"

class GameController : public Entity
{
public:
	~GameController();
	static GameController* Get();

	void Load() override;
	void Update(float deltaTime) override;
	void Render() override;
	void Unload() override;

	// Return player's world position for other systems (e.g. enemies)
	const X::Math::Vector2& GetPlayerPosition() const;

	X::SoundId mMusicId = 0;

private:
	GameController();
	static GameController* mInstance;

	Player mPlayer;
};