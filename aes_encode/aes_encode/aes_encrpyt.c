
#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <math.h>
static const unsigned char sbox[16][16] = {
	{0x63,0x7C,0x77,0x7B,0xF2,0x6B,0x6F,0xC5,0x30,0x01,0x67,0x2B,0xFE,0xD7,0xAB,0x76},
	{0xCA,0x82,0xC9,0x7D,0xFA,0x59,0x47,0xF0,0xAD,0xD4,0xA2,0xAF,0x9C,0xA4,0x72,0xC0},
	{0xB7,0xFD,0x93,0x26,0x36,0x3F,0xF7,0xCC,0x34,0xA5,0xE5,0xF1,0x71,0xD8,0x31,0x15},
	{0x04,0xC7,0x23,0xC3,0x18,0x96,0x05,0x9A,0x07,0x12,0x80,0xE2,0xEB,0x27,0xB2,0x75},
	{0x09,0x83,0x2C,0x1A,0x1B,0x6E,0x5A,0xA0,0x52,0x3B,0xD6,0xB3,0x29,0xE3,0x2F,0x84},
	{0x53,0xD1,0x00,0xED,0x20,0xFC,0xB1,0x5B,0x6A,0xCB,0xBE,0x39,0x4A,0x4C,0x58,0xCF},
	{0xD0,0xEF,0xAA,0xFB,0x43,0x4D,0x33,0x85,0x45,0xF9,0x02,0x7F,0x50,0x3C,0x9F,0xA8},
	{0x51,0xA3,0x40,0x8F,0x92,0x9D,0x38,0xF5,0xBC,0xB6,0xDA,0x21,0x10,0xFF,0xF3,0xD2},
	{0xCD,0x0C,0x13,0xEC,0x5F,0x97,0x44,0x17,0xC4,0xA7,0x7E,0x3D,0x64,0x5D,0x19,0x73},
	{0x60,0x81,0x4F,0xDC,0x22,0x2A,0x90,0x88,0x46,0xEE,0xB8,0x14,0xDE,0x5E,0x0B,0xDB},
	{0xE0,0x32,0x3A,0x0A,0x49,0x06,0x24,0x5C,0xC2,0xD3,0xAC,0x62,0x91,0x95,0xE4,0x79},
	{0xE7,0xC8,0x37,0x6D,0x8D,0xD5,0x4E,0xA9,0x6C,0x56,0xF4,0xEA,0x65,0x7A,0xAE,0x08},
	{0xBA,0x78,0x25,0x2E,0x1C,0xA6,0xB4,0xC6,0xE8,0xDD,0x74,0x1F,0x4B,0xBD,0x8B,0x8A},
	{0x70,0x3E,0xB5,0x66,0x48,0x03,0xF6,0x0E,0x61,0x35,0x57,0xB9,0x86,0xC1,0x1D,0x9E},
	{0xE1,0xF8,0x98,0x11,0x69,0xD9,0x8E,0x94,0x9B,0x1E,0x87,0xE9,0xCE,0x55,0x28,0xDF},
	{0x8C,0xA1,0x89,0x0D,0xBF,0xE6,0x42,0x68,0x41,0x99,0x2D,0x0F,0xB0,0x54,0xBB,0x16}
};

static const unsigned char rcon[10][4] = {
	{0x01,0x00,0x00,0x00},
	{0x02,0x00,0x00,0x00},
	{0x04,0x00,0x00,0x00},
	{0x08,0x00,0x00,0x00},
	{0x10,0x00,0x00,0x00},
	{0x20,0x00,0x00,0x00},
	{0x40,0x00,0x00,0x00},
	{0x80,0x00,0x00,0x00},
	{0x1B,0x00,0x00,0x00},
	{0x36,0x00,0x00,0x00}
};

unsigned char cipherKey[4][4] = {
	{0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00}
};

unsigned char cipherText[4][4] = {
	{0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00}
};
unsigned char plainText[4][4] = {
	{0xd4,0xbf,0x5d,0x30},
	{0xe0,0xb4,0x52,0xae},
	{0xb8,0x41,0x11,0xf1},
	{0x1e,0x27,0x98,0xe5}
};
unsigned char multiplier[4][4] = {
	{0x02,0x03,0x01,0x01},
	{0x01,0x02,0x03,0x01},
	{0x01,0x01,0x02,0x03},
	{0x03,0x01,0x01,0x02}
};

void xorRows(unsigned char result[4], unsigned char row[4], unsigned char row1[4]) {
	for (int i = 0; i < 4; i++) {
		result[i] = row[i] ^ row1[i];
	}
}
void copyRow(unsigned char row[4], unsigned char row1[4]) {
	for (int i = 0; i < 4; i++) {
		row[i] = row1[i];
	}
	printf("\n");
}
void copyBlock(unsigned char block[4][4], unsigned char block1[4][4]) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			block[i][j] = block1[i][j];
		}
	}
}

