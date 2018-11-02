#include<stdio.h>
#include<iostream>
#include<string>
#include<fstream>
#include<stdlib.h>

using namespace std;

int main(int argc,char* argv[])
{
	int count_corrected=0;
	
	count_corrected=(atoi(argv[1]))/2;
	
	FILE* f_filted;
	string filename1="0";
	filename1=argv[2];
	const char* filename3;
	filename3=filename1.c_str();
	f_filted=fopen(filename3,"r");

	ofstream fout;
	string filename4="0";
	filename4=argv[3];
	const char* filename5;
	filename5=filename4.c_str();
	fout.open(filename5,ios::trunc);

	int flag=0;
	
	
	
	char ch;
	
	
	
	
int read_ID=0;
string str_ID="";
	ch=fgetc(f_filted);
while(!feof(f_filted))
	{
		if(ch=='>')
		{
			flag=1;
			while(!feof(f_filted))
			{
				if(ch=='_')
				{
					break;
				}
				if(ch!='>')
				{
					str_ID+=ch;
				}
				ch=fgetc(f_filted);
			}
		}
		if(ch=='_'&&flag==1)
		{
			flag=0;
			read_ID=atoi(str_ID.c_str());
			str_ID.clear();
		}
		
		
		if(read_ID>=count_corrected)
		{
			fout<<">"<<read_ID-count_corrected;
			int flagn=0;

			while(!feof(f_filted))
			{
				if(ch=='\n'&&flagn==1)
				{
					fout<<ch;
					break;
				}
				if(ch=='\n'&&flagn==0)
				{
					flagn=1;
				}
				fout<<ch;
				ch=fgetc(f_filted);
			}
		}
		ch=fgetc(f_filted);
	}




	return 0;
}
