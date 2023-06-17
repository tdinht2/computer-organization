#ifndef PROJECT4_H
#define PROJECT4_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define UNUSED(x) ((void)(x))

#define ERROR -1
#define SUCCESS 0

#define MAX_DOCSIZE 600         //600 characters per doc
#define MAX_NUM_DOCS 64         //max 64 docs
#define MAX_NAMESIZE 20         //doc names can be max 20 characters (without extension)
#define MAX_CMD_LEN 50          //max len for a command is 50 characters total

#define VALID_MASK ((uint64_t)0x0000000000000001)
#define NO_SPACE   ((uint64_t)0xFFFFFFFFFFFFFFFF)

#define RMASK 0x04
#define WMASK 0x02
#define XMASK 0x01

/********** Macros to Complete **********/

// Implement this macro; it should evaluate to a non-zero (true) result if and only if
// the "index"th bit is set in doc_valid_vector, which would imply that a doc
// is already present at index.
#define GET_DOC_PRESENT(doc_valid_vector, index) (((doc_valid_vector) >> (index)) & (VALID_MASK))

// Implement this macro; it should evaluate to a copy of doc_valid_vector with the "index"th bit set,
// showing that a doc is now present at index.
#define SET_DOC_PRESENT(doc_valid_vector, index) ((doc_valid_vector) | ((VALID_MASK) << (index)))

// Implement this macro; it should evaluate to a copy of doc_valid_vector with the "index"th bit cleared, 
// showing that no doc is now present at index.
#define CLEAR_DOC_PRESENT(doc_valid_vector, index) ((doc_valid_vector) & (~((VALID_MASK) << (index))))

/********** End of Macros to Complete **********/

typedef enum extension {
    DOC = 0, 
    ASCII = 1
} ext_t;


//DOC type struct
typedef struct document_t {
    char name[MAX_NAMESIZE]; 
    ext_t extension;            //doc's extension
    uint8_t permissions;        //doc's permissions 
    char *data;                 //pointer to the doc's data in the global data array
    int size;                   //size in bytes (characters)
    int index;                  //in doc system
} doc_t;

//Array of possible documents
extern doc_t doc_system[MAX_NUM_DOCS];
//bit vector representing the validity of each doc entry
extern uint64_t doc_valid_vector;
//command line arguments for each user command
extern char *arguments[4];
//data array (space pre-allocated for document contents)
extern char data[MAX_DOCSIZE * MAX_NUM_DOCS];


/********** Function Declaractions **********/
//main method
int my_main(int argc, char const *argv[]);

//Given helper methods
void tokenize(char *line);
void get_name_ext(char *name, ext_t *ext, const char *docname);
void print_doc_name(doc_t doc);
int get_file_size(FILE *file);

//TODO: Complete these methods in project4.c
int new_doc(char *docname);
int remove_doc(doc_t *doc);
int find_doc(const char *docname);

void list(void);
int change_mode(doc_t *doc, char *mode_changes);
int print_doc_data(const doc_t *doc);

int append(doc_t *doc, char *str);
int import(char *docname, FILE *file);
int export(const doc_t *doc, FILE *file);

#endif
