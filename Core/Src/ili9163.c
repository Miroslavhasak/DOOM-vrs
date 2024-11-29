/**
 * @file ili9163lcd.c
 * @brief ILI9163 128x128 LCD Driver
 *
 * This code has been ported from the ili9163lcd library for avr made
 * by Simon Inns, to run on a msp430.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/
 *
 * @author Simon Inns <simon.inns@gmail.com>
 * @author Christopher Vagnetoft (NoccyLabs)
 * @copyright (C) 2012 Simon Inns
 * @copyright parts (C) 2012 NoccyLabs
 *
//merana rychlost vykreslovania:
//najmensi vykreslovaci vykon je 2200 px/s
//maximalny vykreslovaci vykon pre ciaru je 16300 px/s
//maximalny vykreslovaci vykon pre plneny stvorec je 17000 px/s
//priemerny vykreslovaci vykon pre polygonovy stvorec je 14600 px/s
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ili9163.h"
//#include "font5x8.h"
#include "Minimum_font.h"
#include "spi.h"

// Low-level LCD driving functions --------------------------------------------------------------------------

// Reset the LCD hardware
void lcdReset(void)
{
	// Reset pin is active low (0 = reset, 1 = ready)
	res_reset();
	LL_mDelay(100);

	res_set();
	LL_mDelay(100);
}

void lcdWriteCommand(uint8_t address) //urcuje obsah nasledujucich write data/parameter
{
	cd_reset();
	cs_reset();
	readWriteSPI1(address);
	cs_set();
}

void lcdWriteParameter(int16_t parameter) //tymto sa zapisuju rozne veci
{
	cd_set();
	cs_reset();
	readWriteSPI1(parameter); //original
	/*if (parameter > 0xFF) {
		 //If parameter is larger than 8 bits, split it into high and low bytes
		readWriteSPI1((uint8_t)(parameter >> 8)); // Send high byte
		readWriteSPI1((uint8_t)(parameter & 0xFF)); // Send low byte
	}
	else {
		 //If parameter is 8 bits or less, send it directly
		readWriteSPI1((uint8_t)(parameter & 0xFF));
	}*/
	cs_set();
}

void lcdWriteData(uint8_t dataByte1, uint8_t dataByte2)  //tymto sa zapisuje farba
{
	cd_set();
	cs_reset();
	readWriteSPI1(dataByte1); //posielame samostatne dva bajty o farbe
	readWriteSPI1(dataByte2);
	cs_set();
}

