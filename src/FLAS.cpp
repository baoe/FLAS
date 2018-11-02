#include"FLAS.h"
#include"combinecliques2.h"
string cliquefile;
string edgefile;
string printreads;
string printreads2;

string printcliques;
string printmatrix;
string printm4info;
string printm4info2;
string C0str;
string readfile;
string rawreadfile;
string m4file ;
string realireads;

string alignment;
string delete_edges_file;
string delete_edges_file2;

double f1=0.5;
double f2=0.5;
double d1=0.05;
double d2=0.2;
int X=1;
int Y=500;
int C0=100;
int L=10000;
int workflow=0;
int once_num=0;
int parseargs(int argc, char * argv[])
{
//	cout<<"begin parse"<<endl;
	if(argc<2)
	{
		cout<<"Invalid parameters!"<<endl;
		return -1;
	}
	else
	{
		workflow=atoi(argv[1]);
		if(workflow==2)
		{
			once_num=(atoi(argv[2]))/2;
		}
		for(int i=workflow+1;i<argc;i++)
		{
			if(argv[i][0]=='-')
			{
				if(argv[i][1]=='C')//workrflow1 2
                                {
                                        C0str=argv[++i];
                                        continue;
                                }
				if(argv[i][1]=='c')//workrflow1 2
				{
					cliquefile=argv[++i];
					continue;
				}
				if(argv[i][1]=='e')//workrflow1 2
				{
					edgefile=argv[++i];
					continue;
				}
				if(argv[i][1]=='r')//workrflow1 2
				{
					readfile=argv[++i];
					continue;
				}
				if(argv[i][1]=='m')//workrflow1 2
				{
					m4file=argv[++i];
					continue;
				}
				if(argv[i][1]=='a')//workrflow1
				{
					realireads=argv[++i];
					continue;
				}
				if(argv[i][1]=='d'&&argv[i][2]=='e'&&argv[i][3]=='1')//workrflow1
				{
					delete_edges_file=argv[++i];
					continue;
				}
				if(argv[i][1]=='d'&&argv[i][2]=='e'&&argv[i][3]=='2')//workrflow2
				{
					delete_edges_file=argv[++i];
					continue;
				}
				if(argv[i][1]=='d'&&argv[i][2]=='e'&&argv[i][3]=='3')//workrflow2
				{
					delete_edges_file2=argv[++i];
					continue;
				}
				if(argv[i][1]=='w')//workrflow1 2
				{
					rawreadfile=argv[++i];
					continue;
				}
				if(argv[i][1]=='g')//workrflow1 2
				{
					alignment=argv[++i];
					continue;
				}



				if(argv[i][1]=='p'&&argv[i][2]=='c')
				{
					printcliques=argv[++i];
					continue;
				}
				if(argv[i][1]=='p'&&argv[i][2]=='r')
				{
					printreads=argv[++i];
					continue;
				}
				if(argv[i][1]=='p'&&argv[i][2]=='m')
				{
					printmatrix=argv[++i];
					continue;
				}
				if(argv[i][1]=='f'&&argv[i][2]=='1')
				{
					f1=atof(argv[++i]);
					continue;
				}
				if(argv[i][1]=='f'&&argv[i][2]=='2')
				{
					f2=atof(argv[++i]);
					continue;
				}
				if(argv[i][1]=='d'&&argv[i][2]=='1')
				{
					d1=atof(argv[++i]);
					continue;
				}
				if(argv[i][1]=='d'&&argv[i][2]=='2')
				{
					d2=atof(argv[++i]);
					continue;
				}
			}
		}
	}
}

int main(int argc,char * argv[])
{
//	cout<<"begin"<<endl;
//	cout<<"###########"<<endl;
	parseargs(argc,argv);
	
//	cout<<"finish parseargs"<<endl;
	combine();
//	cout<<"finish combine"<<endl;
	return 0;
}
