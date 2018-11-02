#include<stdio.h>
#include<iostream>
#include<string>
#include <algorithm>
#include<fstream>
#include<cstring>
#include<cmath>
#include<queue>
#include<vector>
#include<map>
using namespace std;
#define pi acos(-1.0)
#define ls o<<1
#define rs o<<1|1
//define MS(x,y) memset(x,y,sizeof(x));
//<span style="font-family: Arial, Helvetica, sans-serif;">
#define MAXN 100000000
//const int MAXN = 101;
const double eps = 1e-8;

int S ;

int N, M;
/*
int ** cliques = (int **)malloc(sizeof(int *) * MAXN);

int ** g = (int **)malloc(sizeof(int *) * MAXN);
int ** all = (int **)malloc(sizeof(int *) * MAXN);
int ** some = (int **)malloc(sizeof(int *) * MAXN);
int ** none = (int **)malloc(sizeof(int *) * MAXN);
*/
/*
void MS1(int **matric)
{
	for(int i=0;i<MAXN;i++)
	{
		for(int j=0;j<MAXN;j++)
		{
			matric[i][j]=0;
		}
	}
}
void MS2(int *matric)
{
	for(int i=0;i<MAXN;i++)
	{
		
			matric[i]=0;
		
	}
}
void mallocdata(int ** matric)
{
	for(int i=0;i<MAXN;i++)
	{
		matric[i]= (int *)malloc(sizeof(int )*MAXN);
	}
}*/
/*
void freedata(int ** matric)
{
	for(int i=0;i<MAXN;i++)
	{
		free(matric[i]);
	}
}
*/
typedef struct adjacency_node
{
	int index;
	int value;
	adjacency_node* next;
}adjacency_node;
vector<adjacency_node*> g_list;
vector<adjacency_node*> all_list;
vector<adjacency_node*> some_list;
vector<adjacency_node*> none_list;
vector<adjacency_node*> cliques_list;
/*
struct adjacency_node g_list[MAXN];
struct adjacency_node all_list[MAXN];
struct adjacency_node some_list[MAXN];
struct adjacency_node none_list[MAXN];
struct adjacency_node cliques_list[MAXN];
*/

string clique="";
string graph="";
string edge="";
FILE * f_fp;//输入图
ofstream fout1,fout2;
string readname="";
int readnametonum[MAXN];
int nodenum[MAXN]={0};
int readnum=0;
int writenum=0;
int cliquenum=0;
int writencliquenum=0;
int g_list_index=0;
int x=0,y=0;
//all为已取顶点集，some为未处理顶点集，none为不取的顶点集
//我们求最大团顶点数时只要some，要求记录路径时要all和some，这里求极大团数量，需要all、some、none

typedef struct hashreads
{
	int flag;
	int g_list_index;
}hashreads;
hashreads readslist[MAXN];

void init_readslist()
{
	for(int i=0;i<MAXN;i++)
	{
		readslist[i].flag=0;
		readslist[i].g_list_index=0;
	}
}

int alignment[MAXN][2];
int countalignment=0;
void insertHash()
{
	char ch;
	int flag_name=0,flag_find=0;
	ch=fgetc(f_fp);
	while(!feof(f_fp))
	{	
		if(ch==-1)
		{
			break;
		}
		if(ch=='\t'&&flag_name==0)
		{
			flag_name=1;
			x=atoi(readname.c_str())+1;
			if(x>=readnum)
			{
				readnum=x;
			}
			
			//readnametonum[x]=readname.c_str();
			alignment[countalignment][0]=x;
			readslist[x].flag=1;
			readname.clear();
		}
		else if(ch=='\t'&&flag_name==1)
		{
			flag_name=2;
			y=atoi(readname.c_str())+1;
			if(y>=readnum)
			{
				readnum=y;
			}
			
			//readnametonum[y]=readname.c_str();
			alignment[countalignment][1]=y;
			readslist[y].flag=1;
			readname.clear();
		}
		else if(ch=='\n'&&flag_name==2)
		{
			if(countalignment%1000==0)
			{
				//cout<<"countalignment = "<<countalignment<<endl;
			}
			countalignment++;
			readname.clear();
			flag_name=0;
		}
		else
		{
			readname+=ch;
		}
		ch=fgetc(f_fp);
	}
	countalignment++;
	fclose(f_fp);
	
	for(int i=0;i<readnum+1;i++)
	{
		if(readslist[i].flag==1)
		{
			g_list_index++;
			readslist[i].g_list_index=g_list_index;
			readnametonum[g_list_index]=i-1;
		}
	}
}