void lcdInitialise(uint8_t orientation)
{
	// Set up the IO ports for communication with the LCD


	// Hardware reset the LCD
	lcdReset();

    lcdWriteCommand(EXIT_SLEEP_MODE);
    LL_mDelay(100); //Delay(10000); // Wait for the screen to wake up

    lcdWriteCommand(SET_PIXEL_FORMAT);
    lcdWriteParameter(0x05); // 16 bits per pixel

    lcdWriteCommand(SET_GAMMA_CURVE);
    lcdWriteParameter(0x04); // Select gamma curve 3

    lcdWriteCommand(GAM_R_SEL);
    lcdWriteParameter(0x01); // Gamma adjustment enabled

    lcdWriteCommand(POSITIVE_GAMMA_CORRECT);
    lcdWriteParameter(0x3f); // 1st Parameter
    lcdWriteParameter(0x25); // 2nd Parameter
    lcdWriteParameter(0x1c); // 3rd Parameter
    lcdWriteParameter(0x1e); // 4th Parameter
    lcdWriteParameter(0x20); // 5th Parameter
    lcdWriteParameter(0x12); // 6th Parameter
    lcdWriteParameter(0x2a); // 7th Parameter
    lcdWriteParameter(0x90); // 8th Parameter
    lcdWriteParameter(0x24); // 9th Parameter
    lcdWriteParameter(0x11); // 10th Parameter
    lcdWriteParameter(0x00); // 11th Parameter
    lcdWriteParameter(0x00); // 12th Parameter
    lcdWriteParameter(0x00); // 13th Parameter
    lcdWriteParameter(0x00); // 14th Parameter
    lcdWriteParameter(0x00); // 15th Parameter

    lcdWriteCommand(NEGATIVE_GAMMA_CORRECT);
    lcdWriteParameter(0x20); // 1st Parameter
    lcdWriteParameter(0x20); // 2nd Parameter
    lcdWriteParameter(0x20); // 3rd Parameter
    lcdWriteParameter(0x20); // 4th Parameter
    lcdWriteParameter(0x05); // 5th Parameter
    lcdWriteParameter(0x00); // 6th Parameter
    lcdWriteParameter(0x15); // 7th Parameter
    lcdWriteParameter(0xa7); // 8th Parameter
    lcdWriteParameter(0x3d); // 9th Parameter
    lcdWriteParameter(0x18); // 10th Parameter
    lcdWriteParameter(0x25); // 11th Parameter
    lcdWriteParameter(0x2a); // 12th Parameter
    lcdWriteParameter(0x2b); // 13th Parameter
    lcdWriteParameter(0x2b); // 14th Parameter
    lcdWriteParameter(0x3a); // 15th Parameter

    lcdWriteCommand(FRAME_RATE_CONTROL1);
    lcdWriteParameter(0x08); // DIVA = 8
    lcdWriteParameter(0x08); // VPA = 8

    lcdWriteCommand(DISPLAY_INVERSION);
    lcdWriteParameter(0x07); // NLA = 1, NLB = 1, NLC = 1 (all on Frame Inversion)

    lcdWriteCommand(POWER_CONTROL1);
    lcdWriteParameter(0x0a); // VRH = 10:  GVDD = 4.30
    lcdWriteParameter(0x02); // VC = 2: VCI1 = 2.65

    lcdWriteCommand(POWER_CONTROL2);
    lcdWriteParameter(0x02); // BT = 2: AVDD = 2xVCI1, VCL = -1xVCI1, VGH = 5xVCI1, VGL = -2xVCI1

    lcdWriteCommand(VCOM_CONTROL1);
    lcdWriteParameter(0x50); // VMH = 80: VCOMH voltage = 4.5
    lcdWriteParameter(0x5b); // VML = 91: VCOML voltage = -0.225

    lcdWriteCommand(VCOM_OFFSET_CONTROL);
    lcdWriteParameter(0x40); // nVM = 0, VMF = 64: VCOMH output = VMH, VCOML output = VML

    lcdWriteCommand(SET_COLUMN_ADDRESS);
    lcdWriteParameter(0x00); // XSH
    lcdWriteParameter(0x00); // XSL
    lcdWriteParameter(0x01); // End Column High Byte
    lcdWriteParameter(0x3F); // End Column Low Byte 319

    lcdWriteCommand(SET_PAGE_ADDRESS);
    lcdWriteParameter(0x00);
    lcdWriteParameter(0x00);
    lcdWriteParameter(0x00); // End Page High Byte
    lcdWriteParameter(0xEF); // End Page Low Byte (239, 0x00EF in hex)

	// Select display orientation
    lcdWriteCommand(SET_ADDRESS_MODE);
	lcdWriteParameter(orientation);

	// Set the display to on
    lcdWriteCommand(SET_DISPLAY_ON);
    lcdWriteCommand(WRITE_MEMORY_START);
}

// LCD graphics functions -----------------------------------------------------------------------------------

void lcdClearDisplay(uint16_t colour)
{
	uint16_t pixel;

	// Set the column address to 0-127
	lcdWriteCommand(SET_COLUMN_ADDRESS);
	lcdWriteParameter(0x00);
	lcdWriteParameter(0x00);
	lcdWriteParameter(0x00);
	lcdWriteParameter(0x7f);

	// Set the page address to 0-127
	lcdWriteCommand(SET_PAGE_ADDRESS);
	lcdWriteParameter(0x00);
	lcdWriteParameter(0x00);
	lcdWriteParameter(0x00);
	lcdWriteParameter(0x7f);

	// Plot the pixels
	lcdWriteCommand(WRITE_MEMORY_START);
	for(pixel = 0; pixel < 16385; pixel++)
	{
		lcdWriteData(colour >> 8, colour);
	}
}

// changing a single pixel on display
void lcdPlot(int16_t x, int16_t y, uint16_t colour)
{
	// Horizontal Address Start Position
	lcdWriteCommand(SET_COLUMN_ADDRESS);
	lcdWriteParameter(0x00);
	lcdWriteParameter(x);
	lcdWriteParameter(0x00);
	lcdWriteParameter(0x013F);//319

	// Vertical Address end Position
	lcdWriteCommand(SET_PAGE_ADDRESS);
	lcdWriteParameter(0x00);
	lcdWriteParameter(y);
	lcdWriteParameter(0x00);
	lcdWriteParameter(0xEF);//239

	// Plot the point
	lcdWriteCommand(WRITE_MEMORY_START);
	if((x>=0)&&(x<256)&&(y>=0)&&(y<240)) //check if pixel is inside the bounds
		lcdWriteData(colour >> 8, colour);
}

void lcdLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t colour)
{
	if(x0 == x1){ //vodorovna ciara optimalizovane
		lcdFilledRectangle(x0, y0, x1, y1, colour);
		return;
	}
	else if(y0 == y1){ //zvisla ciara optimalizovane
		lcdFilledRectangle(x0, y0, x1, y1, colour);
		return;
	}

	int16_t dy = y1 - y0;
	int16_t dx = x1 - x0;
	int16_t stepx, stepy;

	if (dy < 0) //derivacie musia byt kladne
	{
		dy = -dy; stepy = -1;
	}
	else stepy = 1;

	if (dx < 0) //derivacie musia byt kladne
	{
		dx = -dx; stepx = -1;
	}
	else stepx = 1;

	dy <<= 1;	//dy*2
	dx <<= 1;	//dx*2

	lcdPlot(x0, y0, colour);	//zaciatocny pixel
	uint16_t partialLineStartCoord = 0;

	if (dx > dy) {  //menej nez 45 stupnov
		partialLineStartCoord = x0-1;
		int fraction = dy - (dx >> 1);	//dy-(dx/2)
		while (x0 != x1)
		{

			if (fraction >= 0) //when we move up 1 pixel on Y axis
			{
				lcdFilledRectangle(partialLineStartCoord+1, y0, x0, y0, colour);
				partialLineStartCoord = x0;
				y0 += stepy;
				fraction -= dx;
			}

			x0 += stepx;
			fraction += dy;
		}
		lcdFilledRectangle(partialLineStartCoord+1, y0, x0, y0, colour);
	}
	else			//viac nez 45 stupnov
	{
		partialLineStartCoord = y0-1;
		int fraction = dx - (dy >> 1);
		while (y0 != y1)
		{
			if (fraction >= 0)	//when we move up 1 pixel on X axis
			{
				lcdFilledRectangle(x0, partialLineStartCoord+1, x0, y0, colour);
				partialLineStartCoord = y0;
				x0 += stepx;
				fraction -= dy;
			}

			y0 += stepy;
			fraction += dx;
		}
		lcdFilledRectangle(x0, partialLineStartCoord+1, x0, y0, colour);
	}
}

void lcdDottedLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t colour, uint16_t step)
{
	int16_t dy = y1 - y0;
	int16_t dx = x1 - x0;
	int16_t stepx, stepy;
	int16_t delta = 0;
	int16_t astep = 0;

	if (dy < 0)
	{
		dy = -dy; stepy = -step;
	}
	else stepy = step;

 	if (dx < 0)
	{
		dx = -dx; stepx = -step;
	}
	else stepx = step;

	dy <<= 1;
	dx <<= 1;

	if (dx > dy) {
		int fraction = dy - (dx >> 1);
		delta = abs(x0 - x1);
		astep = abs(stepx);
		while (delta > 0)
		{
			if (fraction >= 0)
			{
				y0 += stepy;
				fraction -= dx;
			}

			lcdPlot(x0, y0, colour);
   			x0 += stepx;
   			delta = delta - astep;
   			fraction += dy;

		}
	}
	else
	{
		int fraction = dx - (dy >> 1);
		delta = abs(y0 - y1);
		astep = abs(stepy);
		while (delta > 0)
		{
			if (fraction >= 0)
			{
				x0 += stepx;
				fraction -= dy;
			}

			lcdPlot(x0, y0, colour);
			y0 += stepy;
			delta = delta - astep;
			fraction += dx;

		}
	}
}

// Draw a rectangle between x0, y0 and x1, y1
void lcdRectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t colour)
{
	lcdLine(x0, y0, x0, y1, colour);
	lcdLine(x0, y1, x1, y1, colour);
	lcdLine(x1, y0, x1, y1, colour);
	lcdLine(x0, y0, x1, y0, colour);
}

// Draw a dotted rectangle between x0, y0 and x1, y1
void lcdDottedRectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t colour, uint16_t step)
{
	lcdDottedLine(x0, y0, x0, y1, colour, step);
	lcdDottedLine(x0, y1, x1, y1, colour, step);
	lcdDottedLine(x1, y0, x1, y1, colour, step);
	lcdDottedLine(x0, y0, x1, y0, colour, step);
}

// Draw a filled rectangle
void lcdFilledRectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t colour)
{
	uint16_t pixels;
	int16_t pom = 0;
	if (x0>x1){	//x1 must be greater than x0
		pom = x0;
		x0 = x1;
		x1 = pom;
	}
	if (y0>y1){	//y1 must be greater than y0
		pom = y0;
		y0 = y1;
		y1 = pom;
	}

	// To speed up plotting we define a x window with the width of the
	// rectangle and then just output the required number of bytes to
	// fill down to the end point
	lcdWriteCommand(SET_COLUMN_ADDRESS); // Horizontal Address Start Position
	lcdWriteParameter(0x00);
	lcdWriteParameter(x0);
	lcdWriteParameter(0x00);
	lcdWriteParameter(x1);

	lcdWriteCommand(SET_PAGE_ADDRESS); // Vertical Address end Position
	lcdWriteParameter(0x00);
	lcdWriteParameter(y0);
	lcdWriteParameter(0x00);
	lcdWriteParameter(y1);

	lcdWriteCommand(WRITE_MEMORY_START);
	for (pixels = 0; pixels < (((x1+1) - x0) * ((y1+1) - y0)); pixels++)
			lcdWriteData(colour >> 8, colour);
}

void lcdFilledDottedRectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t colour, uint16_t step)
{
	for (int16_t y = y0; y < y1; y += step) { // Skip every n rows
		for (int16_t x = x0; x < x1; x += step) { // Skip every n columns
			lcdPlot(x, y, colour);
		}
	}
}

void lcdPolygon(int16_t *points, int16_t sides, uint16_t colour)
{
	for(int16_t i = 0; i<sides; i++){
		int16_t x0 = points[(2*i)];
		int16_t y0 = points[(2*i + 1)];
		int16_t x1 = points[(2*((i+1)%sides))];
		int16_t y1 = points[(2*((i+1)%sides) + 1)];
		lcdLine(x0, y0, x1, y1, colour);
	}
}

void lcdDottedPolygon(int16_t *points, int16_t sides, uint16_t colour, uint16_t step)
{
	for(int16_t i = 0; i<sides; i++){
		int16_t x0 = points[(2*i)];
		int16_t y0 = points[(2*i + 1)];
		int16_t x1 = points[(2*((i+1)%sides))];
		int16_t y1 = points[(2*((i+1)%sides) + 1)];
		lcdDottedLine(x0, y0, x1, y1, colour, step);
	}
}

void lcdFilledTriangle(int16_t *points, uint16_t colour)
{
	// zoradim body od vzostupne podla osi X
	for(int16_t i = 2; i>=0; i--){
		int16_t max = -30000;
		int16_t maxInd = 0;
		for(int16_t k = 0; k<=i; k++){
			if (points[k*2]>=max){
				max = points[k*2];
				maxInd = k;
			}
		}
		int16_t pomX = points[maxInd*2]; //ulozim si maximum
		int16_t pomY = points[maxInd*2+1];
		points[maxInd*2] = points[i*2]; //na jeho poziciu premiestnim data z konca
		points[maxInd*2+1] = points[i*2+1];
		points[i*2] = pomX; //na jeho koniec dat dam maximum
		points[i*2+1] = pomY;
	}

	float_t der[3];

	//treba osetrit delenie nulou pri vypocte strmosti priamky
	if((points[2] - points[0]) != 0){ //medzi bodmi 1 a 2
		der[0] = (float_t)(points[3] - points[1]) / (float_t)(points[2] - points[0]);
	}
	if((points[4] - points[2]) != 0){ //medzi bodmi 2 a 3
		der[1] = (float_t)(points[5] - points[3]) / (float_t)(points[4] - points[2]);
	}
	if((points[4] - points[0]) != 0){ //medzi bodmi 1 a 3
		der[2] = (float_t)(points[5] - points[1]) / (float_t)(points[4] - points[0]);
	}

	for (int16_t x = points[0]; x <= points[4]; x++) {	// vykreslit trojuholnik
		if (x < points[2]) {
			lcdLine(x, (int16_t)(der[0] * (x-points[0]) + points[1]), x, (int16_t)(der[2] * (x-points[0]) + points[1]), colour);
		} else {
			lcdLine(x, (int16_t)(der[1] * (x-points[2]) + points[3]), x, (int16_t)(der[2] * (x-points[0]) + points[1]), colour);
		}
	}

}

void lcdFilledDottedTriangle(int16_t *points, uint16_t colour, uint16_t step)
{
	// zoradim body od vzostupne podla osi X
	for(int16_t i = 2; i>=0; i--){
		int16_t max = -30000;
		int16_t maxInd = 0;
		for(int16_t k = 0; k<=i; k++){
			if (points[k*2]>=max){
				max = points[k*2];
				maxInd = k;
			}
		}
		int16_t pomX = points[maxInd*2]; //ulozim si maximum
		int16_t pomY = points[maxInd*2+1];
		points[maxInd*2] = points[i*2]; //na jeho poziciu premiestnim data z konca
		points[maxInd*2+1] = points[i*2+1];
		points[i*2] = pomX; //na jeho koniec dat dam maximum
		points[i*2+1] = pomY;
	}

	float_t der[3];

	//treba osetrit delenie nulou pri vypocte strmosti priamky
	if((points[2] - points[0]) != 0){ //medzi bodmi 1 a 2
		der[0] = (float_t)(points[3] - points[1]) / (float_t)(points[2] - points[0]);
	}
	if((points[4] - points[2]) != 0){ //medzi bodmi 2 a 3
		der[1] = (float_t)(points[5] - points[3]) / (float_t)(points[4] - points[2]);
	}
	if((points[4] - points[0]) != 0){ //medzi bodmi 1 a 3
		der[2] = (float_t)(points[5] - points[1]) / (float_t)(points[4] - points[0]);
	}

	for (int16_t x = points[0]; x <= points[4]; x+=step) {	// vykreslit trojuholnik
		if (x < points[2]) {
			lcdDottedLine(x, (int16_t)(der[0] * (x-points[0]) + points[1]), x, (int16_t)(der[2] * (x-points[0]) + points[1]), colour, step);
		} else {
			lcdDottedLine(x, (int16_t)(der[1] * (x-points[2]) + points[3]), x, (int16_t)(der[2] * (x-points[0]) + points[1]), colour, step);
		}
	}

}

