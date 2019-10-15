#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include<vector>
using std::string;

class User {
    public:
        string name;
        string public_key;
        int balance;
};
class Transactions{
    public:


};
string Hashish(string &);
User GenerateUser(int i);
void MainFunction();
string ToHex(const string &, bool);
string valueCheck(int &, string &, string , int);
string Compress(string &, string, int);
#endif