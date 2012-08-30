/* linked list example, pr101, 1994 */
#include <string.h>
#include <alloc.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <conio.h>

/* definition of a node */
struct node {
    char data[20];
    struct node *next;
};


struct node * initialise( void );
void freenodes( struct node * );
int insert( struct node * );
void delete( struct node *, struct node * );
void list( struct node * );
void menu( struct node *, struct node * );
void readline( char [] );



void readline( char buff[] )
{
    int ch, loop = 0;

    ch = getche();
    while( ch != '\r' ) {
       buff[loop] = ch;
       loop++;
       ch = getche();
    }
    buff[loop] = 0;
}

struct node * initialise( void )
{
    return( (struct node *) calloc(1, sizeof( struct node *) ));
}

/* free memory allocated for node */
void freenodes( struct node *headptr )
{
    struct node *temp;
    while( headptr ) {
       temp = headptr->next;
       free( headptr );
       headptr = temp;
    }
}

/* insert a new node after nodeptr, return 1 = success */
int insert( struct node *nodeptr )
{
    char buffer[20];
    struct node *newptr;

    newptr = initialise(); /* allocate a new node */
    if( newptr == NULL ) {
        return 0;
    }
    else {                 /* fill in its data and add to the list */
        newptr->next = nodeptr->next;
        nodeptr->next = newptr;
        nodeptr = newptr;
        printf("\nEnter data --->");
        readline( buffer );
        strcpy( nodeptr->data, buffer );
    }
    return 1;
}

/* delete a node from list */
void delete( struct node *headptr, struct node *nodeptr )
{
    struct node *deletepointer, *previouspointer;
    char buffer[20];

    deletepointer = headptr->next;
    previouspointer = headptr;
    /* find the entry */
    printf("\nEnter name to be deleted --->");
    readline( buffer );
    while( deletepointer ) {
        if( strcmp( buffer, deletepointer->data ) == 0 ) {
            /* delete node pointed to by delete pointer */
            previouspointer->next = deletepointer->next;
            break;
        }
        else {
            /* goto next node in list */
            deletepointer = deletepointer->next;
            previouspointer = previouspointer->next;
        }
    }
    /* did we find it? */
    if( deletepointer == NULL )
        printf("\n\007Error, %s not found or list empty\n", buffer);
    else {
        free( deletepointer );
        /* adjust nodeptr to the last node in list */
        nodeptr = headptr;
        while( nodeptr->next != NULL )
            nodeptr = nodeptr->next;
    }
}

/* print out the list */
void list( struct node *headptr )
{
    struct node *listpointer;

    listpointer = headptr->next;
    if( listpointer == NULL )
        printf("\nThe list is empty.\n");
    else {
        while( listpointer ) {
            printf("Name : %20s\n", listpointer->data );
            listpointer = listpointer->next;
        }
    }
}

/* main menu system */
void menu( struct node *headp, struct node *nodep )
{
    int menuchoice = 1;
    char buffer[20];

    while( menuchoice != 4 ) {
       printf("1  insert a node\n");
       printf("2  delete a node\n");
       printf("3  list nodes\n");
       printf("4  quit\n");
       printf("Enter choice -->");
       readline( buffer );
       menuchoice = atoi( buffer );
       switch( menuchoice ) {
           case 1 : if( insert( nodep ) == 0 )
                       printf("\n\007Insert failed.\n");
                    break;
           case 2 : delete( headp, nodep );  break;
           case 3 : list( headp );    break;
           case 4 : break;
           default : printf("\n\007Invalid option\n"); break;
       }
    }
}

main()
{
    struct node *headptr, *nodeptr;
    headptr = initialise();
    nodeptr = headptr;
    headptr->next = NULL;
    menu( headptr, nodeptr );
    freenodes( headptr );
}

