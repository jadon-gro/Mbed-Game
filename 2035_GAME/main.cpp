// Project includes
#include "globals.h"
#include "hardware.h"
#include "map.h"
#include "graphics.h"
#include "speech.h"
#include "assets.h"

// Functions in this file
int get_action (GameInputs inputs);
int update_game (int action);
void draw_game (int init);
void init_main_map ();
int main ();

/**
 * The main game state. Must include Player locations and previous locations for
 * drawing to work properly. Other items can be added as needed.
 */
struct {
    int x,y;    // Current locations
    int px, py; // Previous locations
    
    // 0 - down
    // 1 - right
    // 2 - up
    // 3 - left
    int direction; // Direction Player is facing
    
    // -1 - no quest
    // 0  - after quest begins but no key
    // 1  - after quest begins and has key
    int has_key;
    // You can add other properties for the Player here
    int has_bluePotion;
    
    int has_greenPotion;
    
    int dead;
} Player;

/**
 * Given the game inputs, determine what kind of update needs to happen.
 * Possbile return values are defined below.
 */
#define NO_ACTION 0
#define ACTION_BUTTON 1
#define MENU_BUTTON 2
#define GO_LEFT 3
#define GO_RIGHT 4
#define GO_UP 5
#define GO_DOWN 6
int get_action(GameInputs inputs)
{
    if (inputs.bU == 1) {
        return MENU_BUTTON;
    }
    if (inputs.bR == 1) {
        return ACTION_BUTTON;
    }
    if (abs(inputs.ax) > abs(inputs.ay)) {
        if (inputs.ax > .3) {
            return GO_DOWN;
        }
        if (inputs.ax < -.3) {
            return GO_UP;
        }
        return NO_ACTION;
    }
    if (abs(inputs.ax) < abs(inputs.ay)) {
        if (inputs.ay > .3) {
            return GO_LEFT;
        }
        if (inputs.ay < -.3) {
            return GO_RIGHT;
        }
        return NO_ACTION;
    }
    
    return NO_ACTION;
}

/**
 * Update the game state based on the user action. For example, if the user
 * requests GO_UP, then this function should determine if that is possible by
 * consulting the map, and update the Player position accordingly.
 * 
 * Return values are defined below. FULL_DRAW indicates that for this frame,
 * draw_game should not optimize drawing and should draw every tile, even if
 * the Player has not moved.
 */
