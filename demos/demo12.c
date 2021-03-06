/*
 *       _____
 * ANSI / ___/
 *     / /__  
 *     \___/  
 *
 * Filename: demo12.c
 * Author  : Dan Levin
 * Date    : Fri Feb 20 13:23:46 2015
 * Version : 0.51
 * ---
 * Description: A demo program testing/showing the Graph ADT 
 * 
 * Revision history: (this is where you document the diffs between versions...)
 * Date   Revision
 * 150204 Created this demo - and made it menu-driven.
 * 150220 Moved some utility functions from here - to file ../utils.c
 * 150220 This source ready for version 0.5!
 * 150320 This source ready for version 0.51!
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "graph.h"
#include "utils.h"

/* --- MACRO DEFINITIONS --- */
#ifndef OK
#define OK 0
#endif

#define ERR_DUPL 1
#define ERR_VTX_OR_EDGE_MISSING -2
#define ERR_VTX_NOT_ISOL -3
#define ERR_FATAL -1

#define NR_OF_VERTICES 7
#define NR_OF_EDGES 20

/* Some string macros for the main menu... */
#define MAIN_MENU_ROW "--- GRAPH DEMO ---\nMENU: 0=Exit 1=Add_Vertex 2=Rem_Vertex 3=Add_Edge 4=Rem_Edge 5=Print\nSelection "

/* FUNCTION DECLARATIONS */
void my_destroy(void *data);
void print(const void *data);
int my_cmp(const void *key1, const void *key2);
int my_match(const void *k1, const void *k2);

/* Menu selections */
void ins_vertex(Graph gr);
void rem_vertex(Graph gr);
void ins_edge(Graph gr);
void rem_edge(Graph gr);
void print_graph(Graph gr);
void final_status(Graph gr);

/* Misc. application functions.. */
void create_rand_vertices(Graph gr, int nr_of_nodes);
/* END-OF-FUNCTION-DECLARATIONS */

/* FUNCTION DEFINITIONS - the rest of the program */
/* --- Function: void my_destroy(void *data) --- */
void my_destroy(void *data)
{
  free(data);
}

/* --- Function: void printvtx(const void *data) --- */
void printvtx(const void *data)
{
  printf("%02d", *(int *)data);
}

/* --- Function: void printvtx(const void *data) --- */
void printedge(const void *data)
{
  printf("%02d ", *(int *)data);
}

/* --- Function: int my_cmp(const int *key1, const int *key2) --- */
int my_cmp(const void *key1, const void *key2)
{
  return (*(int *)key1 - *(int *)key2);
}

/* --- Function: int my_match(const void *k1, const void *k2) --- */
int my_match(const void *k1, const void *k2)
{
  return *(int *)k1 == *(int *)k2;
}

/* --- Function: void add_nodes(Graph gr, int nr_of_nodes) --- */
void create_rand_vertices(Graph gr, int nr_of_nodes)
{
  int i=0, *pi, retval, dupctr=0;

  do
    {
      pi = (int *)malloc(sizeof(int));
      *pi = rand_int(1,99);
      
      if ((retval = GRAPHinsvertex(gr, pi)) != OK) /* Insertion failed... */
        {
          if (retval == ERR_DUPL) /* Duplicate key value.. */
            {
              dupctr++;
              my_destroy(pi); /* Free node - since duplicate..  */
            }
          else
            {
              if (retval == ERR_FATAL)
                {
                  prompt_and_pause("\nError: Fatal error - bailing out..!\n");
                  exit(-1);
                }
            }
        }
    } while (++i < nr_of_nodes);

  my_clearscrn();
  printf("--- INITIALIZING A GRAPH, %d VERTICES, RANDOM INTEGER DATA ---", NR_OF_VERTICES);
  printf("\n\nCurrent graph status:");
  print_graph(gr);
  printf("\n\n%d/%d successful insertion(s) -- %d duplicates rejected...", GRAPHvcount(gr), nr_of_nodes, dupctr);
  prompt_and_pause("\n\n");
}

