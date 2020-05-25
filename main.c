#include "catalog.h"

Tnode root = { .up = NULL, .left = NULL, .right = NULL, .type = ROOT, .mat = NONE, .spec = "ROOT" };
Tlist head = { .up = NULL, .node = NULL, .down = NULL};

int clean_scr()
{
#ifdef WIN32
	system("cls");
#else
	system("clear");
#endif
	return 1;
}

int chooses()
{
	puts("Hello! Choose an action:");
	puts("1. Add a (sub)section/item");
	puts("2. Modify a (sub)section/item");
	puts("3. Delete a (sub)section/item");
	puts("4. Move a (sub)section/item");
	puts("5. Search items by material");
	puts("6. Display (sub)sections tree");
	puts("7. Display items in a (sub)section");
	puts("8. Exit");
}	

int main()
{
	Tnode *node;
	Tlist *list;
	int prompt;
	
	while (prompt != 8)
	{
		clean_scr();
		chooses();
		if (scanf("%d", &prompt) != 1) continue;
		
		printf("\n");
		
		switch(prompt)
		{
			case 1: if (create_node() == 0) puts("Action aborted."); break;
			case 2: if (modify_node() == 0) puts("Action aborted."); break;
			case 3: if (del_node() == 0) puts("Action aborted."); break; 
			case 4: if (move_node() == 0) puts("Action aborted."); break;
			case 5: if (search() == 0) puts("Action aborted."); break;
			case 6: if (root.left != NULL) show_tree(&root); else puts("Action aborted."); break;
			case 7: if (display_items() == 0) puts("Action aborted."); break;
			default: continue;
		}
		getchar();
		while (getchar() != '\n');
	}

	return 0;
}