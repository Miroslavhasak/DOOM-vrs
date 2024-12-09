#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ili9163.h"
#include "gameLogic.h"
#include "spi.h"

void menu(){
	lcdPutSSized("DOOM", 215, 0, decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0),8);
	lcdPutSSized("NEW GAME", 180, 64, decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0),2);
	lcdPutSSized("OPTIONS", 180, 80, decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0),2);
	lcdPutSSized("LOAD GAME", 180, 96, decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0),2);
	lcdPutSSized("SAVE GAME", 180, 112, decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0),2);
	lcdPutSSized("READ THIS!", 180, 128, decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0),2);
	lcdPutSSized("QUIT GAME", 180, 144, decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0),2);

	int16_t ammo = 24;
	int16_t health = 78;
	int16_t armor = 0;
	char ammoText[16];
	char healthText[16];
	char armorText[16];


	lcdPutS("AMMO", 250, 230, decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0));
	sprintf(ammoText, "%d", ammo);
	lcdPutS(ammoText, 240, 222, decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0));

	lcdPutS("HEALTH", 145, 230, decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0));
	sprintf(healthText, "%d%%", health);
	lcdPutS(healthText, 125, 222, decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0));

	lcdPutS("ARMOR", 35, 230, decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0));
	sprintf(armorText, "%d%%", armor);
	lcdPutS(armorText, 20, 222, decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0));

		int select = 0;
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
				lcdPutSSized("NEW GAME", 180, 64, decodeRgbValue(0, 0, 0), decodeRgbValue(0, 0, 0),2);
				lcdPutSSized("NEW GAME", 180, 64, decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0),2);
			}
			if(select == 1)
			{
				lcdPutSSized("OPTIONS", 180, 80, decodeRgbValue(0, 0, 0), decodeRgbValue(0, 0, 0),2);
				lcdPutSSized("OPTIONS", 180, 80, decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0),2);
			}
			if(select == 2)
			{
				lcdPutSSized("LOAD GAME", 180, 96, decodeRgbValue(0, 0, 0), decodeRgbValue(0, 0, 0),2);
				lcdPutSSized("LOAD GAME", 180, 96, decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0),2);
			}
			if(select == 3)
			{
				lcdPutSSized("SAVE GAME", 180, 112, decodeRgbValue(0, 0, 0), decodeRgbValue(0, 0, 0),2);
				lcdPutSSized("SAVE GAME", 180, 112, decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0),2);
			}
			if(select == 4)
			{
				lcdPutSSized("READ THIS!", 180, 128, decodeRgbValue(0, 0, 0), decodeRgbValue(0, 0, 0),2);
				lcdPutSSized("READ THIS!", 180, 128, decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0),2);
			}
			if(select == 5)
			{
				lcdPutSSized("QUIT GAME", 180, 144, decodeRgbValue(0, 0, 0), decodeRgbValue(0, 0, 0),2);
				lcdPutSSized("QUIT GAME", 180, 144, decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0),2);
			}

			//confirm your option
			if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3) == GPIO_PIN_RESET && select == 0)
				select = 6;


		}

	//delete the menu
	lcdPutSSized("DOOM", 215, 0, decodeRgbValue(0, 0, 0), decodeRgbValue(0, 0, 0),8);
	lcdPutSSized("NEW GAME", 180, 64, decodeRgbValue(0, 0, 0), decodeRgbValue(0, 0, 0),2);
	lcdPutSSized("OPTIONS", 180, 80, decodeRgbValue(0, 0, 0), decodeRgbValue(0, 0, 0),2);
	lcdPutSSized("LOAD GAME", 180, 96, decodeRgbValue(0, 0, 0), decodeRgbValue(0, 0, 0),2);
	lcdPutSSized("SAVE GAME", 180, 112, decodeRgbValue(0, 0, 0), decodeRgbValue(0, 0, 0),2);
	lcdPutSSized("READ THIS!", 180, 128, decodeRgbValue(0, 0, 0), decodeRgbValue(0, 0, 0),2);
	lcdPutSSized("QUIT GAME", 180, 144, decodeRgbValue(0, 0, 0), decodeRgbValue(0, 0, 0),2);

	lcdPutS("AMMO", 250, 230, decodeRgbValue(0, 0, 0), decodeRgbValue(0, 0, 0));
	sprintf(ammoText, "%d", ammo);
	lcdPutS(ammoText, 240, 222, decodeRgbValue(0, 0, 0), decodeRgbValue(0, 0, 0));

	lcdPutS("HEALTH", 145, 230, decodeRgbValue(0, 0, 0), decodeRgbValue(0, 0, 0));
	sprintf(healthText, "%d%%", health);
	lcdPutS(healthText, 125, 222, decodeRgbValue(0, 0, 0), decodeRgbValue(0, 0, 0));

	lcdPutS("ARMOR", 35, 230, decodeRgbValue(0, 0, 0), decodeRgbValue(0, 0, 0));
	sprintf(armorText, "%d%%", armor);
	lcdPutS(armorText, 20, 222, decodeRgbValue(0, 0, 0), decodeRgbValue(0, 0, 0));

	startNewGame();
}


