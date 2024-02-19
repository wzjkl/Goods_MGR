#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "goods_list.h"
#include "user_mgr.h"
#include "sales_record.h"
#include "md5.h" 

void init_ul(void);
void init_gl(void);
void init_sl(void);

void admin_op(void);
void user_op();

extern char uname[20];


int main()
{
	init_ul();
	init_gl();
	init_sl();

	//create_user();
	if(access(USER_INFO_FILE, R_OK) == 0)
	{
		if(!login_auth(uname))
		{
			printf("进入用户操作界面！\n");
			user_op();
		}
		else
		{ 
			printf("进入管理员操作界面\n");
			admin_op();
		}
	}
	
	return 0;
}

void init_gl(void)
{
	gl = create();
	
	// 将商品信息数据文件中的数据都读取到链表中来
	struct goods g;
	FILE* fp = fopen(GOODS_INFO_FILE, "rb");
	if(NULL == fp) return;

	while(fread(&g, sizeof(g), 1, fp) == 1)
	{
		push_back(gl, &g);
	}

	fclose(fp);
}

void init_ul(void)
{
	ul = create_ul();
	
	// 将商品信息数据文件中的数据都读取到链表中来
	user_info u;
	FILE* fp = fopen(USER_INFO_FILE, "rb");
	if(NULL == fp) return;

	while(fread(&u, sizeof(u), 1, fp) == 1)
	{
		push_back_user(ul, &u);
	}

	fclose(fp);
}

void init_sl(void)
{
	sl = create_sl();
	
	// 将商品信息数据文件中的数据都读取到链表中来
	sale_record s;
	FILE* fp = fopen(SALE_RECORD_FILE, "rb");
	if(NULL == fp) return;

	while(fread(&s, sizeof(s), 1, fp) == 1)
	{
		push_back_sale(sl, &s);
	}

	fclose(fp);
}


void admin_op(void)
{
	int op;
	
	printf("欢迎使用商品信息管理系统！\n");
	printf("\n使用帮助：\n");
	printf("录入商品信息请输入1\n");	
	printf("删除商品信息清输入2\n");
	printf("修改商品信息清输入3\n");
	printf("查询所有商品信息清输入4\n");
	printf("添加新用户请输入5\n");
	printf("删除一名用户输入6\n");
	printf("修改一名用户信息请输入7\n");
	printf("查询收银员信息请输入8\n");
	printf("查询收银员销售记录输入9\n");
	printf("按收银员和时间段统计销售额请按10\n");
	printf("退出系统请输入0\n");
	
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
			printf("\n谢谢使用，下次再见！\n");
			return;
			
			case 1:
			input_goods();
			break;
			
			case 2:
			delete_goods();
			break;
			
			case 3:
			update_goods();
			break;
		
			case 4:
			printf("模糊查询请按1\n");
			printf("查询全部请按2\n");
			printf("返回上一级菜单请按0\n");
			second_query();
			break;
			
			case 5:
			create_user();
			break;
			
			case 6:
			delete_user();
			break;
			
			case 7:
			modify_user();
			break;		
	
			case 8:
			search_user();
			break;
			
			case 9:
			search_sales_record();
			break;
				
			case 10:
			sum_sales_time();
			break;
				
			default:
			printf("\n输入错误，请重新输入！\n");
		}
	}

}


void user_op()
{
	int op;
	
	printf("欢迎收银员！\n");
	printf("\n使用帮助：\n");
	printf("修改登录密码请输入1\n");
	printf("查看个人信息清输入2\n"); 
	printf("查看销售记录和销售额请输入3\n");
	printf("查询库存商品信息请输入4\n");
	printf("结账请输入5\n");
	printf("退出系统请输入0\n");
	
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
			printf("\n谢谢使用，下次再见！\n");
			return;
			
			case 1:
			reset_passwd();
			break;
			
			case 2:
			show_user_info();
			break;
			
			case 3:
			show_sales_record();
			break;
		
			case 4:
			search_goods();
			break;
			
			case 5:
			checkout();
			break;
			
			default:
			printf("\n输入错误，请重新输入！\n");
		}
	}

}









