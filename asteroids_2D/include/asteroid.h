#ifndef ASTEROID_H
#define ASTEROID_H

#include "raylib.h"


#define   ASTEROID_LARGE_TOP_LEFT_X       .0F
#define   ASTEROID_LARGE_TOP_LEFT_Y       0.0F
#define   ASTEROID_LARGE_WIDTH            160.0F
#define   ASTEROID_LARGE_HEIGHT           160.0F

#define   ASTEROID_MEDIUM_TOP_LEFT_X      0.0F
#define   ASTEROID_MEDIUM_TOP_LEFT_Y      160.0F
#define   ASTEROID_MEDIUM_WIDTH           96.0F
#define   ASTEROID_MEDIUM_HEIGHT          96.0F

#define   ASTEROID_SMALL_TOP_LEFT_X       0.0F
#define   ASTEROID_SMALL_TOP_LEFT_Y       256.0F
#define   ASTEROID_SMALL_WIDTH            64.0F
#define   ASTEROID_SMALL_HEIGHT           64.0F
    
#define   ASTEROID_MIN_SPEED              100.0F  
#define   ASTEROID_MAX_SPEED              300.0F


typedef enum AsteroidSize {
  ASTEROID_SIZE_LARGE,
  ASTEROID_SIZE_MEDIUM,
  ASTEROID_SIZE_SMALL
} AsteroidSize_t;

typedef struct Asteroid {
  Vector2           m_direction;
  Vector2           m_position; 
  Vector2           m_image_location_x_y;
  float             m_width;
  float             m_height;
  float             m_speed;
  float             m_lifetime;
  AsteroidSize_t    m_size;
  bool              m_active;
} Asteroid_t;

#endif // ASTEROID_H
