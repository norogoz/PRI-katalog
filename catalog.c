#include "catalog.h"

extern Tnode root;
extern Tlist head;

int create_node()
{
	int type = 0;
	
	puts("What to create?");
	puts("SECTION 1\nSUBSECTION 2\nITEM 3");
	
	if(scanf("%d", &type) == 1) 
		switch(type)
		{
			case SEC: if(add_sec()) break; return 0;
			case SUBSEC: if(add_subsec()) break; return 0;
			case ITEM: if(add_item()) break; return 0;
			default: return 0;
		}
	else return 0;
	return 1;
} //create_node

int add_sec()
{
	int position = 1; 
	Tnode *prev, *curr, *next;
	curr = root.left;
	
	if (curr != NULL) // if any section exists, choose position for the new one
	{
		puts("On which position?");
		list_children(&root, &position, SEC, 1);
		printf("%d. \n", position);
		if (scanf("%d", &position) != 1) return 0;
	}
	
	prev = find_child(&root, position - 1, SEC);
	
	if (position <= 1) curr = build_first(prev);
	else
	{
		next = prev->right;
		curr = build_between(prev, next);
	}
	curr->type = SEC;
	curr->mat = NONE;
	
	printf("Name: ");
	scanf("%s", curr->spec); 

	return 1;
} //add_sec

int add_subsec()
{
	int sec;
	int position = 1; 
	Tnode *prev = NULL, *curr, *next;
	if ((curr = root.left) == NULL) return 0; // no sections => can't create
	
	printf("In which section?\n");
	list_children(&root, &position, SEC, 1);
	if (scanf("%d", &sec) != 1) return 0;
	
	curr = find_child(&root, sec, SEC);
	if (curr == &root) return 0;

	if (list_children(curr, &position, SUBSEC, 0) != 0) // if the section contains any subsections, choose position
	{	
		puts("On which position?");
		list_children(curr, &position, SUBSEC, 1);
		printf("%d. \n", position);
		if (scanf("%d", &position) != 1) return 0;
		prev = find_child(curr, position - 1, SUBSEC);
	}	

	if (prev == NULL || position <=1) curr = build_first(curr);
	else
	{
		next = prev->right;
		curr = build_between(prev, next);
	}
	curr->type = SUBSEC;
	curr->mat = NONE;
	
	printf("Name: ");
	scanf("%s", curr->spec);

	return 1;
} //add_subsec

int add_item()
{
	int sec, subsec, mat;
	int position = 1; 
	Tnode *prev = root.left, *curr;
	if (prev == NULL) return 0; // no sections => can't create
	
	printf("In which section?\n");
	list_children(&root, &position, SEC, 1);
	if (scanf("%d", &sec) != 1) return 0;

	prev = find_child(&root, sec, SEC);
	if (prev == &root) return 0;

	if (list_children(prev, &position, SUBSEC, 0) != 0) // no subsections => skip subsection choice
	{
		printf("In which subsection?\n0. NONE \n");
		list_children(prev, &position, SUBSEC, 1);
		if (scanf("%d", &position) != 1) return 0;
		prev = find_child(prev, position, SUBSEC);
	}
	
	curr = build_first(prev);
	
	if (!(assign_num(curr))) {free(curr); return 0;}
	
	puts("Item's material?");
	puts("CLOTH 1\nGUM 2\nGLASS 3\nLIQUID 4\nMETAL 5\nPLASTIC 6\nOTHER 7");
	if (scanf("%d", &mat) != 1) curr->mat = OTHER;
	else if (mat < 1 && mat > 7) curr->mat = OTHER;
	else curr->mat = mat;	
	
	printf("Name: ");
	scanf("%s", curr->spec);
	
	return 1;
}//add_item

