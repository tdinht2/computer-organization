#include "project4.h"

//Array of possible documents
doc_t doc_system[MAX_NUM_DOCS];
//bit vector representing the validity of each doc entry
uint64_t doc_valid_vector;
//command line arguments for each user command
char *arguments[4];
//data array (space pre-allocated for document contents)
char data[MAX_DOCSIZE * MAX_NUM_DOCS];

/**
 * @brief Creates a new document in our system if a document of the same name doesn't exist already. 
 * If docname is NULL, a document with the same name already exists, or there is no more space to create documents, do nothing and return -1
 * 
 * @param docname the name of the doc to create, including the extension
 * @return the index of the new document, or -1 in case of error
 */
int new_doc(char *docname) {
    if (!docname || doc_valid_vector == NO_SPACE) {
        return ERROR;
    }
    char tempName[MAX_NAMESIZE];
    ext_t tempExt = ASCII;
    get_name_ext(tempName, &tempExt, docname);
    int validIndex = -1;
    for (int x = 0; x < MAX_NUM_DOCS; x++) {
        if (strcmp(tempName, doc_system[x].name) == 0 && tempExt == doc_system[x].extension) {
            return ERROR;
        }
        if (GET_DOC_PRESENT(doc_valid_vector, x) == 0 && validIndex == -1) {
            validIndex = x;
        }
    }
    doc_system[validIndex].permissions = 7;
    strcpy(doc_system[validIndex].name, tempName);
    doc_system[validIndex].extension = tempExt;
    doc_system[validIndex].index = validIndex;
    doc_valid_vector = SET_DOC_PRESENT(doc_valid_vector, validIndex);
    doc_system[validIndex].data = &data[validIndex * MAX_DOCSIZE];
    return validIndex;
}

/**
 * @brief Find a doc in the doc system array given its name, including the extension
 * If docname is NULL or the document is not present, return -1
 * 
 * @param docname the name of the doc to find, including the extension
 * @return index referring to the document in the doc_system, -1 on error
 */
int find_doc(const char *docname) {
    if (!docname) {
        return ERROR;
    }
    char tempName[MAX_NAMESIZE];
    ext_t tempExt = ASCII;
    get_name_ext(tempName, &tempExt, docname);
    int validIndex = -1;
    for (int x = 0; x < MAX_NUM_DOCS; x++) {
        if (strcmp(tempName, doc_system[x].name) == 0 && tempExt == doc_system[x].extension) {
            validIndex = x;
            break;
        }
    }
    if (validIndex != -1) {
        return validIndex;
    }
   return ERROR;
}

/**
 * @brief List all doc names in the doc array. Only list those that are marked as
 * valid by the doc valid vector. The helper function print_doc_name will be useful.
 * 
 */
void list(void){
    for (int x = 0; x < MAX_NUM_DOCS; x++) {
        if (GET_DOC_PRESENT(doc_valid_vector, x) != 0) {
            print_doc_name(doc_system[x]);
        }
    }
    return;
}

/**
 * @brief Append a string to a doc. This operation fails if the string is too large to append to the doc
 * given the length of the string, the current size of the doc, and the max doc size constant. Remember that
 * the data for each document must be zero-terminated. This operation also fails if doc or str is NULL, or if the doc to append to does not have write permissions enabled.
 * 
 * @param doc the doc to append to
 * @param str the string to append to the doc
 * @return an int representing the success of the operation. 0 if successful, -1 otherwise.
 */
int append(doc_t *doc, char *str) {
    if (!doc || !str || ((doc -> permissions) & WMASK) == 0) {
        return ERROR;
    }
    signed int totalLength = strlen(doc -> data) + strlen(str);
    if (totalLength > MAX_DOCSIZE - 1) {
        return ERROR;
    }
    char app[MAX_DOCSIZE] = "";
    strcpy(app, doc -> data);
    strcat(app, str);
    strcpy(doc -> data, app);
    doc -> size = strlen(app);
    return SUCCESS;
}

/**
 * @brief Import the contents of an external file into this doc system.
 * This function should create a new document to import the contents into.
 * Fails if the contents of the file, with the zero-terminator added at the end, are larger than the max doc size,
 * there is not enough space for a new doc, or if docname or file is NULL.
 * 
 * @param docname the name of the new document to create, including the extension
 * @param file the file pointer to read data from
 * @return an int representing the success of the operation. 0 if successful, -1 otherwise.
 */
int import(char *docname, FILE *file) {
    if (!docname || !file || get_file_size(file) > MAX_DOCSIZE|| doc_valid_vector == NO_SPACE) {
        return ERROR;
    }
    int index = new_doc(docname);
    fread(doc_system[index].data, 1, MAX_DOCSIZE, file);
    doc_system[index].size = get_file_size(file);
    return SUCCESS;
}

/**
 * @brief Prints the internal data of the given doc, followed by a newline.
 * Fails if doc is NULL, or if the doc does not have read permissions enabled.
 * 
 * @param doc the doc whose data we print
 * @return an int representing the success of the operation. 0 if successful, -1 otherwise.
 */
