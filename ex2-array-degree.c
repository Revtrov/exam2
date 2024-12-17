#include <stdio.h>
#include <stdlib.h>

typedef struct Node Node; // nodes for double linked list
struct Node {
  int key;
  int val;
  Node * next;
  Node * prev;
};

#define NODESIZE sizeof(Node) // macro for node size

typedef struct DLL DLL; // doubly linked list
struct DLL{
  Node * head;
  Node * tail;
};

DLL * createDLL(){ // dll creator helper function
  DLL * dll = malloc(sizeof(DLL));
  dll->head = NULL;
  dll->tail = dll->head;
  return dll;
}

void traverse(DLL * dll, int dir){ // for testing, prints list in specified order
  switch(dir){
    case -1:{
      Node * cur = dll->tail;
      while(cur != NULL){
        printf("(%d, %d)\n",cur->key, cur->val);
        cur = cur->prev;
      }
      break;
    }
    default:{
      Node * cur = dll->head;
      while(cur != NULL){
        printf("(%d, %d)\n",cur->key, cur->val);
        cur = cur->next;
      }
      break;
    }
  }
}

Node * appendNode(DLL * dll, Node * new){ // append to DLL
  if(dll->tail == NULL){ // case if list empty
    new->prev = NULL;
    new->next = NULL;
    dll->head = new; // new node becomes head
    return new;
  }
  dll->tail->next = new;
  new->prev = dll->tail;
  new->next = NULL;
  return new; // return new so tail becomes new
}

void updateOrAppend(DLL * dll, int key){ // search through list, update node if key found else add new node with key
  Node * cur = dll->head;
  while(cur != NULL){ // search
    if(cur->key == key){ // increment existing node if found
      cur->val += 1;
      return;
    }
    cur = cur->next;
  }
  Node * new = malloc(NODESIZE); // else create new node with key and value of one
  new->key = key;
  new->val = 1;
  dll->tail = appendNode(dll, new);
  
}

DLL * calcFrequencies(DLL * dll){ // make a DLL whos nodes map number : frequency
  DLL * frequencies = createDLL();
  Node * cur = dll->head;
  int i = 0;
  while(cur != NULL){
    updateOrAppend(frequencies, cur->key); // for each node in dll add or update frequency node
    cur = cur->next;
  }
  return frequencies;
}

Node * maxNodeValue(DLL * dll){ // find the node with the highest value
  Node * max = dll->head;
  Node * cur = max;
  while(cur != NULL){
    if(cur->val > max->val){
      max = cur;
    }
    cur = cur->next;
  }
  return max;
}

void populateDLL(int * listLength, DLL * dll, char ** argv){
  for(int i = 0; i < *listLength; i++){ // populate list
    Node * new = malloc(NODESIZE);
    new->key = atoi(*(argv+1+i));
    new->val = 1;
    dll->tail = appendNode(dll, new);
  }
}

void freeDLL(DLL * dll){
  Node * cur = dll->head;
  while(cur != NULL){
    Node *temp = cur;
    cur = cur->next;
    free(temp); 
  }
  free(dll);
}

int main(int argc, char ** argv){
  int listLength = argc - 1;
  DLL * dll = createDLL(); // gather numbers into doubly linked list
  populateDLL(&listLength, dll, argv);
  DLL * frequencies = calcFrequencies(dll); // get frequencies
  printf("%d\n", maxNodeValue(frequencies)->val); // get highest frequency
  freeDLL(dll);
  freeDLL(frequencies);
  return 0;
}