#define NO_RESULT 0
#define GAME_OVER 1
#define FULL_DRAW 2
#define MONSTERX 40
#define MONSTERY 6
#define MONSTER2X 20
#define MONSTER2Y 20
#define DOORX    25
#define DOORY    40
int update_game(int action, GameInputs in)
{
    // Save Player previous location before updating
    Player.px = Player.x;
    Player.py = Player.y;
    
    // Do different things based on the each action.
    // You can define functions like "go_up()" that get called for each case.
    switch(action)
    {
        case GO_UP:     
        Player.direction = 2;
        if (Player.y != map_height() - 1) {
            if (in.bL == 1 || get_north(Player.x, Player.y)->walkable) {
              Player.y = Player.y + 1;
            }
        }
        return 2;
        break;
        case GO_LEFT:
        Player.direction = 3;
        if (Player.x != 0) {
            if (in.bL == 1 || get_west(Player.x, Player.y)->walkable) {
              Player.x = Player.x - 1;
              return 2;
            }
        }
        
        break;            
        case GO_DOWN:
        Player.direction = 0;
        if (Player.y != 0) {
            if (in.bL == 1 || get_south(Player.x, Player.y)->walkable) {
                Player.y = Player.y - 1;
            }
        }
        return 2;
        
        break;
        case GO_RIGHT:
        Player.direction = 1;
        if (Player.x != map_width() - 1) {
            if (in.bL == 1 || get_east(Player.x, Player.y)->walkable) {
                Player.x = Player.x + 1;
            }
        }
        return 2;
        break;
        case ACTION_BUTTON:
        
        //Portal
        MapItem* north = get_south(Player.x, Player.y);
        if (north->type == PORTAL) {
            StairsData theData= *(StairsData*) get_south(Player.x, Player.y)->data;
            set_active_map(theData.tm);
            Player.x = theData.tx;
            Player.y = theData.ty;
            return 2;
        }   
               // 0 - down
               // 1 - right
             // 2 - up
          // 3 - left
          
        // This portion deals with talking with the NPC
        if ((get_north(Player.x, Player.y)->type == NPC and Player.direction == 2) or (get_south(Player.x, Player.y)->type == NPC and Player.direction == 0) or (get_west(Player.x, Player.y)->type == NPC and Player.direction == 3) or (get_east(Player.x, Player.y)->type == NPC and Player.direction == 1)) {  
            if (Player.has_key == -1) {
                //uLCD.BLIT(59, 31, 11, 11, exclamation);
//                wait_ms(500);
//                draw_game(true);
                const char* speech1[] = {"Hello traveler!", "My name is Joe!", "I need you to", "kill two", "monsters!", "I promise you", "It's worth it!"};
                long_speech(speech1, 5, 0, 1);
                const char* options[] = {"Heck yes!", "No way!!"};
                int choice = prompt(options, 2);
                if (!choice) {
                    add_portal(25, 10, 25, 25, 1);
                    speech("Ok! I've opened", "a portal!", 1, 0);
                    speech("Here's a potion", "to kill each!", 1, 0);
                    Player.has_key = 0;
                    Player.has_bluePotion = 1;
                    Player.has_greenPotion = 1;
                    draw_game(true);
                }
                if (choice) {
                    speech("No worries! come", "back if you want!", 1, 0);
                    draw_game(true);
                }
                wait_ms(200);
            } else {
                if (Player.has_key == 0) {
                    speech("Good luck!", "I'll wait here...", 1, 0);
                    draw_game(true);
                }
            }
            if (Player.has_key == 2) {
                const char* speech2[] = {"YAY, we've", "been saved! :)", "Here's a key", "For your effort."};
                long_speech(speech2, 4, 0, 0);
                Player.has_key = 1;
                draw_game(true);
            }
        }
        if ((get_north(Player.x, Player.y)->type == MONSTER and Player.direction == 2) or (get_south(Player.x, Player.y)->type == MONSTER and Player.direction == 0) or (get_west(Player.x, Player.y)->type == MONSTER and Player.direction == 3) or (get_east(Player.x, Player.y)->type == MONSTER and Player.direction == 1)) {
            const char* spells[] = {"G Potion", "Sludge", "Medicine", "B Potion"};
            const char* spells2[] = {"G Potion", "Sludge", "Medicine"};
            speech("What should I", "throw at it?", 1, 1);
            int spellChoice;
            if (!Player.has_bluePotion) {
                spellChoice = prompt(spells2, 3);
            } else {
                spellChoice = prompt(spells, 4);
            }
            if (!spellChoice) {
                map_erase(MONSTERX, MONSTERY);
                speech("Yay! It's dead!","",  1, 0);
                if (!Player.has_bluePotion) {
                    Player.has_key = 2;
                }
                Player.has_greenPotion = 0;
                draw_game(true);
                
                return 2;
            }
            
            if (spellChoice == 1 || spellChoice == 3 || spellChoice == 0) {
                speech("Ah it got so", "much stronger!", 1, 0);
                draw_game(true);
                Player.dead = 1;
                return 1;
            }
        }
        if ((get_north(Player.x, Player.y)->type == MONSTER2 and Player.direction == 2) or (get_south(Player.x, Player.y)->type == MONSTER2 and Player.direction == 0) or (get_west(Player.x, Player.y)->type == MONSTER2 and Player.direction == 3) or (get_east(Player.x, Player.y)->type == MONSTER2 and Player.direction == 1)) {
            const char* spells[] = {"Green P", "Sludge", "Blue P", "Medicine"};
            const char* spells2[] = {"Medicine", "Sludge", "Blue P"};
            speech("What should I", "throw at it?", 1, 1);
            int spellChoice;
            if (!Player.has_greenPotion) {
                spellChoice = prompt(spells2, 3);
            } else {
                spellChoice = prompt(spells, 4);
            }
            if (spellChoice == 2) {
                map_erase(MONSTER2X, MONSTER2Y);
                speech("Yay! It's dead!","",  1, 0);
                if (!Player.has_greenPotion) {
                    Player.has_key = 2;
                }
                Player.has_bluePotion = 0;
                draw_game(true);
                
                return 2;
            }
            if (spellChoice == 1 || spellChoice == 3 || spellChoice == 0) {
                speech("Ah it got so", "much stronger!", 1, 0);
                draw_game(true);
                Player.dead = 1;
                return 1;
            }
        }
        if (get_north(Player.x, Player.y)->type == DOOR and Player.has_key == 1) {
            uLCD.printf("tripped");
            map_erase(DOORX, DOORY);
            draw_game(true);
            speech("Yay! I'm free!","",  1, 0);
            draw_game(true);
            return 1;
        }
            
        break;
        case MENU_BUTTON:
        
        break;
        default:
        return 0;
    }
    return NO_RESULT;
}

