#ifndef BULLET_H
#define BULLET_H

#include "raylib.h"

#define   MAX_ACTIVE_BULLET_COUNT       10

#define   BULLET_TOP_LEFT_X             480.0F
#define   BULLET_TOP_LEFT_Y             288.0F
#define   BULLET_WIDTH                  32.0F
#define   BULLET_HEIGHT                 32.0F

#define   BULLET_SPEED                  500.0F

typedef struct Bullet {
  Vector2   m_direction;
  Vector2   m_position; 
  bool      m_active;
} Bullet_t;

#endif // BULLET_H