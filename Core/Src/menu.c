#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ili9163.h"
#include "gameLogic.h"
#include "spi.h"

int16_t ammo = 24;
int16_t health = 78;
int16_t armor = 0;
char ammoText[16];
char healthText[16];
char armorText[16];

void drawMenu() {
	lcdPutSSized("DOOM", 250, 0, decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0),8);
	lcdPutSSized("NEW GAME", 205, 64, decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0),2);
	lcdPutSSized("OPTIONS", 205, 80, decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0),2);
	lcdPutSSized("LOAD GAME", 205, 96, decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0),2);
	lcdPutSSized("SAVE GAME", 205, 112, decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0),2);
	lcdPutSSized("READ THIS!", 205, 128, decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0),2);
	lcdPutSSized("QUIT GAME", 205, 144, decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0),2);

	lcdPutS("AMMO", 305, 230, decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0));
	sprintf(ammoText, "%d", ammo);
	lcdPutS(ammoText, 295, 222, decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0));

	lcdPutS("HEALTH", 160, 230, decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0));
	sprintf(healthText, "%d%%", health);
	lcdPutS(healthText, 145, 222, decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0));

	lcdPutS("ARMOR", 35, 230, decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0));
	sprintf(armorText, "%d%%", armor);
	lcdPutS(armorText, 20, 222, decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0));
}

void clearMenu() {
    lcdPutSSized("DOOM", 250, 0, decodeRgbValue(0, 0, 0), decodeRgbValue(0, 0, 0), 8);
    lcdPutSSized("NEW GAME", 205, 64, decodeRgbValue(0, 0, 0), decodeRgbValue(0, 0, 0), 2);
    lcdPutSSized("OPTIONS", 205, 80, decodeRgbValue(0, 0, 0), decodeRgbValue(0, 0, 0), 2);
    lcdPutSSized("LOAD GAME", 205, 96, decodeRgbValue(0, 0, 0), decodeRgbValue(0, 0, 0), 2);
    lcdPutSSized("SAVE GAME", 205, 112, decodeRgbValue(0, 0, 0), decodeRgbValue(0, 0, 0), 2);
    lcdPutSSized("READ THIS!", 205, 128, decodeRgbValue(0, 0, 0), decodeRgbValue(0, 0, 0), 2);
    lcdPutSSized("QUIT GAME", 205, 144, decodeRgbValue(0, 0, 0), decodeRgbValue(0, 0, 0), 2);

    lcdPutS("AMMO", 305, 230, decodeRgbValue(0, 0, 0), decodeRgbValue(0, 0, 0));
    lcdPutS("", 295, 222, decodeRgbValue(0, 0, 0), decodeRgbValue(0, 0, 0));

    lcdPutS("HEALTH", 160, 230, decodeRgbValue(0, 0, 0), decodeRgbValue(0, 0, 0));
    lcdPutS("", 145, 222, decodeRgbValue(0, 0, 0), decodeRgbValue(0, 0, 0));

    lcdPutS("ARMOR", 35, 230, decodeRgbValue(0, 0, 0), decodeRgbValue(0, 0, 0));
    lcdPutS("", 20, 222, decodeRgbValue(0, 0, 0), decodeRgbValue(0, 0, 0));
}

void lowerMenu() {
	lcdPutS("AMMO", 305, 230, decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0));
	sprintf(ammoText, "%d", ammo);
	lcdPutS(ammoText, 295, 222, decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0));

	lcdPutS("HEALTH", 160, 230, decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0));
	sprintf(healthText, "%d%%", health);
	lcdPutS(healthText, 145, 222, decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0));

	lcdPutS("ARMOR", 35, 230, decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0));
	sprintf(armorText, "%d%%", armor);
	lcdPutS(armorText, 20, 222, decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0));
}

void drawDifficulty() {
	lcdPutSSized("options", 240, 0, decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0),4);
	lcdPutSSized("EASY", 210, 80, decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0),3);
	lcdPutSSized("MEDIUM", 210, 112, decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0),3);
	lcdPutSSized("HARD", 210, 144, decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0),3);
}

void clearDifficulty() {
	lcdPutSSized("options", 240, 0, decodeRgbValue(0, 0, 0), decodeRgbValue(0, 0, 0),4);
	lcdPutSSized("EASY", 210, 80, decodeRgbValue(0, 0, 0), decodeRgbValue(0, 0, 0),3);
	lcdPutSSized("MEDIUM", 210, 112, decodeRgbValue(0, 0, 0), decodeRgbValue(0, 0, 0),3);
	lcdPutSSized("HARD", 210, 144, decodeRgbValue(0, 0, 0), decodeRgbValue(0, 0, 0),3);
}

