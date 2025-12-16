#include <XEngine.h>

#include "Collidable.h"

Collidable::Collidable() 
	: mCollidableFilter(-1)
{

}
Collidable::~Collidable()
{

}
void Collidable::SetCollisionFilter(int filter)
{
	mCollidableFilter = filter;
}
int Collidable::GetCollisionFilter() const
{
	return mCollidableFilter;
}
void Collidable::SetRect(const X::Math::Rect& rect)
{
	mRect = rect;
}
const X::Math::Rect& Collidable::GetRect() const
{
	return mRect;
}
bool Collidable::DidCollide(const Collidable* collidable) const
{
	if (collidable == nullptr)
	{
		return false;
	}

	int otherFilter = collidable->GetCollisionFilter();
	int myFilter = GetCollisionFilter();

	// If either filter is zero, treat as non-collidable
	if (otherFilter == 0 || myFilter == 0)
	{
		return false;
	}

	// Allow collision if either object is interested in the other's type
	bool filtersAllow = ((myFilter & collidable->GetType()) != 0) ||
		((otherFilter & GetType()) != 0);

	if (filtersAllow)
	{
		return X::Math::Intersect(mRect, collidable->GetRect());
	}

	return false;
}