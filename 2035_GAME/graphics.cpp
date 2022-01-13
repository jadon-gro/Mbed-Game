#include "graphics.h"

#include "globals.h"

#include "assets.h"

#include "map.h"

/*
In this file put all your graphical functions (don't forget to declare them first
in graphics.h). So when you want to draw something use this file. One cool function
to look at would be uLCD.blit() there are more like filled_rectangle etc... 
https://os.mbed.com/users/4180_1/notebook/ulcd-144-g2-128-by-128-color-lcd/
This website is a good resource.
*/

void draw_player(int u, int v, int key, int direction)
{
    //if (direction == 2) {
//        //up
//         uLCD.BLIT(u, v, 11, 11, playerBack);
//    } else if (direction == 3) {
//        //left
//         uLCD.BLIT(u, v, 11, 11, playerLeft);
//    } else if (direction == 0) {
//        //down
//         uLCD.BLIT(u, v, 11, 11, playerFront);
//    } else if (direction == 1) {
//        //right
//         uLCD.BLIT(u, v, 11, 11, playerRight);
//    }
    uLCD.filled_rectangle(u, v, u+10, v+10, RED);

}

#define YELLOW 0xFFFF00
#define BROWN  0xD2691E
#define PURPLE 0x6A0DAD
#define DIRT   BROWN
#define WHITE  0xFFFFFF
void draw_img(int u, int v, const char* img)
{
    int colors[11*11];
    for (int i = 0; i < 11*11; i++)
    {
        if (img[i] == 'R') colors[i] = RED;
        else if (img[i] == 'Y') colors[i] = YELLOW;
        else if (img[i] == 'G') colors[i] = GREEN;
        else if (img[i] == 'D') colors[i] = DIRT;
        else if (img[i] == '5') colors[i] = LGREY;
        else if (img[i] == '3') colors[i] = DGREY;
        else colors[i] = BLACK;
    }
    uLCD.BLIT(u, v, 11, 11, colors);
    wait_us(250); // Recovery time!
}

void draw_nothing(int u, int v)
{
    // Fill a tile with blackness
    uLCD.filled_rectangle(u, v, u+10, v+10, BLACK);
}

void draw_wall(int u, int v)
{
    uLCD.BLIT(u, v, 11, 11, (int*)brick);
}

void draw_plant(int u, int v)
{
    pc.printf("t1\n");
    if (u < 0 || v < 0) {
        return;
        
    }
    if (get_map_index() == 0) uLCD.BLIT(u, v, 11, 11, (int*)tree);
    if (get_map_index() == 1) uLCD.BLIT(u, v, 11, 11, deadTree);
    pc.printf("t2\n");
}

void draw_portal(int u, int v)
{
    uLCD.BLIT(u, v, 11, 11, portal);
}

void draw_NPC(int u, int v)
{
    uLCD.BLIT(u, v, 11, 11, npc);
}
void draw_monster(int u, int v)
{
    uLCD.BLIT(u, v, 11, 11, monster);
}
void draw_monster2(int u, int v)
{
    uLCD.BLIT(u, v, 11, 11, blueMonster);
}
void draw_door(int u, int v)
{
    uLCD.filled_rectangle(u, v, u+10, v+10, BROWN);
    
}
void draw_upper_status(int x, int y, int k)
{
    // Draw bottom border of status bar
    uLCD.filled_rectangle(0, 0, 127, 8, WHITE);
    uLCD.locate(0, 0);
    uLCD.color(BLACK);
    uLCD.textbackground_color(WHITE);
    uLCD.printf("x: %d, y: %d", x, y);
    if (k == 0 or k == 2) uLCD.printf(", K: N");
    if (k == 1) uLCD.printf(", K: Y");
}

void draw_lower_status(int b, int g, int k)
{
    // Draw top border of status bar
    uLCD.filled_rectangle(0, 117, 127, 127, WHITE);
    uLCD.locate(1, 15);
    uLCD.color(BLACK);
    uLCD.textbackground_color(WHITE);
    uLCD.printf("Inventory:");
    if (b) uLCD.BLIT(78, 117, 11, 11, bluePotion);
    if (g) uLCD.BLIT(93, 117, 11, 11, greenPotion);
    if (k == 1)  uLCD.BLIT(85, 117, 11, 11, key);
    
    // Add other status info drawing code here
}

void draw_border()
{
    uLCD.filled_rectangle(0,     9, 127,  14, WHITE); // Top
    uLCD.filled_rectangle(0,    13,   2, 114, WHITE); // Left
    uLCD.filled_rectangle(0,   114, 127, 117, WHITE); // Bottom
    uLCD.filled_rectangle(124,  14, 127, 117, WHITE); // Right
}


