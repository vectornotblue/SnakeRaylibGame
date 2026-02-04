#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <deque>
#include <string>
#define width cellSize*cellCount
#define height cellSize*cellCount



Color blue = {70, 70, 245, 255};
Color darkBlue = {5, 5, 40, 255};

int cellSize = 30;
int cellCount = 25;
int offset = 75;


bool ElementInDeque(Vector2 element, std::deque<Vector2> deque){
    for (unsigned int i = 0; i < deque.size(); i++){
        if(Vector2Equals(deque[i], element)){
            return true;
        }
    }
    return false;
}

double lastUpdateTime = 0;
bool eventTriggered(double interval){
    double currentTime = GetTime();
    if(currentTime - lastUpdateTime >= interval)
    {
        lastUpdateTime = currentTime;
        return true;
    } 
    return false;
}

class Snake {
public:
    std::deque  <Vector2> body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
    Vector2 direction = {1, 0};
    Vector2 nextDirection = direction;    

    bool addSegment = false;

    void Draw(){
        for(unsigned int i = 0; i < body.size(); i++){
            float x = body[i].x;
            float y = body[i].y;
            Rectangle segment = Rectangle{offset + x*cellSize, offset + y*cellSize, cellSize, cellSize};
            DrawRectangleRounded(segment, .5, 3, WHITE);
        }
    }

    void Update(){
        direction = nextDirection;
        body.push_front(Vector2Add(body[0], direction));
        if (addSegment){
            addSegment = false;
        }else{
            body.pop_back();
            
        }
        
    }

    void Reset(){
        body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
        direction = {1, 0};
        nextDirection = direction;
    }
};

class Food {
public:
    Vector2 position;
    Texture2D texture;

    Food(std::deque<Vector2> snakeBody)
    {
        position = GenerateRandomPos(snakeBody);
        Image image = LoadImage("assets/food.png");
        texture = LoadTextureFromImage(image);
        UnloadImage(image);
    }

    ~Food(){
        UnloadTexture(texture);
    }


    void Draw(){
        DrawTexture(texture,offset + position.x*cellSize,offset + position.y*cellSize, WHITE);
    }


    Vector2 GenerateRandomCell(){
        float x = GetRandomValue(0, cellCount-1);
        float y = GetRandomValue(0, cellCount-1);
        return Vector2{x,y};
    }

    Vector2 GenerateRandomPos(std::deque<Vector2> snakeBody){

        position = GenerateRandomCell();
        while(ElementInDeque(position, snakeBody)){
            position = GenerateRandomCell(); 
        } return position;              
    }
};

class Game
{
public:
    int score = 0;
    Snake snake = Snake();
    Food food = Food(snake.body);
    bool running = true;


    void Draw()
    {
        food.Draw();
        snake.Draw();
    }

    void Update()
    {
        if (running){
            snake.Update();
            CheckCollisionWithFood();
            CheckCollisionWithEdges();
            CheckCollisionWithTail();
        }
    }

    bool CheckCollisionWithFood(){
        if(Vector2Equals(snake.body[0], food.position))
        { 
            food.position = food.GenerateRandomPos(snake.body);
            snake.addSegment = true;
            score ++;
            return true;
        }
        return false;
    }

    void CheckCollisionWithEdges(){
        if((snake.body[0].x > cellCount) || (snake.body[0].x < 0) || (snake.body[0].y > cellCount) || (snake.body[0].y < 0))
        {
            GameOver();
        }
    }
    void CheckCollisionWithTail(){
        for(unsigned int i = 1; i < snake.body.size(); i++){
            if(Vector2Equals(snake.body[0], snake.body[i]))
            {
                GameOver();
            }
        }
    }

    void GameOver(){
        snake.Reset();
        score = 0;
        food.position = food.GenerateRandomPos(snake.body);
        running = false;
    }
};

void DrawCheckers();

int main() 
{
    InitWindow(width+2*offset, height+2*offset, "Pong Game Raylib");
    SetTargetFPS(60);
    

    Game game = Game();

    while(!WindowShouldClose()){
        
        

        BeginDrawing();

        if(eventTriggered(.2)){
            game.Update();
        }
        
        if(IsKeyPressed(KEY_UP) && game.snake.direction.y != 1) {
            game.running = 1;
            game.snake.nextDirection = {0, -1};
            
        }
        if (IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1) {
            game.running = 1;
            game.snake.nextDirection = {0, 1};
            
        }
    
        if(IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1) {
            game.running = 1;
            game.snake.nextDirection = {1, 0};
        }
        if(IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1) {
            game.running = 1;
            game.snake.nextDirection = {-1, 0};
        }
        
        
        
        ClearBackground(BLACK);

        DrawRectangleLinesEx(Rectangle{(float)offset-5, (float)offset-5, (float)width+10, (float)height+10}, 5, WHITE);
        DrawCheckers();
        DrawText("Raylib Snake Game", offset-5, 20, 40, WHITE);
        std::string textscore = "Score: "  + std::to_string(game.score);
        DrawText(textscore.c_str(), width*4/5 , 20, 40, WHITE);
        game.Draw();
        EndDrawing();
    }

    CloseWindow();
    return 0;
} 

void DrawCheckers(){
    
    for(int i = 0; i< cellCount; i++){
        for (int j = 0; j < cellCount; j++){
            if((i+j)%2 == 0){
                Rectangle checkerBlock = Rectangle{i*cellSize+offset, j*cellSize+offset, cellSize, cellSize};
                DrawRectangleRounded(checkerBlock, .2, 3, darkBlue);
            }
        }
    }
}