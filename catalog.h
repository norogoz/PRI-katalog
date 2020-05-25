#ifndef CATALOG_H
#define CATALOG_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//types
#define ROOT 0
#define SEC 1
#define SUBSEC 2
#define ITEM 3

//materials
#define NONE 0
#define CLOTH 1
#define GUM 2
#define GLASS 3
#define LIQUID 4
#define METAL 5
#define PLASTIC 6
#define OTHER 7

typedef struct node 
	{struct node* up; 
	 struct node* left; 
	 struct node* right; 
	 int type; // type or index number
	 int mat; 
	 char spec[63]; } Tnode; 
	 
typedef struct list 
	{struct list* up; 
	 struct node* node; 
	 struct list* down; } Tlist; 

int create_node(); // (sub)section/item creation menu
int add_sec();
int add_subsec();
int add_item();
Tnode* find_child(Tnode* node, int position, int type); // returns a pointer for child No. position of node
Tnode* build_first(Tnode *prev); // allocates mem. for node and puts it as a first prev's child
Tnode* build_between(Tnode* prev, Tnode* next); // -||- and puts it between prev and next
int put_first(Tnode *prev, Tnode* curr); // doesn't allocate mem., puts it as a first prev's child
int put_between(Tnode *prev, Tnode* curr, Tnode *next); // -||-, puts it between prev and next
int assign_num(Tnode *node); // choses number for an item
int modify_node(); // (sub)section/item modification menu
int mod_sec();
int mod_subsec();
int mod_item();
int unplug_node(Tnode* node); // takes node away from its place
int delefe(Tnode *node); // frees the memory of node and its children
int del_node(); // (sub)section/item deletion menu
int del_sec();
int del_subsec();
int del_item();
int move_node(); // (sub)section/item deletion menu
int move_sec();
int move_subsec();
int move_item();
int search(); // prints a list of items
int show_tree(Tnode* node); // prints sections and theirs subsections
int list_children(Tnode* node, int* position, int type, int print); // prints all the children of node that match the type
int create_list(Tnode* node); // creates list element for every item in the tree
Tlist* create_el(Tnode* node); // allocates mem. for list elements
int sort_abc(Tlist* list); // sorts list alphabetically
int sort_mat(Tlist* list); // sorts list by material of items
int show_list(Tlist* list); // prints the list
int destroy_list(Tlist* list);
int display_items(); // menu for managing the list
#endif