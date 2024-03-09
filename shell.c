// ----------------------------------------------
// Libraries used
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>
#include "shell.h"

const char* valid_builtin_commands[] = {"cd", "exit", NULL};


void parse( char* line, command_t* p_cmd ) {
    p_cmd->path = malloc(sizeof(char)*100);
    p_cmd->argv = malloc(sizeof(char*)*50);
    for (int i = 0; i<50; i++ ) p_cmd->argv[i] = malloc(sizeof(char)*100);

    if(line == NULL){
        p_cmd->argc = 0;
        return;
    }
    char cpyline[200];
    strcpy(cpyline, line);

    int i = 0;
    char *p = (char*) malloc(sizeof(line));
    p = strtok(cpyline, " \n");

    while (p != NULL)
    {   
        strcpy(p_cmd->argv[i++], p);
        p = strtok (NULL, " \n");
    }

    p_cmd->argv[i] = NULL;
    strcpy(p_cmd->path, p_cmd->argv[0]);

    p_cmd->argc = i;

    if(!is_builtin(p_cmd)){
        if(!find_fullpath(p_cmd->argv[0], p_cmd)){
            strcpy(p_cmd->path, p_cmd->argv[0]);
            p_cmd->argc = ERROR;
        }
    }

    return;
} // end parse function


int find_fullpath( char* command_name, command_t* p_cmd ) {
    char fullpath[200];
    char cpyPath[200];
    int exists = FALSE;

    char* path_env_variable;
    path_env_variable = getenv( "PATH" );
    strcpy(cpyPath, path_env_variable);

    struct stat buffer;

    int i = 0;
    char *p = (char*) malloc(sizeof(char)*100); //allocate memory
    p = strtok(cpyPath, ":\n"); 

    while (p != NULL)
    {
        strcpy(fullpath, p);
        strcat(fullpath, "/");
        strcat(fullpath, p_cmd->argv[0]);
        char* file_or_dir = fullpath;

        exists = stat( file_or_dir, &buffer );

        if ( exists == 0 && ( S_IFREG & buffer.st_mode ) ) {
            strcpy(p_cmd->path, file_or_dir);
            exists = TRUE;
            return exists;
        } else { 
            i++;
            p = strtok(NULL, ":\n");
        } // Not a valid file or directory
    }

    exists = FALSE;

    return exists;

} // end find_fullpath function


int execute( command_t* p_cmd ) {
    int status = SUCCESSFUL;
    int child_process_status;
    pid_t child_pid;
    child_pid = fork();
    if ( child_pid == 0 ) {
        // This is the child
        // Execute in same environment as parent
        execv(p_cmd->path, p_cmd->argv);
        cleanup(p_cmd);
        perror("Execute terminated with an error condition!\n");
        status = ERROR;
        exit(ERROR);
    }
    // This is the parent - wait for the child to terminate
    child_process_status = wait(NULL);
    if(child_process_status == ERROR){
        status = ERROR;
        return status;
    }
    cleanup(p_cmd);
    return status;

} // end execute function


int is_builtin( command_t* p_cmd ) {

    int cnt = 0;

    while ( valid_builtin_commands[cnt] != NULL ) {

        if ( equals( p_cmd->path, valid_builtin_commands[cnt] ) ) {

            return TRUE;

        }

        cnt++;

    }

    return FALSE;

} // end is_builtin function


int do_builtin( command_t* p_cmd ) {

    // only builtin command is cd

    if ( DEBUG ) printf("[builtin] (%s,%d)\n", p_cmd->path, p_cmd->argc);

    struct stat buff;
    int status = ERROR;

    if ( p_cmd->argc == 1 ) {

        // -----------------------
        // cd with no arg
        // -----------------------
        // change working directory to that
        // specified in HOME environmental 
        // variable

        status = chdir( getenv("HOME") );

    } else if ( ( stat( p_cmd->argv[1], &buff ) == 0 && ( S_IFDIR & buff.st_mode ) ) ) {


        // -----------------------
        // cd with one arg 
        // -----------------------
        // only perform this operation if the requested
        // folder exists

        status = chdir( p_cmd->argv[1] );

    } 

    return status;

} // end do_builtin function



void cleanup( command_t* p_cmd ) {

    int i=0;

    while ( p_cmd->argv[i] != NULL ) {
        free( p_cmd->argv[i] );
        i++;
    }

    free( p_cmd->argv );
    free( p_cmd->path );	

} // end cleanup function


int equals( char* str1, const char* str2 ) {
    int len[] = {0,0};

    char* b_str1 = str1;
    const char* b_str2 = str2;

    while( (*str1) != '\0' ) { 
        len[0]++;
        str1++;
    }

    while( (*str2) != '\0' ) {
        len[1]++;
        str2++;
    }

    if ( len[0] != len[1] ) {

        return FALSE;

    } else {

        while ( (*b_str1) != '\0' ) {

            if ( tolower( (*b_str1)) != tolower((*b_str2)) ) {

                return FALSE;

            }

            b_str1++;
            b_str2++;

        }

    } 

    return TRUE;


} // end compare function definition
