// list/list.c
//
// Implementation for linked list.
//
// <Author>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>  // For bool type

#include "list.h"

// Allocates memory for a new list and initializes it
list_t *list_alloc() { 
    list_t *mylist = (list_t *) malloc(sizeof(list_t)); 
    mylist->head = NULL;
    return mylist;
}

// Frees the memory allocated for the list and its nodes
void list_free(list_t *l) {
    node_t *current = l->head;
    node_t *next;
    while (current != NULL) {
        next = current->next;
        free(current);  // Free each node
        current = next;
    }
    free(l);  // Finally, free the list itself
}

// Prints all elements in the list
void list_print(list_t *l) {
    node_t *cur = l->head; 
    while (cur != NULL) {
        printf("%d -> ", cur->value);
        cur = cur->next;
    }
    printf("NULL\n");
}

// Converts the list to a string representation
char *listToString(list_t *l) {
    char *buf = (char *) malloc(sizeof(char) * 1024);
    buf[0] = '\0';  // Initialize buffer as empty string
    char tbuf[20];

    node_t *curr = l->head;
    while (curr != NULL) {
        sprintf(tbuf, "%d -> ", curr->value);
        strcat(buf, tbuf);
        curr = curr->next;
    }
    strcat(buf, "NULL");
    return buf;
}

// Returns the number of elements in the list
int list_length(list_t *l) {
    int length = 0;
    node_t *cur = l->head;
    while (cur != NULL) {
        length++;
        cur = cur->next;
    }
    return length;
}

// Adds a new element to the back of the list
void list_add_to_back(list_t *l, elem value) {
    node_t *new_node = (node_t *) malloc(sizeof(node_t));
    new_node->value = value;
    new_node->next = NULL;

    if (l->head == NULL) {
        l->head = new_node;  // List was empty
        return;
    }

    node_t *cur = l->head;
    while (cur->next != NULL) {
        cur = cur->next;
    }
    cur->next = new_node;  // Add to end
}

// Adds a new element to the front of the list
void list_add_to_front(list_t *l, elem value) {
    node_t *new_node = (node_t *) malloc(sizeof(node_t));
    new_node->value = value;
    new_node->next = l->head;  // Point to current head
    l->head = new_node;  // Update head
}

void list_add_at_index(list_t *l, elem value, int index) {
    if (index < 0) return;  // Invalid index

    node_t *new_node = (node_t *) malloc(sizeof(node_t));
    new_node->value = value;

    if (index == 0) {
        new_node->next = l->head;
        l->head = new_node;
        return;
    }

    node_t *cur = l->head;
    int i;  // Declare the loop variable outside the for loop
    for (i = 0; i < index - 1 && cur != NULL; i++) {
        cur = cur->next;
    }

    if (cur == NULL) {
        free(new_node);
        return;
    }

    new_node->next = cur->next;
    cur->next = new_node;
}


// Removes and returns the last element of the list
elem list_remove_from_back(list_t *l) {
    if (l->head == NULL) return -1;  // List is empty

    node_t *cur = l->head;
    if (cur->next == NULL) {
        elem value = cur->value;
        free(cur);
        l->head = NULL;
        return value;
    }

    node_t *prev = NULL;
    while (cur->next != NULL) {
        prev = cur;
        cur = cur->next;
    }

    elem value = cur->value;
    free(cur);
    prev->next = NULL;
    return value;
}

// Removes and returns the first element of the list
elem list_remove_from_front(list_t *l) {
    if (l->head == NULL) return -1;  // List is empty

    node_t *cur = l->head;
    elem value = cur->value;
    l->head = cur->next;  // Move head to next node
    free(cur);
    return value;
}

elem list_remove_at_index(list_t *l, int index) {
    if (index < 0 || l->head == NULL) return -1;  // Invalid index or empty list

    // Special case for removing the first element
    if (index == 0) {
        return list_remove_from_front(l);
    }

    node_t *cur = l->head;
    node_t *prev = NULL;
    int i;  // Declare the loop variable outside the for loop
    for (i = 0; i < index && cur != NULL; i++) {
        prev = cur;
        cur = cur->next;
    }

    if (cur != NULL) {
        elem value = cur->value;
        prev->next = cur->next;  // Bypass the node to remove
        free(cur);  // Free the removed node
        return value;
    }

    return -1;  // Index out of bounds
}


// Returns true if the element is in the list
bool list_is_in(list_t *l, elem value) {
    node_t *cur = l->head;
    while (cur != NULL) {
        if (cur->value == value) return true;
        cur = cur->next;
    }
    return false;
}

elem list_get_elem_at(list_t *l, int index) {
    if (index < 0) return -1;  // Invalid index

    node_t *cur = l->head;
    int i;  // Declare the loop variable outside the for loop
    for (i = 0; i < index && cur != NULL; i++) {
        cur = cur->next;
    }

    if (cur != NULL) {
        return cur->value;
    }

    return -1;  // Index out of bounds
}


// Returns the index of the element in the list, or -1 if not found
int list_get_index_of(list_t *l, elem value) {
    node_t *cur = l->head;
    int index = 0;
    while (cur != NULL) {
        if (cur->value == value) return index;
        cur = cur->next;
        index++;
    }
    return -1;  // Element not found
}
