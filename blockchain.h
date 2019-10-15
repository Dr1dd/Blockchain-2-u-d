#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include<vector>
#include <ctime>
using std::string;

class User {
    public:
        string name;
        string public_key;
        double balance;
};
class Transactions{
    public:
        string _sender;
        string _receiver;
        double _value;
        std::time_t _date;

        Transactions();
        Transactions(string sender, string receiver, double value){
            _sender = sender;
            _receiver= receiver;
            _value = value;
            _date = std::time(nullptr);

        }


};
class Block{
    string _previousBlock;
    std::time_t Timestamp;
    string version = "0.1";
    string merkelHash;
    unsigned int Nonce;
    double difficultyTarget;

    std::vector<Transactions> TransactionBlock;

};

string Hashish(string &);
User GenerateUser(int i);
void MainFunction();
string ToHex(const string &, bool);
string valueCheck(int &, string &, string , int);
string Compress(string &, string, int);
#endif