/* --- Function: void ins_vertex(Graph gr) --- */
void ins_vertex(Graph gr)
{
  int tmp, *pi, retval;
  char mess[BUFSIZ];

  do
    {
      my_clearscrn();
      printf("--- ADD VERTEX ---");
      printf("\n\nCurrent graph status(%d vertices/%d edges): ", GRAPHvcount(gr), GRAPHecount(gr));
      print_graph(gr);

      tmp = read_int("\nEnter data for vertex to be inserted (-1=Quit): ", 0, 0);

      if (tmp == -1)
        break;

      pi = (int *)malloc(sizeof(int));
      *pi = tmp;

      if ((retval = GRAPHinsvertex(gr, pi)) != OK) /* Insertion failed... */
        {
          /* Do some error handling here.. */
          if (retval == ERR_DUPL) /* Duplicate key value.. */
            {
              sprintf(mess, "\nError: Vertex %d duplicate - insertion failed..", *pi);
              prompt_and_pause(mess);
              my_destroy(pi); /* Free node - since being duplicate..  */
            }
          else
            {
              prompt_and_pause("\nError: Fatal error - bailing out..!\n");
              exit(-1);
            }
        }
      else
        {
          /* Insertion succesful - notify user.. */
          sprintf(mess, "\nVertex %d will be inserted..", *(int *)pi);
          prompt_and_pause(mess);
        }
    } while (1);
}

/* --- Function: void rem_vertex(Graph gr) --- */
void rem_vertex(Graph gr)
{
  int tmp, *pi, retval;
  char mess[BUFSIZ];

  do
    {
      my_clearscrn();
      printf("--- REMOVE VERTEX ---");
      printf("\n\nCurrent graph status(%d vertices/%d edges): ", GRAPHvcount(gr), GRAPHecount(gr));
      print_graph(gr);

      tmp = read_int("\nEnter data for vertex to be removed (-1=Quit): ", 0, 0);

      if (tmp == -1)
        break;

      pi = &tmp;

      if ((retval = GRAPHremvertex(gr, (void **)&pi)) != OK) /* Vertex removal failed.. */
        {
          /* Do some error handling here.. */
          if (retval == ERR_VTX_OR_EDGE_MISSING)
            {
              sprintf(mess, "\nError: Vertex %d not found - removal failed..", *(int *)pi);
              prompt_and_pause(mess);
            }
          else
            {
              if (retval == ERR_VTX_NOT_ISOL)
                {
                  sprintf(mess, "\nError: Vertex %d not isolated - removal failed..", *(int *)pi);
                  prompt_and_pause(mess);
                }
              else
                {
                  printf("\nError: Fatal failure - bailing out...");
                  exit(retval);
                }
            }
        }
      else
        {
          /* Removal succesful - notify user.. */
          sprintf(mess, "\nVertex %d will be removed..!", *(int *)pi);
          prompt_and_pause(mess);
          /* Free node - after being removed from graph.. */
          my_destroy(pi);
        }
    } while (1);
}

/* --- Function: void ins_edge(Graph gr) --- */
void ins_edge(Graph gr)
{
  int tmp1, tmp2, *pi, retval;
  char mess[BUFSIZ];

  do
    {
      my_clearscrn();
      printf("--- ADD EDGE ---");
      printf("\n\nCurrent graph status(%d vertices/%d edges): ", GRAPHvcount(gr), GRAPHecount(gr));
      print_graph(gr);

      tmp1 = read_int("\nEnter data for first vertex of edge to be created (-1=Quit): ", 0, 0);

      if (tmp1 == -1)
        break;

      tmp2 = read_int("Enter data for adjacent vertex of edge to be created (-1=Quit): ", 0, 0);

      if (tmp2 == -1)
        break;

      pi = (int *)malloc(sizeof(int));
      *pi = tmp2;

      if ((retval = GRAPHinsedge(gr, &tmp1, pi)) != OK) /* Edge insertion failed.. */
        {
          /* Do some error handling here.. */
          if (retval == ERR_DUPL)
            {
              sprintf(mess, "\nError: Duplicate edge - between %d and %d..", tmp1, *(int *)pi);
              prompt_and_pause(mess);
              my_destroy(pi);
            }
          else
            {
              if (retval == ERR_VTX_OR_EDGE_MISSING)
                {
                  sprintf(mess, "\nError: Vertex not found - insertion failed..");
                  prompt_and_pause(mess);
                  my_destroy(pi);
                }
              else
                {
                  printf("\nError: Fatal failure - bailing out...");
                  exit(retval);
                }
            }
        }
      else
        {
          /* Insertion succesful - notify user.. */
          sprintf(mess, "\nEdge between vertex %d and %d will inserted..!", tmp1, *(int *)pi);
          prompt_and_pause(mess);
        }
    } while (1);
}

