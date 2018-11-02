#include<stdio.h>
#include<iostream>
#include<string.h>
#include<fstream>
#include<stdlib.h>
#include <vector>
using namespace std;
FILE* f_orim4;
FILE* f_ali;
FILE* f_del;
FILE* f_newm4;
FILE* f_read;
typedef struct m4info
{
	string readname1;
	int start1;
	int end1;
	string info3;
	string info4;
	string info5;
	string readname2;
	int start2;
	int end2;
	string info8;
	string info9;
	string info12;
	string info13;
}m4info;

typedef struct m4info_node
{
	m4info info;
	struct m4info_node * next;
}m4info_node;

typedef struct delpair_node
{
	int readID;
	struct delpair_node * next;
}delpair_node;
typedef struct delpair_list
{
	int num;
	struct delpair_node * next;
}delpair_list;

vector <string> orim4list;
delpair_list* delpairlist;

int countFileLineNum(string filename)
{
	ifstream aFile(filename.c_str());
	int lines_count = 0;
	string line;
	while(std::getline(aFile,line))
		++ lines_count;

	return lines_count;
}
int num_read=0;
void init_global_variables(string filename)
{
//	cout<<"init_global_variables()"<<endl;
	num_read = countFileLineNum(filename) / 2;
	delpairlist = new delpair_list[num_read];
}

void readorim4file(string filename)
{
	ifstream aFile(filename.c_str());
	string line;
	while(std::getline(aFile,line))
	{
		orim4list.push_back(line);
	}
	aFile.close();
}


void readdelfile(string filename)
{
	int count=0;
	string delline;
	ifstream aFile(filename.c_str());
	while(getline(aFile,delline))
	{
		int flag=0;
		string readID1;
		string readID2;
		for(int i = 0 ;i < strlen(delline.c_str());i++)
		{
			
			if(delline[i]==' ')
			{
				flag = 1;
			}
			if(flag== 0)
			{
				readID1+=delline[i];
			}
			if(flag==1)
			{
				readID2+=delline[i];
			}
		}
		if(delpairlist[atoi(readID1.c_str())].num<100)
		{
			delpair_node * delnode1=new (delpair_node);
			delnode1->readID=atoi(readID2.c_str());
			delnode1->next=delpairlist[atoi(readID1.c_str())].next;
			delpairlist[atoi(readID1.c_str())].next=delnode1;
			delpairlist[atoi(readID1.c_str())].num++;
		}
		
		if(delpairlist[atoi(readID2.c_str())].num<100)
		{
			delpair_node * delnode2=new (delpair_node);
			delnode2->readID=atoi(readID1.c_str());
			delnode2->next=delpairlist[atoi(readID2.c_str())].next;
			delpairlist[atoi(readID2.c_str())].next=delnode2;
			delpairlist[atoi(readID2.c_str())].num++;
		}
	}
	aFile.close();
}

void breakpoint()
{}


ofstream fout;
void writenewm4()
{
	for(int j=0;j<orim4list.size();j++)
	{
		int flag=0;
		string readID1;
		string readID2;
		for(int i = 0 ;i < strlen(orim4list[j].c_str());i++)
		{
			
			if(orim4list[j][i]=='\t'&&flag==0)
			{
				flag = 1;
				continue;
			}
			if(flag== 0)
			{
				readID1+=orim4list[j][i];
			}
			if(flag==1)
			{
				readID2+=orim4list[j][i];
			}
			if(orim4list[j][i]=='\t'&&flag==1)
			{
				break;
			}
		}
		int read1=0;
		int read2=0;
		read1= atoi(readID1.c_str());
		read2= atoi(readID2.c_str());

		int  flag_del=0;
		delpair_node *node;
		node = delpairlist[read1].next;
		while(node!=NULL)
		{
			if(node->readID == read2)
			{
				flag_del=1;
				break;
			}
			node =node->next;
		}
		if(flag_del==0)
		{
			fout<<orim4list[j].c_str()<<endl;
		}
	}
}

int main(int argc,char* argv[])
{

	string m4file="";
	m4file=argv[1];
	const char* filename1;
	//filename1=m4file.c_str();
	//f_orim4=fopen(filename1,"r");
	
	string readfile="";
	readfile=argv[3];
	//const char* filename3;
	//filename3=readfile.c_str();
	//f_read=fopen(filename3,"r");
	
	init_global_variables(readfile);
//	cout<<"init_global_variables(readfile);"<<endl;
	string delreadsfile="";
	delreadsfile=argv[2];
	const char* filename2;
	filename2=delreadsfile.c_str();
	f_del=fopen(filename2,"r");
	
	readdelfile(readfile);
//	cout<<"readdelfile();"<<endl;
	readorim4file(m4file);
//	cout<<"readorim4file();"<<endl;
	//deleteedge();
	//cout<<"deleteedge();"<<endl;
	string newm4file="";
	newm4file=argv[4];
	const char* filename4;
	filename4=newm4file.c_str();
	
	fout.open(filename4,ios::trunc);
	writenewm4();
//	cout<<"writenewm4();"<<endl;

	return 0;
}
