#include "player.h"
#include "raymath.h"  // Vector2Normalize

void PlayerGoRight(Player_t* p_player, float deltaTime)
{
  p_player->m_directions.x = 1.0f;
  Vector2Normalize(p_player->m_directions);
  p_player->m_coordinates.x += p_player->m_directions.x * deltaTime * PLAYER_SPEED;
}

void PlayerGoLeft(Player_t* p_player, float deltaTime)
{
  p_player->m_directions.x = -1.0f;
  Vector2Normalize(p_player->m_directions);
  p_player->m_coordinates.x += p_player->m_directions.x * deltaTime * PLAYER_SPEED;
}

void PlayerGoUp(Player_t* p_player, float deltaTime)
{
  p_player->m_directions.y = -1.0f;
  Vector2Normalize(p_player->m_directions);
  p_player->m_coordinates.y += p_player->m_directions.y * deltaTime * PLAYER_SPEED;
}

void PlayerGoDown(Player_t* p_player, float deltaTime)
{
  p_player->m_directions.y = 1.0f;
  Vector2Normalize(p_player->m_directions);
  p_player->m_coordinates.y += p_player->m_directions.y * deltaTime * PLAYER_SPEED;
}
