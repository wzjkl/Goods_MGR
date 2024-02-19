#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "user_mgr.h"
#include "goods_list.h"
#include "sales_record.h"
#include "md5.h"

ulist ul;
extern char uname[20];


char f_name[20];
char f_manufactor[20];

// 创建一个空链表（只有一个头结点）
ulist create_ul(void)                                                                                                                                                                                                       
{
	unode* p = malloc(sizeof(unode));
	
	if(NULL == p)
	{
		perror("malloc error");
		return NULL;
	}
	
	p -> next = NULL;
	
	ulist l = NULL;
	
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

// 插入新节点（尾插）
void push_back_user(ulist l, user_info* data)
{
	unode* p  = malloc(sizeof(unode));
	
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

// 给某 个节点修改数据域
int update_user(ulist l, char* uid, user_info* new_value)
{
	unode* p = l -> head -> next;
	
	while(p != NULL)
	{
		if(strcmp(p -> data.uid, uid) == 0)
		{
			p -> data = *new_value;
			return 1;
		}
		p = p -> next;
	}
	
	return 0;
}




// 在链表中查找某个节点
user_info* find_user(ulist l, char* name, int* pos)
{
	unode* p = l -> head -> next;
		
	if(pos != NULL) *pos = 0;
	
	while(p != NULL)
	{
		if(strcmp(p -> data.uname, name) == 0) return &(p -> data);
	
		p = p -> next;
		if(pos != NULL) (*pos)++;
	}
	
	return NULL;
}

// 遍历节点
// visit 为访问函数，用它实现对每个节点的具体访问操作
void traverse_user(ulist l, int(*visit)(user_info*))
{
         unode* p = l->head->next;
 
         while(p != NULL)
         {
                 // 如果某次调用访问函数返回值为 0，就停止继续遍历>    链表
                 //if(!visit(&(p->data))) break;
 
                 visit(&(p->data));
                 p = p->next;
         }
}








// 判断用户名是否存在
int check_user_name(const char* uname)
{
    FILE* fp = fopen(USER_INFO_FILE, "rb");

    if(NULL == fp) return 1;

    user_info ui;
    int exist_flag = 0;

    while(fread(&ui, sizeof(ui), 1, fp) == 1)
    {
        if(strcmp(ui.uname, uname) == 0)
        {
            exist_flag = 1;
            break;
        }
    }

    fclose(fp);

    return !exist_flag;
}



// 创建新用户
void create_user(void)
{
    user_info ui;
    char password[51];

    while(1)
    {
        printf("\n用户名：");
        scanf("%s", ui.uname);

        if(check_user_name(ui.uname))
            break;

        printf("\n用户名已存在，请重新输入！\n");
    }

    printf("\n密码：");
    scanf("%s", password);  // 密码输入应该无回显或回显圆点等其他符号，防止旁边的人偷窥
    getchar();

    printf("\n性别：");
    scanf("%c", &(ui.sex));
	
    while(1)
    {
    	printf("\n手机号：");
    	scanf("%s", ui.phone);
	
	if(strlen(ui.phone) == 11) break;	
	
	printf("手机号长度应该为11位\n");
    }
	
    while(1)
    {
    	printf("\n身份证：");
    	scanf("%s", ui.uid);
	
	if(strlen(ui.uid) == 18) break;
	
	printf("身份证长度应该为18位\n");
    }
    printf("\n省：");
    scanf("%s", ui.addr.prov);
	
    printf("\n市：");
    scanf("%s", ui.addr.city);
	
    printf("\n街道：");	
    scanf("%s", ui.addr.street);
	
    printf("\n小区：");
    scanf("%s", ui.addr.community);    

    md5_string(password, ui.upass);    

    ui.delete_flag = 0;
    ui.role = 2;
    ui.reg_time = time(NULL);
    ui.last_login_time = 0;

    push_back_user(ul, &ui);	

    FILE* fp = fopen(USER_INFO_FILE, "ab");

    if(NULL == fp)
    {
        perror("fopen");
        return;
    }

    fwrite(&ui, sizeof(ui), 1, fp);

    fclose(fp);

    printf("\n创建用户成功！\n");
}



// 登录验证
int login_auth(char* name)
{
    char uname[51], upass[51], upass_md5[33];
    int pos;
    printf("\n请登录。\n");
    printf("用户名：");
    scanf("%s", uname);
    printf("密码：");
    system("stty -echo");
    scanf("%s", upass);
    system("stty echo");

    md5_string(upass, upass_md5);

    FILE* fp = fopen(USER_INFO_FILE, "r+b");

    user_info ui;
    int auth_flag = 0;

    while(fread(&ui, sizeof(ui), 1, fp) == 1)
    {
        if(strcmp(ui.uname, uname) == 0 && strcmp(ui.upass, upass_md5) == 0)
        {
            strcpy(name, ui.uname);

            char p_flag;
            if(ui.last_login_time == 0)
            {
                printf("\n否修改密码：Y/N\n");
                scanf("\n%c", &p_flag);

                if(p_flag == 'Y')
                {
                    char upass[51];
                    char pass_md5[33];

                    printf("请输入密码：");
                    scanf("%s", upass);
                    
                    md5_string(upass, pass_md5);
                    
                    strcpy(ui.upass, pass_md5);

                    printf("\n修改密码成功！\n");
                }            
            }

            user_info* user = find_user(ul, uname, &pos);
            user->last_login_time = time(NULL);
            ui.last_login_time = time(NULL);

            fseek(fp, pos * sizeof(ui), SEEK_SET);
            fwrite(&ui, sizeof(ui), 1, fp);

            auth_flag = 1;
            break;
        }
    }

    fclose(fp);

    if(auth_flag)
    {
	if(ui.delete_flag == 1)
	{
        	printf("\n用户名或密码错误，登录失败！\n");
        	exit(1);
	}

        printf("\n用户(%s)，你好，欢迎使用本系统！\n", uname);
        ui.last_login_time = time(NULL);
	if(ui.role == 1) return 1;
    } 
    else
    {
        printf("\n用户名或密码错误，登录失败！\n");
        exit(1);
    }
   
    return 0;
}

void second_query()
{
	
	int op;
	while(1)
	{
		printf("\n:> ");

		if(scanf("%d", &op) != 1)
		{
			while(getchar() != '\n'); // 清空缓冲区

			op = -1;
		}

		switch(op)
		{
			case 0:
				printf("\n返回上一级菜单！\n");
				return;

			case 1:
				search_fuzzy_goods();
				break;	

			case 2:
				search_goods();
				break;

			defualt:
				printf("\n输入错误，请重新输入\n");	
		}

	}
}

int show_fuzzy_goods(struct goods* g, char* name, char* manufactor)
{
	
	if(!g->is_delete && strstr(g->name, name) != NULL && strstr(g->manufactor,manufactor) != NULL)
		printf("%d %s %g %s %d %d\n", g->gid, g->name, g->price, g->manufactor, g->num, g->margin);
		return 1;
	return 0;
}


void search_fuzzy_goods(void)
{	
	//while(getchar() != '\n'); // 清空缓冲区

	printf("请输入商品名称：");
	scanf("%s", f_name);
	getchar();
	printf("请输入厂家名称：");
	scanf("%s", f_manufactor);
		
	traverse_fuzzy(gl, show_fuzzy_goods);
}

void checkout()
{
	int gid;
	float price;
	int op;
	float sum = 0;
	int pos;
	printf("扫码请按1\n");
	printf("结账请按2\n");
	
	sale_record s;

	while(1)
	{
		
		printf("\n:> ");
		
		if(scanf("%d", &op) != 1)
		{
			while(getchar() != '\n'); // 清空缓冲区
			
			op = -1;
		}
		
		if(op == 1)
		{
			printf("请输入商品条形码：");
			scanf("%d", &gid);

			struct goods* g = find(gl, gid, &pos);
			printf("%s %.1f\n", g -> name, g -> price);
			if(g -> is_delete == 1)
			{
				printf("该商品不存在\n");
				continue;
			}
			
			sum += g -> price;
			s.gid = g->gid;
			s.total_price = g -> price;	
			g -> margin--;
			s.sale_time = time(NULL);
			if(g -> margin == 0)
			{
				printf("该商品已经卖完\n");
			}
			
			FILE* fp = fopen(GOODS_INFO_FILE, "r+b");
			fseek(fp, pos * sizeof(struct goods), SEEK_SET);
			fwrite(g, sizeof(struct goods), 1, fp);
			fclose(fp);
			
			strcpy(s.name, uname);
	
			push_back_sale(sl, &s);
	
			FILE* p = fopen(SALE_RECORD_FILE, "ab");
			fwrite(&s, sizeof(sale_record), 1, p);
			fclose(p);
	
		}
		else break;					
	}
	
	printf("应付款%.1f元\n", sum);	
			
	printf("请输入付款金额：");
	scanf("%f", &price);
			
	printf("找零%.1lf元\n", price - sum);


	return;
}



void reset_passwd()
{
	char upass[51];
	char re_upass[51];
	char pass_md5[33];
	int pos;
	user_info* user = find_user(ul, uname, &pos);
	
	printf("请输入密码：");
	scanf("%s", upass);
	printf("确认密码：");
	
	while(1)
	{	
				
		while(getchar() != '\n'); // 清空缓冲区
		
		scanf("%s", re_upass);

		if(strcmp(upass, re_upass) == 0) break;
		
		printf("\n密码不一致，重新输入密码：");
	}	
   	md5_string(upass, pass_md5);
	
	strcpy(user->upass, pass_md5);		

	if(user == NULL)
	{
		printf("\n用户不存在\n");
	}
	else
	{
		// 在文件中修改相应的用户密码
		FILE* fp = fopen(USER_INFO_FILE, "r+b");
		fseek(fp, pos * sizeof(user_info), SEEK_SET);
		fwrite(user, sizeof(user_info), 1, fp);
		fclose(fp);		

		printf("\n密码修改成功成功！\n");
	}

}
	
void show_user_info()
{
	struct tm *info;

	int pos;
	user_info* user = find_user(ul, uname, &pos);
	
	info = localtime(&user->reg_time);

	printf("%s %s %s %s %s %d:%d:%d ", user->uname, user->upass, user->sex == '1' ? "男": "女", user->phone, user->addr.prov,info->tm_hour, info->tm_min, info->tm_sec);
	
	info = localtime(&user-> last_login_time);
	printf("%d:%d:%d\n", info->tm_hour, info -> tm_min, info-> tm_sec);
}


void delete_user()
{
	char name[20];

	printf("\n请输入要删除用户的姓名：");
	scanf("%s", name);

	int pos;
	user_info* user = find_user(ul, name, &pos);

	if(user == NULL)
	{
		printf("\n用户不存在，删除失败！\n");
	}
	else
	{
		user->delete_flag = 1;  // 在链表中删除相应的商品信息

		// 在文件中删除相应的商品信息
		FILE* fp = fopen(USER_INFO_FILE, "r+b");
		fseek(fp, pos * sizeof(user_info), SEEK_SET);
		fwrite(user, sizeof(user_info), 1, fp);
		fclose(fp);		

		printf("\n删除用户成功！\n");
	}

}
void modify_user()
{
	char name[20];
	char upass[50];
	char pass_md5[33];
	int pos;

	printf("请输入要修改用户的名称：");
	scanf("%s", name);
	
	user_info* user = find_user(ul, name, &pos);

	if(user == NULL)
	{
		printf("\n用户不存在，修改失败！\n");
	}
	else
	{
		printf("请输入用户名称：");
		scanf("%s", user->uname);
		
		printf("请输入用户密码：");
		scanf("%s", upass);
		
		md5_string(upass, pass_md5);
		strcpy(user->upass, pass_md5);		

		getchar();
	
		printf("请输入性别：");
		scanf("%c", &user->sex);
		
		printf("请输入手机号码：");
		scanf("%s", user->phone);
		
		printf("请输入身份证号码：");
		scanf("%s", user->uid);
		
		printf("请输入用户所在省份：");
		scanf("%s", user->addr.prov);

		printf("请输入用户所在市：");
		scanf("%s", user->addr.city);
		
		printf("请输入用户所在街道：");
		scanf("%s", user->addr.street);
		
		printf("请输入用户所在住址：");
		scanf("%s", user->addr.community);
		
		// 在文件中删除相应的商品信息
		FILE* fp = fopen(USER_INFO_FILE, "r+b");
		fseek(fp, pos * sizeof(user_info), SEEK_SET);
		fwrite(user, sizeof(user_info), 1, fp);
		fclose(fp);		

		printf("\n修改用户信息成功！\n");
	}

}

int show_user_all(user_info* user)
{
	struct tm *info;
	
	info = localtime(&user->reg_time);
	
	if(!user->delete_flag)
	{
		printf("%s %s %s %s %s %d:%d:%d ", user->uname, user->upass, user->sex == '1' ? "男": "女", user->phone, user->addr.prov,info->tm_hour, info->tm_min, info->tm_sec);
	
		info = localtime(&user-> last_login_time);
		printf("%d:%d:%d\n", info->tm_hour, info -> tm_min, info-> tm_sec);

		return 1;
	}
         return 0;	
}

void search_user()
{
	traverse_user(ul, show_user_all);
}

