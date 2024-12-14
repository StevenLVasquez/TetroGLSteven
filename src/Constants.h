#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

// Size of one single block (in pixels)
const int BLOCK_WIDTH = 30;
const int BLOCK_HEIGHT = 30;

// Size of the matrix (in blocks)
const int MATRIX_WIDTH = 10;
const int MATRIX_HEIGHT = 19;
const int FRAME_TIME = 30;


// Enumeration for the different block colors.
enum EBlockColor
{
	bcNone = 0,
	bcCyan,
	bcBlue,
	bcOrange,
	bcYellow,
	bcGreen,
	bcPurple,
	bcRed
};

#endif  // _CONSTANTS_H_