#include "raylib.h"
#include "raymath.h"  // Vector2Subtract
#include "player.h"

#include <stdio.h>  // printf

#define SCREEN_WIDTH          900
#define SCREEN_HEIGHT         600
#define SCREEN_FPS            144

#define PLAYER_WIDTH          96.0f
#define PLAYER_HEIGHT         64.0f

#define CROSSHAIR_WIDTH       24
#define CROSSHAIR_HEIGHT      24
#define CROSSHAIR_THICKNESS   3.0f
#define CROSSHAIR_COLOR       GREEN

// #define DEBUG


static void LimitMousePosition(Vector2* const p_mousePosition);
static void UpdatePlayerPosition(Player_t* const p_player, float deltaTime);
static void UpdatePlayerScreenBounds(Player_t* const p_player);
static void UpdatePlayerRotation(Player_t* const player, Vector2 mousePosition);

static void DrawCrosshair(Vector2 position);
static void DrawPlayer(const Texture2D* const p_playerTexture, const Player_t* const p_player);

// static void CheckCrosshairBounds(Vector2* crosshairPosition);

int main(void)
{
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Fresh Start Files");
  SetTargetFPS(SCREEN_FPS);
  DisableCursor();


  Texture2D playerTexture = LoadTexture("assets/asteroids-2x.png");
  Player_t player = {
    .m_location_inside_image = { 288.0f, 256.0f },
    .m_size = { PLAYER_WIDTH, PLAYER_HEIGHT },
    .m_coordinates = {  SCREEN_WIDTH / 2 - PLAYER_WIDTH / 2, 
                        SCREEN_HEIGHT / 2 - PLAYER_HEIGHT / 2 },
  };

  float deltaTime = 0.0f;
  Vector2 mousePosition = { 0 };
  
  while (!WindowShouldClose())
  {
    deltaTime       = GetFrameTime();
    mousePosition   = GetMousePosition();

    BeginDrawing();
    ClearBackground(BLACK);

    // ------------------ UPDATE ------------------
    LimitMousePosition(&mousePosition);
    UpdatePlayerPosition(&player, deltaTime);
    UpdatePlayerScreenBounds(&player);
    UpdatePlayerRotation(&player, mousePosition);

    // ------------------ DRAW ------------------
    DrawCrosshair(mousePosition);
    DrawPlayer(&playerTexture, &player);

    EndDrawing();
  }
  
  UnloadTexture(playerTexture);
  CloseWindow();
  return 0;
}

static void LimitMousePosition(Vector2* const p_mousePosition)
{
  if (p_mousePosition->x < 0.0f)
    p_mousePosition->x = 0.0f;
  else if (p_mousePosition->x > SCREEN_WIDTH)
    p_mousePosition->x = (float)SCREEN_WIDTH;

  if (p_mousePosition->y < 0)
    p_mousePosition->y = 0.0f;
  else if (p_mousePosition->y > SCREEN_HEIGHT)
    p_mousePosition->y = SCREEN_HEIGHT;

  #ifdef DEBUG
    printf("mouse x = %f, mouse y = %f\n", p_mousePosition->x, p_mousePosition->y);
  #endif
}

static void UpdatePlayerScreenBounds(Player_t* const p_player)
{
  float playerLeft    = p_player->m_coordinates.x - p_player->m_size.x / 2;
  float playerRight   = p_player->m_coordinates.x + p_player->m_size.x / 2;
  float playerBottom  = p_player->m_coordinates.y + p_player->m_size.y / 2;
  float playerTop     = p_player->m_coordinates.y - p_player->m_size.y / 2;

  if (playerLeft < 0)
    p_player->m_coordinates.x = p_player->m_size.x / 2;
  else if (playerRight > SCREEN_WIDTH)
    p_player->m_coordinates.x = SCREEN_WIDTH - p_player->m_size.x / 2;

  if (playerTop < 0)
    p_player->m_coordinates.y = p_player->m_size.y / 2;
  else if (playerBottom > SCREEN_HEIGHT)
    p_player->m_coordinates.y = SCREEN_HEIGHT - p_player->m_size.y / 2;
}

static void UpdatePlayerRotation(Player_t* player, Vector2 mousePosition)
{
  Vector2 direction = Vector2Subtract(mousePosition, player->m_coordinates);
  player->m_rotation_degrees = atan2f(direction.y, direction.x) * (180.0f / PI);
}

static void UpdatePlayerPosition(Player_t* const p_player, float deltaTime)
{
  if (IsKeyDown(KEY_D))
    PlayerGoRight(p_player, deltaTime);
  if (IsKeyDown(KEY_A))
    PlayerGoLeft(p_player, deltaTime);
  if (IsKeyDown(KEY_W))
    PlayerGoUp(p_player, deltaTime);
  if (IsKeyDown(KEY_S))
    PlayerGoDown(p_player, deltaTime);
}

static void DrawPlayer(const Texture2D* const p_playerTexture, const Player_t* const p_player)
{
  DrawTexturePro( *p_playerTexture, 
                  (Rectangle){p_player->m_location_inside_image.x, 
                              p_player->m_location_inside_image.y, 
                              p_player->m_size.x, 
                              p_player->m_size.y}, 
                  (Rectangle){p_player->m_coordinates.x, 
                              p_player->m_coordinates.y, 
                              p_player->m_size.x, 
                              p_player->m_size.y}, 
                  (Vector2){p_player->m_size.x / 2, p_player->m_size.y / 2}, 
                  p_player->m_rotation_degrees, 
                  WHITE);
}

static void DrawCrosshair(Vector2 mousePosition)
{
  // Clamp the crosshair center position for drawing so the whole crosshair stays visible
  const float halfWidth = (float)CROSSHAIR_WIDTH / 2.0f;
  const float halfHeight = (float)CROSSHAIR_HEIGHT / 2.0f;
  
  // Define the valid bounds for the crosshair center
  Vector2 minBounds = { halfWidth, halfHeight };
  Vector2 maxBounds = { SCREEN_WIDTH - halfWidth, SCREEN_HEIGHT - halfHeight };
  
  // Clamp the mouse position to keep crosshair fully on screen
  Vector2 center = Vector2Clamp(mousePosition, minBounds, maxBounds);
  
  // Define offsets for crosshair arms using vector math
  Vector2 horizontalOffset = { halfWidth, 0.0f };
  Vector2 verticalOffset = { 0.0f, halfHeight };
  
  // Draw horizontal line (left to right)
  DrawLineEx(Vector2Subtract(center, horizontalOffset), 
             Vector2Add(center, horizontalOffset), 
             CROSSHAIR_THICKNESS,
             CROSSHAIR_COLOR);
  
  // Draw vertical line (top to bottom)
  DrawLineEx(Vector2Subtract(center, verticalOffset), 
             Vector2Add(center, verticalOffset), 
             CROSSHAIR_THICKNESS,
             CROSSHAIR_COLOR);
}