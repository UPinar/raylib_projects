#ifndef COLLISION_H
#define COLLISION_H

#include "bullet.h" 
#include "asteroid.h"

#include <stddef.h>   // size_t

int Collision_CheckCollisionsBetweenBulletsAndAsteroids( Bullet_t* bulletsArray, 
                                                         Asteroid_t* asteroidsArray, 
                                                         size_t currentAsteroidCount);

#endif // COLLISION_H