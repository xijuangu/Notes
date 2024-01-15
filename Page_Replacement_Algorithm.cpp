#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <string>
#include <windows.h>

using namespace std;


struct Memory
{
	int id;					   // 序号
	int priority;			   // 最前面的内存优先级为0，往后依次加1
	int distance;			   // 下次访问与当前距离
} memory[1010], memory2[1010]; // 内存序列，临时数组
int que[1010];				   // 页面序列
void input();				   // 输入函数
void output(int k);			   // 输出函数
void OPT();					   // 最佳置换算法
void FIFO();				   // 先进先出算法
void LRU();					   // 最近最久未使用算法
int sig;					   // 算法选择标志
int memoryk, num, total;	   // 内存块数，页面数量，缺页次数
int pageFlag;				   // 缺页标志
const int isPage = 1;		   // 命中
const int noPage = 0;		   // 缺页



int main()
{
	SetConsoleOutputCP(65001);
	// 选择算法
	while (1)
	{
		// 程序输入
		cout << "请选择：\n1. OPT \n2. FIFO \n3. LRU\n";
		input();

		switch (sig)
		{
		case 1:
			cout << "已选择OPT算法：\n";
			OPT();
			break;
		case 2:
			cout << "已选择FIFO算法：\n";
			FIFO();
			break;
		case 3:
			cout << "已选择LRU算法：\n";
			LRU();
			break;
		}
	}

	return 0;
}


void input()
{
	// freopen("osin.txt", "r", stdin);
	// freopen("osout.txt", "w", stdout);

	sig = 0;
	cin >> sig; // 算法选择标志

	memoryk = 0;
	total = 0;
	cout << "请输入内存块数：\n";
	cin >> memoryk; // 内存块数
	cout << "请输入页面序列，用“，”分隔\n";
	num = 0;
	char c;
	while (scanf("%d", &que[num])) // 输入页面序列
	{
		num++;
		c = getchar();
		if (c == '\n') // 遇到换行符则输入结束
		{
			break;
		}
		else if (c == ',') // 遇到逗号则继续读取
		{
			continue;
		}
	}
	for (int i = 0; i < memoryk; i++) // 内存初始化
	{
		memory[i].id = -1;
		memory[i].priority = i;
	}
}


void output(int k)
{
	for (int i = 0; i < memoryk; i++) // 输出内存中页面序列
	{
		if (memory[i].id != -1) // 内存有页面
		{
			printf("%d,", memory[i].id);
		}
		else // 内存没有页面
		{
			printf("-,");
		}
	}
	if (k != num - 1) // 没到最后一页
	{
		if (pageFlag == 0)
			printf("未命中\t");
		if (pageFlag == 1)
			printf("命中\t");
		
		cout << "\n";
	}
	else // 最后一页
	{
		if (pageFlag == 0)
		{
			printf("未命中\n");
			printf("缺页次数：%d\n", total);
			printf("缺页率：%f", (float)total / (float)memoryk);
		}
		if (pageFlag == 1)
		{
			printf("命中\n");
			printf("缺页次数：%d\n", total);
			printf("缺页率：%f", (float)total / (float)memoryk);
		}
	}
}


void OPT()
{
	int optFlag = 0; // OPT页面替换标志，替换下标为optFlag的页面
	for (int i = 0; i < num; i++)
	{
		int tmp = 0; // 内存块下标
		while (tmp < memoryk)
		{
			if (que[i] == memory[tmp].id) // 内存中有此页就输出
			{
				pageFlag = isPage;
				output(i);
				break;
			}
			if (memory[tmp].id == -1) // 内存中无此页则加入内存
			{
				memory[tmp].id = que[i];
				total++;
				pageFlag = noPage;
				output(i);
				break;
			}
			tmp++;
		}
		// 运行到此处证明找遍内存仍未命中页面
		// 淘汰下次访问距当前最远的那些页中序号最小的页，距离相等则淘汰优先级低的页
		if (tmp == memoryk)
		{
			for (int j = 0; j < memoryk; j++) // 距离初始化
			{
				memory[j].distance = 99999;
			}
			for (int j = 0; j < memoryk; j++) // 计算距离
			{
				int dis = 0;
				for (int k = i + 1; k < num; k++) // 记录下一个序号相同页面的距离
				{
					dis++;
					if (que[k] == memory[j].id) // 更新距离
					{
						memory[j].distance = dis;
						break;
					}
				}
			}
			int max_dis = memory[0].distance;	  // 找最大距离
			int min_prority = memory[0].priority; // 找最小优先级
			for (int k = 0; k < memoryk; k++)
			{
				if (memory[k].distance == max_dis)
				{
					if (memory[k].priority <= min_prority)
					{
						min_prority = memory[k].priority;
						max_dis = memory[k].distance;
						optFlag = k;
					}
				}
				else if (memory[k].distance > max_dis)
				{
					min_prority = memory[k].priority;
					max_dis = memory[k].distance;
					optFlag = k;
				}
			}
			// 调整优先级
			memory[optFlag].priority = memoryk;
			for (int k = 0; k < memoryk; k++)
			{
				memory[k].priority--;
			}
			// 缺页处理
			memory[optFlag].id = que[i];
			pageFlag = noPage;
			total++;
			output(i);
		}
	}
}