// nesmie mat stred mimo svojej plochy
void lcdFilledPolygon(int16_t *points, int16_t sides, uint16_t colour){
	float_t avgX = 0;
	float_t avgY = 0;
	for(int8_t i=0; i<sides; i++){
		avgX += (float_t)(points[i*2]);
		avgY += (float_t)(points[i*2+1]);
	}
	avgX = (float_t)(avgX)/(float_t)(sides);
	avgY = (float_t)(avgY)/(float_t)(sides);

	for(int8_t i=0; i<sides; i++){
		int16_t pointsT[] = {points[i*2], points[i*2+1], points[(2*((i+1)%sides))], points[(2*((i+1)%sides) + 1)], (int16_t)(avgX), (int16_t)(avgY)};
		lcdFilledTriangle( pointsT, colour );
	}
}

void lcdFilledDottedPolygon(int16_t *points, int16_t sides, uint16_t colour, uint16_t step){
	float_t avgX = 0;
	float_t avgY = 0;
	for(int8_t i=0; i<sides; i++){
		avgX += (float_t)(points[i*2]);
		avgY += (float_t)(points[i*2+1]);
	}
	avgX = (float_t)(avgX)/(float_t)(sides);
	avgY = (float_t)(avgY)/(float_t)(sides);

	for(int8_t i=0; i<sides; i++){
		int16_t pointsT[] = {points[i*2], points[i*2+1], points[(2*((i+1)%sides))], points[(2*((i+1)%sides) + 1)], (int16_t)(avgX), (int16_t)(avgY)};
		lcdFilledDottedTriangle( pointsT, colour, step );
	}
}

void lcdCircle(int16_t xCentre, int16_t yCentre, int16_t radius, uint16_t colour)
{
	int16_t x = 0, y = radius;
	int16_t d = 3 - (2 * radius);

    while(x <= y)
	{
		lcdPlot(xCentre + x, yCentre + y, colour);
		lcdPlot(xCentre + y, yCentre + x, colour);
		lcdPlot(xCentre - x, yCentre + y, colour);
		lcdPlot(xCentre + y, yCentre - x, colour);
		lcdPlot(xCentre - x, yCentre - y, colour);
		lcdPlot(xCentre - y, yCentre - x, colour);
		lcdPlot(xCentre + x, yCentre - y, colour);
		lcdPlot(xCentre - y, yCentre + x, colour);

		if (d < 0) d += (4 * x) + 6;
		else
		{
			d += (4 * (x - y)) + 10;
			y -= 1;
		}

		x++;
	}
}

void lcdDottedCircle(int16_t xCentre, int16_t yCentre, int16_t radius, uint16_t colour, uint16_t step)
{
	int16_t x = 0, y = radius;
	int16_t d = 3 - (2 * radius);

    while(x <= y)
	{
		lcdPlot(xCentre + x, yCentre + y, colour);
		lcdPlot(xCentre + y, yCentre + x, colour);
		lcdPlot(xCentre - x, yCentre + y, colour);
		lcdPlot(xCentre + y, yCentre - x, colour);
		lcdPlot(xCentre - x, yCentre - y, colour);
		lcdPlot(xCentre - y, yCentre - x, colour);
		lcdPlot(xCentre + x, yCentre - y, colour);
		lcdPlot(xCentre - y, yCentre + x, colour);

		if (d < 0) d += (4 * x) + 6;
		else
		{
			d += (4 * (x - y)) + 10;
			y -= step;
		}

		x+= step;
	}
}

void lcdFilledCircle(int16_t xCentre, int16_t yCentre, int16_t radius, uint16_t colour)
{
	int16_t x = 0, y = radius;
	int16_t d = 3 - (2 * radius);

	while(x <= y)
	{
		lcdLine(xCentre + x, yCentre + y, xCentre + x, yCentre -y, colour);
		lcdLine(xCentre -x, yCentre + y, xCentre - x, yCentre -y, colour);
		lcdLine(xCentre + y, yCentre + x, xCentre + y, yCentre -x, colour);
		lcdLine(xCentre -y, yCentre + x, xCentre -y, yCentre -x, colour);

		if (d < 0) d += (4 * x) + 6;
		else
		{
			d += (4 * (x - y)) + 10;
			y -= 1;
		}

		x+= 1;
	}
}