void breakpoint()
{

}

void dfs(int d, int an, int sn, int nn,vector<adjacency_node*>&cliques_list,vector<adjacency_node*> &all_list,vector<adjacency_node*> &some_list,vector<adjacency_node*>& none_list,vector<adjacency_node*>& g_list)
{
	//cout<<"d = "<<d<<"\tan = "<<an<<"\tsn = "<<sn<<"\tnn = "<<nn<<endl;
	//if(cliquenum >g_list_index*0.1)
	if(cliquenum >9000)

	{
		return ;
	}
	if(sn == 0 && nn == 0) //sn==0搜索到终点，只有nn==0时，才是一个极大团
	{
		for(int i=0;i<an;i++)
		{
			int alldi=0;
			adjacency_node * newanode;
			newanode=all_list[d];
			while(newanode!=NULL)
			{
				if(newanode->index==i)
				{
					alldi=newanode->value;
					break;
				}
				else
				{
					newanode=newanode->next;
				}
			}
			adjacency_node * newcnode;
			newcnode=cliques_list[S];
			int flag_find=0;
			while(newcnode!=NULL)
			{
				if(newcnode->index==i)
				{
					flag_find=1;
					break;
				}
				else
				{
					newcnode=newcnode->next;
				}
			}
			if(flag_find==1)
			{
				newcnode->value=alldi;
				flag_find=0;
			}
			else
			{
				adjacency_node * newcnode=new(adjacency_node);
				newcnode->index=i;
				newcnode->value=alldi;
				newcnode->next=cliques_list[S];
				cliques_list[S]=newcnode;
			}
			//cliques[S][i]=all[d][i];
		}
		nodenum[S]=an;
		if(an>=writenum)
		{
			cliquenum++;
		}
		/*
		if(an>=writenum)
		{
			for(int i=0;i<an;i++)
			{
				//cout<<all[d][i]<<" ";
			}
			//cout<<endl;
		}
		*/
		++ S;
		//cout<<"S = "<<S<<" cliquenum = "<<cliquenum<<endl;
		
	}
	int u=0;
	adjacency_node * snode;
	snode=some_list[d];
	while(snode!=NULL)
	{
		if(snode->index==0)
		{
			u=snode->value;
			break;
		}
		else
		{
			snode=snode->next;
		}
	}
	
	//u=some_list[d]->value;
	//int u = some[d][0];//pivot vertex
    for(int i = 0; i < sn; i ++)
	{
		adjacency_node * gnode;
		gnode=g_list[u];
		int v=0;
		adjacency_node * inode;
		inode=some_list[d];
		while(inode!=NULL)
		{
			if(inode->index==i)
			{
				v=inode->value;
				break;
			}
			else
			{
				inode=inode->next;
			}
		}
        //int v = some[d][i];
		int guv=0;
		
		while(gnode!=NULL)
		{
			if(gnode->index==v)
			{
				guv=gnode->value;
				break;
			}
			else
			{
				gnode=gnode->next;
			}
		}
        if(guv) continue;
        int tsn = 0, tnn = 0;

		
		for(int j = 0; j < an; j ++)
		{
			adjacency_node * allnode;
			allnode=all_list[d];
			int tmpalldj=0;
			int flag_find=0;
			while(allnode!=NULL)
			{
				if(allnode->index==j)
				{
					tmpalldj=allnode->value;
					break;
				}
				else
				{
					allnode=allnode->next;
				}
			}
			allnode=all_list[d+1];
			while(allnode!=NULL)
			{
				if(allnode->index==j)
				{
					flag_find=1;
					break;
				}
				else
				{
					allnode=allnode->next;
				}
			}
			if(flag_find==1)
			{
				allnode->value=tmpalldj;
				flag_find=0;
			}
			else
			{
				adjacency_node * newanode=new(adjacency_node);
				newanode->index=j;
				newanode->value=tmpalldj;
				newanode->next=all_list[d+1];
				all_list[d+1]=newanode;
			}
			//all[d + 1][j] = all[d][j];
		}
		adjacency_node * allnode;
		int flag_find=0;
		allnode=all_list[d+1];
		while(allnode!=NULL)
		{
			if(allnode->index==an)
			{
				flag_find=1;
				break;
			}
			else
			{
				allnode=allnode->next;
			}
		}
		if(flag_find==1)
		{
			allnode->value=v;
			flag_find=0;
		}
		else
		{
			adjacency_node * newanode=new(adjacency_node);
			newanode->index=an;
			newanode->value=v;
			newanode->next=all_list[d+1];
			all_list[d+1]=newanode;
		}

		
        //all[d + 1][an] = v;

        for(int j = 0; j < sn; j ++)
		{
			int gvsomedj=0;
			int tmpsomedj=0;
			int flag_find=0;
			adjacency_node *somedj;
			somedj=some_list[d];
			while(somedj!=NULL)
			{
				if(somedj->index==j)
				{
					//gvsomedj=somedj->value;
					break;
				}
				else
				{
					somedj=somedj->next;
				}
			}
			adjacency_node* gnode;
			gnode=g_list[v];
			while(gnode!=NULL)
			{
				if(gnode->index==somedj->value)
				{
					flag_find=1;
					break;

				}
				else
				{
					gnode=gnode->next;
				}
			}
			if(flag_find==1)
			{
				flag_find=0;
				gvsomedj=gnode->value;
			}
			if(gvsomedj)
				//if(g[v][some[d][j]])
			{
				int somedj=0;
				adjacency_node * snode;
				snode=some_list[d];
				while(snode!=NULL)
				{
					if(snode->index==j)
					{
						somedj=snode->value;
						break;
					}
					else
					{
						snode=snode->next;
					}
				}
				snode=some_list[d+1];
				while(snode!=NULL)
				{
					if(snode->index==tsn)
					{
						flag_find=1;
						break;
					}
					else
					{
						snode=snode->next;
					}
				}
				if(flag_find==1)
				{
					snode->value=somedj;
					tsn++;
				}
				else
				{
					adjacency_node * snode1=new(adjacency_node);
					snode1->index=tsn;
					tsn++;
					snode1->value=somedj;
					snode1->next=some_list[d+1];
					some_list[d+1]=snode1;
				}
				
				//some[d + 1][tsn ++] = some[d][j];
			}
		}

        for(int j = 0; j < nn; j ++)
		{
			int gvnonedj=0;
			int flag_find=0;
			adjacency_node *nonedj;
			nonedj=none_list[d];
			while(nonedj!=NULL)
			{
				if(nonedj->index==j)
				{
					//gvnonedj=nonedj->value;
					break;
				}
				else
				{
					nonedj=nonedj->next;
				}
			}
			adjacency_node* gnode;
			gnode=g_list[v];
			while(gnode!=NULL)
			{
				if(gnode->index==nonedj->value)
				{
					flag_find=1;
					break;

				}
				else
				{
					gnode=gnode->next;
				}
			}
			if(flag_find==1)
			{
				flag_find=0;
				gvnonedj=gnode->value;
			}
			if(gvnonedj)
			//if(g[v][none[d][j]]) 
			{
				int nonedj=0;
				adjacency_node * nnode;
				nnode=none_list[d];
				while(nnode!=NULL)
				{
					if(nnode->index==j)
					{
						nonedj=nnode->value;
						break;
					}
					else
					{
						nnode=nnode->next;
					}
				}
				nnode=none_list[d+1];
				while(nnode!=NULL)
				{
					if(nnode->index==tnn)
					{
						flag_find=1;
						break;
					}
					else
					{
						nnode=nnode->next;
					}
				}
				if(flag_find==1)
				{
					flag_find=0;
					nnode->value=nonedj;
					tnn++;
				}
				else
				{
					adjacency_node * nnode1=new(adjacency_node);
					nnode1->index=tnn;
					tnn++;
					nnode1->value=nonedj;
					nnode1->next=none_list[d+1];
					none_list[d+1]=nnode1;
				}
				
				//none[d + 1][tnn ++] = none[d][j];
			}
		}


		dfs(d + 1, an + 1, tsn, tnn,cliques_list,all_list,some_list,none_list, g_list);
		if(cliquenum >g_list_index*0.1)
		{
			return ;
		}
		//把v从some取出，放入none


		adjacency_node * snode;
		 
		snode=some_list[d];
		while(snode!=NULL)
		{
			if(snode->index==i)
			{
				flag_find=1;
				break;
			}
			else
			{
				snode=snode->next;
			}
		}
		if(flag_find==1)
		{
			flag_find=0;
			snode->value=0;
		}
		else
		{
			adjacency_node * snode=new(adjacency_node);
			snode->index=i;
			snode->value=0;
			snode->next=some_list[d];
			some_list[d]=snode;
		}
		adjacency_node * nnode;
		nnode=none_list[d];
		while(nnode!=NULL)
		{
			if(nnode->index==nn)
			{
				flag_find=1;
				break;
			}
			else
			{
				nnode=nnode->next;
			}
		}
		if(flag_find==1)
		{
			flag_find=0;
			nnode->value=v;
		}
		else
		{
			adjacency_node * nnode=new(adjacency_node);
			nnode->index=nn;
			nnode->value=v;
			nnode->next=none_list[d];
			none_list[d]=nnode;
		}
		nn++;
        //some[d][i] = 0, none[d][nn ++] = v;
    }
}
void process(vector<adjacency_node*>& cliques_list,vector<adjacency_node*>& all_list,vector<adjacency_node*>& some_list,vector<adjacency_node*> &none_list,vector<adjacency_node*> &g_list)
{
	
	S = 0;//S是最大团的个数
	some_list[0]->index=0;
	some_list[0]->value=1;
	some_list[0]->next=NULL;

	for(int i = 1; i < N; i++)//初始化S集为全部点 N为点的个数
	{
		adjacency_node *a=new(adjacency_node);
		a->index=i;
		a->value=i+1;
		a->next=some_list[0];
		some_list[0]=a;

		//some[0][i] = i + 1;
	}
	dfs(0, 0, N, 0,cliques_list,all_list,some_list,none_list, g_list);
}
void writefile(vector<adjacency_node*>& cliques_list)
{
	for(int i=0;i<S;i++)
	{
		if(nodenum[i]>=writenum)
		{
			//cout<<"writing clique "<<writencliquenum++<<"...\n";
			adjacency_node * cliquesnode;
			cliquesnode=cliques_list[i];
			while(cliquesnode!=NULL)
			{
				fout1<<readnametonum[cliquesnode->value]<<" ";
				cliquesnode=cliquesnode->next;
			}
			/*
			for(int j=0;j<nodenum[i];j++)
			{
				fout1<<readnametonum[cliques[i][j]]<<" ";
			}*/
			fout1<<endl;
			
		}
		if(nodenum[i]<writenum&&nodenum[i]>1)//writenum=3
		{
			//cout<<"writing edge "<<i<<"...\n";
			adjacency_node * cliquesnode;
			cliquesnode=cliques_list[i];
			while(cliquesnode!=NULL)
			{
				fout2<<readnametonum[cliquesnode->value]<<" ";
				cliquesnode=cliquesnode->next;
			}
			fout2<<endl;
			/*
			for(int j=0;j<nodenum[i]-1;j++)
			{
				for(int k=j+1;k<nodenum[i];k++)
				{
					fout2<<readnametonum[cliques[i][j]]<<" ";
					fout2<<readnametonum[cliques[i][k]]<<" ";
					fout2<<endl;
				}
			}
			*/

		}

	}
	//cout<<"S = "<<S<<" cliquenum = "<<cliquenum<<endl;
		
}
int parseargs(int argc, char * argv[])
{
	
	graph=argv[1];
	const char* filename1;
	filename1=graph.c_str();
	f_fp=fopen(filename1,"r");
	if(f_fp==NULL)
	{
		perror("open");
	}
	clique=argv[2];//输出文件
	const char* filename2;
	filename2=clique.c_str();
	fout1.open(filename2,ios::trunc);

	edge=argv[3];//输出文件
	const char* filename3;
	filename3=edge.c_str();
	fout2.open(filename3,ios::trunc);
	writenum=atoi(argv[4]);
}
void get_graph(vector<adjacency_node*>& g_list)
{
	for(int i=0;i<g_list_index+1;i++)
	{
		adjacency_node *a=new(adjacency_node);
		a->index=i;
		a->value=0;
		a->next=NULL;
		g_list.push_back(a);
	}
	char ch;
	int flag_name=0,flag_find=0;

	for(int i=0;i<countalignment;i++)
	{
		if(alignment[i][0]!=alignment[i][1])
		{
			adjacency_node * newnode=new(adjacency_node);
			newnode->index=readslist[alignment[i][1]].g_list_index;
			newnode->value=1;
			newnode->next=g_list[readslist[alignment[i][0]].g_list_index]->next;
			g_list[readslist[alignment[i][0]].g_list_index]->next=newnode;


			adjacency_node * newnode2=new(adjacency_node);
			newnode2->index=readslist[alignment[i][0]].g_list_index;
			newnode2->value=1;
			newnode2->next=g_list[readslist[alignment[i][1]].g_list_index]->next;
			g_list[readslist[alignment[i][1]].g_list_index]->next=newnode2;
		}
	}


	/*

	ch=fgetc(f_fp);
	while(!feof(f_fp))
	{	
		if(ch==-1)
		{
			break;
		}
		if(ch=='\t'&&flag_name==0)
		{
			flag_name=1;
			x=atoi(readname.c_str())+1;
			readnametonum[x]=readname.c_str();
			if(readnum<x)
			{
				readnum=x;
			}
			readname.clear();*/
			/*
			for(int i=0;i<readnum;i++)
			{
				if(readnametonum[i+1].compare(readname.c_str())==0)//这条read已经出现，记为i
				{
					x=i+1;
					readname.clear();
					flag_find=1;
					break;
				}
			}
			if(flag_find==0)//加入新read
			{
				
				readnametonum[readnum+1]=readname.c_str();
				readnum++;
				readname.clear();
				x=readnum;
				adjacency_node *a=new(adjacency_node);
				a->index=x;
				a->value=0;
				a->next=NULL;
				g_list.push_back(a);
			}
			else if(flag_find==1)
			{
				flag_find=0;
			}
			*/
		//}
		//else if(ch=='\t'&&flag_name==1)
		//{
			/*
			for(int i=0;i<readnum;i++)
			{
				if(readnametonum[i+1].compare(readname.c_str())==0)//这条read已经出现，记为i
				{
					y=i+1;
					readname.clear();
					flag_find=1;
					break;
				}
			}
			if(flag_find==0)
			{
				
				readnametonum[readnum+1]=readname.c_str();
				readnum++;
				readname.clear();
				y=readnum;
				adjacency_node *a=new(adjacency_node);
				a->index=y;
				a->value=0;
				a->next=NULL;
				g_list.push_back(a);
			}
			else if(flag_find==1)
			{
				flag_find=0;
			}
			*//*
			y=atoi(readname.c_str())+1;
			readnametonum[y]=readname.c_str();
			if(readnum<y)
			{
				readnum=y;
			}
			readname.clear();
			if(x!=y)
			{*/
				/*
				adjacency_node * anodex;
				anodex=g_list[x];
				while(anodex!=NULL)
				{
					anodex=anodex->next;
				}
				*/
				/*
				adjacency_node * newnode=new(adjacency_node);
				newnode->index=y;
				newnode->value=1;
				newnode->next=g_list[x]->next;
				g_list[x]->next=newnode;

				adjacency_node * newnode2=new(adjacency_node);
				newnode2->index=x;
				newnode2->value=1;
				newnode2->next=g_list[y]->next;
				g_list[y]->next=newnode2;

				//g[x][y]=1;
				//g[y][x]=1;

				if(countalignment%1000==0)
				{
					//cout<<"count = "<<countalignment<<endl;
				}
			}
			flag_name=2;
		}
		else if(ch=='\n'&&flag_name==2)
		{
			countalignment++;
			
			readname.clear();
			flag_name=0;
		}
		else
		{
			readname+=ch;
		}
		ch=fgetc(f_fp);
	}
	fclose(f_fp);
	/*
	/*
	for(int i=0;i<readnum;i++)
	{
		if(i%1000==0)
		{
			cout<<"i="<<i<<endl;
		}
		if(readnametonum[i].c_str()=="")
		{
			cout<<i<<" = NULL"<<endl;
		}
		for(int j=0;j<readnum;j++)
		{
			if(readnametonum[i].compare(readnametonum[j].c_str())==0&&i!=j)
			{
				cout<<"i = "<<i<<" j = "<<j<<"already exist!\n";
			}
		}
	}
	*/
}