Tnode* find_child(Tnode *node, int position, int type)
{	
	Tnode *child;
	child = node->left;
	
	if (child == NULL || position <= 0) return node; 
	
	if (type != ITEM)
	{
		if (child->type == type) position --;

		while (position > 0 && child->right != NULL) // each time it finds a children of correct type, counter--
		{
			child = child->right;
			if (child->type == type) position--;
		}
	
		while (child->type != type) { if (child->up != NULL) child = child->up; else return node; } // if found node doesn't have a matching type, go up
	}
	else if (type == ITEM)
	{
		if (child->type >= 10000) position --;

		while (position > 0 && child->right != NULL) // each time it finds a children of correct type, counter--
		{
			child = child->right;
			if (child->type >= 10000) position--;
		}
	
		while (child->type < 10000) { if (child->up != NULL) child = child->up; else return node; } // if found node doesn't have a matching type, go up
	}
	return child;
}//find_child

Tnode* build_first(Tnode *prev)
{
	Tnode *curr, *next;
	curr = malloc(sizeof(Tnode));
	next = prev->left;
	
	if (next != NULL) next->up = curr;
	curr->right = next;
	curr->up = prev;
	prev->left = curr;

	curr->left = NULL;
	
	return curr;
}//build_first

Tnode* build_between(Tnode *prev, Tnode *next)
{
	Tnode *curr;
	curr = malloc(sizeof(Tnode));

	if (next != NULL) next->up = curr;
	curr->right = next;
	curr->up = prev;
	prev->right = curr;

	curr->left = NULL;
	
	return curr;
}//build_between

int put_first(Tnode *prev, Tnode* curr)
{
	Tnode *next;
	next = prev->left;
	
	if (next != NULL) next->up = curr;
	curr->right = next;
	curr->up = prev;
	prev->left = curr;
	
	return 1;
}//put_first

int put_between(Tnode *prev, Tnode* curr, Tnode *next)
{
	if (next != NULL) next->up = curr;
	curr->right = next;
	curr->up = prev;
	prev->right = curr;
	
	return 1;
}//put_between

int assign_num(Tnode *node)
{
	int number;
		
	puts("Item number? (must have 5 digits)");
	
	if (scanf("%d", &number) != 1) return 0;
	if (number == 0) node->type = 0;
	else if (number >= 10000 && number <= 99999) node->type = number;
	else return 0;
	
	return 1;
}//assign_num

int modify_node()
{
	int type = 0;
	
	puts("What to modify?");
	puts("SECTION 1\nSUBSECTION 2\nITEM 3");
	
	if(scanf("%d", &type) == 1) 
		switch(type)
		{
			case SEC: if(mod_sec()) break; return 0;
			case SUBSEC: if(mod_subsec()) break; return 0;
			case ITEM: if(mod_item()) break; return 0;
			default: return 0;
		}
	else return 0;
	return 1;
}//modify_node

int mod_sec()
{
	Tnode *curr;
	int position;
	
	if (root.left == NULL) return 0;
	puts("Which section?");
	list_children(&root, &position, SEC, 1);
	if (scanf("%d", &position) != 1) return 0;
	
	curr = find_child(&root, position, SEC);
	if (curr == &root) return 0;

	printf("Set a new name: ");
	scanf("%s", curr->spec);
	return 1;	
}//mod_sec

int mod_subsec()
{
	Tnode *curr;
	int position;
	
	if (root.left == NULL) return 0;
	puts("In which section?");
	list_children(&root, &position, SEC, 1);
	if (scanf("%d", &position) != 1) return 0;
	
	curr = find_child(&root, position, SEC);
	if (curr == &root) return 0;
	
	if (list_children(curr, &position, SUBSEC, 0) == 0) return 0;
	puts("Which subsection?");
	list_children(curr, &position, SUBSEC, 1);
	if (scanf("%d", &position) != 1) return 0;
	
	curr = find_child(curr, position, SUBSEC);
	if (curr->type == SEC) return 0;
	
	printf("Set a new name: ");
	scanf("%s", curr->spec);
	return 1;
}//mod_subsec

