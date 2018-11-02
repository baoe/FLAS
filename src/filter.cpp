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

string ALL="";

string twice="";
string allreads="";
string ADD="";
char **sample = NULL;
char **readname=NULL;
int *length=NULL;
int *name_length=NULL;
int read_count=0;
char **TmpReadName=NULL;
int *TmpReadNameLength=NULL;
int TmpReadName_count=0;

FILE * f_ALL;
FILE * f_once;

int i=0,j=0;
string Qname="";
string Tname="";
ofstream f_twice;
int once_num;
int parseargs(int argc, char * argv[])
{
	ALL=argv[1];

	once_num=(atoi(argv[2]))/2;
	twice=argv[3];
	
}

void read_allcorrectedreads()
{
	char ch ;
	ch=fgetc(f_ALL);
	int flag=0;
	char readname[100];
	int namelen=0;
	int flag_write=0;
	while(!feof(f_ALL))
	{	
		if(ch==-1)
		{
			break;
		}
		if(ch=='\n'&&flag==0)
		{
			flag=1;
			string str="";
			for(int i=1;i<namelen;i++)
			{
				if(readname[i]=='_')
				{
					break;
				}
				str+=readname[i];
			}
			
			if(atoi(str.c_str())>once_num)
			{
				flag_write=1;
			}
			else
			{
				namelen=0;
			}
			str.clear();
		}
		if(flag_write==1&&flag==1)
		{
			flag=0;
			flag_write=0;
			for(int i=0;i<namelen;i++)
			{
				f_twice<<readname[i];
			}
			f_twice<<endl;;
			namelen=0;
			while(!feof(f_ALL))
			{	
				ch=fgetc(f_ALL);
				f_twice<<ch;
				if(ch=='\n')
				{
					break;
				}
				
			}
		}

		else if(flag_write==0&&flag==1)
		{
			flag=0;
			while(!feof(f_ALL))
			{		
				ch=fgetc(f_ALL);
				if(ch=='\n')
				{
					break;
				}
				
			}
		}
		else if(flag_write==0&&flag==0)
		{
			readname[namelen]=ch;
			namelen++;
		}
		ch=fgetc(f_ALL);
		
	}
	fclose(f_ALL);
}

int main(int argc,char * argv[])
{
	parseargs(argc,argv);
	
	const char* filename;
	filename=ALL.c_str();
	f_ALL=fopen(filename,"r");

	
	const char* filename1;
	filename1=twice.c_str();
	f_twice.open(filename1,ios::trunc);

	read_allcorrectedreads();
	
	
	
	return 0;
}