/**
 * Entry point for frame drawing. This should be called once per iteration of
 * the game loop. This draws all tiles on the screen, followed by the status 
 * bars. Unless init is nonzero, this function will optimize drawing by only 
 * drawing tiles that have changed from the previous frame.
 */
void draw_game(int init)
{
    // Draw game border first
    if(init) draw_border();
    
    // Iterate over all visible map tiles
    for (int i = -5; i <= 5; i++) // Iterate over columns of tiles
    {
        for (int j = -4; j <= 4; j++) // Iterate over one column of tiles
        {
            // Here, we have a given (i,j)
            
            // Compute the current map (x,y) of this tile
            int x = i + Player.x;
            int y = j + Player.y;
            
            // Compute the previous map (px, py) of this tile
            int px = i + Player.px;
            int py = j + Player.py;
                        
            // Compute u,v coordinates for drawing
            int u = (i+5)*11 + 3;
            int v = (j+4)*11 + 15;
            
            // Figure out what to draw
            DrawFunc draw = NULL;
            if (init && i == 0 && j == 0) // Only draw the Player on init
            {
                draw_player(u, v, Player.has_key, Player.direction);
                continue;
            }
            else if (x >= 0 && y >= 0 && x < map_width() && y < map_height()) // Current (i,j) in the map
            {
                MapItem* curr_item = get_here(x, y);
                MapItem* prev_item = get_here(px, py);
                if (init || curr_item != prev_item) // Only draw if they're different
                {
                    if (curr_item) // There's something here! Draw it
                    {
                        draw = curr_item->draw;
                    }
                    else // There used to be something, but now there isn't
                    {
                        draw = draw_nothing;
                    }
                }
            }
            else if (init) // If doing a full draw, but we're out of bounds, draw the walls.
            {
                draw = draw_wall;
            }

            // Actually draw the tile
            if (draw) draw(u, v);
        }
    }
}


/**
 * Initialize the main world map. Add walls around the edges, interior chambers,
 * and plants in the background so you can see motion. Note: using the similar
 * procedure you can init the secondary map(s).
 */
void init_main_map()
{
    // "Random" plants
    Map* map = set_active_map(0);
    for(int i = map_width() + 3; i < map_area(); i += 39)
    {
        add_plant(i % map_width(), i / map_width());
//        uLCD.printf("afterfor");
    }
//    uLCD.printf("finishfor");
    pc.printf("plants\r\n");
        
    pc.printf("Adding walls!\r\n");
    add_wall(0,              0,              HORIZONTAL, map_width());
    add_wall(0,              map_height()-1, HORIZONTAL, map_width());
    add_wall(0,              0,              VERTICAL,   map_height());
    add_wall(map_width()-1,  0,              VERTICAL,   map_height());
    pc.printf("Walls done!\r\n");
    add_wall(DOORX - 5, DOORY, HORIZONTAL, 11);
    add_wall(DOORX - 5, DOORY, VERTICAL, map_height() - DOORY);
    add_wall(DOORX + 5, DOORY, VERTICAL, map_height() - DOORY);
    add_NPC(10, 10);
    add_door(DOORX, DOORY);
}

