/**
 * CS 2110 - Fall 2019 - Project 5
 *
 * @author Tien Tran
 *
 * list.c: Complete the functions!
 */

/**
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!-IMPORTANT-!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * For any function that must use malloc, if malloc returns NULL, the function
 * itself should return NULL if needs to return something (or return 0 if
 * the function returns a int).
 */

// Do not add ANY additional includes!!!
#include "list.h"
// This is used so that you guys can print stuff out
#include <stdio.h>

/* You should NOT have any global variables. */


/** create_process
 *
 * Helper function that creates a struct process by allocating memory for it on the heap
 *
 * If malloc returns NULL, you should return NULL to indicate failure
 *
 * @param name A pointer to char that points to the name of the process
 * ( be sure to malloc space for the string this points to and
 * copy this string over to the malloc-ed space in the process you return )
 * @param priority The priority of the process
 * @param pid The pid (process id) of the process
 * @return a struct process
 */
Process *create_process(char *name, int priority) {
    static int pid;
    Process *newProcess = (Process *) malloc(sizeof(Process));
    if (!newProcess) {
        return NULL;
    }
    char *newName = (char *) malloc((strlen(name) + 1) * sizeof(char));
    if (!newName) {
        free(newProcess);
        return NULL;
    }
    strcpy(newName, name);
    newProcess->name = newName;
    newProcess->priority = priority;
    newProcess->pid = pid++;
    return newProcess;
}


/** create_node
 *
 * Helper function that creates a struct list_node by allocating memory for it on the heap.
 * Be sure to initialize its next pointer to NULL and prev pointer to NULL.
 *
 * If malloc returns NULL, you should return NULL to indicate failure.
 *
 * @param data a void pointer to data the user wants to store in the struct list
 * @return a struct list_node
 */
ListNode *create_node(Process *data) {
    ListNode *newNode = (ListNode *) malloc(sizeof(ListNode));
    if (!newNode) {
        return NULL;
    }
    newNode->next = NULL;
    newNode->prev = NULL;
    newNode->data = data;
    return newNode;
}


/** create_list
 *
 * Creates a struct list by allocating memory for it on the heap.
 * Be sure to initialize head to NULL.
 *
 * If malloc returns NULL, you should return NULL to indicate failure.
 *
 * @return a pointer to a new struct list or NULL on failure
 */
List *create_list(void) {
    List *newList = (List *) malloc(sizeof(List));
    if (!newList) {
        return NULL;
    }
    newList->head = NULL;
    return newList;
}


/** push_front
 *
 * Adds a ListNode to the front of the list
 *
 * @param list a pointer to the List structure
 * @param proc The process to add to the list
 * @return
 *      0, if
 *          - the list is NULL
 *          - any other failure in the function
 *      1, otherwise
 *
 * Note: You must add the process to the list even if it is NULL
 */
int push_front(List *list, Process *node) {
    if (!list) {
        return 0;
    }
    ListNode *newHead = create_node(node);
    if (!newHead) {
        return 0;
    }
    if (!(list->head)) {
        list->head = newHead;
    } else {
        list->head->prev = newHead;
        newHead->next = list->head;
        list->head = newHead;
    }
    return 1;
}


/** push_back
 *
 * Adds the ListNode to the back of the list
 *
 * @param list a pointer to the List structure
 * @param proc The process to add to the list
 * @return
 *      0, if
 *          - the list is NULL
 *          - any other failure in the function
 *      1, otherwise
 *
 * Note: You must add the process to the list even if it is NULL
 */
int push_back(List *list, Process *node) {
    if (!list) {
        return 0;
    }
    ListNode *newBack = create_node(node);
    if (!newBack) {
        return 0;
    }
    if (!(list->head)) {
        list->head = newBack;
    } else {
        ListNode *temp = list->head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newBack;
        newBack->prev = temp;
    }
    return 1;
}

/** remove_node
  *
  * Remove the first node with the passed in `pid`
  *
  * The node should be removed completely, but the node's data should be returned through the `dataOut` parameter
  *
  * @param list a pointer to the struct list structure
  * @param dataOut a double pointer to return the Process data that is being removed from the list
  * @param pid The pid of the process that you want to remove
  * @return
  *     0, if
  *         - the struct list is NULL
  *         - there is no process with the specified pid
  *         - the dataOut is NULL
  *     1, otherwise
  */