int print_doc_data(const doc_t *doc) {
    if (!doc) {
        return ERROR;
    }
    if ((((*doc).permissions) & RMASK) == 0) {
        return ERROR;
    } else {
        printf("%s\n", doc -> data);
        return SUCCESS;
    }
    return ERROR;
}

/**
 * @brief Export the contents of a doc in our system to an external file.
 * Fails if doc or file is NULL or if the doc does not have read permissions enabled.
 * 
 * @param doc the doc whose data we export
 * @param file the file to export to
 * @return an int representing the success of the operation. 0 if successful, -1 otherwise.
 */
int export(const doc_t *doc, FILE *file) {
    if (!doc || !file || ((doc -> permissions) & RMASK) == 0) {
        return ERROR;
    }
    fprintf(file, "%s", doc->data);
    return SUCCESS;
}

/**
 * @brief Remove the given doc from the doc system. This doc, if not NULL, is guaranteed to exist in the system.
 * Fails if doc was NULL.
 * 
 * @param doc the doc to remove
 * @return an int representing the success of the operation. 0 if successful, -1 otherwise.
 */
int remove_doc(doc_t *doc) {
    if (!doc) {
        return ERROR;
    }
    int index = doc->index;
    doc_valid_vector = CLEAR_DOC_PRESENT(doc_valid_vector, index);
    return SUCCESS;
}

/**
 * @brief Change the read, write, execute permissions of a doc
 * Fails if doc or mode_changes is NULL.
 * 
 * @param doc the doc to modify the permissions of
 * @param mode_change the string representing the changes to be made (i.e. '+rw' or '-x')
 * @return an int representing the success of the operation. 0 if successful, -1 otherwise.
 */
int change_mode(doc_t *doc, char *mode_changes) {
    if (!doc || !mode_changes) {
        return ERROR;
    }
    int x = 0;
    char input = mode_changes[0];
    if (input == 43) {
        while (input != 0) {
            x++;
            input = mode_changes[x];
            if (input == 114) {
                doc -> permissions = ((doc -> permissions) | RMASK);
            } else if (input == 119) {
                doc -> permissions = ((doc -> permissions) | WMASK);
            } else if (input == 120) {
                doc -> permissions = ((doc -> permissions) | XMASK);
            }
        }
        return SUCCESS;
    } else if (input == 45) {
        while (input != 0) {
            x++;
            input = mode_changes[x];
            if (input == 114) {
                doc -> permissions = ((doc -> permissions) & ~RMASK);
            } else if (input == 119) {
                doc -> permissions = ((doc -> permissions) & ~WMASK);
            } else if (input == 120) {
                doc -> permissions = ((doc -> permissions) & ~XMASK);
            }
        }
        return SUCCESS;
    }
    return ERROR;
}



/**
 * @brief Main method that waits for user input to modify the docsystem. This method starts
 * up a mockup shell and tokenizes each user command using the tokenize helper method.
 * 
 * For example, if the user inputs: 
 * >> cmd name.doc other.txt
 * 
 * The global arguments array will be populated as : {"cmd", "name.doc", "other.txt"}
 * You may use the arguments array to get the arguments for whatever operation needs to be performed.
 * 
 * Note: the command to perform (i.e. cp, new, exit...) will always be in arguments[0]
 * 
 * @param argc unused int
 * @param argv unused array of strings
 * @return integer representing the success of the main method (0 on exit, 1 otherwise)
 */
int my_main(int argc, char const *argv[]) {
    UNUSED(argc);
    UNUSED(argv);
    char *shell_string = ">> ";

    //set up buffer to read in command line input
    char buffer[MAX_CMD_LEN]; 

    //will loop infinitely unless the user inputs 'exit'
    while(1) {
        printf("%s", shell_string);
        //get user input into buffer and tokenize it using the tokenize method
        fgets(buffer, MAX_CMD_LEN, stdin);
        tokenize(buffer);
        char *cmd = arguments[0];
        //TODO: Add new cases for different commands. You can follow the example given for new.
        if(strncmp(cmd, "new", 3) == 0) {
            char *name = arguments[1];
            new_doc(name); 
        } 
        else if(strncmp(cmd, "exit", 4) == 0) {
            return 0;
        } else if(strncmp(cmd, "ls", 2) == 0) {
            list();
        } else if(strncmp(cmd, "import", 6) == 0) {
            FILE * imp = fopen(arguments[1], "r");
            import(arguments[2], imp);
            fclose(imp);
        } else if(strncmp(cmd, "export", 6) == 0) {
            FILE * exp = fopen(arguments[2], "w");
            int index = find_doc(arguments[1]);
            export(&doc_system[index], exp);
            fclose(exp);
        } else if(strncmp(cmd, "print", 5) == 0) {
            int index = find_doc(arguments[1]);
            print_doc_data(&doc_system[index]);
        } else if(strncmp(cmd, "append", 6) == 0) {
            int index = find_doc(arguments[1]);
            append(&doc_system[index], arguments[2]);
        } else if(strncmp(cmd, "chmod", 5) == 0) {
            int index = find_doc(arguments[1]);
            change_mode(&doc_system[index], arguments[2]);
        } else if(strncmp(cmd, "rm", 2) == 0) {
            int index = find_doc(arguments[1]);
            remove_doc(&doc_system[index]);
        } 
    }
    return 1;
}

