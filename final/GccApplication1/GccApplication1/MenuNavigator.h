struct menuItem{
	char name[16];
	int highlighted;
	struct menuItem* nextItem;
	struct menuItem* prevItem;
	struct menuItem* subMenuItem;
	};
	
void printMenu(struct menuItem *top);
void menuNavigator();
struct menuItem* MenuInit();