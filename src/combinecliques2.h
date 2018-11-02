#ifndef _COMBINECLIQUES2_
#define _COMBINECLIQUES2_

#include <vector>
#include <algorithm>
#include<iostream>
#include<stdio.h>
#include<time.h>
#include<fstream>
#include<string>
#include<math.h>
#include<cstdlib>
#include<string.h>
#include<malloc.h>
#include<stdlib.h>
#include <sys/stat.h>
#include<sys/types.h>
#include <vector>
#include <algorithm>
using namespace std;

#define LEN_READNODE sizeof(read_node)
#define LEN_CLIQUENODE sizeof(clique_node)



typedef struct read_node//read 节点的结构：数据部分存储read名称，指针部分指向下一个read
{
	string read;
	int read_ID;
	int d;
	int length;
	int region_flag;
	struct read_node *next;
}read_node;

typedef struct clique_node
{
	string clique;
	int clique_ID;
	int flag;//1 read in clique 2 read neighbor clique
	int d;
	double identity;
	struct clique_node *next;
}clique_node;

typedef struct clique_list
{
	string clique_name;
	int clique_ID;
	int read_num;
	struct read_node *next;
}clique_list;

typedef struct matrix_node
{
	string clique_name;
	int clique_ID;
	int read_num;
	struct read_node *next_read_node;
	struct matrix_node *next_matrix_node;

}matrix_node;

typedef struct matrix_list
{
	string clique_name;
	int clique_ID;
	int clique_num;
	struct matrix_node *next;
}matrix_list;

typedef struct region_node
{
	int count_clique1;
	int count_clique2;
}region_node;

typedef struct m4info
{
	string readname1;
	int start1;
	int end1;
	string readname2;
	int start2;
	int end2;
	double identity;
}m4info;

typedef struct m4info_node
{
	m4info info;
	struct m4info_node * next;
}m4info_node;

typedef struct deletepair_node
{
	string readname1;
	string readname2;
}deletepair_node;

typedef struct read_list
{
	string read_name;
	int read_ID;
	int flag;
	int length;
	int region_flag;
	int flag_realign;
	vector <region_node> region;
	vector <int> region2;
	struct m4info_node * info;
	struct clique_node *next;
}read_list;

typedef struct clique_matrix
{
	string clique_name1;
	int clique_ID1;
	string clique_name2;
	int clique_ID2;
	int num_reads;
	struct read_node * next;
}clique_matrix;

typedef struct combine_read_node
{
	string readname;
	struct combine_read_node * next;
}combine_read_node;

extern int count_clique,count_cliques,count_read,count_reads;


void combine();


#endif
