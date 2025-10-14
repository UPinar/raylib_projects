#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"

#define   PLAYER_TOP_LEFT_X   196.0F
#define   PLAYER_TOP_LEFT_Y   256.0F
#define   PLAYER_WIDTH        96.0F
#define   PLAYER_HEIGHT       64.0F

#define   SPACESHIP_SPEED     500.0F

typedef struct Spaceship {
  Vector2 m_center;
  Vector2 m_direction;
  float m_rotation;
} Spaceship_t;

void SpaceshipGoRight(Spaceship_t* spaceship, float deltaTime);
void SpaceshipGoLeft(Spaceship_t* spaceship, float deltaTime);
void SpaceshipGoUp(Spaceship_t* spaceship, float deltaTime);
void SpaceshipGoDown(Spaceship_t* spaceship, float deltaTime);

#endif // PLAYER_H