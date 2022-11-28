// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"

void FTile::InitTile(int _x, int _y)
{
	x = _x;
	y = _y;
	GateCnt = 0;

	up = down = left = right = false;

}

bool FTile::IsVisited()
{
	return up || down || left || right;
}

void FTile::GateCount()
{
	if (up)
		GateCnt++;
	if (down)
		GateCnt++;
	if (left)
		GateCnt++;
	if (right)
		GateCnt++;
}
