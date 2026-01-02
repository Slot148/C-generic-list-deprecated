/**
 * @mainpage Tlist - A Generic Singly Linked List Library in C
 *
 * @section intro_sec Introduction
 *
 * Tlist is a simple singly linked list implementation in C, designed to be
 * type-flexible and type-safe. It uses an object-oriented approach in C,
 * where the list structure contains pointers to its own functions (methods).
 *
 * The list can be configured at creation time to store different data types,
 * managing memory appropriately for each type.
 *
 * @section features_sec Features
 * - **Supported Data Types:**
 *   - `INT`: Stores copies of integer values.
 *   - `FLOAT`: Stores copies of floating-point values.
 *   - `DOUBLE`: Stores copies of double-precision floating-point values.
 *   - `STRING`: Stores copies of C strings (char*).
 *   - `T`: Stores generic pointers (`void*`), leaving memory management of the data to the user.
 * - **Object-Oriented Interface:** Interact with the list through its methods, such as `list->push(list, data)`.
 * - **Memory Management:** The library manages memory allocation and deallocation for primitive types and strings.
 *
 * @section usage_sec Usage Example
 *
 * ```c
 * #include "Tlist.h"
 * #include <stdio.h>
 * #include <stdlib.h>
 *
 * int main() {
 *     // Creates a new list to store integers
 *     List l = newList(INT);
 *
 *     // Adds elements
 *     l->push(l, 10);
 *     l->push(l, 20);
 *
 *     // Prints the list
 *     l->print(l); // Output: [10, 20]
 *
 *     // Frees the list nodes
 *     l->free(l);
 *     // Frees the list structure
 *     free(l);
 *
 *     return 0;
 * }
 * ```
 *
 * @file Tlist.c
 * @brief Implementation of a generic, type-aware singly linked list in C.
 *
 * For primitive types (INT, FLOAT, DOUBLE) and STRING, the list manages memory
 * internally by copying values. For the generic pointer type (T), the list
 * only stores the pointer itself, and the caller is responsible for managing
 * the memory of the pointed-to data.
 */

#include "Tlist.h"
#include "TlistPrivate.h"


/** @copydoc newList */
List newList(Type type){
    List this = (List)malloc(sizeof(struct Lista));
    if(this == NULL) {
        fprintf(stderr, "Error in newList(): Failed to allocate memory for the new list.\n");
        exit(EXIT_FAILURE);
    }
    this->_head = NULL;
    this->_tail = NULL;
    this->_type = type;
    this->_length = 0;

    // list methods
    this->print = print;
    this->free = destroyList;
    this->push = push;
    this->pop = pop;
    this->len = len;
    this->get = get;
    this->set = set;
    this->remove = delete;
    this->insert = insert;
    this->pick = pick;
    this->foreach = foreach;

    switch(type){
        case INT:
            this->_size = sizeof(int);
            break;
        case STRING:
            this->_size = sizeof(char *);
            break;
        case DOUBLE:
            this->_size = sizeof(double);
            break;
        case FLOAT:
            this->_size = sizeof(float);
            break;
        case T:
            this->_size = sizeof(void *);
            break;
    }

    return this;
}

/**
 * @brief Creates a new list node and allocates memory for its value.
 *
 * For `INT`, `FLOAT`, and `DOUBLE`, it allocates `size` bytes and copies the value.
 * For `STRING`, it allocates memory for a new string and copies the content.
 * For `T`, it does not allocate memory for the value but stores the pointer `val` directly.
 *
 * @param val A pointer to the value to be stored in the node.
 * @param size The size of the data type (for value types).
 * @param type The `Type` of the data.
 * @return A pointer to the newly created `Node`.
 * @private
 */
Node newNode(void *val, size_t size, Type type){
    Node node = (Node)malloc(sizeof(struct Node));    
    if(node == NULL) {
        fprintf(stderr, "Error in newNode(): Failed to allocate memory for a new node.\n");
        exit(EXIT_FAILURE);
    }
    if (type == STRING) {
        if (val == NULL) {
            fprintf(stderr, "Error in newNode(): Cannot create a STRING node from a NULL pointer.\n");
            exit(EXIT_FAILURE);
        }
        node->_val = malloc(strlen((char *)val) + 1);
        if (node->_val == NULL) {
            fprintf(stderr, "Error in newNode(): Failed to allocate memory for the node's string value.\n");
            exit(EXIT_FAILURE);
        }
        strcpy((char *)node->_val, (char *)val);
    }
    else if (type == T) {
        node->_val = val;
    }
    else {
        node->_val = malloc(size);
        if (node->_val == NULL) {
            fprintf(stderr, "Error in newNode(): Failed to allocate memory for the node's value.\n");
            exit(EXIT_FAILURE);
        }
        memcpy(node->_val, val, size);
    }   
    node->_nextNode = NULL;
    return node;
}