void printRow(unsigned char row[4]) {
	for (int i = 0; i < 4; i++)
		printf("%02X ", row[i]);
	printf("\n");
}
void printBlock(unsigned char block[4][4]) {
	for (int i = 0; i < 4; i++) {
		printf("\n");
		for (int j = 0; j < 4; j++) {
			printf("%02X ", block[i][j]);
		}
	}
	printf("\n");
}

void getSboxValues(unsigned char row[4]) {
	for (int i = 0; i < 4; i++) {
		unsigned char temp = row[i];
		int temp1 = (int)temp & 0x0f;
		int temp2 = (int)(temp >> 4) & 0x0f;
		//printf("%x\n", sbox[temp1][temp2]);
		row[i] = sbox[temp2][temp1];
	}
}

unsigned char hexMultiply(unsigned char aa, unsigned char bb) {
	int a = (int)aa & 0xff, b1 = (int)bb & 0xff, b = (int)bb & 0xff, flag = 0, twos = 0, one = 0, x = b;
	if (a > 1) {
		unsigned char xor1B = 0x1b;
		if (x >> 7 == 0x01) {
			flag = 1;
		}
		int temp = a;
		while (temp > 0) {

			if (temp >= 2) {
				twos++;
			}
			if (temp == 1)
				one = 1;
			temp = temp - 2;
		}
		for (int i = 0; i < twos; i++) {
			b = (b << 1) & 0xff;
		}
		if (one == 1) {
			b = b ^ b1;
		}
		if (flag == 1) {
			b = b ^ xor1B;
		}
	}
	return  (unsigned char)b;
}


unsigned char roundKeys[11][4][4] = { 0x00 };

void keyScheduleRow1(int roundnumber) { //start from roundnumber=1
	unsigned char temp[4];
	temp[3] = roundKeys[roundnumber - 1][3][0];
	for (int i = 0; i < 3; i++) {
		temp[i] = roundKeys[roundnumber - 1][3][i + 1];
	}
	getSboxValues(temp);
	xorRows(temp, temp, rcon[roundnumber - 1]);
	xorRows(temp, temp, roundKeys[roundnumber - 1][0]);
	copyRow(roundKeys[roundnumber][0], temp);
	//printRow(temp);
}
void keyScheduleRow2to4(int roundnumber) {
	unsigned char temp[4];
	for (int i = 1; i < 4; i++) {
		xorRows(temp, roundKeys[roundnumber - 1][i], roundKeys[roundnumber][i - 1]);
		copyRow(roundKeys[roundnumber][i], temp);
	}
}
void keySchedule() {
	copyBlock(roundKeys[0], cipherKey);
	printBlock(roundKeys[0]);
	printf("Starting Scheduling: \n\n");
	for (int i = 1; i <= 10; i++) {
		keyScheduleRow1(i);
		keyScheduleRow2to4(i);
	}
}

void subBytes(unsigned char block[4][4]) {
	for (int i = 0; i < 4; i++) {
		getSboxValues(block[i]);
	}
	//printf("after sub bytes : \n");
	//printBlock(block);
}
void shiftRows(unsigned char block[4][4]) {
	//row 1
	unsigned char temp = block[0][1];
	block[0][1] = block[1][1];
	block[1][1] = block[2][1];
	block[2][1] = block[3][1];
	block[3][1] = temp;
	//row2
	temp = block[0][2];
	block[0][2] = block[1][2];
	block[1][2] = block[2][2];
	block[2][2] = block[3][2];
	block[3][2] = temp;
	temp = block[0][2];
	block[0][2] = block[1][2];
	block[1][2] = block[2][2];
	block[2][2] = block[3][2];
	block[3][2] = temp;
	//row3
	temp = block[0][3];
	block[0][3] = block[1][3];
	block[1][3] = block[2][3];
	block[2][3] = block[3][3];
	block[3][3] = temp;
	temp = block[0][3];
	block[0][3] = block[1][3];
	block[1][3] = block[2][3];
	block[2][3] = block[3][3];
	block[3][3] = temp;
	temp = block[0][3];
	block[0][3] = block[1][3];
	block[1][3] = block[2][3];
	block[2][3] = block[3][3];
	block[3][3] = temp;

	//printf("after shift rows : \n");
	//printBlock(block);
}
void mixColumns(unsigned char block[4][4]) {
	//unsigned char temp = 0x00;
	for (int i = 0; i < 4; i++) {
		unsigned char row[4] = { 0x00 };
		for (int j = 0; j < 4; j++) {
			unsigned char temp[4] = { 0x00 };
			for (int k = 0; k < 4; k++) {
				//printf(" %02x * %02x \n", multiplier[j][k], block[i][k]);
				temp[k] = hexMultiply(multiplier[j][k], block[i][k]);
				//printf(" tempk %02x\n",temp[k] );
			}
			row[j] = temp[0] ^ temp[1] ^ temp[2] ^ temp[3];
			//printf("columnValue: %02x \n", block[i][j]);
		}
		copyRow(block[i], row);

	}
	//printf("after multiplication : \n");
	//printBlock(block);
}
void addRoundKey(unsigned char block[4][4], unsigned char block1[4][4]) {
	for (int i = 0; i < 4; i++) {
		xorRows(block[i], block[i], block1[i]);
	}
}


