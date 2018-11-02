#include<iostream>
#include<stdio.h>
#include<time.h>
#include<fstream>
#include<string>
#include<math.h>
#include<cstdlib>
#include"string.h"
#include<malloc.h>
#include<stdlib.h>
#include <sys/stat.h>
#include<sys/types.h>

using namespace std;

string BEFORE="";
string AFTER="";
string Cluster="";
char **sample = NULL;
char **readname=NULL;
int *length=NULL;
int *name_length=NULL;
int read_count=0;
char **TmpReadName=NULL;
int *TmpReadNameLength=NULL;
int TmpReadName_count=0;
int parseargs(int argc, char * argv[])
{
	BEFORE=argv[1];
	AFTER=argv[2];
	//Cluster=argv[3];
}

void ReadSample()
{
	int count=0;
	char ch;
	FILE * f_fp;
	int i=0,j=0;
	int flag=0;
	string Qname="";
	string Tname="";
	const char* filename;
	filename=BEFORE.c_str();
	f_fp=fopen(filename,"r");
	const char* filename1;
	filename1=AFTER.c_str();
	ofstream fout;
	fout.open(filename1,ios::trunc);
	if(f_fp==NULL)
	{
		perror("open");
	}
	ch=fgetc(f_fp);

	while(!feof(f_fp))
	{	
		if(ch==-1)
		{
			break;
		}
		if(flag==0)
		{
			ch=fgetc(f_fp);
			flag=1;
			fout<<">"<<count<<endl;
			//cout<<count<<endl;
			while(!feof(f_fp))
			{
				ch=fgetc(f_fp);

				if(ch=='\n')
				{
					break;
				}
			}
			count++;
		}
		if(flag==1)
		{
			while(!feof(f_fp))
			{
				if(ch!='\n')
				{
					fout<<ch;
				}
				
				ch=fgetc(f_fp);
				if(ch=='>')
				{
					fout<<endl;
					flag=0;
					break;
				}
			}
		}
		
	}	
	fclose(f_fp);
}


int main(int argc,char * argv[])
{
	parseargs(argc,argv);
	ReadSample();
	return 0;
}