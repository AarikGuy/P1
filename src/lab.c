#include "lab.h"
#include <stdio.h>

/**
 * @brief Initialize a new list with a sentinel node.
 */
list_t *list_init(void (*destroy_data)(void *), int (*compare_to)(const void *, const void *)) {
    list_t *list = malloc(sizeof(list_t));
    if (!list){
        return NULL;
    } 

    list->destroy_data = destroy_data;
    list->compare_to = compare_to;
    list->size = 0;

    // Creates the sentinel node
    list->head = malloc(sizeof(node_t));
    if (!list->head) {
        free(list);
        return NULL;
    }

    list->head->data = NULL;
    list->head->next = list->head;
    list->head->prev = list->head;

    return list;
}

/**
 * @brief Destroys the list and frees all associated memory.
 */
void list_destroy(list_t **list) {
    if (!list || !(*list)){
        return;
    }

    node_t *current = (*list)->head->next;
    while (current != (*list)->head) {
        node_t *next = current->next;
        if ((*list)->destroy_data) {
            (*list)->destroy_data(current->data); //line43
        }
        free(current);
        current = next;
    }

    free((*list)->head);
    free(*list);
    *list = NULL;
}

/**
 * @brief Add data to the front of the list.
 */
list_t *list_add(list_t *list, void *data) {
    if (!list) {
        return NULL;
    }

    node_t *new_node = malloc(sizeof(node_t));
    if (!new_node){
        return NULL;
    } 

    new_node->data = data;
    new_node->next = list->head->next;
    new_node->prev = list->head;

    list->head->next->prev = new_node;
    list->head->next = new_node;

    list->size++;
    return list;
}

/**
 * @brief Remove the node at the specified index and return its data.
 */
void *list_remove_index(list_t *list, size_t index) {
    if (!list || index >= list->size){
        return NULL;
    }

    node_t *current = list->head->next;
    for (size_t i = 0; i < index; i++) {
        current = current->next;
    }

    void *data = current->data;
    current->prev->next = current->next;
    current->next->prev = current->prev;

    free(current);
    list->size--;

    return data;
}

/**
 * @brief Find the index of the first occurrence of data in the list.
 */
int list_indexof(list_t *list, void *data) {
    if (!list){
        return -1;
    }

    node_t *current = list->head->next;
    int index = 0;

    while (current != list->head) {
        if (list->compare_to(data, current->data) == 0) {
            return index;
        }
        current = current->next;
        index++;
    }

    return -1;
}
