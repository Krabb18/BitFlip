#include "raylib.h"
#include "miniaudio.h"
#include <vector>
#include <iostream>

#include "SoundPlayer.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

using namespace std;

bool inAddingMode = false;
int boxToAddNumber = 0;

struct SoundBox
{
    int boxNum = 0;
    Vector2 boxPos;
    Vector2 boxScale;
};

vector<SoundBox> soundBoxes;

int allX = 0;
int allY = 300;
bool isDragging = false;

bool isPlaying = false;

SoundBox playerBox;

void HandlePlayLine();
void HandleRecording();

SoundPlayer soundPlayer;

int main()
{
    InitWindow(1400, 900, "BitFlip");
    SetTargetFPS(60);

    soundPlayer.ClearEncoder();
    soundPlayer.InitPlayer();

    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(GRAY);

            soundPlayer.UpdatePlayer();
            Vector2 mousePos = GetMousePosition();
            

            //playerBox.boxPos.x = 0;
            playerBox.boxScale.y = 900;
            playerBox.boxScale.x = 3;
            DrawRectangle(playerBox.boxPos.x, playerBox.boxPos.y, playerBox.boxScale.x, playerBox.boxScale.y, PURPLE);
            DrawRectangle(playerBox.boxPos.x, playerBox.boxPos.y + 100, 30, 30, PURPLE);
            if(CheckCollisionRecs((Rectangle){ mousePos.x, mousePos.y, 100, 100 }, (Rectangle){ playerBox.boxPos.x, playerBox.boxPos.y + 100, 30, 30 }) && IsMouseButtonDown(MOUSE_BUTTON_LEFT))
            {
                isDragging = true;
                playerBox.boxPos.x = mousePos.x;
            }
            
            GuiPanel((Rectangle){ 0, 0, 1400, 100 }, " ");
            
            if(!isPlaying)
            {
                if (GuiButton((Rectangle){ 10, 40, 30, 20 }, "#133#")){ isPlaying = true;}
            }
            else
            {
                if (GuiButton((Rectangle){ 10, 40, 30, 20 }, "#134#")){ isPlaying = false;}
            }
            

            if (GuiButton((Rectangle){ 0 + allX, 34 + allY, 320, 80 }, "Add")){inAddingMode = true; boxToAddNumber = 0;}
            if (GuiButton((Rectangle){ 0 + allX, 114 + allY, 320, 80 }, "Add")){inAddingMode = true; boxToAddNumber = 1;}
            if (GuiButton((Rectangle){ 0 + allX, 194 + allY, 320, 80 }, "Add")){inAddingMode = true; boxToAddNumber = 2;}

            if(inAddingMode)
            {
                Color col = RED;
                int yPos = 0;
                if(boxToAddNumber == 0){col = RED; yPos = 32;}
                else if(boxToAddNumber == 1){col = BLUE; yPos = 114;}
                else if(boxToAddNumber == 2){col = YELLOW; yPos = 194;}

                DrawRectangle(mousePos.x, yPos + allY, 320, 80, col);
                if(IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                {
                    SoundBox soundBoxInst;
                    soundBoxInst.boxNum = boxToAddNumber;
                    soundBoxInst.boxPos.x = mousePos.x;
                    soundBoxInst.boxPos.y = yPos + allY;
                    soundBoxInst.boxScale.x = 320;
                    soundBoxInst.boxScale.y = 80;
                    soundBoxes.push_back(soundBoxInst);
                    inAddingMode = false;
                }

                if(IsMouseButtonDown(MOUSE_BUTTON_RIGHT)){inAddingMode = false;}

            }

            

            //Draw all soundboxes
            bool playSound1 = false;
            bool playSound2 = false;
            bool playSound3 = false;
            int counter = 0;
            for(auto &box : soundBoxes)
            {
                Color col = RED;
                if(box.boxNum == 0){col = RED;}
                else if(box.boxNum == 1){col = BLUE;}
                else if(box.boxNum == 2){col = YELLOW;}

                DrawRectangle(box.boxPos.x, box.boxPos.y, 320, 80, col);

                if(CheckCollisionRecs((Rectangle){ mousePos.x, mousePos.y, 300, 100 }, (Rectangle){ box.boxPos.x,box.boxPos.y + 100, 320, 80 }) && IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                {
                    box.boxPos.x = mousePos.x;
                }
                else if(CheckCollisionRecs((Rectangle){ mousePos.x, mousePos.y, 300, 100 }, (Rectangle){ box.boxPos.x,box.boxPos.y + 100, 320, 80 }) && IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
                {
                    soundBoxes.erase(soundBoxes.begin() + counter);
                    continue;
                }

                
                if(isPlaying && CheckCollisionRecs((Rectangle){playerBox.boxPos.x, playerBox.boxPos.y, playerBox.boxScale.x, playerBox.boxScale.y }, (Rectangle){box.boxPos.x, box.boxPos.y, box.boxScale.x, box.boxScale.y }))
                {

                    cout << "Collliding" << endl;
                    if(box.boxNum == 0){playSound1 = true;}
                    else if(box.boxNum == 1){playSound2 = true;}
                    else if(box.boxNum == 2){playSound3 = true;}
                }

                counter++;

            }

            soundPlayer.playSound1 = playSound1;
            soundPlayer.playSound2 = playSound2;
            soundPlayer.playSound3 = playSound3;


        HandleRecording();
        HandlePlayLine();
        EndDrawing();
    }

    soundPlayer.Delete();
    CloseWindow();
    return 0;
}

void HandleRecording()
{
    if(!soundPlayer.isRecording)
    {
        if (GuiButton((Rectangle){ 60, 40, 30, 20 }, "#135#")){ soundPlayer.isRecording = true;}
    }
    else
    {
        if (GuiButton((Rectangle){ 60, 40, 30, 20 }, "#134#")){ soundPlayer.isRecording = false;}
    }
}

void HandlePlayLine()
{
    if(isPlaying)
    {
        playerBox.boxPos.x += 100.0f * GetFrameTime();

        //Hier glecih gucken welche blöcke alles berührt werden
    }
    else
    {
        if(!isDragging){playerBox.boxPos.x = allX + 320;}
    }
}