void initialRound() {
	addRoundKey(plainText, cipherKey);
	printf("after initial: \n");
	printBlock(plainText);
}
void Round1to9() {
	for (int i = 1; i < 10; i++) {
		printf("round  %d \n\n", i);
		subBytes(plainText);
		printf("after subbytes: \n");
		printBlock(plainText);
		shiftRows(plainText);
		printf("after sr: \n");
		printBlock(plainText);
		mixColumns(plainText);
		printf("after mc: \n");
		printBlock(plainText);
		addRoundKey(plainText, roundKeys[i]);
		printf("after adkey: \n");
		printBlock(plainText);
	}
}
void Round10() {
	subBytes(plainText);
	printf("after sb: \n");
	printBlock(plainText);
	shiftRows(plainText);
	printf("after sr: \n");
	printBlock(plainText);
	addRoundKey(plainText, roundKeys[10]);
	printf("after adkey: \n");
	printBlock(plainText);
	copyBlock(cipherText, plainText);
	//copyBlock(cipherKey, plainText);
	printf("cipherText: \n");
	printBlock(cipherText);
}

void encrypt() {
	initialRound();
	Round1to9();
	Round10();
}

int readBlockFromFile(unsigned char block[4][4], FILE *fp) {
	int isEOF = 0, count = 0;
	int temp = 0x00;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (isEOF == 0) {
				if (fscanf(fp, "%02x", &temp) == EOF) {
					block[i][j] = " ";
					isEOF = count == 0 ? 1 : 0;
				}
				else {
					count++;
					block[i][j] = (unsigned char)temp & 0xFF;
				}
			}

			else {
				block[i][j] = " ";
			}
		}

	}
	return isEOF;
}

int readBlockFromImage(unsigned char block[4][4], FILE *fp) {
	int isEOF = 0, count = 0;
	int temp = 0x00;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (isEOF == 0) {
				if ((temp = fgetc(fp) )== EOF) {
					block[i][j] = " ";
					isEOF = count == 0 ? 1 : 0;
				}
				else {
					count++;
					block[i][j] = temp & 0xFF;
					printf("%02x", block[i][j]);
				}
			}

			else {
				block[i][j] = " ";
			}
		}

	}
	return isEOF;
}

void writeBlockToFile(unsigned char block[4][4], FILE *fp) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			fprintf(fp, "%02X", block[i][j]);
		}
	}
}
void writeBlockToImage(unsigned char block[4][4], FILE *fp) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			fputc(block[i][j], fp);
		}
	}
}

int main() {
	int option;
	printf("AES Encryption\n\n");
	printf("Place plainText.txt or plainImage.jpg  and cipherKey.txt files in parent directory \n\n");
	printf("Press to encrypt: \n1 for Text file. \n2 for JPG file.\n\n");
	option = getch();
	FILE *ck = fopen("E:\\8th semester\\cs\\mid project\\projectSpace\\cipherKey.txt", "r");
	readBlockFromFile(cipherKey, ck);
	printf("ck");
	printBlock(cipherKey);
	fclose(ck);
	keySchedule();
	if (option == '1') {
		int buf = 0x00, count = 0;
		FILE *pt, *ct;
		pt = fopen("E:\\8th semester\\cs\\mid project\\projectSpace\\plainText.txt", "r");
		ct = fopen("E:\\8th semester\\cs\\mid project\\projectSpace\\cipherText.txt", "w");
		
		while (!readBlockFromFile(plainText, pt)) {
			printf("pt");
			printBlock(plainText);
			encrypt();
			writeBlockToFile(cipherText, ct);
		}
		fclose(pt);
		fclose(ct);
	}
	else if (option == '2') {
		FILE *pi, *ci;
		int temp;
		unsigned char temp1;
		pi = fopen("E:\\8th semester\\cs\\mid project\\projectSpace\\plainImage.jpg", "rb");
		ci = fopen("E:\\8th semester\\cs\\mid project\\projectSpace\\cipherImage.jpg", "wb");
		while (!readBlockFromImage(plainText,pi))
		{
			encrypt();
			writeBlockToImage(cipherText, ci);
		}
		fclose(pi);
		fclose(ci);
	}
	return 0;
}