void lcdFilledDottedCircle(int16_t xCentre, int16_t yCentre, int16_t radius, uint16_t colour, uint16_t step)
{
	int16_t x = 0, y = radius;
	int16_t d = 3 - (2 * radius);

	while(x <= y)
	{
		lcdDottedLine(xCentre + x, yCentre + y, xCentre + x, yCentre -y, colour, step);
		lcdDottedLine(xCentre -x, yCentre + y, xCentre - x, yCentre -y, colour, step);
		lcdDottedLine(xCentre + y, yCentre + x, xCentre + y, yCentre -x, colour, step);
		lcdDottedLine(xCentre -y, yCentre + x, xCentre -y, yCentre -x, colour, step);

		if (d < 0) d += (4 * x) + 6;
		else
		{
			d += (4 * (x - y)) + 10;
			y -= step;
		}

		x+= step;
	}
}

// LCD text manipulation functions --------------------------------------------------------------------------
#define pgm_read_byte_near(address_short) (uint16_t)(address_short)
// Plot a character at the specified x, y co-ordinates (top left hand corner of character)
void lcdPutCh(unsigned char character, uint8_t x, uint8_t y, uint16_t fgColour, uint16_t bgColour)
{
	uint8_t row, column;

	// To speed up plotting we define a x window of 6 pixels and then
	// write out one row at a time.  This means the LCD will correctly
	// update the memory pointer saving us a good few bytes

	lcdWriteCommand(SET_COLUMN_ADDRESS); // Horizontal Address Start Position
	lcdWriteParameter(0x00);
	lcdWriteParameter(x);
	lcdWriteParameter(0x00);
	lcdWriteParameter(x+5);

	lcdWriteCommand(SET_PAGE_ADDRESS); // Vertical Address end Position
	lcdWriteParameter(0x00);
	lcdWriteParameter(y);
	lcdWriteParameter(0x00);
	lcdWriteParameter(0x7f);

	lcdWriteCommand(WRITE_MEMORY_START);

	// Plot the font data
	for (row = 0; row < 8; row++)
	{
		for (column = 0; column < 6; column++)
		{
			// Calculate the mirrored column index
			uint8_t mirroredColumn = 5 - column;
			// Check the bit corresponding to the current row in the mirrored column
			if ((fontus[character][mirroredColumn]) & (1 << row))
				lcdWriteData(fgColour >> 8, fgColour);
			else
				lcdWriteData(bgColour >> 8, bgColour);
		}
	}
}

void lcdPutChSized(unsigned char character, uint8_t x, uint8_t y, uint16_t fgColour, uint16_t bgColour, uint8_t size)
{
	uint8_t row, column;

	// To speed up plotting we define a x window of 6 pixels and then
	// write out one row at a time.  This means the LCD will correctly
	// update the memory pointer saving us a good few bytes

	lcdWriteCommand(SET_COLUMN_ADDRESS); // Horizontal Address Start Position
	lcdWriteParameter(0x00);
	lcdWriteParameter(x);
	lcdWriteParameter(0x00);
	lcdWriteParameter(x+5*size);

	lcdWriteCommand(SET_PAGE_ADDRESS); // Vertical Address end Position
	lcdWriteParameter(0x00);
	lcdWriteParameter(y);
	lcdWriteParameter(0x00);
	lcdWriteParameter(0x7f);

	lcdWriteCommand(WRITE_MEMORY_START);

	// Plot the font data
	for (row = 0; row < 8*size; row+=size)
	{
		for (column = 0; column < 6*size; column+=size)
		{
			uint8_t mirroredColumn = 5*size - column;
			if ((fontus[character][(uint8_t)(mirroredColumn/size)]) & (1 << (uint8_t)(row/size)))
				lcdFilledRectangle(x+column, y+row, x+column+size-1, y+row+size-1, fgColour);
		}
	}
}

// Translates a 3 byte RGB value into a 2 byte value for the LCD (values should be 0-31)
uint16_t decodeRgbValue(uint8_t r, uint8_t g, uint8_t b)
{
	return (b << 11) | (g << 6) | (r);
}

// This routine takes a row number from 0 to 20 and
// returns the x coordinate on the screen (0-127) to make
// it easy to place text
uint8_t lcdTextX(uint8_t x) { return x*6; }

// This routine takes a column number from 0 to 20 and
// returns the y coordinate on the screen (0-127) to make
// it easy to place text
uint8_t lcdTextY(uint8_t y) { return y*8; }

// Plot a string of characters to the LCD
void lcdPutS(const char *string, uint8_t x, uint8_t y, uint16_t fgColour, uint16_t bgColour)
{
	uint8_t origin = x;
	uint8_t characterNumber;

	for (characterNumber = 0; characterNumber < strlen(string); characterNumber++)
	{
		// Check if we are out of bounds and move to
		// the next line if we are
		if (x < 10)
		{
			x = origin;
			y += 8;
		}

		// If we move past the bottom of the screen just exit
		if (y > 230) break;

		// Plot the current character
		lcdPutCh(string[characterNumber], x, y, fgColour, bgColour);
		x -= 6;
	}
}

