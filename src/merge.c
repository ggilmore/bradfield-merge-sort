#include <stdio.h>
#include <stdlib.h>

#define INPUT_LENGTH 100

int NUMBERS[INPUT_LENGTH] = {
  70, 18, 80, 72, 22, 46,  91, 41, 92, 82, 25, 24, 65, 73, 94, 29, 85,
  68, 74, 56, 23, 27, 6,   7,  12, 31, 57, 8,  42, 36, 58, 76, 3,  11,
  28, 86, 1,  50, 61, 100, 97, 60, 30, 67, 15, 81, 40, 89, 38, 99, 14,
  66, 83, 43, 90, 79, 78,  26, 54, 2,  95, 34, 9,  33, 19, 96, 88, 55,
  39, 52, 5,  37, 64, 98,  53, 87, 51, 49, 69, 84, 21, 16, 71, 47, 45,
  13, 48, 35, 17, 32, 63,  59, 4,  62, 10, 20, 75, 77, 93, 44
};

void
mergeSort(int* input, int start, int end);

void
merge(int* input, int start, int middle, int end);

int
main()
{
  int end = INPUT_LENGTH + 1;
  mergeSort(NUMBERS, 0, end);

  int i;
  for (i = 0; i < end; i++) {
    printf("%d ", i);
  }
  printf("\n");
}

void
mergeSort(int* input, int start, int end)
{
  if (start >= end - 1) {
    return;
  }

  int middle = (start + end) / 2;

  mergeSort(input, start, middle);
  mergeSort(input, middle, end);

  merge(input, start, middle, end);
}

void
merge(int* input, int start, int middle, int end)
{
  int i, j, k, len;

  len = end - start - 1;
  int* out = malloc((len) * sizeof(int));

  for (i = start, j = middle, k = 0; i < middle && j < end;) {
    if (input[i] <= input[j]) {
      out[k++] = input[i++];
    } else {
      out[k++] = input[j++];
    }
  }

  while (i < middle) {
    out[k++] = input[i++];
  }

  while (j < end) {
    out[k++] = input[j++];
  }

  for (i = 0; i < end; i++) {
    input[i] = out[i];
  }

  free(out);
}
