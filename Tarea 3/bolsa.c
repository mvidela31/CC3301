#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <limits.h>
#include <stdio.h>

#include "bolsa.h"


char current_buyer[100];
char cheapest_seller[100];
int cheapest_price = INT_MAX;
int selling = 0;
pthread_mutex_t m;
pthread_cond_t waitForBuyer, waitForEndOfSelling;

int vendo(int precio, char *vendedor, char *comprador) {
	pthread_mutex_lock(&m);
	while (selling) {
		pthread_cond_wait(&waitForEndOfSelling, &m);
	}
	while (*current_buyer == '\0') {
		if (precio >= cheapest_price) {
			pthread_mutex_unlock(&m);
			return 0;
		}
		strcpy(cheapest_seller, vendedor);
		cheapest_price = precio;
		pthread_cond_broadcast(&waitForBuyer);
		pthread_cond_wait(&waitForBuyer, &m);
		if (strcmp(cheapest_seller, vendedor) != 0) {
			pthread_mutex_unlock(&m);
			return 0;
		}
	}
	strcpy(comprador, current_buyer);
	*current_buyer = '\0';
	*cheapest_seller = '\0';
	cheapest_price = INT_MAX;
	selling = 0;
	pthread_cond_broadcast(&waitForEndOfSelling);
	pthread_mutex_unlock(&m);
	return 1;
}

int compro(char *comprador, char *vendedor) {
	pthread_mutex_lock(&m);
	while (selling) {
		pthread_cond_wait(&waitForEndOfSelling, &m);
	}
	if (*cheapest_seller == '\0') {
		pthread_mutex_unlock(&m);
		return 0;
	}
	selling = 1;
	strcpy(vendedor, cheapest_seller);
	strcpy(current_buyer, comprador);
	int selling_price = cheapest_price;
	pthread_cond_broadcast(&waitForBuyer);
	pthread_mutex_unlock(&m);
	return selling_price;
}
