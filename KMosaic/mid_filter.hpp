#include<cstring>
void swap(int &a,int &b)
{
	int temp;
	temp=a;
	a=b;
	b=temp;
	return ;
}

void bubble_sort(int a[],int len)
{
	for(int i=0;i<len;i++)
		for(int j=0;j<len-1-i;j++)
			if(a[j]>a[j+1])
				swap(a[j],a[j+1]);
	return ;
}

void mid_filter(int a[],int len,int filter_size)
{
	if(filter_size%2==0)
	{
		cout<<"filter size shoul be an odd number!"<<endl;
		return ;
	}
	int *a_cpy=(int*)malloc(sizeof(int)*len);
	memcpy(a_cpy,a,sizeof(int)*len);

	int *temp=(int*)malloc(sizeof(int)*filter_size);
	int i;
	for(i=(filter_size-1)/2;i<len-(filter_size-1)/2;i++)
	{
		memcpy(temp,a_cpy+i-(filter_size-1)/2,sizeof(int)*filter_size);		
		bubble_sort(temp,filter_size);
		a[i]=temp[(filter_size-1)/2];
	}
	return ;
}


