#include <stdlib.h>
#include <stdio.h>
#include "array_heap.h"

int array_init(array *arr, int size)
{
	arr->data = malloc(sizeof(void*) * size);
	if(!arr->data){
		printf("array init failed.\n");
		return -1;
	}

	arr->length = size;
	arr->index = 0;
	
	return 0;
}

int array_push(array *arr, void *data)
{
	arr->data[arr->index] = (struct sock_ev_client*)data;
	arr->index++;
	if(arr->index >= arr->length){
		if(array_grow(arr, arr->length * 2) < 0){
			printf("array grow failed.\n");
			return -1;
		}
	}

	return arr->index - 1;
}

int array_grow(array *arr, int size)
{
	if(size <= arr->length){
		return -1;
	}
	arr->data = realloc(arr->data, arr->length * 2);
	if(!arr->data){
		return -1;
	}

	arr->length = size;

	return 0;
}

void array_free(array *arr, void (*free)(void *))
{
	int i;
	for(i = 0; i < arr->index; i +=1){
		free(arr->data[i]);
	}
	free(arr->data);
	arr->index = 1;
	arr->length = 0;
	arr->data = NULL;
}
