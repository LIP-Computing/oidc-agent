#include <stdlib.h>
#include <string.h>

#include "oidc_array.h"



void* arr_sort(void* arr, size_t numberElements, size_t elementSize, int (*comp_callback)(const void*, const void*)) {
  qsort(arr, numberElements, elementSize, comp_callback);
  return arr;
}

void* arr_find(void* arr, size_t numberElements, size_t elementSize, void* element, int (*comp_callback)(const void*, const void*)) {
  arr_sort(arr, numberElements, elementSize, comp_callback);
  return bsearch(element, arr, numberElements, elementSize, comp_callback);
}

void* arr_addElement(void* array, size_t* numberElements, size_t elementSize, void* element) {
  array = realloc(array, elementSize * (*numberElements + 1));
  memcpy(array + *numberElements, element, elementSize);
  (*numberElements)++;
  return array;
}

void* arr_removeElement(void* array, size_t* numberElements, size_t elementSize, void* element, int (*comp_callback)(const void*, const void*)) {
  void* pos = arr_find(array, *numberElements, elementSize, element, comp_callback);
  if(NULL==pos)
    return NULL;
  memmove(pos, array + *numberElements - 1, elementSize);
  (*numberElements)--;
  array = realloc(array, elementSize * (*numberElements));
  return array;
}

