#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sales_record.h"


slist sl;
char uname[20];
float sum = 0;
char name[20];

slist create_sl(void)
{
	snode* p = malloc(sizeof(snode));
	
	if(NULL == p)
	{
		perror("malloc error");
		return NULL;
	}
	
	p -> next = NULL;
	
	slist l = NULL;
	
	l = malloc(sizeof(*l));
	
	if(NULL == l)
	{
		perror("malloc error");
		return NULL;
	}
	
	l -> head = p;
	l -> tail = p;
  	
 	l -> size = 0;
	
	return l;

}

// 插入为节点
void push_back_sale(slist l, sale_record* data)
{
	snode* p = malloc(sizeof(snode));
	
	if(NULL == p)
	{
		perror("malloc fail");
		return ;
	}
	
	p -> data = *data;
	
	p -> next = NULL;
	
	l -> tail -> next = p;
	
	l -> tail = p;
	
	l -> size++;


}


void traverse_sale(slist l, int(*visit)(sale_record*))
{
	snode* p = l->head->next;

	while(p != NULL)
	{
		//if(!visit(&(p->data))) break;
		
		visit(&(p->data));
		
		p = p->next;
	}
}



// 在链表中查找某个节点
sale_record* find_record(slist l, char* name, int* pos)
{
	snode* p = l -> head -> next;
		
	if(pos != NULL) *pos = 0;
	
	while(p != NULL)
	{
		if(strcmp(p -> data.name, name) == 0) return &(p -> data);
	
		p = p -> next;
		if(pos != NULL) (*pos)++;
	}
	
	return NULL;
}

int show_sale_name(sale_record* s)
{
	struct tm* info;
	
	info = localtime(&s->sale_time);

	if(!strcmp(s -> name, uname))
	{
		sum += s -> total_price;
		printf("用户名：%s\n 条形码：%d\n 商品价格：%.1f\n 售出时间：%d:%d:%d\n\n", s->name, s->gid, s -> total_price, info->tm_hour, info->tm_min,info-> tm_sec);	
		return 1;
	}
	return 0;
}


void show_sales_record()
{
	traverse_sale(sl, show_sale_name);
	printf("销售额为%.1f元\n", sum);
	sum = 0;
}

int show_sale_time(sale_record* s)
{
	struct tm* info;
	
	info = localtime(&s->sale_time);

	if(!strcmp(s -> name, name))
	{
		sum += s -> total_price;
		printf("用户名：%s\n 条形码：%d\n 商品价格：%.1f\n 售出时间：%d:%d:%d\n\n", s->name, s->gid, s -> total_price, info->tm_hour, info->tm_min,info-> tm_sec);	
		return 1;
	}
	return 0;
}

void sum_sales_time()
{
	printf("请输入用户名：");
	scanf("%s", name);
	
	traverse_sale(sl, show_sale_time);
	printf("销售额是%.1f\n", sum);
	sum = 0;
}

int show_sale_all(sale_record* s)
{
	struct tm* info;
	
	info = localtime(&s->sale_time);
		
		printf("用户名：%s\n 条形码：%d\n 商品价格：%.1f\n 售出时间：%d:%d:%d\n\n", s->name, s->gid, s -> total_price, info->tm_hour, info->tm_min,info-> tm_sec);	
}

void search_sales_record()
{
	traverse_sale(sl, show_sale_all);
}





























