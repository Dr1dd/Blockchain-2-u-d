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
        string _hash;
        string _sender;
        int _sendersId;
        string _receiver;
        double _value;
        std::time_t _date;

    public:

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
        void setHash(string hash){
            _hash = hash;
        }
        void setSender(string a){
            _sender = a;
        }
        void setSendersID(int a){
            _sendersId = a;
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
        int getSendersID(){
            return _sendersId;
        }
        double getValue(){
            return _value;
        }
        const string &getHash() const {
            return _hash;
        }
    const time_t &getDate() const {
        return _date;
    }
};
class Block{
private:
    string previousBlock;
    string currentBlock;
    std::time_t Timestamp;
    string version = "0.1";
    string merkleHash;
    unsigned int Nonce;
    string difficultyTarget;

    std::vector<Transactions> TransactionBlock;
public:
    const string &getPreviousBlock() const;

    void setPreviousBlock(const string &previousBlock);

    const string &getCurrentBlock() const;

    void setCurrentBlock(const string &currentBlock);

    time_t getTimestamp() const;

    void setTimestamp(time_t Timestamp);

    const string &getVersion() const;

    void setVersion(const string &version);

    const string &getMerkleHash() const;

    void setMerkleHash(const string &merkelHash);

    unsigned int getNonce() const;

    void setNonce(unsigned int Nonce);

    string getDifficultyTarget() const;

    void setDifficultyTarget(string difficultyTarget);

    const std::vector<Transactions> &getTransactionBlock() const;

    void setTransactionBlock(const std::vector<Transactions> &TransactionBlock);


};

string Hashish(string &);
User GenerateUser(int i);
Transactions GenerateTransactions(std::vector<User>);
std::vector<string> MerkleTree(std::vector<string>);
Block newBlock(std::vector<Block> , std::vector<Transactions>, std::vector<User>, std::vector<int> &, int & , int);
void MainFunction();
string ToHex(const string &, bool);
string valueCheck(int &, string &, string , int);
string Compress(string &, string, int);
void blockBodyGeneration(int , std::vector<Transactions> , std::vector<User> , std::vector<Transactions> &, std::vector<int> &);
Block mineBlock(Block, int, int );
#endif