int remove_node(List *list, Process **dataOut, int pid) {
    if (!list || !dataOut) {
        return 0;
    }
    int noPID = 1;
    if (!(list->head)) {
        return 0;
    } else {
        ListNode *temp = list->head;
        if (temp->data->pid == pid) {
            *dataOut = temp->data;
            noPID = 0;
            if (!(temp->next)) {
                list->head = NULL;
                free(temp);
            } else {
                list->head = list->head->next;
                list->head->prev = NULL;
                free(temp);
            }
        } else {
            while (temp != NULL && noPID != 0) {
                if (temp->data->pid == pid) {
                    *dataOut = temp->data;
                    noPID = 0;
                    if (!temp->next) {
                        temp->prev->next = NULL;
                        free(temp);
                        break;
                    } else {
                        temp->prev->next = temp->next;
                        temp->next->prev = temp->prev;
                        free(temp);
                        break;
                    }
                }
                temp = temp->next;
            }
        }
    }
    if (noPID == 1 || !dataOut) {
        return 0;
    }
    return 1;
}

/** destroy_process
 *
 * Destroys all data in the process. Make sure to free any process attributes that have memory allocated for them.
 *
 * @param proc The process to destroy
*/
void destroy_process(Process *proc) {
    if (!proc)
    {
        return;
    }
    if (proc->name) {
        free(proc->name);
    }
    free(proc);
    return;
}

/** destroy
  *
  * Destroys the list. This frees the list, nodes within the list, and data within the nodes
  * (`destroy_proc` can help with free'ing the data within the nodes).
  *
  * Make sure to check that the list is not NULL
  *
  * @param list A pointer to the struct list structure
  */
void destroy(List *list)
{
    if (!list) {
        return;
    }
    if (list->head != NULL) {
        ListNode *temp = list->head;
        while (temp->next) {
            temp = temp->next;
        }
        while (temp != NULL) {
            destroy_process(temp->data);
            temp = temp->prev;
            if (!temp) {
                free(list->head);
            } else {
                free(temp->next);
            }
        }
    }
    free(list);
    return;
}

/** copy_processs
 *
 * A function that will **deep** copy the process and all its attributes.
 *
 * For any malloc failures, you must not leak memory. This means that
 * if any memory failures occur, before you return 0, you must go back
 * and free any data that you allocated memory for.
 *
 *
 * @param from The process to copy
 * @param to The address of the pointer that will point to the copy
 *           of the process
 * @return
 *      0, if
 *          - malloc fails
 *          - from is NULL
 *          - to is NULL
 *      1, otherwise
 */
int copy_process(Process *from, Process **to) {
    if (!from || !to) {
        return 0;
    }
    Process *copyProc = (Process *) malloc(sizeof(Process));
    if (!copyProc) {
        return 0;
    }
    char *copyName = (char *) malloc((strlen(from->name) + 1) * sizeof(char));
    if (!copyName) {
        free(copyProc);
        return 0;
    }
    strcpy(copyName, from->name);
    copyProc->name = copyName;
    copyProc->pid = from->pid;
    copyProc->priority = from->priority;
    *to = copyProc;
    return 1;
}

/** copy_list
 *
 * Create a new list structure, new nodes, and new deep copies of the data
 * (use the copy_process function for this).
 *
 * If listToCopy is NULL or empty return NULL. For memory allocations
 * failures, including if copy_process returns 0 (indicating a memory
 * allocation failure), your code must not leak memory. This means that if any
 * memory allocation failures occur, before you return NULL, you must go back
 * and free all data in the new list, nodes in the new list, and
 * the new list itself (you may find the destroy function useful for
 * this).
 *
 *
 * @param listToCopy A pointer to the struct list structure to make a copy of
 * @return The list structure created by copying the old one, or NULL on
 *         failure (includes any of the parameters being null)
 */
List *copy_list(List *listToCopy)
{
    if (!listToCopy || !listToCopy->head) {
        return NULL;
    }
    List *newList = create_list();
    if (!newList) {
        return NULL;
    }
    ListNode *temp = listToCopy->head;
    ListNode **temp2 = &newList->head;
    while (temp != NULL) {
        int check = push_back(newList, NULL);
        if (check == 0) {
            destroy(newList);
            return NULL;
        } else {
            int check2 = copy_process(temp->data, &((*temp2)->data));
            if (check2 == 0) {
                destroy(newList);
                return NULL;
            }
        }
        temp = temp->next;
        temp2 = &((*temp2)->next);
    }
    return newList;
}

