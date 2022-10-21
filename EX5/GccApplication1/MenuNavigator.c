#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdint.h>
#include "MenuNavigator.h"
#include "OledDriver.h"
#include "controller.h"



struct menuItem* MenuInit(){
	struct menuItem *item1 = malloc (sizeof(struct menuItem));
	strcpy(item1->name,"Play");
	item1->highlighted=1;
	item1->subMenuItem = NULL;
	struct menuItem *item2 = malloc (sizeof(struct menuItem));
	item1->nextItem = item2;
	item1->prevItem = NULL;
	
	strcpy(item2->name , "Options");
	item2->highlighted=0;
	struct menuItem *option_item1 = malloc (sizeof(struct menuItem));
	item2->subMenuItem = option_item1;
	struct menuItem *item3 = malloc (sizeof(struct menuItem));
	item2->nextItem = item3;
	item2->prevItem = item1;
	
	
	strcpy(option_item1->name , "Hard");
	option_item1->highlighted=1;
	option_item1->subMenuItem = NULL;
	struct menuItem *option_item2 = malloc (sizeof(struct menuItem));
	option_item1->nextItem = option_item2;
	option_item1->prevItem = NULL;
	
	
	strcpy(option_item2->name , "Super Hard");
	option_item2->highlighted=0;
	option_item2->subMenuItem = NULL;
	struct menuItem *option_item3 = malloc (sizeof(struct menuItem));
	option_item2->nextItem = option_item3;
	option_item2->prevItem = option_item1;
	
	strcpy(option_item3->name , "Extremely Hard");
	option_item3->highlighted=0;
	option_item3->subMenuItem = NULL;
	struct menuItem *option_item4 = malloc (sizeof(struct menuItem));
	option_item3->nextItem = option_item4;
	option_item3->prevItem = option_item2;
	
	strcpy(option_item4->name , "Suicide");
	option_item4->highlighted=0;
	option_item4->subMenuItem = NULL;
	struct menuItem *option_item5 = malloc (sizeof(struct menuItem));
	option_item4->nextItem = option_item5;
	option_item4->prevItem = option_item3;
	
	strcpy(option_item5->name , "Back");
	option_item5->highlighted=0;
	option_item5->subMenuItem = item1;
	//struct menuItem *option_item4 = malloc (sizeof(struct menuItem));
	option_item5->nextItem = NULL;
	option_item5->prevItem = option_item4;
	
	strcpy(item3->name , "Exit");
	item3->highlighted=0;
	item3->subMenuItem = NULL;
	item3->nextItem = NULL;
	item3->prevItem = item2;
	
	return item1;
}

void menuNavigator(){
	oled_init();
	struct menuItem *firstItem = MenuInit();
	struct menuItem *topItem = firstItem;
	struct menuItem *currentItem = firstItem;
	printMenu(firstItem);
	while(1){
		_delay_ms(750);
		struct controller control = get_controller();
		if(control.joystick_pressed == 1){
			if(currentItem->subMenuItem!=NULL){
				topItem = currentItem->subMenuItem;
				currentItem->highlighted=0;
				currentItem = currentItem->subMenuItem;
				currentItem->highlighted=1;
				printMenu(topItem);
			}
		}
		else if(control.pos == DOWN && currentItem->nextItem!=NULL){
			currentItem->highlighted=0;
			currentItem = currentItem->nextItem;
			currentItem->highlighted =1;
			printMenu(topItem);
		}
		else if(control.pos == UP && currentItem->prevItem!=NULL){
			currentItem->highlighted=0;
			currentItem = currentItem->prevItem;
			currentItem->highlighted =1;
			printMenu(topItem);
		}
			
		
	}
}

void printMenu(struct menuItem *top){
	oled_reset();
	int line = 1;
	while(top != NULL){
		oled_print(top->name,top->highlighted);
		oled_goto_line(line++);
		oled_goto_column(0x00,0x00);
		top = top->nextItem;
	}
}