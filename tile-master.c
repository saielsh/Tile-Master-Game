#include<stdio.h>
#include<raylib.h>
#include<stdlib.h>
#include<time.h>
#include<stdbool.h>

int main(){

    const int screenwidth = 800;
    const int screenheight = 600;

    // getting random value
    SetRandomSeed(time(NULL));

    // variables
    int velocity = 0;
    int gap = 7;
    int speed = 10;
    bool game = false;
    bool turn = false;
    bool out = false;
    int score = 0;

    // player
    Rectangle player = {(screenwidth/2) - 30, screenheight - 30, 60, 10}; 
    

    // ball
    Vector2 ball = {screenwidth/2, screenheight-170};
    Vector2 oldball = ball;

    int radius = 5;
    int bspeed = 3;

    // tile
    int totaltile = 238;
    Rectangle t[totaltile];
    bool active[totaltile];

    // putting value inside first tile
    t[0].width = 50;
    t[0].height = 20;
    t[0].x = gap;
    t[0].y = 5;
    active[0] = true;

    // putting value inside other tiles
    for(int i=1; i<totaltile; i++){

        t[i].width = t[i-1].width;
        t[i].height = t[i-1].height;

        if(i % 14 == 0){

            t[i].y = (t[i-1].y + t[i].height) + 5;
            t[i].x = gap;

        }
        else{

            t[i].x = (t[i-1].x + t[i].width) + gap;
            t[i].y = t[i-1].y;

        }

        active[i] = true;
    }

    InitWindow(screenwidth, screenheight, "Tile Master");
    SetTargetFPS(60);

    while(!WindowShouldClose()){

        // Start screen
        if(game == false && out == false){

            if(IsKeyPressed(KEY_ENTER)){

                game = true;

            }

        }

        // Restart logic
        if(out == true){

            if(IsKeyPressed(KEY_ENTER)){

                // reset player
                player.x = (screenwidth/2) - 30;

                // reset ball
                ball.x = screenwidth/2;
                ball.y = screenheight-170;

                // reset ball movement
                velocity = 0;
                turn = false;

                // reset tiles
                for(int i=0; i<totaltile; i++){

                    active[i] = true;

                }
                score = 0;

                game = true;
                out = false;

            }

        }

        // player movement
        if(game == true){

            if(IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)){
                player.x -= speed;
            }

            if(IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)){
                player.x += speed;
            }

            // collision for player and game border
            if(player.x < 0){
                player.x = 0;
            }

            if(player.x > (screenwidth - player.width)){
                player.x = screenwidth - player.width;
            }

            // store previous ball position
            oldball = ball;

            // logic for ball
            if(turn == true){

                ball.y -= bspeed;
                ball.x += velocity;

            }
            else{

                ball.y += bspeed;
                ball.x += velocity;

            }

            // collision for ball and tile
            for(int i=0; i<totaltile; i++){

                if(active[i] == true){

                    if(CheckCollisionCircleRec(ball, radius, t[i])){

                        // destroy tile
                        active[i] = false;
                        score += 10;

                        // reverse vertical direction
                        turn = !turn;

                        // prevent ball from staying inside tile
                        if(oldball.y < t[i].y){

                            ball.y = t[i].y - radius;

                        }
                        else if(oldball.y > t[i].y + t[i].height){

                            ball.y = t[i].y + t[i].height + radius;

                        }
                        else{

                            // horizontal collision
                            if(oldball.x < t[i].x){

                                ball.x = t[i].x - radius;
                                velocity = -abs(velocity);

                            }
                            else{

                                ball.x = t[i].x + t[i].width + radius;
                                velocity = abs(velocity);

                            }

                        }

                        // only collide with one tile per frame
                        break;

                    }

                }

            }

            // collision for player and ball
            if(turn == false &&
               CheckCollisionCircleRec(ball, radius, player)){

                turn = true;

                // prevent ball from getting stuck inside player
                ball.y = player.y - radius;

                // random horizontal movement
                velocity = GetRandomValue(-5, 5);

            }

            // collision for ball and left border
            if(ball.x - radius <= 0){

                ball.x = radius;
                velocity = abs(velocity);

            }

            // collision for ball and right border
            if(ball.x + radius >= screenwidth){

                ball.x = screenwidth - radius;
                velocity = -abs(velocity);

            }

            // collision for ball and top border
            if(ball.y - radius <= 0){

                ball.y = radius;
                turn = false;

            }

            // Gameover logic
            if(ball.y - radius >= screenheight){

                game = false;
                out = true;

            }

        }

        // Drawing
        BeginDrawing();

        ClearBackground(BLACK);

        // Start screen
        if(game == false && out == false){

            DrawText("TILE MASTER", 280, 200, 40, WHITE);
            DrawText("Press ENTER to start", 200, 350, 30, WHITE);

        }

        // Gameover screen
        if(out == true){

            DrawText("GAME OVER", 280, 200, 40, RED);
            DrawText("Press ENTER to Restart", 200, 350, 30, WHITE);
            DrawText("Developed by Sailesh Dahal", 10, 580, 20, GREEN);
            // total score
            DrawText(TextFormat("Total Score: %d", score), 280, 300, 30, YELLOW);

        }

        // Game screen
        if(game == true){

            // Drawing tiles
            for(int i=0; i<totaltile; i++){

                if(active[i] == true){

                    if(t[i].x <= 399){

                        DrawRectangleRec(t[i], BLUE);

                    }
                    else{

                        DrawRectangleRec(t[i], RED);

                    }

                }

            }

            DrawRectangleRec(player, WHITE);
            DrawCircleV(ball, radius, GREEN);
            // drawig score
            DrawText(TextFormat("Score: %d", score), 10, 10, 25, WHITE);
            DrawText("Developed by Sailesh Dahal", 10, 580, 20, GREEN);

        }

        EndDrawing();

    }

    CloseWindow();

    return 0;
}