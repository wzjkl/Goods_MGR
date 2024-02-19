#ifndef USER_MGR_H
#define USER_MGR_H
#define USER_INFO_FILE "./user_info.dat"

#include <time.h>


struct address
{
	char prov[20];
	char city[20];
	char street[30];
	char community[30];
};

typedef struct
{
    char uname[51];
    char upass[33];
    char sex;
    char phone[12];
    time_t reg_time;         // 注册时间
    time_t last_login_time;  // 上一次登录时间
    int delete_flag;
    int role;  // 1 表示系统管理员，2 表示普通用户，......
    char uid[20];
    struct address addr;	
    //...
    
} user_info;

typedef struct unode
{
         user_info data;     // 数据域，存放数据元素
         struct unode* next;  // 指针域（指向后继节点） 
} unode;


typedef struct
{
        unode* head;  // 链表头节点指针
        unode* tail;  // 链表尾节点指针
        int size;    // 链表长度
} *ulist;


extern ulist ul;



int update_user(ulist l, char* uid, user_info* new_value);
user_info* find_user(ulist l, char* uid, int* pos);
ulist create_ul(void);
void push_back_user(ulist l, user_info* data);
void traverse_user(ulist l, int(*visit)(user_info*));

// 检查用户是否存在
int check_user_name(const char* uname);
// 登录验证
int login_auth(char* name);


// 添加用户信息
void create_user(void);
// 删除用户信息
void delete_user();
// 修改用户信息
void modify_user();
// 查询全部用户信息
void search_user();
int show_user_all();

// 修改密码
void reset_passwd();
// 模糊查询商品
void search_fuzzy_goods(void);

// 搜索商品的二级菜单
void second_query();
//void fuzzy_query();

// 结账函数
void checkout();
// 查询当前用户信息
void show_user_info();

#endif