/* --- Function: void rem_edge(Graph gr) --- */
void rem_edge(Graph gr)
{
  int tmp1, tmp2, *pi, retval;
  char mess[BUFSIZ];

  do
    {
      my_clearscrn();
      printf("--- REMOVE EDGE ---");
      printf("\n\nCurrent graph status(%d vertices/%d edges): ", GRAPHvcount(gr), GRAPHecount(gr));
      print_graph(gr);

      tmp1 = read_int("\nEnter data for first vertex of edge to be removed (-1=Quit): ", 0, 0);

      if (tmp1 == -1)
        break;

      tmp2 = read_int("Enter data for adjacent vertex of edge to be removed (-1=Quit): ", 0, 0);

      if (tmp2 == -1)
        break;

      pi = &tmp2;

      if ((retval = GRAPHremedge(gr, &tmp1, (void **)&pi)) != OK) /* Edge removal failed.. */
        {
          /* Do some error handling here.. */
          if (retval == ERR_VTX_OR_EDGE_MISSING)
            {
              sprintf(mess, "\nError: Edge between %d and %d not found..", tmp1, *(int *)pi);
              prompt_and_pause(mess);
            }
          else
            {
              printf("\nError: Fatal error - bailing out...");
              exit(retval);
            }
        }
      else
        {
          /* Removal succesful - notify user.. */
          sprintf(mess, "\nEdge between %d and %d will be removed..!", tmp1, *(int *)pi);
          prompt_and_pause(mess);
          /* Free node - after being removed from graph.. */
          my_destroy(pi);
        }
    } while (1);
}

/* --- Function: void print_graph(Graph gr) --- */
void print_graph(Graph gr)
{
  GRAPHprint(gr, printvtx, printedge);  
}

/* --- Function: void final_status(Graph gr) --- */
void final_status(Graph gr)
{
  my_clearscrn();
  printf("--- FINAL STATUS ---");
  printf("\n\nFinal graph status(%d vertices/%d edges): ", GRAPHvcount(gr), GRAPHecount(gr));
  GRAPHprint(gr, printvtx, printedge);
}

int main(void)
{
  /* Declare YOUR variables here ! */
  Graph mygraph;
  int menu_choice;

  srand((unsigned int)time(NULL));

  if ((mygraph = GRAPHinit(my_match, my_destroy)) == NULL)
    {
      printf("\nFatal error - bailing out...\n!");
      exit(-1);
    }
  
  /* Initialize - and add random vertices to the graph... */
  create_rand_vertices(mygraph, NR_OF_VERTICES);

  /* Enter menu loop.. */
  do
    {
      menu_choice = menu(MAIN_MENU_ROW, 0, 5);

      switch (menu_choice)
        {
        case 1:
          ins_vertex(mygraph);
          break;
        case 2:
          rem_vertex(mygraph);
          break;
        case 3:
          ins_edge(mygraph);
          break;
        case 4:
          rem_edge(mygraph);
          break;
        case 5:
          my_clearscrn();
          printf("--- PRINT GRAPH AND EDGES ---\n");
          print_graph(mygraph);
          prompt_and_pause("\n\n");
          break;
        default:
          final_status(mygraph);
          break;
        }
    }
  while (menu_choice); 

  prompt_and_pause("\n\nLet's tidy up and destroy the graph..- Bye!");

  GRAPHdestroy(mygraph);

  return 0;

}
