#include <iostream>
using namespace std;
int main()
{
    int arr[10];
    int num,i,n;
    cout<<"enter no.of elements in an array:";
    cin>>n;
    cout<<"enter array elements:";
    for(i=0;i<n;i++)
    {
        cout<<"";
        cin>>arr[i];
    }
    cout<<"enter element to search in an array:";
    cin>>num;
    for(i=0;i<n;i++)
    {
        if(arr[i]==num){
          cout<<"element found at:"<<i+1;
          break;
        }
    }
    if(i==n)
       cout<<"element not found";
    return 0;
}

