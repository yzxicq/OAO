#include "common.h"

int XChar2bLen(XChar2b *string){
	int j = 0;
	for(j = 0; string[j].byte1 || string[j].byte2; j ++ )
			;
	return j;
}

int utf8toXChar2b(XChar2b *output_r, int outsize, const char *input, int inlen){
	int j, k;
	for(j =0, k=0; j < inlen && k < outsize; j ++){
		unsigned char c = input[j];
		if (c < 128)  {
			output_r[k].byte1 = 0;
			output_r[k].byte2 = c; 
			k++;
		} else if (c < 0xC0) {
			/* we're inside a character we don't know  */
			continue;
		} else switch(c&0xF0){
		case 0xC0: case 0xD0: /* two bytes 5+6 = 11 bits */
			if (inlen < j+1){ return k; }
			output_r[k].byte1 = (c&0x1C) >> 2;
			j++;
			output_r[k].byte2 = ((c&0x3) << 6) + (input[j]&0x3F);
			k++;
			break;
		case 0xE0: /* three bytes 4+6+6 = 16 bits */ 
			if (inlen < j+2){ return k; }
			j++;
			output_r[k].byte1 = ((c&0xF) << 4) + ((input[j]&0x3C) >> 2);
			c = input[j];
			j++;
			output_r[k].byte2 = ((c&0x3) << 6) + (input[j]&0x3F);
			k++;
			break;
		case 0xFF:
			/* the character uses more than 16 bits */
			continue;
		}
	}
	output_r[k].byte2 = 0;
	output_r[k].byte1 = 0;
	return k;
}