// Plot a string of characters to the LCD
void lcdPutSSized(const char *string, uint8_t x, uint8_t y, uint16_t fgColour, uint16_t bgColour, uint8_t size)
{
	x = x-size*6;
	uint8_t origin = x;
	uint8_t characterNumber;

	for (characterNumber = 0; characterNumber < strlen(string); characterNumber++)
	{
		// Check if we are out of bounds and move to
		// the next line if we are
		if (x < 1*size)
		{
			x = origin;
			y += 8*size;
		}

		// If we move past the bottom of the screen just exit
		if (y > (239-6*size)) break;

		// Plot the current character
		lcdPutChSized(string[characterNumber], x, y, fgColour, bgColour, size);
		if ((x-6*size)>0) x -= 6*size;
		else
		{
			x = origin;
			y += 8*size;
		}
	}
}

void demoPlot(){
	int16_t Triangle[] = {20, 170, 230, 150, 140, 40};
	int16_t Square[] = {30, 30, 170, 30, 170, 170, 30, 170};
	int16_t Pentagon[] = {50, 20, 150, 20, 180, 120, 100, 180, 10, 120};

	lcdPutS("opakovany vypis:", 220, 10, decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0));
	lcdRectangle(15, 225, 240, 250, decodeRgbValue(255, 255, 255)); //dolny status bar
	lcdCircle(127,232,5,decodeRgbValue(255, 255, 255)); //akysi kruh, v povodnej doom je tam hlava hraca
	//test printing text (normal, sized, with numbers)
	lcdPutS("HELLO WORLD", 220, 230, decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0));
	lcdPutSSized("DOOM-vrs", 230, 190, decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0),4);
	int16_t score = 56;
	char scoreText[16];
	sprintf(scoreText, "SCORE = %d", score);
	lcdPutS(scoreText, 100, 230, decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0));
	lcdPutS("opakovany vypis:", 220, 10, decodeRgbValue(0, 0, 0), decodeRgbValue(0, 0, 0));

	lcdPutS("TEXT velkosti 3:", 220, 10, decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0));
	char demoText[] = "abcdefghijklmnopqrstuvwxyz0123456789,.!?()+-_*/=%";
	lcdPutSSized(demoText, 220, 20, decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0), 3);
	LL_mDelay(2000);
	lcdPutSSized(demoText, 220, 20, decodeRgbValue(0, 0, 0), decodeRgbValue(0, 0, 0), 3);
	lcdPutS("TEXT velkosti 3:", 220, 10, decodeRgbValue(0, 0, 0), decodeRgbValue(0, 0, 0));
	lcdPutS("ANIMACIE:", 220, 10, decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0));
	// animovane stvorce
	for (int16_t i = 0; i<20; i++){
	  lcdRectangle(20+i*10, 50, 100+i*10, 130, decodeRgbValue(255, 255, 255));
	  lcdRectangle(20+i*10, 50, 100+i*10, 130, decodeRgbValue(0, 0, 0));
	}
	for (int16_t i = 0; i<20; i++){
	  lcdDottedRectangle(20+i*10, 50, 100+i*10, 130, decodeRgbValue(255, 255, 255), 3);
	  lcdDottedRectangle(20+i*10, 50, 100+i*10, 130, decodeRgbValue(0, 0, 0), 3);
	}
	for (int16_t i = 0; i<20; i++){
	  lcdDottedRectangle(20+i*10, 50, 100+i*10, 130, decodeRgbValue(255, 255, 255), 5);
	  lcdDottedRectangle(20+i*10, 50, 100+i*10, 130, decodeRgbValue(0, 0, 0), 5);
	}
	// animovany kruh
	for (int16_t i = 0; i<10; i++){
	  lcdDottedCircle(60+i*10, 100, 50, decodeRgbValue(255, 255, 255), 3);
	  lcdDottedCircle(60+i*10, 100, 50, decodeRgbValue(0, 0, 0), 3);
	}
	lcdPutS("ANIMACIE:", 220, 10, decodeRgbValue(0, 0, 0), decodeRgbValue(0, 0, 0));
	lcdPutS("POLYGONY:", 220, 10, decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0));
	//testujem polygon
	lcdPolygon( Pentagon, sizeof(Pentagon) / (2 * sizeof(Pentagon[0])), decodeRgbValue(255, 255, 255));
	LL_mDelay(200);
	lcdPolygon( Pentagon, sizeof(Pentagon) / (2 * sizeof(Pentagon[0])), decodeRgbValue(0, 0, 0));
	lcdDottedPolygon( Pentagon, sizeof(Pentagon) / (2 * sizeof(Pentagon[0])), decodeRgbValue(255, 255, 255), 3);
	LL_mDelay(200);
	lcdDottedPolygon( Pentagon, sizeof(Pentagon) / (2 * sizeof(Pentagon[0])), decodeRgbValue(0, 0, 0), 3);
	//testujem plneny trojuholnik a taktiez plneny polygon
	lcdFilledTriangle( Triangle, decodeRgbValue(255, 255, 255) );
	LL_mDelay(200);
	lcdFilledTriangle( Triangle, decodeRgbValue(0, 0, 0) );
	lcdFilledPolygon( Square, sizeof(Square) / (2 * sizeof(Square[0])), decodeRgbValue(255, 255, 255) );
	LL_mDelay(200);
	lcdFilledPolygon( Square, sizeof(Square) / (2 * sizeof(Square[0])), decodeRgbValue(0, 0, 0) );
	lcdFilledPolygon( Pentagon, sizeof(Pentagon) / (2 * sizeof(Pentagon[0])), decodeRgbValue(255, 255, 255) );
	LL_mDelay(200);
	lcdFilledPolygon( Pentagon, sizeof(Pentagon) / (2 * sizeof(Pentagon[0])), decodeRgbValue(0, 0, 0) );
	lcdFilledDottedPolygon( Pentagon, sizeof(Pentagon) / (2 * sizeof(Pentagon[0])), decodeRgbValue(255, 255, 255), 3 );
	LL_mDelay(200);
	lcdFilledDottedPolygon( Pentagon, sizeof(Pentagon) / (2 * sizeof(Pentagon[0])), decodeRgbValue(0, 0, 0), 3 );
	lcdPutS("POLYGONY:", 220, 10, decodeRgbValue(0, 0, 0), decodeRgbValue(0, 0, 0));
	lcdPutS("STVORCE:", 220, 10, decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0));
	//TESTUJEM VYKRESLENIE STVORCOV
	lcdRectangle(30, 30, 170, 170, decodeRgbValue(255, 255, 255));
	LL_mDelay(200);
	lcdRectangle(30, 30, 170, 170, decodeRgbValue(0, 0, 0));
	lcdDottedRectangle(30, 30, 170, 170, decodeRgbValue(255, 255, 255), 3);
	LL_mDelay(200);
	lcdDottedRectangle(30, 30, 170, 170, decodeRgbValue(0, 0, 0), 3);
	// testujem vykreslenie plneneho stvorca
	lcdFilledRectangle(30, 30, 170, 170, decodeRgbValue(255, 255, 255));
	LL_mDelay(200);
	lcdFilledRectangle(30, 30, 170, 170, decodeRgbValue(0, 0, 0));
	//testujem vykreslenie bodkovaneho stvorca
	lcdFilledDottedRectangle(30, 30, 170, 170, decodeRgbValue(255, 255, 255), 5);
	LL_mDelay(200);
	lcdFilledDottedRectangle(30, 30, 170, 170, decodeRgbValue(0, 0, 0), 5);
	lcdPutS("STVORCE:", 220, 10, decodeRgbValue(0, 0, 0), decodeRgbValue(0, 0, 0));
	lcdPutS("CIARY:", 220, 10, decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0));
	// testujem vykreslenie ciary v oboch smeroch
	lcdLine(10, 10, 190, 300, decodeRgbValue(255, 255, 255));
	LL_mDelay(200);
	lcdLine(10, 10, 190, 300, decodeRgbValue(0, 0, 0));
	lcdLine(10, 10, 300, 150, decodeRgbValue(255, 255, 255));
	LL_mDelay(200);
	lcdLine(10, 10, 300, 150, decodeRgbValue(0, 0, 0));
	lcdPutS("CIARY:", 220, 10, decodeRgbValue(0, 0, 0), decodeRgbValue(0, 0, 0));
	lcdPutS("KRUHY:", 220, 10, decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0));
	// testujem vykreslenie kruhu
	lcdCircle(100, 100, 80, decodeRgbValue(255, 255, 255));
	LL_mDelay(200);
	lcdCircle(100, 100, 80, decodeRgbValue(0, 0, 0));
	lcdDottedCircle(100, 100, 80, decodeRgbValue(255, 255, 255), 3);
	LL_mDelay(200);
	lcdDottedCircle(100, 100, 80, decodeRgbValue(0, 0, 0), 3);
	//testujem vykreslenie plneho kruhu
	lcdFilledCircle(100, 100, 80, decodeRgbValue(255, 255, 255));
	LL_mDelay(200);
	lcdFilledCircle(100, 100, 80, decodeRgbValue(0, 0, 0));
	lcdFilledDottedCircle(100, 100, 80, decodeRgbValue(255, 255, 255), 5);
	LL_mDelay(500);
	lcdFilledDottedCircle(100, 100, 80, decodeRgbValue(0, 0, 0), 5);
	lcdPutS("KRUHY:", 220, 10, decodeRgbValue(0, 0, 0), decodeRgbValue(0, 0, 0));
}
