#ifndef GOODS_LIST_H
#define GOODS_LIST_H


#define GOODS_INFO_FILE "./goods_info.dat"


typedef struct goods
{
        int gid;	// 条形码
        char name[50];  // 商品名字
        float price;	// 商品价格
	int is_delete;  // 1 表示已删除，0 表示正常
	char manufactor[20]; // 厂家
	int num;  	// 商品数量
	int margin;	// 商品余量
        //...

} elem_type;


typedef struct node
{
         elem_type data;     // 数据域，存放数据元素
         struct node* next;  // 指针域（指向后继节点） 
} node;


typedef struct
{
        node* head;  // 链表头节点指针
        node* tail;  // 链表尾节点指针
        int size;    // 链表长度
} *list;


extern list gl;


list create(void);
void destroy(list l);
void clear(list l);
int empty(list l);
int size(list l);
void push_front(list l, elem_type* data);
void push_back(list l, elem_type* data);
void insert(list l, node* pos, elem_type* data);
void erase(list l, node* pos);
int remove2(list l, int gid);
int update(list l, int gid, elem_type* new_value);
elem_type* at(list l, int pos);
elem_type* find(list l, int gid, int* pos);
void traverse(list l, int(*visit)(elem_type*));
void traverse_fuzzy(list l, int(*visit)(elem_type*, char*, char*));

// 增加商品
void input_goods(void);
// 删除商品
void delete_goods(void);
// 修改商品
void update_goods(void);
// 查询商品
void search_goods(void);

#endif


