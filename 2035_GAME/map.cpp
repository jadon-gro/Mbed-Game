#include "map.h"

#include "globals.h"
#include "graphics.h"
#include "hash_table.h"

/**
 * The Map structure. This holds a HashTable for all the MapItems, along with
 * values for the width and height of the Map.
 */
 
 #define BUCKET_NUM  13
 
struct Map {
    HashTable* items;
    int w, h;
};

/**
 * Storage area for the maps.
 * This is a global variable, but can only be access from this file because it
 * is static.
 */
 

static Map start_map;
static Map dungeon;

 // start_map = 0
static int active_map;

/**
 * The first step in HashTable access for the map is turning the two-dimensional
 * key information (x, y) into a one-dimensional unsigned integer.
 * This function should uniquely map (x,y) onto the space of unsigned integers.
 */
static unsigned XY_KEY(int X, int Y) {
    unsigned int key = X*map_height()+Y;
    return key;
}

/**
 * This is the hash function actually passed into createHashTable. It takes an
 * unsigned key (the output of XY_KEY) and turns it into a hash value (some
 * small non-negative integer).
 */
unsigned map_hash(unsigned key)
{
//      uLCD.printf("hash");
    return key % BUCKET_NUM;
}

void maps_init()
{
    HashTable* start_map_items = createHashTable(map_hash, BUCKET_NUM);
    start_map.items = start_map_items;
    start_map.w = 50;
    start_map.h = 50;
    HashTable* dungeon_map_items = createHashTable(map_hash, BUCKET_NUM);
    dungeon.items = dungeon_map_items;
    dungeon.w = 50;
    dungeon.h = 50;
    
    
    // TODO: Implement!    
    // Initialize hash table
    // Set width & height
}
int get_map_index() {
    return active_map;
}

Map* get_active_map()
{
    if (active_map == 0) {
        return &start_map;
    }
    else {
        if (active_map == 1) {
            return &dungeon;
        }
    }
    return NULL;
}
Map* set_active_map(int m)
{
    active_map = m;
    return get_active_map();
}

void print_map()
{
    // As you add more types, you'll need to add more items to this array.
    char lookup[] = {'W', 'P'};
    for(int y = 0; y < map_height(); y++)
    {
        for (int x = 0; x < map_width(); x++)
        {
            MapItem* item = get_here(x,y);
            if (item) pc.printf("%c", lookup[item->type]);
            else pc.printf(" ");
        }
        pc.printf("\r\n");
    }
}

int map_width()
{
    return get_active_map()->w;
}

int map_height()
{
    return get_active_map()->h;
}

int map_area()
{
    return map_height() * map_width();
}

MapItem* get_north(int x, int y)
{
    return (MapItem*) getItem(get_active_map()->items, XY_KEY(x, y + 1));
}

MapItem* get_south(int x, int y)
{
    return (MapItem*) getItem(get_active_map()->items, XY_KEY(x, y - 1));
}

MapItem* get_east(int x, int y)
{
    return (MapItem*) getItem(get_active_map()->items, XY_KEY(x + 1, y));
}

MapItem* get_west(int x, int y)
{
    return (MapItem*) getItem(get_active_map()->items, XY_KEY(x - 1, y));
}

MapItem* get_here(int x, int y)
{
    return (MapItem*) getItem(get_active_map()->items, XY_KEY(x, y));
}


void map_erase(int x, int y)
{
    deleteItem(get_active_map()->items, XY_KEY(x,y));
}

void add_wall(int x, int y, int dir, int len)
{
    for(int i = 0; i < len; i++)
    {
        MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
        w1->type = WALL;
        w1->draw = draw_wall;
        w1->walkable = false;
        w1->data = NULL;
        unsigned key = (dir == HORIZONTAL) ? XY_KEY(x+i, y) : XY_KEY(x, y+i);
        void* val = insertItem(get_active_map()->items, key, w1);
        if (val) free(val); // If something is already there, free it
    }
}

void add_plant(int x, int y)
{
    
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = PLANT;
    w1->draw = draw_plant;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}
void add_portal(int x, int y, int targetX, int targetY, int targetMap)
{
    
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = PORTAL;
    w1->draw = draw_portal;
    w1->walkable = false;
    StairsData* stair_data = (StairsData*) malloc(sizeof(StairsData));
    stair_data->tx = targetX;
    stair_data->ty = targetY;
    stair_data->tm = targetMap;
    w1->data = stair_data;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}
void add_NPC(int x, int y) {
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = NPC;
    w1->draw = draw_NPC;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}
void add_monster(int x, int y) {
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = MONSTER;
    w1->draw = draw_monster;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}
void add_monster2(int x, int y) {
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = MONSTER2;
    w1->draw = draw_monster2;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}
void add_door(int x, int y) {
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = DOOR;
    w1->draw = draw_door;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}
