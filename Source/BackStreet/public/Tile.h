// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class BACKSTREET_API FTile
{
public:
	char stagemap;
	int x;
	int y;
	int GateCnt;

public:
	bool up, down, left, right;


public:
	void InitTile(int width, int hight);
	bool IsVisited();
	void GateCount();
};