/** compare_process
 *
 * This method compares the pids of two processes
 * A null process has a larger pid than a non-null process
 *
 * @param a A process
 * @param b Another process
 * @return >0 if process a's pid > process b's pid
 *          0 if process a's pid = process b's pid
 *         <0 if process a's pid < process b's pid
 **/
// TODO doc
int compare_pid(Process *a, Process *b) {
    if (!a && !b) {
        return 0;
    }
    if (!a && b) {
        return 1;
    }
    if (a && !b) {
        return -1;
    }
    if (a->pid == b->pid) {
        return 0;
    } else if (a->pid > b->pid) {
        return 1;
    } else {
        return -1;
    }
}

/** compare_name
 *
 * This method compares the name of two processes
 * A null process has a larger name than a non-null process
 * (Hint) Take a look at the strcmp man page
 *
 * @param a A process
 * @param b Another process
 * @return >0 if process a's name > process b's name
 *          0 if process a's name = process b's name
 *         <0 if process a's name < process b's name
 **/
int compare_name(Process *a, Process *b) {
    if (!a && !b) {
        return 0;
    }
    if (!a && b) {
        return 1;
    }
    if (a && !b) {
        return -1;
    }
    if (strcmp(a->name, b->name) == 0) {
        return 0;
    } else if (strcmp(a->name, b->name) > 0) {
        return 1;
    } else {
        return -1;
    }
}

/** swap_nodes
 *
 * This method swaps two nodes in place. For example if our list was
 * A -> B -> C -> D and we called swap_nodes(b, d, list), the resulting list would be
 * A -> D -> C -> B
 * Note: The nodes will either be NULL or will exist in the list
 *
 * @param node_a A node to swap
 * @param node_b The other node to swap
 * @param list The list within which the nodes will be swapped
 * @return 0, if
 *              - any of the parameters are null
 *              - node_a = node_b
 *         1, otherwise
**/
int swap_nodes(ListNode *node_a, ListNode *node_b, List *list) {
    if (!node_a || !node_b || !list || node_a == node_b) {
        return 0;
    }
    if (node_a->next != node_b && node_b->next != node_a) {
        ListNode *temp = create_node(NULL);
        temp->next = node_b->next;
        temp->prev = node_b->prev;
        node_b->next = node_a->next;
        if (node_b->next) {
            node_b->next->prev = node_b;
        }
        node_b->prev = node_a->prev;
        if (node_b->prev) {
            node_b->prev->next = node_b;
        } else {
            list->head = node_b;
        }
        node_a->next = temp->next;
        if (node_a->next) {
            node_a->next->prev = node_a;
        }
        node_a->prev = temp->prev;
        if (node_a->prev) {
            node_a->prev->next = node_a;
        } else {
            list->head = node_a;
        }
        free(temp);
    } else if (node_a->next == node_b) {
        node_b->prev = node_a->prev;
        if (!node_b->prev) {
            list->head = node_b;
        } else {
            node_b->prev->next = node_b;
        }
        node_a->next = node_b->next;
        if (node_a->next != NULL) {
            node_a->next->prev = node_a;
        }
        node_b->next = node_a;
        node_a->prev = node_b;
    } else if (node_b->next == node_a) {
        node_a->prev = node_b->prev;
        if (!node_a->prev) {
            list->head = node_a;
        } else {
            node_a->prev->next = node_a;
        }
        node_b->next = node_a->next;
        if (node_b->next != NULL) {
            node_b->next->prev = node_b;
        }
        node_a->next = node_b;
        node_b->prev = node_a;
    }
    return 1;
}

/** sort
 *
 * A function to sort the nodes in ascending order (in place) using the comparator function passed in.
 * Recommend using bubble sort (Hint: Utilize the swap_nodes() function above)
 *
 * @param list The list to sort
 * @param compare_func A function pointer that denotes which compare method (written above) will be utilized.
 *                     Assume that the return value of the function pointer will be the return value of the comparator methods
 *                     written above
 * @return
 *      0, if
 *          - list is null
 *          - the list is empty
 *          - compare_func is null
 *      1, otherwise
 */