void init_dungeon_map()
{
    // "Random" plants
    Map* map = set_active_map(1);
    for(int i = map_width() + 3; i < map_area(); i += 39)
    {
        add_plant(i % map_width(), i / map_width());
//        uLCD.printf("afterfor");
    }
//    uLCD.printf("finishfor");
    pc.printf("plants\r\n");
        
    pc.printf("Adding walls!\r\n");
    add_wall(0,              0,              HORIZONTAL, map_width());
    add_wall(0,              map_height()-1, HORIZONTAL, map_width());
    add_wall(0,              0,              VERTICAL,   map_height());
    add_wall(map_width()-1,  0,              VERTICAL,   map_height());
    pc.printf("Walls done!\r\n");
    add_monster(MONSTERX, MONSTERY);
    add_monster2(MONSTER2X, MONSTER2Y);
    add_portal(25, 10, 25, 25, 0);
}
/**
 * Program entry point! This is where it all begins.
 * This function orchestrates all the parts of the game. Most of your
 * implementation should be elsewhere - this holds the game loop, and should
 * read like a road map for the rest of the code.
 */
int main()
{
    
    // First things first: initialize hardware
    ASSERT_P(hardware_init() == ERROR_NONE, "Hardware init failed!");
    // Initialize the maps
    maps_init();
    init_main_map();
    init_dungeon_map();
    
    // Initialize game state
    set_active_map(0);
    Player.x = Player.y = 25;
    Player.has_key = -1;
    Player.direction = 0;
    Player.dead = 0;
    int has_bluePotion = 0;
    int has_greenPotion = 0;
    uLCD.cls();
    uLCD.locate(1, 1);
    uLCD.color(WHITE);
    uLCD.textbackground_color(BLACK);
    uLCD.printf("Press");
    uLCD.filled_circle(49, 12, 2, RED);
    uLCD.locate(1, 2);
    uLCD.printf("To Start");
    while(!read_inputs().bR){
        
    }
    
    // Initial drawing
    draw_game(true);
    // Main game loop
    while(1)
    {
        // Timer to measure game update speed
        Timer t; t.start();
        
        // Read inputs
        GameInputs in = read_inputs();
        
        // Determine action
        int action = get_action(in);
        
        // Update game
        int update = update_game(action, in);
        
        // Check for game over
        if (update == 1) {
            break;
        }
        
        // Display the game if there is an update.
        if (update == 2) {
            draw_game(true);
            draw_upper_status(Player.x, Player.y, Player.has_key);
            draw_lower_status(Player.has_bluePotion, Player.has_greenPotion, Player.has_key);
        }
        
        // Frame pause: holding the Down button will result in faster movement (like running)
        if(in.bD) {
            t.stop();
            int dt = t.read_ms();
            if (dt < 50) wait_ms(50 - dt);
        } else {
            t.stop();
            int dt = t.read_ms();
            if (dt < 500) wait_ms(500 - dt);
        }
        
    }
    uLCD.cls();
    uLCD.locate(1, 1);
    uLCD.color(WHITE);
    uLCD.textbackground_color(BLACK);
    uLCD.printf("GAME OVER");
    if (Player.dead == 1) {
        uLCD.locate(3, 3);
        uLCD.set_font_size(2, 2);
        uLCD.color(RED);
        uLCD.printf("YOU DIED!");
    } else {
        uLCD.locate(3, 3);
        uLCD.set_font_size(2, 2);
        uLCD.color(GREEN);
        uLCD.printf("YOU WON!");
    }
}
