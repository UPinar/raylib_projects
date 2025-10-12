#include "player.h"
#include "raymath.h"  // Vector2Normalize

void PlayerGoRight(Player_t* const player, float deltaTime)
{
  player->m_directions.x = 1.0f;
  Vector2Normalize(player->m_directions);
  player->m_coordinates.x += player->m_directions.x * deltaTime * PLAYER_SPEED;
}

void PlayerGoLeft(Player_t* const player, float deltaTime)
{
  player->m_directions.x = -1.0f;
  Vector2Normalize(player->m_directions);
  player->m_coordinates.x += player->m_directions.x * deltaTime * PLAYER_SPEED;
}

void PlayerGoUp(Player_t* const player, float deltaTime)
{
  player->m_directions.y = -1.0f;
  Vector2Normalize(player->m_directions);
  player->m_coordinates.y += player->m_directions.y * deltaTime * PLAYER_SPEED;
}

void PlayerGoDown(Player_t* const player, float deltaTime)
{
  player->m_directions.y = 1.0f;
  Vector2Normalize(player->m_directions);
  player->m_coordinates.y += player->m_directions.y * deltaTime * PLAYER_SPEED;
}