/**
 * @brief Prints the contents of the list to standard output.
 * @param this A pointer to the list.
 * @private
 */
void print(List this){
    if (this == NULL) {
        fprintf(stderr, "Error in print(): The provided list instance is NULL.\n");
        return;
    }
    printf("[");
    for (Node current = this->_head; current != NULL; current = current->_nextNode){
        switch (this->_type){
            case INT:
                printf("%d", *(int *)current->_val);
                break;
            case STRING:
                printf("\"%s\"", (char *)current->_val);
                break;
            case DOUBLE:
                printf("%.2f", *(double *)current->_val);
                break;
            case FLOAT:
                printf("%.2f", *(float *)current->_val);
                break;
            case T:
                printf("%p", current->_val);
                break;
        }
        if (current->_nextNode != NULL){
            printf(", ");
        }
    }
    printf("]");
    printf("\n");
}

/**
 * @brief Frees all the nodes in the list and the data they contain.
 *
 * It iterates through the list, freeing each node. For all types except `T`,
 * it also frees the memory allocated for the node's value (`val`). For type `T`,
 * it is the caller's responsibility to free the pointed-to data before or after
 * calling this function (e.g., using `foreach`). This function does NOT free
 * the `List` struct itself.
 * @param this A pointer to the list.
 */
void destroyList(List this){
    if (this == NULL) {
        fprintf(stderr, "Error in destroyList(): The provided list instance is NULL.\n");
        return;
    }
    Node current = this->_head;
    while (current != NULL){
        Node temp = current;
        current = temp->_nextNode;
        if(this->_type != T) free(temp->_val);
        free(temp);
    }
    this->_head = NULL;
    this->_tail = NULL;
    this->_length = 0;
}

/**
 * @brief Helper function to add a node to the end of the list.
 * @param this A pointer to the list.
 * @param node The node to be added.
 * @private
 */
void underPush(List this, Node node){
    if (this->_head == NULL) {
        this->_head = node;
        this->_tail = node;
    } else {
        this->_tail->_nextNode = node;
        this->_tail = node;
    }
    this->_length++; 
}

/**
 * @brief Adds a new element to the end of the list.
 *
 * This is a variadic function. The type of the argument after `this` must
 * match the list's `Type`.
 * - For `INT`: `int`
 * - For `FLOAT`, `DOUBLE`: `double` (due to default argument promotion)
 * - For `STRING`: `char*`
 * - For `T`: `void*`
 * @param this A pointer to the list.
 */
void push(List this, ...){
    if (this == NULL) {
        fprintf(stderr, "Error in push(): The provided list instance is NULL.\n");
        return;
    }
    va_list args;
    va_start(args, this);
    switch (this->_type){
        case INT:{
            int val = va_arg(args, int);
            underPush(this, newNode(&val, this->_size, this->_type));
            break;
        }
        case STRING:{
            char *str = va_arg(args, char *);
            underPush(this, newNode(str, this->_size, this->_type));
            break;
        }
        case DOUBLE:{
            double dbl = va_arg(args, double);
            underPush(this, newNode(&dbl, this->_size, this->_type));
            break;
        }
        case FLOAT:{
            float flt = (float)va_arg(args, double);
            underPush(this, newNode(&flt, this->_size, this->_type));
            break;
        }
        default:{
            void *unkown = va_arg(args, void *);
            underPush(this, newNode(unkown, this->_size, this->_type));
            break;
        }
    }
    va_end(args);
}

/**
 * @brief Calculates and returns the number of elements in the list.
 * @param this A pointer to the list.
 * @return The number of elements.
 */
int len(List this){
    if (this == NULL) {
        fprintf(stderr, "Error in len(): The provided list instance is NULL.\n");
    }
    return this->_length;
}

/**
 * @brief Removes the first element (head) of the list and returns its value.
 *
 * The caller takes ownership of the returned pointer and is responsible for
 * freeing it. For type `T`, the returned pointer is the one that was originally
 * inserted. For other types, it's a pointer to a heap-allocated copy.
 *
 * @param this A pointer to the list.
 * @return A pointer to the value of the removed element, or `NULL` if the list is empty.
 */
void *pop(List this){
    if (this == NULL) {
        fprintf(stderr, "Error in pop(): The provided list instance is NULL.\n");
        return NULL;
    }
    if (this->_head == NULL){
        return NULL;
    }else {
        Node current = this->_head;
        this->_head = current->_nextNode;
        void *val = current->_val;
        free(current);
        this->_length--;
        if (this->_head == NULL) {
            this->_tail = NULL;
        }
        return val;
    }
}

