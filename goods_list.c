#include <stdio.h>
#include <stdlib.h>
#include "goods_list.h"

list gl;
extern char f_name[20];
extern char f_manufactor[20];

// 创建一个空链表（只有一个头结点）
list create(void)
{
	node* p = malloc(sizeof(node));
	
	if(NULL == p)
	{
		perror("malloc error");
		return NULL;
	}
	
	p -> next = NULL;
	
	list l = NULL;
	
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

// 销毁链表（删除所有节点）
void destory(list l)
{
	node* p;
	
	// 释放链表 l 的所有节点
	while(l -> head != NULL)
	{
		p = l -> head -> next;
		free(l -> head);
		l -> head = p;
	}
	
	// 释放链表信息结构体
	free(l);
}


// 清空链表（除了头节点，删除其他所有节点）
void clear(list l)
{
	node* p, *q;
	
	// 断链
	p = l -> head -> next;
	l -> head -> next = NULL;
	
	// 删除除头结点之外的所有其他节点
	while(p != NULL)
	{
		q = p -> next;
		free(p);
		p = q;
	}
	
	l -> tail = l -> head;
	l -> size = 0;
}

// 判空，时间复杂度为O(1)
int empty(list l)
{
	return !(l -> size);
}

// 求长度
int size(list l)
{
	return l -> size;
}


// 插入新节点（头插）
void push_fornt(list l, elem_type* data)
{
	node* p = malloc(sizeof(node));
	
	if(NULL == p)
	{
		perror("malloc error");
		return;
	}
	
	p -> data = *data;
	
	p -> next = l -> head -> next;
	l -> head -> next = p;
	
	if(l -> size == 0) l -> tail = p;
	
	l -> size++;
}


// 插入新节点（尾插）
void push_back(list l, elem_type* data)
{
	node* p  = malloc(sizeof(node));
	
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


// 从链表中删除一个节点
void erase(list l, node* pos)
{
	node* p = pos -> next;
	pos -> next = p -> next;
	
	free(p);
	
	if(pos -> next == NULL) l -> tail = pos;
	
	l -> size--;

}

// 根据数据域删除节点
int remove2(list l, int gid)
{
	node* p = l -> head, *q;
	
	// 找到要删除节点的前驱节点
	while(p -> next != NULL && p -> next -> data.gid != gid)
		p = p -> next;
	
	// 如果不存在目标节点 
	if(NULL == p -> next) return 0;
	
	// 删除目标节点
	q = p -> next;
	p -> next = q -> next;
	free(q);
	
	if(p -> next == NULL) l -> tail = p;
	
	l -> size--;
	
	return 1;	
}

// 给某 个节点修改数据域
int update(list l, int gid, elem_type* new_value)
{
	node* p = l -> head -> next;
	
	while(p != NULL)
	{
		if(p -> data.gid == gid)
		{
			p -> data = *new_value;
			return 1;
		}
		p = p -> next;
	}
	
	return 0;
}


// 随机访问某个数据元素
elem_type* at(list l, int pos)
{
	node* p = l -> head -> next;
	
	while(pos--) p = p -> next;
	
	return &(p -> data);
}

// 在链表中查找某个节点
elem_type* find(list l, int gid, int* pos)
{
	node* p = l -> head -> next;
		
	if(pos != NULL) *pos = 0;
	
	while(p != NULL)
	{
		if(p -> data.gid == gid) return &(p -> data);
	
		p = p -> next;
		if(pos != NULL) (*pos)++;
	}
	
	return NULL;
}




// 遍历节点
// visit 为访问函数，用它实现对每个节点的具体访问操作
void traverse(list l, int(*visit)(elem_type*))
{
	node* p = l->head->next;

	while(p != NULL)
	{
		// 如果某次调用访问函数返回值为 0，就停止继续遍历链表
		if(!visit(&(p->data))) break;
		
		p = p->next;
	}
}

void traverse_fuzzy(list l, int(*visit)(elem_type*, char*, char*))
{
	node* p = l->head->next;

	while(p != NULL)
	{
		// 如果某次调用访问函数返回值为 0，就停止继续遍历链表
		if(!visit(&(p->data), f_name, f_manufactor)) break;
		
		p = p->next;
	}
}


void input_goods(void)
{
	struct goods g;

	printf("\n请按照提示依次录入商品信息。\n");
	printf("商品ID：");
	scanf("%d", &(g.gid));
	printf("商品名称：");
	scanf("%s", g.name);
	printf("商品单价：");
	scanf("%f", &(g.price));
	printf("厂家：");
	scanf("%s", g.manufactor);
	printf("数量：");
	scanf("%d", &g.num);
	printf("余量：");
	scanf("%d", &g.margin);
	
	g.is_delete = 0;
	push_back(gl, &g);

	FILE* fp = fopen(GOODS_INFO_FILE, "ab");
	fwrite(&g, sizeof(g), 1, fp);
	fclose(fp);

	printf("\n商品信息录入成功！\n");
}




void delete_goods(void)
{
	int gid;

	printf("\n请输入要删除商品的ID：");
	scanf("%d", &gid);

	int pos;
	struct goods* pg = find(gl, gid, &pos);

	if(pg == NULL)
	{
		printf("\n商品不存在，删除失败！\n");
	}
	else
	{
		pg->is_delete = 1;  // 在链表中删除相应的商品信息

		// 在文件中删除相应的商品信息
		FILE* fp = fopen(GOODS_INFO_FILE, "r+b");
		fseek(fp, pos * sizeof(struct goods), SEEK_SET);
		fwrite(pg, sizeof(struct goods), 1, fp);
		fclose(fp);		

		printf("\n删除商品成功！\n");
	}
}

void update_goods(void)
{
	int gid;

	printf("\n请输入要修改商品的ID：");
	scanf("%d", &gid);

	int pos;
	struct goods* pg = find(gl, gid, &pos);

	if(pg == NULL)
	{
		printf("\n商品不存在，删除失败！\n");
	}
	else
	{
		printf("请输入商品名称：");
		scanf("%s", pg->name);
		
		printf("请输入商品价格：");
		scanf("%f", &pg->price);
		
		printf("请输入商品厂家：");
		scanf("%s", pg->manufactor);
		
		printf("请输入商品数量：");
		scanf("%d", &pg->num);
		
		printf("请输入商品余量：");
		scanf("%d", &pg->margin);

		// 在文件中删除相应的商品信息
		FILE* fp = fopen(GOODS_INFO_FILE, "r+b");
		fseek(fp, pos * sizeof(struct goods), SEEK_SET);
		fwrite(pg, sizeof(struct goods), 1, fp);
		fclose(fp);		

		printf("\n修改商品信息成功！\n");
	}

}


int show_goods(struct goods* g)
{
	if(!g->is_delete)
		printf("%d %s %g %s %d %d\n", g->gid, g->name, g->price, g->manufactor, g->num, g->margin);
		return 1;
	return 0;
}


void search_goods(void)
{
	traverse(gl, show_goods);
}





