void FIFO()
{
	int fifoFlag = 0; // FIFO页面替换标志，替换下标为fifoFlag的页面
	for (int i = 0; i < num; i++)
	{
		int tmp = 0; // 内存块下标
		while (tmp < memoryk)
		{
			if (que[i] == memory[tmp].id) // 内存中有此页就输出
			{
				pageFlag = isPage;
				output(i);
				break;
			}
			if (memory[tmp].id == -1) // 内存中无此页则加入内存
			{
				memory[tmp].id = que[i];
				total++;
				pageFlag = noPage;
				output(i);
				break;
			}
			tmp++;
		}
		// 运行到此处证明找遍内存仍未命中页面
		// 按照FIFO的顺序进行页面淘汰
		if (tmp == memoryk)
		{
			if (fifoFlag == memoryk) // 保证替换范围在[0-memoryk)之间
			{
				fifoFlag = 0;
			}
			// 缺页处理
			memory[fifoFlag].id = que[i];
			fifoFlag++;
			pageFlag = noPage;
			total++;
			output(i);
		}
	}
}


void LRU()
{
	int empty;		 // 内存块中是否含有空闲区
	int lruFlag = 0; // LRU页面替换标志，记录第一个空闲区下标
	int x;			 // 临时数组下标
	for (int i = 0; i < num; i++)
	{
		empty = 0;
		lruFlag = 0;

		for (int j = 0; j < memoryk; j++) // 寻找空闲区
		{
			if (memory[j].id == -1)
			{
				empty = 1;
				lruFlag = j;
				break;
			}
		}

		int tmp = 0; // 内存块下标
		while (tmp < memoryk)
		{
			if (memory[tmp].id == que[i]) // 内存中有此页
			{
				if (empty == 1) // 有空闲区
				{
					x = 0;
					// 调整输出顺序
					for (int k = tmp + 1; k < lruFlag; k++)
					{
						memory2[x].id = memory[k].id;
						x++;
					}
					x = 0;
					for (int k = tmp; k < lruFlag - 1; k++)
					{
						memory[k].id = memory2[x].id;
						x++;
					}
					memory[lruFlag - 1].id = que[i];

					// 输出
					pageFlag = isPage;
					output(i);
					break;
				}
				else // 没有空闲区
				{
					x = 0;
					// 调整输出顺序
					for (int k = tmp + 1; k < memoryk; k++)
					{
						memory2[x].id = memory[k].id;
						x++;
					}
					x = 0;
					for (int k = tmp; k < memoryk - 1; k++)
					{
						memory[k].id = memory2[x].id;
						x++;
					}
					memory[memoryk - 1].id = que[i];

					// 输出
					pageFlag = isPage;
					output(i);
					break;
				}
			}
			tmp++;
		}
		// 运行到此处证明找遍内存仍未命中页面
		// 淘汰上次使用距当前最远的页
		if (tmp == memoryk)
		{
			if (empty == 1) // 有空闲区
			{
				memory[lruFlag].id = que[i]; // 直接装入
				pageFlag = noPage;
				total++;
				output(i);
			}
			else // 淘汰页面
			{
				// 调整输出顺序
				int y = 0;
				for (int k = 1; k < memoryk; k++)
				{
					memory2[y].id = memory[k].id;
					y++;
				}
				y = 0;
				for (int k = 0; k < memoryk - 1; k++)
				{
					memory[k].id = memory2[y].id;
					y++;
				}
				memory[memoryk - 1].id = que[i];

				// 缺页处理
				pageFlag = noPage;
				total++;
				output(i);
			}
		}
	}
}
