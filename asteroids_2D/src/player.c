#include "player.h"
#include "raymath.h"  // Vector2Normalize

void SpaceshipGoRight(Spaceship_t* spaceship, float deltaTime)
{
  spaceship->m_direction.x = 1.0f;
  spaceship->m_direction = Vector2Normalize(spaceship->m_direction);
  spaceship->m_center.x += spaceship->m_direction.x * deltaTime * SPACESHIP_SPEED;
}

void SpaceshipGoLeft(Spaceship_t* spaceship, float deltaTime)
{
  spaceship->m_direction.x = -1.0f;
  spaceship->m_direction = Vector2Normalize(spaceship->m_direction);
  spaceship->m_center.x += spaceship->m_direction.x * deltaTime * SPACESHIP_SPEED;
}

void SpaceshipGoUp(Spaceship_t* spaceship, float deltaTime)
{
  spaceship->m_direction.y = -1.0f;
  spaceship->m_direction = Vector2Normalize(spaceship->m_direction);
  spaceship->m_center.y += spaceship->m_direction.y * deltaTime * SPACESHIP_SPEED;
}

void SpaceshipGoDown(Spaceship_t* spaceship, float deltaTime)
{
  spaceship->m_direction.y = 1.0f;
  spaceship->m_direction = Vector2Normalize(spaceship->m_direction);
  spaceship->m_center.y += spaceship->m_direction.y * deltaTime * SPACESHIP_SPEED;
}