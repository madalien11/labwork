struct menuItem{
	char name[16];
	int highlighted;
	struct menuItem nextItem;
	struct menuItem subMenuItem ;
};