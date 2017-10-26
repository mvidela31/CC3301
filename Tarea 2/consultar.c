#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "consultar.h"

void erase_blanks(char *str) {
	char *i = str + strlen(str) - 1;
		while(*i == ' ') {
			i--;
		}
	i++;
	*i='\0';
}

void search(FILE *in, char *buf, char *key, char *r_key, char *value, int init_pos) {
	if (init_pos == -1) {
		printf("palabra no encontrada\n");
		return;
	}
	size_t size;
	fseek(in, 100 * init_pos, SEEK_SET);	
	if ((size = fread(buf, 1, 100, in)) != 0) {
		strncpy(r_key, buf + 20, 20);
		erase_blanks(r_key);
		if (strcmp(key, r_key) > 0) {
			search(in, buf, key, r_key, value, atoi(buf + 10));
		}
		else if (strcmp(key, r_key) < 0) {
			search(in, buf, key, r_key, value, atoi(buf));
		}
		else {
			strncpy(value, buf + 40, 59);
			erase_blanks(value);
			printf("%s\n", value);
			return;
		}
	}
}

int main(int argc, char *argv[]) {
    FILE *in;
	if (argc != 3) {
		fprintf(stderr, "<diccionario.txt> <busqueda>\n");
	    exit(1);
	}
	char *key = argv[2];
	if ((in = fopen(argv[1], "r")) == NULL) {
 		fprintf(stderr, "No se puede leer '%s'\n", argv[1]);
 		exit(1);
 	}
 	char buf[100];
 	char value[59];
 	char r_key[20];
    search(in, buf, key, r_key, value, 0);
    fclose(in);
}
