#include <raylib.h>
#include <iostream>

Color Green = Color{38, 185, 154, 255};
Color Dark_Green = Color{20, 160, 133, 255};
Color Light_Green = Color{129, 204, 184, 255};
Color Yellow = Color{243, 213, 91, 255};

int player_score1 = 0;
int player_score2 = 0;
bool gameOver = false;
bool inMenu = true;

class Ball {
public:
    float x, y;
    int speed_x, speed_y;
    int radius;

    void Draw() {
        DrawCircle(x, y, radius, Yellow);
    }

    void Update() {
        x += speed_x;
        y += speed_y;

        if (y + radius >= GetScreenHeight() || y - radius <= 0) {
            speed_y *= -1;
        }
        // Player 2 wins
        if (x + radius >= GetScreenWidth()) {
            player_score2++;
            ResetBall();
        }

        // Player 1 wins
        if (x - radius <= 0) {
            player_score1++;
            ResetBall();
        }
    }

    void ResetBall() {
        x = GetScreenWidth() / 2;
        y = GetScreenHeight() / 2;

        int speed_choices[2] = {-1, 1};
        speed_x *= speed_choices[GetRandomValue(0, 1)];
        speed_y *= speed_choices[GetRandomValue(0, 1)];
    }
};

class Paddle {
protected:
    void LimitMovement() {
        if (y <= 0) {
            y = 0;
        }
        if (y + height >= GetScreenHeight()) {
            y = GetScreenHeight() - height;
        }
    }

public:
    float x, y;
    float width, height;
    int speed;

    void Draw() {
        DrawRectangleRounded(Rectangle{x, y, width, height}, 0.8, 0, WHITE);
    }

    void Update(int upKey, int downKey) {
        if (IsKeyDown(upKey)) {
            y -= speed;
        }
        if (IsKeyDown(downKey)) {
            y += speed;
        }
        LimitMovement();
    }
};

Ball ball;
Paddle player1;
Paddle player2;

void ResetGame() {
    player_score1 = 0;
    player_score2 = 0;
    gameOver = false;
    ball.ResetBall();
}

void ShowHomePage() {
    ClearBackground(Dark_Green);
    DrawText("Pong Game", 440, 200, 80, WHITE);
    DrawText("Press Enter to Start", 420, 400, 40, WHITE);
}

void ShowGameOverPage() {
    ClearBackground(Dark_Green);
    DrawText("Game Over!", 440, 200, 80, WHITE);
    if (player_score1 >= 5) {
        DrawText("Player 1 Wins!", 520, 300, 40, WHITE);
    } else {
        DrawText("Player 2 Wins!", 520, 300, 40, WHITE);
    }
    DrawText("Press R to Play Again", 460, 400, 40, WHITE);
}

int main() {
    std::cout << "Starting the game" << std::endl;
    const int screen_width = 1280;
    const int screen_height = 800;
    InitWindow(screen_width, screen_height, "My Pong Game!");
    SetTargetFPS(60);
    ball.radius = 20;
    ball.x = screen_width / 2;
    ball.y = screen_height / 2;
    ball.speed_x = 7;
    ball.speed_y = 7;

    // Player 1 settings
    player1.width = 25;
    player1.height = 120;
    player1.x = 10; // Left side
    player1.y = screen_height / 2 - player1.height / 2;
    player1.speed = 8;

    // Player 2 settings
    player2.width = 25;
    player2.height = 120;
    player2.x = screen_width - player2.width - 10; // Right side
    player2.y = screen_height / 2 - player2.height / 2;
    player2.speed = 3;

    while (WindowShouldClose() == false) {
        BeginDrawing();

        if (inMenu) {
            ShowHomePage();
            if (IsKeyPressed(KEY_ENTER)) {
                inMenu = false; // Start the game
            }
        } else {
            if (!gameOver) {
                // Updating
                ball.Update();
                player1.Update(KEY_W, KEY_S); // Player 1 controls
                player2.Update(KEY_UP, KEY_DOWN); // Player 2 controls

                // Checking for collisions
                if (CheckCollisionCircleRec({ball.x, ball.y}, ball.radius, {player1.x, player1.y, player1.width, player1.height})) {
                    ball.speed_x *= -1;
                }

                if (CheckCollisionCircleRec({ball.x, ball.y}, ball.radius, {player2.x, player2.y, player2.width, player2.height})) {
                    ball.speed_x *= -1;
                }

                // Check for game over
                if (player_score1 >= 5 || player_score2 >= 5) {
                    gameOver = true;
                }

                // Drawing
                ClearBackground(Dark_Green); // Clear the previous frame
                DrawRectangle(screen_width / 2, 0, screen_width / 2, screen_height, Green);
                DrawCircle(screen_width / 2, screen_height / 2, 150, Light_Green);
                DrawLine(screen_width / 2, 0, screen_width / 2, screen_height, WHITE);
                ball.Draw();
                player1.Draw();
                player2.Draw();
                DrawText(TextFormat("%i", player_score1), screen_width / 4 - 20, 20, 80, WHITE);
                DrawText(TextFormat("%i", player_score2), 3 * screen_width / 4 - 20, 20, 80, WHITE);
            } else {
                // Game Over page
                ShowGameOverPage();
                if (IsKeyPressed(KEY_R)) {
                    ResetGame();
                }
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