void menu(){
	//int16_t difficulity = 2;		//v options sa bude dat menit

	//difficulty();
	//clearMenu();
	drawMenu();
	LL_mDelay(3000);

/*
	int16_t ammo = 24;
	int16_t health = 78;
	int16_t armor = 0;
	char ammoText[16];
	char healthText[16];
	char armorText[16];
*/
	//lowerMenu();

		int select = 1;

		while(select != 6)	//main menu cycle
		{

			if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1) == GPIO_PIN_RESET)	//choose lower option
			{
				if(select == 0)
					select = 5;
				else if(select != 0)
					select--;
			}

			if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0) == GPIO_PIN_RESET)	//choose option above
			{
				if(select == 5)
					select = 0;
				else if(select != 5)
					select++;
			}

			if(select == 0)
			{
				lcdPutSSized("NEW GAME", 205, 64, decodeRgbValue(0, 0, 0), decodeRgbValue(0, 0, 0),2);
				lcdPutSSized("NEW GAME", 205, 64, decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0),2);
			}
			if(select == 1)
			{
				lcdPutSSized("OPTIONS", 205, 80, decodeRgbValue(0, 0, 0), decodeRgbValue(0, 0, 0),2);
				lcdPutSSized("OPTIONS", 205, 80, decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0),2);

				lcdPutS("AMMO", 305, 230, decodeRgbValue(0, 0, 0), decodeRgbValue(0, 0, 0));
				sprintf(ammoText, "%d", ammo);
				lcdPutS(ammoText, 295, 222, decodeRgbValue(0, 0, 0), decodeRgbValue(0, 0, 0));

				lcdPutS("HEALTH", 160, 230, decodeRgbValue(0, 0, 0), decodeRgbValue(0, 0, 0));
				sprintf(healthText, "%d%%", health);
				lcdPutS(healthText, 145, 222, decodeRgbValue(0, 0, 0), decodeRgbValue(0, 0, 0));

				lcdPutS("ARMOR", 35, 230, decodeRgbValue(0, 0, 0), decodeRgbValue(0, 0, 0));
				sprintf(armorText, "%d%%", armor);
				lcdPutS(armorText, 20, 222, decodeRgbValue(0, 0, 0), decodeRgbValue(0, 0, 0));
			}
			if(select == 2)
			{
				lcdPutSSized("LOAD GAME", 205, 96, decodeRgbValue(0, 0, 0), decodeRgbValue(0, 0, 0),2);
				lcdPutSSized("LOAD GAME", 205, 96, decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0),2);
			}
			if(select == 3)
			{
				lcdPutSSized("SAVE GAME", 205, 112, decodeRgbValue(0, 0, 0), decodeRgbValue(0, 0, 0),2);
				lcdPutSSized("SAVE GAME", 205, 112, decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0),2);
			}
			if(select == 4)
			{
				lcdPutSSized("READ THIS!", 205, 128, decodeRgbValue(0, 0, 0), decodeRgbValue(0, 0, 0),2);
				lcdPutSSized("READ THIS!", 205, 128, decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0),2);
			}
			if(select == 5)
			{
				lcdPutSSized("QUIT GAME", 205, 144, decodeRgbValue(0, 0, 0), decodeRgbValue(0, 0, 0),2);
				lcdPutSSized("QUIT GAME", 205, 144, decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0),2);
			}

			//confirm your option
			if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1) == GPIO_PIN_RESET && select == 0)
				select = 6;


			// odkaz pre Mira: tak ako robil Matej menu, tak skus podobne urobit options a aj readme.
			// V options sa bude prepinat obtiaznost a ked zacnes hru, tak sa hodnota obtiaznosti
			// priradi hre. Obtiaznost => easy, medium, hard (0 - 1 - 2)

			// takto nejako si predstavujem options, skus to rozvinut
			//confirm your option
			if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3) == GPIO_PIN_RESET && select == 1){
				clearMenu();
				//lcdPutSSized("options", 240, 0, decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0),4);
				drawDifficulty();
				while(select != 0){
					if(select == 0) // tu namiesto select by asi davalo zmysel dat premennu difficulity ci to j eteraz len tak alebo ako ?
					{
						lcdPutSSized("EASY", 210, 80, decodeRgbValue(0, 0, 0), decodeRgbValue(0, 0, 0),2);
						LL_mDelay(1000);
						lcdPutSSized("EASY", 210, 80, decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0),2);
					}

					if(select == 1)
					{
						lcdPutSSized("MEDIUM", 210, 112, decodeRgbValue(0, 0, 0), decodeRgbValue(0, 0, 0),2);
						LL_mDelay(500);
						lcdPutSSized("MEDIUM", 210, 112, decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0),2);
					}

					if(select == 2)
					{
						lcdPutSSized("HARD", 210, 144, decodeRgbValue(0, 0, 0), decodeRgbValue(0, 0, 0),2);
						LL_mDelay(750);
						lcdPutSSized("HARD", 210, 144, decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0),2);
					}
					if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3) == GPIO_PIN_RESET && select == 1){
						//clearDifficulty();
						drawMenu;
						//lowerMenu;
					}
				}
			}

	clearMenu();
	//startNewGame(difficulity);
		}
	}

/*
 * po prejdeni z hlavneho menu na options menu mi tam vypise este save game read this quit neviem preco
 * v options sekcii mi nejde manualne menit obtiaznost stale to select sa rovna len jednotke a neviem preco
*/

