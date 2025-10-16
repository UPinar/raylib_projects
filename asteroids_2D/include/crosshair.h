#ifndef CROSSHAIR_H
#define CROSSHAIR_H

#include "raylib.h"

#define   CROSSHAIR_SCALE       1.5f

Vector2 Crosshair_GetCenterPosition(const Texture2D* crosshair);
void Crosshair_Draw(const Texture2D* texture, Vector2 crosshairCenter);

#endif // CROSSHAIR_H