int mod_item()
{
	Tnode *curr;
	int position;
	int mat;
	
	if (root.left == NULL) return 0;
	puts("In which section?");
	list_children(&root, &position, SEC, 1);
	if (scanf("%d", &position) != 1) return 0;
	
	curr = find_child(&root, position, SEC);
	if (curr == &root) return 0;
	
	if (curr->left == NULL) return 0;
	puts("In which subsection?");
	puts("0. NONE");
	list_children(curr, &position, SUBSEC, 1);
	if (scanf("%d", &position) != 1) return 0;
	
	curr = find_child(curr, position, SUBSEC);
	
	if (list_children(curr, &position, ITEM, 0) == 0) return 0;
	puts("Which item?");
	list_children(curr, &position, ITEM, 1);
	if (scanf("%d", &position) != 1) return 0;
	
	curr = find_child(curr, position, ITEM);
	if (curr->type < 10000) return 0;
	
	puts("What to change?");
	puts("NAME 1\nMATERIAL 2\nNUMBER 3");
	if (scanf("%d", &position) != 1) return 0;
	
	switch (position)
	{
		case 1: printf("Set a new name: "); 
				scanf("%s", curr->spec); 
				break;
		case 2: puts("Item's material?"); 
				puts("CLOTH 1\nGUM 2\nGLASS 3\nLIQUID 4\nMETAL 5\nPLASTIC 6\nOTHER 7");
				if (scanf("%d", &mat) != 1) return 0;
				if (mat > 0 && mat < 8) curr->mat = mat; else return 0;
				break;
		case 3: if (assign_num(curr) == 0) return 0; break;
		default: return 0;
	}
	return 1;	
}//mod_item

int unplug_node(Tnode* node)
{
	Tnode *prev, *next;
	prev = node->up;
	next = node->right;
	
	if (prev->type == node->type)
	{
		prev->right = next;
		if (next != NULL) next->up = prev;
	}
	else if (prev->type >= 10000)
	{
		prev->right = next;
		if (next != NULL) next->up = prev;
	}
	else
	{
		prev->left = next;
		if (next != NULL) next->up = prev;		
	}
	
	node->up = node->right = NULL;
	
	return 1;
}//unplug_node
	
int delefe(Tnode* node)
{
	if (node->left != NULL) delefe(node->left);
	if (node->right != NULL) delefe(node->right);
	free(node);
}//delefe

int del_node()
{
	int type = 0;
	
	puts("What to delete?");
	puts("SECTION 1\nSUBSECTION 2\nITEM 3");
	
	if(scanf("%d", &type) == 1) 
		switch(type)
		{
			case SEC: if(del_sec()) break; return 0;
			case SUBSEC: if(del_subsec()) break; return 0;
			case ITEM: if(del_item()) break; return 0;
			default: return 0;
		}
	else return 0;
	return 1;
}//del_node

int del_sec()
{
	int position = 1; 
	char yes = 'a';
	Tnode *curr;
	if (root.left == NULL) return 0;
	
	if (curr != NULL)
	{
		puts("Which section?");
		list_children(&root, &position, SEC, 1);
		if (scanf("%d", &position) != 1) return 0;
	}
	
	curr = find_child(&root, position, SEC);
	if (curr == &root) return 0;
	
	puts("Are you sure? All subsections/items in that section will be deleted as well!");
	puts("y to continue");
	
	getchar();
	scanf("%c", &yes);
	if (yes != 'y') return 0;
	
	unplug_node(curr);
	delefe(curr);	
	
	return 1;
}//del_sec

int del_subsec()
{
	int position = 1;
	char yes = 'a';
	Tnode *prev, *curr;
	if (root.left == NULL) return 0;
	
	puts("In which section?");
	list_children(&root, &position, SEC, 1);
	if (scanf("%d", &position) != 1) return 0;
	
	prev = find_child(&root, position, SEC);
	if (prev == &root) return 0;
	
	if (list_children(prev, &position, SUBSEC, 0) == 0) return 0;
	puts("Which subsection?");
	list_children(prev, &position, SUBSEC, 1);
	if (scanf("%d", &position) != 1) return 0;
	
	curr = find_child(prev, position, SUBSEC);
	if (curr == prev) return 0;
	
	puts("Are you sure? All subsections/items in that section will be deleted as well!");
	puts("y to continue");
	getchar();
	scanf("%c", &yes);
	if (yes != 'y') return 0;
	
	unplug_node(curr);
	delefe(curr);	
	
	return 1;
}//del_subsec

