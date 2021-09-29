#include <iostream>
using namespace std;
int main()
{
    int num;
    cout<<"enter a number:";
    cin>>num;
    int Flag;
    for(int i=num-1;i>=2;i--)
    {
        Flag=0;
        for(int j=2;j<=i/2;j++)
        {
            if(i%j==0){
                Flag=1;
                break;
            }
        }
        if(Flag==0)
        {
            cout<<i;
            break;
        }    
    }
    return 0;
}
