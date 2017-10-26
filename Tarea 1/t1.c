#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "t1.h"

uint insertar_bits(uint x, int pos, uint y, int len) {
	uint high_bits, low_bits, inserted_bits;
  	high_bits = (x & (-1 << pos)) << len;
  	low_bits = x & ~(-1 << pos);
  	inserted_bits = y << pos;
  	return high_bits | inserted_bits | low_bits;
}

void eliminar_espacios(char *s) {
	char *i = s;
	char *j = s;
	char *k = s;
	char *lim = s + strlen(s);
	j++;
	while (j < lim) {
		if ((*i != ' ') || (*j != ' ')) {
			*k = *i;
			k++;
		}
		i++;
		j++;	
	}
	*k = *i;
	k++;
	*k = '\0';
}