/**
 * @brief Retrieves a pointer to the element at a specific index.
 *
 * This function provides direct but read-only access to the internal data.
 * The returned pointer is owned by the list and should not be freed by the caller.
 * Its validity is only guaranteed until the next list-modifying operation.
 * For `INT`, `FLOAT`, `DOUBLE`, this will be a pointer to the heap-allocated value.
 * For `STRING`, a pointer to the internal string copy.
 * For `T`, the original `void*` that was inserted.
 *
 * @param this A pointer to the list.
 * @param index The zero-based index of the element to retrieve.
 * @return A pointer to the element's value, or `NULL` if the index is out of bounds.
 */
void *get(List this, int index){
    if (this == NULL) {
        fprintf(stderr, "Error in get(): The provided list instance is NULL.\n");
        return NULL;
    }
    if (index < 0) {
        fprintf(stderr, "Error in get(): Index %d is negative and invalid.\n", index);
        return NULL;
    }
    int x = 0;
    Node current = this->_head;
    while (current != NULL){
        if (x == index){
            return current->_val;
        }
        current = current->_nextNode;
        x++;
    }
    fprintf(stderr, "Error in get(): Index %d is out of bounds for list of size %d.\n", index, x);
    return NULL;
}

/**
 * @brief Updates the value of an element at a specific index.
 *
 * This is a variadic function. The argument after `index` must match the list's `Type`.
 * For `STRING`, the old string is freed and a new one is allocated and copied.
 * For `T`, the pointer is simply replaced.
 *
 * @param this A pointer to the list.
 * @param index The zero-based index of the element to update.
 */
void set(List this, int index, ...){
    if (this == NULL) {
        fprintf(stderr, "Error in set(): The provided list instance is NULL.\n");
        return;
    }
    if (index < 0) {
        fprintf(stderr, "Error in set(): Index %d is negative and invalid.\n", index);
        return;
    }

    va_list args;
    va_start(args, index);
    Node current = this->_head;
    int x = 0;
    while (current != NULL){
        if (x == index) {
            switch (this->_type){
                case INT:{
                    int val = va_arg(args, int);
                    memcpy(current->_val, &val, this->_size);
                    break;
                }
                case FLOAT:{
                    float flt = (float)va_arg(args, double);
                    memcpy(current->_val, &flt, this->_size);
                    break;
                }
                case DOUBLE:{
                    double dbl = va_arg(args, double);
                    memcpy(current->_val, &dbl, this->_size);
                    break;
                }
                case STRING:{
                    char *chr = va_arg(args, char *);
                    free(current->_val);
                    current->_val = malloc(strlen(chr) + 1);
                    if (current->_val == NULL) {
                        fprintf(stderr, "Error in set(): Failed to allocate memory for the new string value.\n");
                        exit(EXIT_FAILURE);
                    }
                    strcpy((char *)current->_val, chr);
                    break;
                }
                case T:{
                    void *nil = va_arg(args, void *);
                    current->_val = nil;
                    break;
                }
            }
            va_end(args);
            return;
        }
        current = current->_nextNode;
        x++;
    }
    fprintf(stderr, "Error in set(): Index %d is out of bounds for list of size %d.\n", index, x);
    va_end(args);
}

/**
 * @brief Deletes the element at a specific index.
 *
 * This function removes the node and frees the associated memory for its value
 * (unless the list type is `T`).
 * @param this A pointer to the list.
 * @param index The zero-based index of the element to delete.
 */
void delete(List this, int index){
    if (this == NULL) {
        fprintf(stderr, "Error in delete(): The provided list instance is NULL.\n");
        return;
    }
    if (index < 0) {
        fprintf(stderr, "Error in delete(): Index %d is negative and invalid.\n", index);
        return;
    }

    if (index == 0) {
        Node temp = this->_head;
        this->_head = temp->_nextNode;
        if (this->_head == NULL) this->_tail = NULL;
        if(this->_type != T) free(temp->_val);
        free(temp);
        this->_length--;
        return;
    }
    Node current = this->_head;
    int x = 0;
    while (current != NULL){
        if (index - 1 == x && index != 0){
            if (current->_nextNode == NULL) break;
            Node temp = current->_nextNode;
            current->_nextNode = temp->_nextNode;
            if (temp == this->_tail) {
                this->_tail = current;
            }
            if(this->_type != T) free(temp->_val);
            free(temp);
            this->_length--;
            return;
        }
        current = current->_nextNode;
        x++;
    }
    fprintf(stderr, "Error in delete(): Index %d is out of bounds for list of size %d.\n", index, x);
}

/**
 * @brief Helper function to insert a node at a specific index.
 * @param this A pointer to the list.
 * @param index The zero-based index where the node should be inserted.
 * @param node The node to insert.
 * @private
 */
