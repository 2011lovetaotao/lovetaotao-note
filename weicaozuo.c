#include <stdio.h>
#include<stdlib.h>
void shier(int x)
{
    int i, j;
    for (i = 0; i < 32; i++) {
	if ((x >> (31 - i)) & 1)
	    break;
    };
    for (j = i; j < 32; j++) {
	if ((x >> (31 - j)) & 1)
	    printf("1");
	else
	    printf("0");
    };
    printf("\n");
}



int chenfa(int c, int d)
{
    int ax, ay, i, result = 0, flag = 0;
    if ((c >> 31) & 1)
	ax = ~(c - 1);
    else
	ax = c;
    if ((d >> 31) & 1)
	ay = ~(d - 1);
    else
	ay = d;
    if (((c >> 31) & 1) ^ ((d >> 31) & 1))
	flag = 1;
    for (i = 0; i < 31; i++)
	if (ay >> (30 - i) & 1)
	    result = result + (ax << (30 - i));
    if (flag)
	result = -result;
    return result;
}

void da(char *a)
{
    int i;
    char b[256] = { 0 };
    for (i = 0; a[i] != '\0'; i++) {
	if (a[i] >= 'a' && a[i] <= 'z')
	    b[i] = a[i] & 223;
	else if (a[i] >= 'A' && a[i] <= 'Z')
	    b[i] = a[i] | 32;
    }
    printf("%s", b);
}

void jiam(char *i)
{
    char c[256];
    int j;
    for (j = 0; j < 256; j++)
	i[j] = i[j] ^ c[j];
}

void jiemi(char *i)
{
    char c[256];
    int j;
    for (j = 0; j < 256; j++)
	i[j] = i[j] ^ c[j];
}

int main(int argc, const char *argv[])
{
    int n;
    while (1) {
	printf
	    ("1.decimal to binary\n2.x*y\n3.low or up exchange\n4.encryption\n5.Exit\nplease choose[1-5]: ");
	scanf("%d", &n);
	switch (n) {
	case 1:
	    {
		int x;
		printf("please input a number:");
		scanf("%d", &x);
        printf("decimal %d change to binary is:",x);
		shier(x);
       
	    } break;
	case 2:
	    {
		int j, k;
		printf("please input x y:");
		scanf("%d %d", &j, &k);
		printf("%d * %d = %d\n\n", j, k, chenfa(j, k));
	    } break;
	case 3:
	    {
		char a[256];
		printf("please input a zhifuchuan:");
		scanf("%s", a);
		da(a);
		printf("\n");
	    } break;
	case 4:
	    {
		char i[256];
		printf("please input your cipher:\n");
		scanf("%s", i);
		jiam(i);
		printf("%s\n", i);
		jiemi(i);
		printf("%s\n", i);
	    } break;
	default:
	    exit(0);
	}
    }
    return 0;
}
