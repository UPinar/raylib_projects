#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"

#define PLAYER_WIDTH          96.0f
#define PLAYER_HEIGHT         64.0f
#define PLAYER_SPEED 300.0f

typedef struct {
  Vector2 m_location_inside_image;
  Vector2 m_size;
  Vector2 m_coordinates;
  Vector2 m_directions;
  float m_rotation_degrees;
} Player_t;

void PlayerGoRight(Player_t* p_player, float deltaTime);
void PlayerGoLeft(Player_t* p_player, float deltaTime);
void PlayerGoUp(Player_t* p_player, float deltaTime);
void PlayerGoDown(Player_t* p_player, float deltaTime);

#endif // PLAYER_H