int del_item()
{
	int position = 1; 
	char yes = 'a';
	Tnode *prev, *curr;
	if (root.left == NULL) return 0;
	
	puts("In which section?");
	list_children(&root, &position, SEC, 1);
	if (scanf("%d", &position) != 1) return 0;
	
	prev = find_child(&root, position, SEC);
	if (prev == &root) return 0;
	
	puts("In which subsection?");
	puts("0. NONE");
	list_children(prev, &position, SUBSEC, 1);
	if (scanf("%d", &position) != 1) return 0;
	
	curr = find_child(prev, position, SUBSEC);
	
	if (list_children(curr, &position, ITEM, 0) == 0) return 0;
	puts("Which item?");
	list_children(curr, &position, ITEM, 1);
	if (scanf("%d", &position) != 1) return 0;
	
	curr = find_child(curr, position, ITEM);
	if (curr->type < 10000) return 0;
	
	puts("Are you sure? All subsections/items in that section will be deleted as well!");
	puts("y to continue");
	
	getchar();
	scanf("%c", &yes);
	if (yes != 'y') return 0;
	
	unplug_node(curr);
	delefe(curr);	
	
	return 1;
}//del_item

int move_node()
{
	int type = 0;
	
	puts("What to move?");
	puts("SECTION 1\nSUBSECTION 2\nITEM 3");
	
	if(scanf("%d", &type) == 1) 
		switch(type)
		{
			case SEC: if(move_sec()) break; return 0;
			case SUBSEC: if(move_subsec()) break; return 0;
			case ITEM: if(move_item()) break; return 0;
			default: return 0;
		}
	else return 0;
	return 1;
}//move_node

int move_sec()
{
	int position = 1; 
	Tnode *prev, *curr = root.left, *next;
	if (curr == NULL) return 0;
	
	puts("Which section?");
	list_children(&root, &position, SEC, 1);
	if (scanf("%d", &position) != 1) return 0;
	
	curr = find_child(&root, position, SEC);
	if (curr == &root) return 0;
	
	unplug_node(curr);
	
	puts("Where to put?");
	list_children(&root, &position, SEC, 1);
	printf("%d. \n", position);
	if (scanf("%d", &position) != 1) return 0;

	prev = find_child(&root, position - 1, SEC);
	
	if (position <= 1) put_first(prev, curr);
	else
	{
		next = prev->right;
		put_between(prev, curr, next);
	}
	
	return 1;
}//move_sec

int move_subsec()
{
	int position = 1; 
	Tnode *prev, *curr, *next;
	if (root.left == NULL) return 0;
	
	puts("In which section?");
	list_children(&root, &position, SEC, 1);
	if (scanf("%d", &position) != 1) return 0;
	
	prev = find_child(&root, position, SEC);
	if (prev == &root) return 0;
	
	if (list_children(prev, &position, SUBSEC, 0) == 0) return 0;
	puts("Which subsection?");
	list_children(prev, &position, SUBSEC, 1);
	if (scanf("%d", &position) != 1) return 0;
	
	curr = find_child(prev, position, SUBSEC);
	if (curr == prev) return 0;
	
	unplug_node(curr);
	
	puts("Put in which section?");
	list_children(&root, &position, SEC, 1);
	if (scanf("%d", &position) != 1) return 0;

	prev = find_child(&root, position, SEC);
	if (prev == &root) return 0;
	
	puts("Where to put?");
	list_children(prev, &position, SUBSEC, 1);
	printf("%d. \n", position);
	if (scanf("%d", &position) != 1) return 0;

	prev = find_child(prev, position - 1, SUBSEC);
	
	if (position <= 1) put_first(prev, curr);
	else
	{
		next = prev->right;
		put_between(prev, curr, next);
	}
	
	return 1;
}//move_subsec

