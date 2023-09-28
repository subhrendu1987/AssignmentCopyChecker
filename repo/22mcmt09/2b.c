#include <stdio.h>
#include <stdlib.h>

//ip==INPUT ,cvrt==Convert ,op==OUTPUT
void cvrt_base(int ip_numb, int ip_base, int cvrt_base, FILE *op_file) 
{
    int converted_numb = 0;
    int multiplier = 1;

    // First we will convert it to decimal
    while (ip_numb > 0) 
    {
        int value = ip_numb % 10;
        converted_numb += value * multiplier;
        multiplier *= ip_base;
        ip_numb /= 10;
    }

    // after decimal conversion we will convert it to the base
    char converted_str[32];
    snprintf(converted_str, sizeof(converted_str), "%X", converted_numb); 
    
    fprintf(op_file, "%s\n", converted_str);
//Display the value
    
}

//Driver Code
int main()
{
    FILE *ip_file = fopen("ip_numbers.txt", "r");   //Open the Input File
    FILE *op_file = fopen("converted_numbers.txt", "w");  //Open the Output File

    if (ip_file == NULL || op_file == NULL) 
    {
        printf("Error: Unable to open the files.\n");  
        //Displaying the result
        //As input and output file==NULL so we won't be able to open the files
        return 1;
    }

    int ip_numb, ip_base, cvrt_base;

    while (fscanf(ip_file, "%d %d %d", &ip_numb, &ip_base, &cvrt_base) == 3)
    {
        cvrt_base(ip_numb, ip_base, cvrt_base, op_file);
    }

    fclose(ip_file);  //Closing the Input File
    fclose(op_file);  //Closing the Output File

    return 0;
}