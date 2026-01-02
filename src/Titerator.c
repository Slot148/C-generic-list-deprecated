#include "Tlist.h"
#include "TlistPrivate.h"

/**
 * @brief Creates a new iterator for the given list.
 *
 * The iterator allows sequential access to the elements of the list.
 * It starts at the head of the list. The caller is responsible for freeing
 * the iterator using `iterator->free(iterator)` when it is no longer needed.
 *
 * @param list The list to iterate over. Must not be NULL.
 * @return A pointer to the newly created iterator.
 * @warning If memory allocation fails or the provided list is NULL,
 *          the program will exit with `EXIT_FAILURE`.
 */
TIterator newIterator(List list){
    TIterator iterator = malloc(sizeof(struct TIterator));
    if(iterator == NULL) {
        fprintf(stderr, "Error in newIterator(): Failed to allocate memory for the new iterator.\n");
        exit(EXIT_FAILURE);
    }
    if (list == NULL) {
        fprintf(stderr, "Error in newIterator(): The provided list instance is NULL.\n");
        exit(EXIT_FAILURE);
    }
    iterator->_list = list;
    iterator->_current = list->_head;
    iterator->_index = 0;
    iterator->next = next;
    iterator->hasNext = hasNext;
    iterator->free = freeIterator;
    return iterator;
}

/**
 * @brief Returns the next element in the iteration.
 *
 * Advances the iterator to the next position and returns the value of the
 * element at the current position before advancing.
 * @param iterator A pointer to the iterator.
 * @return A pointer to the next element's value, or `NULL` if the end is reached or the iterator is invalid.
 */
void* next(TIterator iterator){
    if (iterator == NULL || iterator->_current == NULL) {
        fprintf(stderr, "Error in next(): No more elements to iterate or invalid iterator.\n");
        return NULL;
    }
    void* val = iterator->_current->_val;
    iterator->_index++;
    iterator->_current = iterator->_current->_nextNode;
    return val;
}

/**
 * @brief Checks if the iteration has more elements.
 *
 * @param iterator A pointer to the iterator.
 * @return `true` if there is at least one more element to iterate over, `false` otherwise.
 */
bool hasNext(TIterator iterator){
    if (iterator == NULL) {
        return false;
    }
    return iterator->_current != NULL;
}
/**
 * @brief Frees the memory allocated for the iterator structure.
 *
 * This function does not affect the list that the iterator was created from.
 * @param iterator A pointer to the iterator to be freed.
 */
void freeIterator(TIterator iterator){
    free(iterator);
}