void underInsert(List this, int index, Node node){
    if (index == 0){
        node->_nextNode = this->_head;
        this->_head = node;
        this->_length++;
        return;
    }

    Node current = this->_head;
    int x = 0;
    while (current != NULL){
        if (x == index - 1){
            Node temp = current->_nextNode;
            current->_nextNode = node;
            node->_nextNode = temp;
            this->_length++;
            return;
        }
        current = current->_nextNode;
        x++;
    }
}

/**
 * @brief Inserts a new element at a specific index.
 *
 * This is a variadic function. The argument after `index` must match the list's `Type`.
 * @param this A pointer to the list.
 * @param index The zero-based index at which to insert the new element.
 */
void insert(List this, int index, ...){
    if (this == NULL) {
        fprintf(stderr, "Error in insert(): The provided list instance is NULL.\n");
        return;
    }

    int list_len = len(this);
    if (index < 0 || index > list_len) {
        fprintf(stderr, "Error in insert(): Index %d is out of bounds. Valid range is 0 to %d.\n", index, list_len);
        return;
    }

    va_list args;
    va_start(args, index);

    switch (this->_type){
        case INT:{
            int val = va_arg(args, int);
            underInsert(this, index, newNode(&val, this->_size, this->_type));
            break;
        }
        case STRING:{
            char *str = va_arg(args, char *);
            underInsert(this, index, newNode(str, this->_size, this->_type));
            break;
        }
        case DOUBLE:{
            double dbl = va_arg(args, double);
            underInsert(this, index, newNode(&dbl, this->_size, this->_type));
            break;
        }
        case FLOAT:{
            float flt = (float)va_arg(args, double);
            underInsert(this, index, newNode(&flt, this->_size, this->_type));
            break;
        }
        default:{
            void *unkown = va_arg(args, void *);
            underInsert(this, index, newNode(unkown, this->_size, this->_type));
            break;
        }
    }

    va_end(args);
}

/**
 * @brief Removes and returns the element at a specific index.
 *
 * The caller takes ownership of the returned pointer and is responsible for
 * freeing it. For type `T`, the returned pointer is the one that was originally
 * inserted. For other types, it's a pointer to a heap-allocated copy.
 *
 * @param this A pointer to the list.
 * @param index The zero-based index of the element to remove.
 * @return A pointer to the value of the removed element, or `NULL` if the index is out of bounds.
 */
void *pick(List this, int index){
    if (this == NULL) {
        fprintf(stderr, "Error in pick(): The provided list instance is NULL.\n");
        return NULL;
    }
    if (index < 0) {
        fprintf(stderr, "Error in pick(): Index %d is negative and invalid.\n", index);
        return NULL;
    }

    if(index == 0){
        return this->pop(this);
    }

    Node current = this->_head;
    int x = 0;
    while (current != NULL){
        if (x == index - 1 && current->_nextNode != NULL){
            Node temp = current->_nextNode;
            current->_nextNode = temp->_nextNode;
            if (temp == this->_tail) {
                this->_tail = current;
            }
            void *n = temp->_val;
            free(temp);
            this->_length--;
            return n;
        }
        current = current->_nextNode;
        x++;
    }

    fprintf(stderr, "Error in pick(): Index %d is out of bounds for list of size %d.\n", index, x);
    return NULL;
}

/**
 * @brief Applies a given function to each element in the list.
 *
 * @param this A pointer to the list.
 * @param function A function pointer that takes a `void*` (the element's data) and returns `void`.
 */
void foreach(List this, void(*function)(void*)){
    if (this == NULL) {
        fprintf(stderr, "Error in foreach(): The provided list instance is NULL.\n");
        return;
    }
    for(Node current = this->_head; current != NULL; current = current->_nextNode){
        function(current->_val);
    }
}

/**
 * @brief Creates and returns a new list that is a deep copy of the original.
 *
 * This function iterates through the original list and adds each element
 * to a new list. For primitive types (INT, FLOAT, DOUBLE) and STRING,
 * new copies of the values are created. For type `T`, it copies the pointers,
 * meaning both lists will point to the same external data.
 *
 * The caller is responsible for freeing the returned list using `list->free(list)`
 * and then `free(list)`.
 *
 * @param this A pointer to the list to be duplicated.
 * @return A new `List` that is a copy of the original, or `NULL` if the
 *         original list was `NULL`.
 */
List duplicate(List this){
    if (this == NULL) {
        fprintf(stderr, "Error in duplicate(): The provided list instance is NULL.\n");
        return NULL;
    }
    List list = newList(this->_type);
    TIterator iterator = newIterator(this);
    
    while(hasNext(iterator)){
        void* val = next(iterator);
        switch (this->_type){
            case INT: push(list, *(int *)val); break;
            case FLOAT: push(list, *(float *)val); break;
            case DOUBLE: push(list, *(double *)val); break;
            case STRING: push(list, (char *)val); break;
            case T: push(list, val); break;
        }
    }
    iterator->free(iterator);
    return list;
}
