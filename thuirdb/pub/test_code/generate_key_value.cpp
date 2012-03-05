#include <string>
#include "time.h"
#include <stdlib.h>
#include <iostream>
using namespace std;

string random_str(int length) {
    static string charset = "abcdefghijklmnopqrstuvwxyz0123456789";
    string result;
    result.resize(length);

    for (int i = 0; i < length; i++)
        result[i] = charset[rand() % charset.length()];

    return result;
}

string random_str2(int length) {
    static string charset = "123456789";
    string result;
    result.resize(length);

    for (int i = 0; i < length; i++)
        result[i] = charset[rand() % charset.length()];

    return result;
}
int main(int argc,char* argv[])
{
	string random_key;
	string random_value;
	string random_value2;

	srand(time(NULL));	
	int cnt = atoi(argv[1]);
	
	cout<<"thuirdb"<<"\t"<<"tsinghua university"<<endl;
	for(int i = 0;i<cnt-1;++i)
	{
		random_key = random_str(8+rand()%8);
		random_value2 = random_str(2+rand()%2);
		cout<<random_key<<"\t"<<random_value2<<endl;
		
	}
	return 0;
}