int move_item()
{
	int position = 1; 
	Tnode *prev, *curr, *next;
	if (root.left == NULL) return 0;
	
	puts("In which section?");
	list_children(&root, &position, SEC, 1);
	if (scanf("%d", &position) != 1) return 0;
	
	prev = find_child(&root, position, SEC);
	if (prev == &root) return 0;
	
	puts("In which subsection?");
	puts("0. NONE");
	list_children(prev, &position, SUBSEC, 1);
	if (scanf("%d", &position) != 1) return 0;
	
	curr = find_child(prev, position, SUBSEC);
	
	if (list_children(curr, &position, ITEM, 0) == 0) return 0;
	puts("Which item?");
	list_children(curr, &position, ITEM, 1);
	if (scanf("%d", &position) != 1) return 0;
	
	curr = find_child(curr, position, ITEM);
	if (curr->type < 10000) return 0;
	
	unplug_node(curr);
	
	puts("Put in which section?");
	list_children(&root, &position, SEC, 1);
	if (scanf("%d", &position) != 1) return 0;

	prev = find_child(&root, position, SEC);
	if (prev == &root) return 0;
	
	puts("Put in which subsection?");
	puts("0. NONE");
	list_children(prev, &position, SUBSEC, 1);
	if (scanf("%d", &position) != 1) return 0;

	prev = find_child(prev, position, SUBSEC);
	
	puts("Where to put?");
	list_children(prev, &position, ITEM, 1);
	printf("%d. \n", position);
	if (scanf("%d", &position) != 1) return 0;

	prev = find_child(prev, position - 1, ITEM);
	
	if (prev->type < 10000) put_first(prev, curr);
	else
	{
		next = prev->right;
		put_between(prev, curr, next);
	}
	
	return 1;
}//move_item

int search()
{
	create_list(&root);
	Tlist *list = head.down;
	int mat;
	
	if (head.down == NULL) return 0;
	
	puts("What material to look for?");
	puts("CLOTH 1\nGUM 2\nGLASS 3\nLIQUID 4\nMETAL 5\nPLASTIC 6\nOTHER 7");
	
	if (scanf("%d", &mat) != 1) return 0;	
	
	while (list != NULL)
	{
		if ((list->node)->mat == mat) 
			switch(mat)
			{
				case 1: printf("%d CLOTH %s\n", (list->node)->type, (list->node)->spec); break;
				case 2: printf("%d GUM %s\n", (list->node)->type, (list->node)->spec); break;
				case 3: printf("%d GLASS %s\n", (list->node)->type, (list->node)->spec); break;
				case 4: printf("%d LIQUID %s\n", (list->node)->type, (list->node)->spec); break;
				case 5: printf("%d METAL %s\n", (list->node)->type, (list->node)->spec); break;
				case 6: printf("%d PLASTIC %s\n", (list->node)->type, (list->node)->spec); break;
				case 7: printf("%d OTHER %s\n", (list->node)->type, (list->node)->spec); break;
				default: return 0;
			}
		list = list->down;
	}
	
	destroy_list(head.down);
	head.down = NULL;
	return 1;
}//search

int show_tree(Tnode* node)
{
	if (node->type == SEC) printf("%s\n", node->spec);
	else if (node->type == SUBSEC) printf("   %s\n", node->spec);
	if (node->left != NULL) show_tree(node->left);
	if (node->right != NULL) show_tree(node->right);
	
	return 1;
}//show_tree

int list_children(Tnode* node, int* position, int type, int print) // if print == 0 check for existing children of given type
{
	node = node->left;
	if (type != ITEM)
	{
		for (*position = 1; node != NULL; (*position)++, node = node->right)
			if (node->type == type) print? printf("%d. %s\n", *position, node->spec): printf("");
			else (*position)--;
	}
	else
	{
		for (*position = 1; node != NULL; (*position)++, node = node->right)
			if (node->type >= 10000) print? printf("%d. %s\n", *position, node->spec): printf("");
			else (*position)--;
	}
	if (*position == 1) return 0;
	return 1;
}//list_children

int create_list(Tnode* node)
{
	if (node->left != NULL) create_list(node->left);
	if (node->right != NULL) create_list(node->right);
	if (node->type >= 10000) create_el(node);
}//create_list

