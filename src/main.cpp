#include <raylib.h>
#include <raymath.h>
#include <deque>
#define width cellSize*cellCount
#define height cellSize*cellCount



Color blue = {70, 70, 245, 255};
Color darkBlue = {0, 0, 15, 255};

int cellSize = 30;
int cellCount = 25;

class Snake {
public:
    std::deque  <Vector2> body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
    Vector2 direction = {1, 0};

    void Draw(){
        for(unsigned int i = 0; i < body.size(); i++){
            float x = body[i].x;
            float y = body[i].y;
            Rectangle segment = Rectangle{x*cellSize, y*cellSize, cellSize, cellSize};
            DrawRectangleRounded(segment, .5, 3, blue);
        }
    }

    void Update(){
        direction.x = IsKeyDown(KEY_RIGHT)-IsKeyDown(KEY_LEFT);
        direction.y = IsKeyDown(KEY_UP)-IsKeyDown(KEY_DOWN);

        body.pop_back();
        body.push_front(Vector2Add(body[0], direction));
    }
};

class Food {
public:
    Vector2 position;
    Texture2D texture;

    Food()
    {
        position = GenerateRandomPos();
        Image image = LoadImage("assets/food.png");
        texture = LoadTextureFromImage(image);
        UnloadImage(image);
    }

    ~Food(){
        UnloadTexture(texture);
    }


    void Draw(){
        DrawTexture(texture, position.x*cellSize, position.y*cellSize, blue);
    }

    Vector2 GenerateRandomPos(){
        float x = GetRandomValue(0, cellCount-1);
        float y = GetRandomValue(0, cellCount-1);
        return Vector2{x, y};
    }
};


int main() 
{
    InitWindow(width, height, "Pong Game Raylib");
    SetTargetFPS(60);

    Snake snake = Snake();
    Food food = Food();

    while(!WindowShouldClose()){
        
        
        BeginDrawing();

        snake.Update();

        ClearBackground(darkBlue);
        snake.Draw();
        food.Draw();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}