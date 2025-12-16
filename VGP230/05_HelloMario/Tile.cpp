#include "Tile.h"

Tile::Tile(TileType tileType, const X::Math::Vector2& position)
	:Entity() 
	, mTileType(tileType)
	, mPosition(position)
	, mRect (0,0,0,0)
	, mImageId(0)
{

}
Tile::~Tile()
{

}
void Tile::Load()
{
	switch (mTileType)
	{
	case TT_GROUND:mImageId = X::LoadTexture("Tileset_Ground.png");break;
	case TT_WALL:mImageId = X::LoadTexture("Trees_Swap.png");break;
	case TT_WATER:mImageId = X::LoadTexture("Tileset_Water.png");break;
	case TT_DOOR:mImageId = X::LoadTexture("Flowers.png");break;
	default:
		XASSERT(false, "Unknown tile type");
	}
	float halfWidth = X::GetSpriteWidth(mImageId) * 0.5f;
	float halfHeight = X::GetSpriteHeight(mImageId) * 0.5f;
	mRect.left = mPosition.x - halfWidth;
	mRect.right = mPosition.x + halfWidth;
	mRect.top = mPosition.y - halfHeight;
	mRect.bottom = mPosition.y + halfHeight;
}
void Tile::Update(float deltaTime)
{

}
void Tile::Render()
{
	X::DrawSprite(mImageId, mPosition);
}
void Tile::Unload()
{

}
const X::Math::Vector2& Tile::GetPosition() const
{
	return mPosition;
}
bool Tile::IsCollidable() const
{
	return mTileType == TT_WALL;
}
bool Tile::HasCollision(const X::Math::Rect& onjRect) const
{
	return X::Math::Intersect(mRect, onjRect);
}
const X::Math::Rect& Tile::GetRect() const
{
	return mRect;
}