#define _POSIX_C_SOURCE 1
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

#include "suma.h"

Set leer(Set fd, void *buf, int n) {
 	if (n == 0)
 		return 0;
 	do {
 		Set rc = read(fd, buf, n);
 		if (rc <= 0)
 			return 1; /* fracaso: error o fin del archivo/pipe/socket */
 		n -= rc; /* descontamos los bytes leídos */
 		buf = (char*)buf + rc; /* avanzamos el buffer */
 	} 
	while (n > 0); /* mientras no leamos todo lo que esperamos */
 	return 0; /* exito */
}

Set buscar(int a[], int n) {
	Set comb = (1 << (n-1) << 1) - 1; // 2^(n)-1: nº de combinaciones
	Set subComb = comb / 8;
	Set zero = 0;	
	pid_t children[8];
	int fds[8][2];
	int i;
	for (i = 0; i < 8; i++) {
		if (pipe(fds[i]) != 0) {
        	perror("Can't create pipe");
            exit(1);
    	}	
    	if ((children[i] = fork()) == -1) {
    		perror("Can't fork'");
    		exit(1);
    	}
    	if (children[i] == 0) {
    		close(fds[i][0]);
    		Set currentComb;
     		for (currentComb = i*subComb + 1; currentComb <= (i + 1)*subComb; currentComb++) {
     			Set sum = 0;
     			for (int j = 0; j < n; j++) {
					if (currentComb & ((Set)1 << j)) {
						sum += a[j];
					}
				}
				if (sum == 0) {
					write(fds[i][1], &currentComb, sizeof(Set));
					close(fds[i][1]);
					exit(1);
				}
			}
			write(fds[i][1], &zero, sizeof(Set));
			close(fds[i][1]);
			exit(1);
    	}
		else {
		}	
    }
    for (i = 0; i < 8; i++) {
		close(fds[i][1]);
	}
    Set result = 0;
    int process = 0;
    while (process < 8) {
    	pid_t finished_pid = waitpid(0, NULL, 0);
    	process++;
		for (i = 0; i < 8; i++) {
			if (children[i] == finished_pid) {
				leer(fds[i][0], &result, sizeof(Set));
				close(fds[i][0]);
				if (result != 0) {	
					int k;
					for (k = 0; k < 8; k++) {
						kill(children[k], SIGTERM);
					}
					process = 8;
				}
			}
		}
	}
	for (i = 0; i < 8; i++) {
	waitpid(children[i], NULL, 0);
	}
    return result;
}
