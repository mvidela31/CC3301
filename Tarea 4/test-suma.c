#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>

#include "suma.h"

Set buscarSeq(int a[], int n) {
  Set comb= (1<<(n-1)<<1)-1; // 2n-1: nro. de combinaciones
  for (Set k= 1; k<=comb; k++) {
    // k es el mapa de bits para el subconjunto { a[i] | bit ki de k es 1 }
    long long sum= 0;
    for (int i= 0; i<n; i++) {
      if ( k & ((Set)1<<i) ) // si bit ki de k es 1
        sum+= a[i];
    }
    if (sum==0) {  // exito: el subconjunto suma 0
      return k;    // y el mapa de bits para el subconjunto es k
  } }
  return 0;        // no existe subconjunto que sume 0
}

void mostrar(int a[], Set k, int n) {
  long long sum= 0;
  if (k==0) {
    printf("subconjunto no existe\n");
    return;
  }
  for (int i= 0; i<n; i++) {
    if ( k & ((Set)1<<i) ) {
      printf("%d ", a[i]);
      sum+= a[i];
    }
  }
  if (sum!=0) {
    fprintf(stderr, "El subconjunto suma %lld, no 0\n", sum);
  }
  printf("\n");
}

long init_time= 0;

long getTime() {
    struct timeval Timeval;

    gettimeofday(&Timeval, NULL);
    return Timeval.tv_sec*1000+Timeval.tv_usec/1000-init_time;
}  

int main() {
  init_time= getTime();
  printf("El ejemplo del enunciado\n");
  int a[]= { -7, -3, -2, 5, 8};
  Set kSeq= buscarSeq(a, 5);
  mostrar(a, kSeq, 5);
  Set k= buscar(a, 5);
  if (k!=kSeq) {
    fprintf(stderr, "La solucion debio ser %lld, no %lld\n", kSeq, k);
    exit(1);
  }
  mostrar(a, k, 5);
  printf("Ok\n");

  printf("Uno ejemplo mas grande con n=26, sin solucion\n");
  int b[]= { 122737, -37364, 287373, -27267, 967923, -25383, 924973, -28973,
             278363, 28272, 98734, -26735, -983267, 674998, 72537, 116725,
             72537, 27263, 82739, 829276, -5383715, 675483, -28334, 38495,
             374943, 278367};
  long ini= getTime();
  k= buscarSeq(b, 26);
  if (k!=0) {
    mostrar(b, k, 26);
    fprintf(stderr, "Bug del profesor: la solucion debio ser %d, no %lld\n",
                    0, k);
    exit(1);
  }
  long seq_time= getTime()-ini;
  printf("Tiempo secuencial es %ld milisegundos\n", seq_time);
  ini= getTime();
  k= buscar(b, 26);
  if (k!=0) {
    mostrar(b, k, 26);
    fprintf(stderr, "La solucion debio ser %d, no %lld\n", 0, k);
    exit(1);
  }
  long par_time= getTime()-ini;
  printf("Ok en paralelo con %ld milisegundos\n", par_time);
  if (par_time>2*seq_time/3) {
    fprintf(stderr, "Su solucion no es suficientemente eficiente\n");
    fprintf(stderr, "Verifique que el procesador es dual-core al menos\n");
    exit(1);
  }
  
  printf("Uno ejemplo muy grande con n=29, con solucion\n");
  int c[]= { -3, -2, 5,
             122737, -37364, 287373, 27267, 967923, -25383, 924973, -28973,
             278363, 28272, 98734, -26735, 983267, 674998, 72537, 116725,
             72537, 27263, 82739, 829276, 5383715, 675483, -28334, 38495,
             374943, 278367};
  ini= getTime();
  k= buscar(c, 29);
  par_time= getTime()-ini;
  mostrar(c, k, 29);
  printf("Ok en paralelo con %ld milisegundos\n", par_time);
  if (par_time>500) {
    fprintf(stderr, "Su solucion no es suficientemente eficiente.\n");
    fprintf(stderr, "Cuando un proceso encuentre una solucion,\n");
    fprintf(stderr, "mate el resto con kill(pid, SIGTERM).\n");
    fprintf(stderr, "Entierrelos y retorne lo antes posible.\n");
    exit(1);
  }
  
  printf("Felicitaciones, su tarea paso todos los tests\n");
  return 0;
}