void g_test()
{
	int count=0;
	for(int i=readnum ;i>0;i--)
	{
		if(g_list[i]->next==NULL)
		{
			g_list.erase(g_list.begin()+i);
			readnum--;
		}
		else
		{
			count++;
		}
		//cout<<i<<"\t"<<readnum<<"\t"<<count<<endl;
	}
	//cout<<count<<endl;
}

void init_g_list()
{
	for(int i=0;i<g_list_index;i++)
	{
		adjacency_node *anode=new(adjacency_node);
		anode->index=0;
		anode->value=0;
		anode->next=NULL;
		all_list.push_back(anode);

		adjacency_node *snode=new(adjacency_node);
		snode->index=0;
		snode->value=0;
		snode->next=NULL;
		some_list.push_back(snode);

		adjacency_node *nnode=new(adjacency_node);
		nnode->index=0;
		nnode->value=0;
		nnode->next=NULL;
		none_list.push_back(nnode);

		adjacency_node *cnode=new(adjacency_node);
		cnode->index=0;
		cnode->value=0;
		cnode->next=NULL;
		cliques_list.push_back(cnode);

	}
}
//int cliques[MAXN][MAXN];



int a[MAXN];

void init_list()
{
	for(int i=0;i<MAXN;i++)
	{
		adjacency_node *gnode=new(adjacency_node);
		gnode->index=0;
		gnode->value=0;
		gnode->next=NULL;
		g_list.push_back(gnode);
	}
}

int main(int argc, char * argv[])
{
	

	parseargs(argc,argv);
	init_readslist();
	insertHash();
	//init_list();
	get_graph(g_list);
	//g_test();
	/*
	for(int i=1;i<=readnum;i++)
	{
		adjacency_node* node;
		node=g_list[i];
		while(node!=NULL)
		{
			fout1<<"( "<<node->index<<" "<<node->value<<" "<<readnametonum[node->index]<<" ) ";
			node=node->next;
		}
		fout1<<endl;
	}
	*/
	N=g_list_index;
	init_g_list();
	process(cliques_list,all_list,some_list,none_list, g_list);
	writefile(cliques_list);
	
	return 0;
}

