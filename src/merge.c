#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/**
The sporadic errors that I get look something like:

❯ ./merge
merge(10702,0x700009d7f000) malloc: Incorrect checksum for freed object
0x7fd0f2c05d80: probably modified after being freed. Corrupt value:
0x6000000000000057 merge(10702,0x700009d7f000) malloc: *** set a breakpoint in
malloc_error_break to debug

fish: Job 1, './merge' terminated by signal SIGABRT
(Abort)

I am running on macOS if that makes a difference.

You can build this by running 'src/build.sh' which creates a binary 'src/merge'.
*/

void
ThreadCreate(pthread_t* thread,
             const pthread_attr_t* attr,
             void* (*start_routine)(void*),
             void* arg);
void
ThreadJoin(pthread_t thread, void** value_ptr);
typedef struct __Sort_Args
{
  /**
  Note that I originally had the input array also passed around in this arg
  struct. I tried removing it after I got the malloc errors and haven't gotten
  around to putting it back in yet.
  */

  int start;
  int end;
} SortArgs;

typedef struct __Merge_Args
{

  /**
  Note that I originally had the input array also passed around in this arg
  struct. I tried removing it after I got the malloc errors and haven't gotten
  around to putting it back in yet.
  */

  int start;
  int middle;
  int end;

} MergeArgs;

void*
mergeSort(void* arg);

void*
merge(void* arg);

#define INPUT_LENGTH 100

int NUMBERS[INPUT_LENGTH] = {
  70, 18, 80, 72, 22, 46,  91, 41, 92, 82, 25, 24, 65, 73, 94, 29, 85,
  68, 74, 56, 23, 27, 6,   7,  12, 31, 57, 8,  42, 36, 58, 76, 3,  11,
  28, 86, 1,  50, 61, 100, 97, 60, 30, 67, 15, 81, 40, 89, 38, 99, 14,
  66, 83, 43, 90, 79, 78,  26, 54, 2,  95, 34, 9,  33, 19, 96, 88, 55,
  39, 52, 5,  37, 64, 98,  53, 87, 51, 49, 69, 84, 21, 16, 71, 47, 45,
  13, 48, 35, 17, 32, 63,  59, 4,  62, 10, 20, 75, 77, 93, 44
};

int
main()
{
  int end = INPUT_LENGTH + 1;
  SortArgs* args = malloc(sizeof(SortArgs));
  args->start = 0;
  args->end = end;
  mergeSort(args);

  int i;
  for (i = 0; i < end; i++) {
    printf("%d ", NUMBERS[i]);
  }
  printf("\n");
}

void*
mergeSort(void* arg)
{

  SortArgs* a = (SortArgs*)arg;
  int start = a->start, end = a->end;
  if (start >= end - 1) {
    return NULL;
  }

  int middle = (start + end) / 2;

  SortArgs* left = malloc(sizeof(SortArgs));
  left->start = start;
  left->end = middle;

  pthread_t left_p;
  ThreadCreate(&left_p, NULL, &mergeSort, left);

  SortArgs* right = malloc(sizeof(SortArgs));
  right->start = middle;
  right->end = end;

  pthread_t right_p;
  ThreadCreate(&right_p, NULL, &mergeSort, right);

  ThreadJoin(left_p, NULL);
  ThreadJoin(right_p, NULL);

  //   free(left);
  //   free(right);

  MergeArgs* mergeArgs = malloc(sizeof(MergeArgs));
  mergeArgs->start = start;
  mergeArgs->middle = middle;
  mergeArgs->end = end;

  merge(mergeArgs);
  //   free(mergeArgs);

  return NULL;
}

void*
merge(void* arg)
{
  MergeArgs* a = (MergeArgs*)arg;
  int start = a->start, middle = a->middle, end = a->end;
  int i, j, k, len;

  len = end - start - 1;
  int* out = malloc((len) * sizeof(int));

  for (i = start, j = middle, k = 0; i < middle && j < end;) {
    if (NUMBERS[i] <= NUMBERS[j]) {
      out[k++] = NUMBERS[i++];
    } else {
      out[k++] = NUMBERS[j++];
    }
  }

  while (i < middle) {
    out[k++] = NUMBERS[i++];
  }

  while (j < end) {
    out[k++] = NUMBERS[j++];
  }

  for (i = start, k = 0; i < end; i++, k++) {
    NUMBERS[i] = out[k];
  }

  free(out);
  return NULL;
}

void
ThreadCreate(pthread_t* thread,
             const pthread_attr_t* attr,
             void* (*start_routine)(void*),
             void* arg)
{
  int result = pthread_create(thread, attr, start_routine, arg);
  if (result != 0) {
    perror("failed to create thread");
    exit(1);
  }
}

void
ThreadJoin(pthread_t thread, void** value_ptr)
{
  int result = pthread_join(thread, value_ptr);
  if (result != 0) {
    perror("failed to join thread");
    exit(1);
  }
}
