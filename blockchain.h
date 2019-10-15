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
        string getPublicKey(){
            return public_key;
        }
};
class Transactions{
    private:
        string _sender;
        string _receiver;
        double _value;

    public:
        std::time_t _date;

           Transactions() {
               _sender = "";
               _receiver ="";
               _value = 0;
               _date = std::time(0);

           }

           ~Transactions(){}
        Transactions(string sender, string receiver, double value) {
            _sender = sender;
            _receiver = receiver;
            _value = value;
            _date = std::time(nullptr);

        }
        void setSender(string a){
            _sender = a;
        }
        void setReceiver(string b){
            _receiver = b;
        }
        void setValue(double c){
            _value = c;
        }
        void setDate(std::time_t date){
            _date = date;
        }
        string getSender(){
            return _sender;
        }
        string getReceiver(){
            return _receiver;
        }
        double getValue(){
            return _value;
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
Transactions GenerateTransactions(std::vector<User> , int);
void MainFunction();
string ToHex(const string &, bool);
string valueCheck(int &, string &, string , int);
string Compress(string &, string, int);
#endif