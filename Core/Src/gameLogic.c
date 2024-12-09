

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ili9163.h"
#include "gameLogic.h"
#include "menu.h"

// Define global variables
Polygon polygons[MAX_POLYGONS];
int polygonCount = 0;
Player player;
int boundX = 1000;
int boundY = 1000;

// Function to initialize a polygon
void initPolygon(Polygon *polygon, int16_t *vertices, int16_t numVertices, int16_t color) {
    polygon->vertices = (int16_t *)malloc(numVertices * 3 * sizeof(int16_t));
    if (polygon->vertices == NULL) {
        printf("Error allocating memory for polygon vertices.\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < numVertices * 3; i++) {
        polygon->vertices[i] = vertices[i];
    }
    polygon->numVertices = numVertices;
    polygon->color = color;
}

// Function to add a new polygon to the polygons array
void addPolygon(int16_t *vertices, int16_t numVertices, int16_t color) {
    if (polygonCount >= MAX_POLYGONS) {
        printf("Cannot add more polygons. Maximum limit reached.\n");
        return;
    }
    initPolygon(&polygons[polygonCount], vertices, numVertices, color);
    polygonCount++;
}

// Function to remove a polygon from the polygons array
void removePolygon(int16_t index) {
    if (index < 0 || index >= polygonCount) {
        printf("Invalid polygon index.\n");
        return;
    }

    free(polygons[index].vertices);

    for (int i = index; i < polygonCount - 1; i++) {
        polygons[i] = polygons[i + 1];
    }

    polygonCount--;
}

// Function to initialize polygons from preset map
void initPolygons() {

	//arena s pouzitim polyline
	for(int8_t i=0; i<5; i++) {
		int16_t EastWall3[] = {200+200*i, 0, 0,   200*i, 0, 0,   200*i, 0, 100,   200+200*i, 0, 100};
		addPolygon(EastWall3, 4, decodeRgbValue(255, 255, 0)); // east
	}
	for(int8_t i=0; i<5; i++) {
		int16_t EastWall4[] = {1000, 200+200*i, 0,   1000, 200*i, 0,   1000, 200*i, 100,   1000, 200+200*i, 100};
		addPolygon(EastWall4, 4, decodeRgbValue(255, 255, 0)); // north
	}
	for(int8_t i=0; i<5; i++) {
		int16_t EastWall3[] = {800-200*i, 1000, 0,   1000-200*i, 1000, 0,   1000-200*i, 1000, 100,   800-200*i, 1000, 100};
		addPolygon(EastWall3, 4, decodeRgbValue(255, 255, 0)); // west
	}
	for(int8_t i=0; i<5; i++) {
		int16_t EastWall4[] = {0, 800-200*i, 0,   0, 1000-200*i, 0,   0, 1000-200*i, 100,   0, 800-200*i, 100};
		addPolygon(EastWall4, 4, decodeRgbValue(255, 255, 0)); // south
	}

}

// Function to free polygon memory
void freePolygons() {
    for (int i = 0; i < polygonCount; i++) {
        free(polygons[i].vertices);
    }
}

// Function to initialize the player
void initPlayer() {
	player.x = 100;        // Initial X position
    player.y = 100;        // Initial Y position
    player.z = 50;         // Initial Z position
    player.rotation = 0.5; // Initial rotation angle
    player.health = 100;    // Starting health
    player.ammo = 50;       // Starting ammo
}

void movePlayer(Player *player, float step) {
	player->x += step*cos(player->rotation);
	player->y += step*sin(player->rotation);
	if ((player->x > boundX)||(player->x < 0)) {	//when hitting wall the player slides
		player->x -= step*cos(player->rotation);
	}
	if ((player->y > boundY)||(player->y < 0)) {
			player->y -= step*sin(player->rotation);
	}
}

void rotatePlayer(Player *player, float angle) {
	player->rotation += angle;
	if (player->rotation > 3.1416) {
		player->rotation -= 6.2832;
	}
	if (player->rotation < -3.1416) {
		player->rotation += 6.2832;
	}
}

void startNewGame(){
	initPolygons(0);
	initPlayer(0);
	gameLoop();
}

void gameLoop(){
	// in this loop we walk down the hallway and at the end we turn back
	int8_t step = 10;	// kazdych 10 pixelov bude bodka

	lcdPutS("test 3D rendera:", 220, 10, decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0));
	lcdRectangle(20, 225, 300, 250, decodeRgbValue(255, 255, 255)); //dolny status bar
	lcdCircle(160,232,5,decodeRgbValue(255, 255, 255)); //akysi kruh, v povodnej doom je tam hlava hraca

	while(1){

		//bodkovane polygony su o 40% rychlejsie
		for(int k = 0; k<20; k++) {
			for (int i = 0; i < polygonCount; i++) {	//nakresli mapu so stenami
				lcd3DPolyline(polygons[i].vertices, polygons[i].numVertices, polygons[i].color, player.x, player.y, player.z, player.rotation, 1, step);
			}
			for (int i = 0; i < polygonCount; i++) {	//nakresli mapu so stenami
				lcd3DPolyline(polygons[i].vertices, polygons[i].numVertices, decodeRgbValue(0, 0, 0), player.x, player.y, player.z, player.rotation, 1, step);
			}
			movePlayer(&player, 50); //moving camera forward
		}
		for(int k = 0; k<10; k++) {
			for (int i = 0; i < polygonCount; i++) {	//nakresli mapu so stenami
				lcd3DPolyline(polygons[i].vertices, polygons[i].numVertices, polygons[i].color, player.x, player.y, player.z, player.rotation, 1, step);
			}
			for (int i = 0; i < polygonCount; i++) {	//nakresli mapu so stenami
				lcd3DPolyline(polygons[i].vertices, polygons[i].numVertices, decodeRgbValue(0, 0, 0), player.x, player.y, player.z, player.rotation, 1, step);
			}
			rotatePlayer(&player, 3.1416/10);
		}
		for (int i = 0; i < polygonCount; i++) {	//nakresli mapu so stenami
			lcd3DPolygon(polygons[i].vertices, polygons[i].numVertices, polygons[i].color, player.x, player.y, player.z, player.rotation, 0, 5);
		}
		for(int k = 0; k<20; k++) {
			for (int i = 0; i < polygonCount; i++) {	//nakresli mapu so stenami
				lcd3DPolyline(polygons[i].vertices, polygons[i].numVertices, polygons[i].color, player.x, player.y, player.z, player.rotation, 1, step);
			}
			for (int i = 0; i < polygonCount; i++) {	//nakresli mapu so stenami
				lcd3DPolyline(polygons[i].vertices, polygons[i].numVertices, decodeRgbValue(0, 0, 0), player.x, player.y, player.z, player.rotation, 1, step);
			}
			movePlayer(&player, 50); //moving camera forward
		}
		for(int k = 0; k<10; k++) {
			for (int i = 0; i < polygonCount; i++) {	//nakresli mapu so stenami
				lcd3DPolyline(polygons[i].vertices, polygons[i].numVertices, polygons[i].color, player.x, player.y, player.z, player.rotation, 1, step);
			}
			for (int i = 0; i < polygonCount; i++) {	//nakresli mapu so stenami
				lcd3DPolyline(polygons[i].vertices, polygons[i].numVertices, decodeRgbValue(0, 0, 0), player.x, player.y, player.z, player.rotation, 1, step);
			}
			rotatePlayer(&player, 3.1416/10);
		}
		//final plot
		for (int i = 0; i < polygonCount; i++) {	//nakresli mapu so stenami
			lcd3DPolygon(polygons[i].vertices, polygons[i].numVertices, polygons[i].color, player.x, player.y, player.z, player.rotation, 0, 5);
		}

		LL_mDelay(3000);


	}
}


