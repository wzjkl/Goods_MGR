#ifndef SALE_RECORD_H
#define SALE_RECORD_H
#define SALE_RECORD_FILE "./record_info.dat"


#include <time.h>

typedef struct
{
	char name[21];
	int gid;
	time_t sale_time;
	float total_price;
	char notes[100];
}sale_record;

typedef struct snode
{
	sale_record data;
	
	struct snode* next;
} snode;

typedef struct
{
	snode* head;
	snode* tail;
	int size;
}*slist;

extern slist sl;

slist create_sl(void);
sale_record* find_sale(slist l, char* name, int* pos);
void push_back_sale(slist l, sale_record* data);
void traverse_sale(slist l, int(*visit)(sale_record*));

// 查询所有销售记录
void search_sales_record();
// 管理员通过名字查询销售记录
void sum_sales_time();

// 用户通过名字查询销售记录，并显示销售额
void show_sales_record();
int show_sale_name(sale_record* s);






#endif