Tlist* create_el(Tnode* node)
{
	Tlist *el, *curr = &head;
	el = malloc(sizeof(Tlist));
	el->node = node;
	
	while (curr->down != NULL) curr = curr->down;
	curr->down = el;
	el->up = curr;
	el->down = NULL;
	
	return el;
}//create_el

int sort_abc(Tlist* list)
{
	Tlist *stat, *dyn;
	stat = list->down;
	
	while(stat != NULL)
	{
		dyn = list;
		while (stat != dyn)
		{
			if (strcmp((stat->node)->spec, (dyn->node)->spec) < 0)
			{
				(stat->up)->down = stat->down;
				if (stat->down != NULL) (stat->down)->up = stat->up;
				
				stat->up = dyn->up;
				stat->down = dyn;
				
				(dyn->up)->down = stat;
				dyn->up = stat;
				
				break;
			}
			else dyn = dyn->down;
		}
		stat = dyn->down;
	}
	
	return 1;
}//sort_abc


int sort_mat(Tlist* list)
{
	Tlist *stat, *dyn;
	stat = list->down;
	
	while(stat != NULL)
	{
		dyn = list;
		while (stat != dyn)
		{
			if ((stat->node)->mat < (dyn->node)->mat)
			{
				(stat->up)->down = stat->down;
				if (stat->down != NULL) (stat->down)->up = stat->up;
				
				stat->up = dyn->up;
				stat->down = dyn;
				
				(dyn->up)->down = stat;
				dyn->up = stat;
				
				break;
			}
			else if ((stat->node)->type < (dyn->node)->type)
			{
				(stat->up)->down = stat->down;
				if (stat->down != NULL) (stat->down)->up = stat->up;
				
				stat->up = dyn->up;
				stat->down = dyn;
				
				(dyn->up)->down = stat;
				dyn->up = stat;
				
				break;
			}
			else dyn = dyn->down;
		}
		stat = dyn->down;
	}
	
	return 1;
}//sort_mat

int show_list(Tlist* list)
{
	switch((list->node)->mat)
	{
		case 1: printf("%d CLOTH %s\n", (list->node)->type, (list->node)->spec); break;
		case 2: printf("%d GUM %s\n", (list->node)->type, (list->node)->spec); break;
		case 3: printf("%d GLASS %s\n", (list->node)->type, (list->node)->spec); break;
		case 4: printf("%d LIQUID %s\n", (list->node)->type, (list->node)->spec); break;
		case 5: printf("%d METAL %s\n", (list->node)->type, (list->node)->spec); break;
		case 6: printf("%d PLASTIC %s\n", (list->node)->type, (list->node)->spec); break;
		case 7: printf("%d OTHER %s\n", (list->node)->type, (list->node)->spec); break;
		default: return 0;
	}
	
	if (list->down != NULL) show_list(list->down);
		
	return 1;
}//show_list

int destroy_list(Tlist* list)
{
	if (list->down != NULL) destroy_list(list->down);
	free(list);
	return 1;
}//destroy_list

int display_items()
{
	int position = 1; 
	int order;
	Tnode *curr;
	if (root.left == NULL) return 0;
	
	puts("In which section?");
	puts("0. ENTIRE TREE");
	list_children(&root, &position, SEC, 1);
	if (scanf("%d", &position) != 1) return 0;
	
	curr = find_child(&root, position, SEC);
	
	if (curr != &root)
	{
		puts("In which subsection?");
		puts("0. NONE");
		list_children(curr, &position, SUBSEC, 1);
		if (scanf("%d", &position) != 1) return 0;
		curr = find_child(curr, position, SUBSEC);
	}	
	
	puts("Sort by:");
	puts("NAME 0\nMATERIAL 1");
	if (scanf("%d", &order) != 1) return 0;
	
	create_list(curr);
	if (head.down == NULL) return 0;
	
	if (order == 0) sort_abc(head.down);
	else if (order == 1) sort_mat(head.down);
	else return 0;
	
	show_list(head.down);
	destroy_list(head.down);
	head.down = NULL;
		
	return 1;
}//display_items