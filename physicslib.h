#pragma once
#include <algorithm>
#include <vector>
#include "objects.h"

float SweptAABB(gameobject b1, gameobject b2, float& normalx, float& normaly);

bool circlecoll(gameobject c1, gameobject c2);

bool sepaxistest(gameobject obj1, gameobject obj2);

void getindices(gameobject obj, std::vector<int>& indices);

bool sepaxistestcircle(gameobject obj1, gameobject circle);