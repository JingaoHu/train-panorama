#include<iostream>
#include<cstdlib>
#include<cstring>
using namespace std;
int main()
{
	int a[3]={1,2,3};
	int *b=(int *)malloc(sizeof(int)*3);
	memcpy(b,a,3*sizeof(int));
	cout<<a[0]<<" "<<a[1]<<" "<<a[2]<<endl;
	return 0;
}
