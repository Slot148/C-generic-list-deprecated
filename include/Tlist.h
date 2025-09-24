#ifndef T_LIST
#define T_LIST

#include <stddef.h>

/**
 * @enum Type
 * @brief Enumeration of data types that can be stored in the list.
 */
typedef enum Type{
    T,      /**< Generic pointer type (void*). The list stores the pointer directly. */
    INT,    /**< Integer type. The list stores a copy of the value. */
    STRING, /**< C-string type (char*). The list stores a copy of the string. */
    FLOAT,  /**< Float type. The list stores a copy of the value. */
    DOUBLE  /**< Double type. The list stores a copy of the value. */
} Type;

/**
 * @brief Opaque pointer to the list structure.
 */
typedef struct Lista *List;

/**
 * @brief Opaque pointer to a list node structure.
 */
typedef struct Node *Node;

/**
 * @brief Opaque pointer to the iterator structure.
 */
typedef struct TIterator* TIterator;

/**
 * @struct Lista
 * @brief Represents a generic singly linked list.
 *
 * This structure encapsulates the state of the list and provides function
 * pointers that act as methods for list manipulation. It's designed to be
 * an opaque type to the user, who should interact with it via the `List` pointer.
 */
struct Lista{
    /* List state */
    Node _head;      /**< Pointer to the first node in the list. */
    Node _tail;      /**< Pointer to the last node in the list. */
    Type _type;      /**< The data type of the elements stored in the list. */
    size_t _size;    /**< The size in bytes of the data type stored (for value types). */
    int _length;     /**< The number of elements in the list. */

    /* Methods */
    /** @brief Adds an element to the end of the list. */
    void (*push)(List this, ...);
    /** @brief Removes and returns the first element of the list. */
    void *(*pop)(List this);
    /** @brief Prints the list contents to stdout. */
    void (*print)(List this);
    /** @brief Returns the number of elements in the list. */
    int (*len)(List this);
    /** @brief Frees all nodes and their contained data. Does not free the List struct itself. */
    void (*free)(List this);
    /** @brief Returns a pointer to the element at the specified index without removing it. */
    void *(*get)(List this, int index);
    /** @brief Updates the element at a specific index. */
    void (*set)(List this, int index, ...);
    /** @brief Removes the element at a specific index. */
    void (*remove)(List this, int index);
    /** @brief Inserts an element at a specific index. */
    void (*insert)(List this, int index, ...);
    /** @brief Removes and returns the element at a specific index. */
    void *(*pick)(List this, int index);
    /** @brief Applies a function to each element of the list. */
    void (*foreach)(List this, void(*function)(void* data));
};

/**
 * @brief Creates a new empty list for a specific data type.
 *
 * This function allocates and initializes a new `List` structure. The caller is
 * responsible for freeing the returned `List` pointer using `free()` after
 * calling the list's own `free` method to release its internal nodes.
 *
 * @param type The data type the list will hold. See the `Type` enum.
 * @return A pointer to the newly created list.
 */
List newList(Type type);

/**
 * @brief Runs a series of tests on the list implementation.
 *
 * This function is intended for development and debugging purposes. It creates
 * lists of various types, performs a sequence of operations (like push, pop, get, set, etc.),
 * and prints the results to demonstrate the functionality and help verify correctness.
 */
void test();


/**
 * @brief Creates a new iterator for the given list.
 *
 * The iterator allows sequential access to the elements of the list.
 * The caller is responsible for freeing the iterator using `iterator->free(iterator)`
 * when it is no longer needed.
 *
 * @param list The list to iterate over.
 * @return A pointer to the newly created iterator.
 */
TIterator newIterator(List list);

#endif
