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

void printXc(XChar2b *string){
	int len = XChar2bLen(string);
	int j;
	char buf[4];
	for (j = 0; j < len; j += 1){
		int ch = string[j].byte1 *256 + string[j].byte2 ;
		if (ch < 128){
			fputc(ch, stdout);
		} else if (ch < (1<<11)) {
			buf[0] = 0xB | (ch >> 6);
			buf[1] = 0x8 | (ch &0x3F);
			buf[2] = 0;
			fputs(buf, stdout);
		} else {
			buf[0] = 0xB | (ch >> 12);
			buf[1] = 0x8 | ((ch>>6) &0x3F);
			buf[2] = 0x8 | (ch &0x3F);
			buf[3] = 0;
			fputs(buf, stdout);
		}
	}
}
