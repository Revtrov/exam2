#include <stdio.h>
#include <stdlib.h>

typedef struct Component Component; // the nodes in linked list / matrix
struct Component
{
  int x;
  int y;
  int value;
  Component *next;
  Component *prev;
};
Component * appendComponent(Component *tail, Component * new){ // append for doubly linked list
  if(tail == NULL){
    return new;
  }
  tail->next = new;
  new->prev = tail;
  new->next = NULL;
  return new;
}
// could have stuck to just an int ** matrix
Component ** generateMatrix(int * width, int * height, Component ** points) {
  Component ** matrix = malloc(sizeof(Component)*(*height)); // make a 2d array
  for(int i = 0; i < *height; i++){
    *(matrix+i)= malloc(sizeof(Component)*(*width)); // give it lenght and height
  }
  for(int i = 0; i < (*width); i++){
    for(int j = 0; j < (*height); j++){
      Component * cur = *(matrix+j)+i; //swap i and j for proper indexing
      cur->x = i; // save x and y in case they would be used in shader calcs
      cur->y = j;
      cur->value = 0; // initialize components to 0
    }
  }
  if(points != NULL){ // only insert if there are points
    Component * point = *points;
    while(point != NULL){ // loop through points
      Component * component = *(matrix + point->y) + point->x; // goto component in matrix
      component->value = point->value; // fill value
      point = point->next; // traverse
    }
  }
  return matrix;
}
// passing in the matrix would have been more efficient.
// matrix indexing with pointers again would be annoying.
int sumDiagonal(int * width, int *height, Component * points) { 
  if(points == NULL || *width != *height){ // return zero if no points or non square matrix
    return 0;
  }
  Component * point = points;
  int sum = 0;
  while(point != NULL){
    if(point->x == point->y){
      sum += point->value; // if indexes equal the are on main diagonal
    }
    point = point->next;
  }
  return sum;
}
void printMatrix(int *width, int * height, Component *** matrix){ // print matrix here
  for (int i = 0; i < *width; i++){
    for(int j = 0; j < *height; j++){
      printf("%d ", (*((*matrix) + j) + i)->value);
    }
    printf("\n");
  } 
}
void freeMatrix(int * height, Component ** matrix) { // free up matrix's memory
  for(int i = 0; i < *height; i++) {
    free(matrix[i]);
  }
  free(matrix);
}

Component * extractComponents(int *pointCount, char ** points){
  Component * head = malloc(sizeof(Component));
  Component * tail = head; // keep index of tail for faster append
  for(int i = 0; i < *pointCount;i++){
    Component * new = malloc(sizeof(Component));
    new->value = atoi(*(points+(i*3)));
    new->x = atoi(*(points+(i*3)+1));
    new->y = atoi(*(points+(i*3)+2));
    tail = appendComponent(tail, new);
  }
  head = head->next;
  free(head->prev); // free the temport head
  head->prev = NULL;
  return head;
}
void freeComponents(Component * head) { // free up linked list memory
  Component * cur = head;
  while(cur != NULL) {
    Component * next = cur->next;
    free(cur);
    cur = next;
  }
}

int main(int argc, char **argv)
{
  int width = atoi(*(argv + 1)); // get matrix dimensions
  int height = atoi(*(argv + 2));
  int pointCount = atoi(*(argv + 3)); // get amount of points
  Component * points = extractComponents(&pointCount, argv + 4);
  Component ** matrix = generateMatrix(&width, &height, &points); // pass points to fill matrix if any
  printMatrix(&width, &height, &matrix);
  printf("Sum of main diagonal: %d\n", sumDiagonal(&width, &height, points));
  freeComponents(points);
  freeMatrix(&height, matrix);
  return 0;
}