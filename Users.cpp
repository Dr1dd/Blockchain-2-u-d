#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <chrono>
#include <random>
#include <cmath>
#include "blockchain.h"

static std::random_device rd;
static std::mt19937 gen(rd());

User GenerateUser(int i){
    std::uniform_int_distribution<std::mt19937::result_type> distribution(100,1000000);
    User myUser;
    myUser.name = "Vardas:" + std::to_string(i);
    myUser.balance = distribution(gen);

    const std::string VALID_CHARS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,./';[])(_-`=+-*";
    std::uniform_int_distribution<> dis(0,VALID_CHARS.size() - 1);
    std::ostringstream oss;
    for (std::size_t i = 0; i < 64; ++i) {
        oss << VALID_CHARS[dis(gen)];
    }
    string HashedKey;
    HashedKey = oss.str();
    //std::cout << HashedKey << " ";
    oss.clear();
    oss.str("");
    string final;
    final =Hashish(HashedKey);
   // std::cout << final << std::endl;

    myUser.public_key = HashedKey;
    return myUser;
}
Transactions GenerateTransactions(std::vector<User> userVector, int i){
    std::uniform_int_distribution<std::mt19937::result_type> distribution(0,999);
    std::uniform_int_distribution<std::mt19937::result_type> dist(1,100000);
    Transactions myTransaction;
    double coin = 0;
        coin = dist(gen);
        string person;
         double randomsk =distribution(gen);
        while(coin > userVector[randomsk].balance) coin = dist(gen);


                myTransaction.setSender(userVector[randomsk].public_key);
                myTransaction.setReceiver(userVector[distribution(gen)].public_key);
                myTransaction.setValue(coin);
                myTransaction.setDate(std::time(nullptr));

                string TransactionHash;
                TransactionHash = myTransaction.getSender() + myTransaction.getReceiver()+ std::to_string(myTransaction.getValue()) + std::to_string(myTransaction.getDate());
                Hashish(TransactionHash);
                myTransaction.setHash(TransactionHash);
       // std::cout << myTransaction.getSender() << " " << myTransaction.getReceiver() << " " << myTransaction.getValue() << std::endl;
    return myTransaction;
}
void MainFunction(){
    std::vector<User> userVector;
    User Person;
    userVector.reserve(1000);
    for(int i = 0; i < 1000; i++){
        Person = GenerateUser(i);
        userVector.push_back(Person);
    }
    Transactions myTransaction;
    std::vector<Transactions> myTransactionsVector;
    for(int j = 0; j< 10000; j++){

        myTransaction = GenerateTransactions(userVector, j);
        myTransactionsVector.push_back(myTransaction);

    }
    std::vector<Transactions> myTransactionsVectorCopy(myTransactionsVector);
    std::vector<string> myTransactionsHashes;
    for(int k = 0; k < myTransactionsVectorCopy.size(); k++){
        myTransactionsHashes.push_back(myTransactionsVectorCopy[k].getHash());
    }
    std::vector<string> MerkleHash;
    MerkleHash = MerkleTree(myTransactionsHashes);
    std::cout << MerkleHash[0] << std::endl;
 /*   for(int j = 0; j < 1000; j++){
        for(int j1 =0;j1<1000;j1++){
          if(userVector[j].public_key == userVector[j1].public_key && j != j1)  std::cout << "sutampa " << j << " " << j1 << std::endl;
        }
    }*/

}
std::vector<string> MerkleTree(std::vector<string> myTransaction){
    string result;
    std::vector<string> TransactionTemp;
    int temp;
    for(int i = 0; i < myTransaction.size()-1; i+=2){
        if(myTransaction.size()%2==0) {
            result = myTransaction[i] + myTransaction[i+1];
            Hashish(result);
            TransactionTemp.push_back(result);
        }
        else{
            if(i == myTransaction.size()-1) result = myTransaction[i];
            else  result = myTransaction[i] + myTransaction[i+1];
            Hashish(result);
            TransactionTemp.push_back(result);
        }
    }
    temp = myTransaction.size()/2;
    if(TransactionTemp.size() == 1) return TransactionTemp;
    MerkleTree(TransactionTemp);

}