int sort(List *list, int (*compare_func)(Process *a, Process *b)) {
    if (!list || !list->head || !compare_func) {
        return 0;
    }
    int madeSwap = 1;
    while (madeSwap == 1) {
        ListNode *temp = list->head;
        ListNode *temp2 = list->head->next;
        madeSwap = 0;
        while (temp2 != NULL) {
            if (compare_func(temp->data, temp2->data) > 0) {
                madeSwap = swap_nodes(temp, temp2, list);
                temp = temp->prev;
                temp2 = temp2->next;
            }
            temp = temp->next;
            temp2 = temp2->next;
        }
    }
    return 1;
}

/** list_to_array
 *
 * This function will create and utilize a new structure to represent our processes.
 * First, create an array of process pointers, one for each process in the linked list
 * Next, iterate through the linked list, setting the process pointer to its new home in the array
 * and removing the linked list structure (Hint: You want to free the linked list but not the process itself)
 * Finally, return the process array
 *
 * @param list The linked list to be converted to an array
 * @param size The number of processes in the linked list
 * @return
 *      NULL, if
 *              - the list is null
 *              - the list is empty
 *              - there is a malloc failure
 *      An array to process pointers, otherwise
 */
Process **list_to_array(List *list, int size) {
    if (!list || !list->head) {
        return NULL;
    }
    Process **arr = (Process **) malloc(sizeof(Process) * size);
    if (!arr) {
        return NULL;
    }
    int x = 0;
    while (list->head != NULL) {
        remove_node(list, &arr[x], list->head->data->pid);
        x++;
    }
    free(list);
    return arr;
}


/** make_idle
 *
 * Append the string " (idle)" to the end of the name
 * of the process passed in.
 *
 * If the string " (idle)" already exists, do not include it
 *
 * You **must** use `realloc`.
 * You **can** use any functions in <string.h>
 *
 * @param proc A pointer to a Process struct
 * @return
 *      0, if
 *          - proc is NULL
 *          - the name of the proc is NULL
 *          - any other failure (for example, if `realloc` fails)
 *      1, otherwise
 */
int make_idle(Process *proc) {
    if (!proc || !proc->name) {
        return 0;
    }
    char *add = " (idle)";
    size_t nameLength = strlen(proc->name);
    size_t idleLength = strlen(add);
    if (!(nameLength < idleLength)) {
        if (strncmp(proc->name + nameLength - idleLength, add, strlen(add)) == 0) {
            return 0;
        }
    }
    char *newName = (char *) realloc(proc->name, (strlen(proc->name) + 1 + idleLength));
    if (!newName) {
        return 0;
    }
    newName = strcat(newName, add);
    proc->name = newName;
    return 1;
}

/** make_active
 *
 * Remove the string " (idle)" from the end of the name of
 * of the process passed in, if it exists
 *
 * You **must** use `realloc`.
 * You **can** use any functions in <string.h>
 *
 * @param proc A pointer to a Process struct
 * @return
 *      0, if
 *          - proc is NULL
 *          - the name of the proc is NULL
 *          - any other failure (for example, if `realloc` fails)
 *      1, otherwise
 */
int make_active(Process *proc) {
    if (!proc || !proc->name) {
        return 0;
    }
    char *add = " (idle)";
    size_t nameLength = strlen(proc->name);
    size_t idleLength = strlen(add);
    if (!(nameLength < idleLength)) {
        if (strncmp(proc->name + nameLength - idleLength, add, strlen(add)) == 0) {
            char *newName = (char *) realloc(proc->name, (strlen(proc->name) + 1 - idleLength));
            if (!newName) {
                return 0;
            }
            newName[nameLength - idleLength] = 0;
            proc->name = newName;
            return 1;
        } else {
            return 0;
        }
    }
    return 0;
}

/** apply
 *
 * Invokes `func` on each process of the linked list
 *
 * @param list A pointer to the list structure
 * @param func A pointer to the function to be invoked on each node
 * @return
 *      0, if
 *          - list is NULL
 *          - func is NULL
 *      1, otherwise
 */
int map_inplace(List *list, int (*map_func)(Process *)) {
    if (!list || !map_func) {
        return 0;
    }
    ListNode *temp = list->head;
    while (temp != NULL) {
        map_func(temp->data);
        temp = temp->next;
    }
    return 1;
}
