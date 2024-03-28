#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#define MAXBITS 32
#define MAXSIZE 8

void permute(const int size, unsigned int arr[size], int n, const int target, int current);