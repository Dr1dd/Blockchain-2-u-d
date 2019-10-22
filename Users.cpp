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
Transactions GenerateTransactions(std::vector<User> userVector){
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
    int tempTx = 0;
    while(tempTx <10000){
        myTransaction = GenerateTransactions(userVector);
       // if(myTransaction.getSender()-myTransaction.getValue() >=0)
        myTransactionsVector.push_back(myTransaction);
        tempTx++;

    }

    std::vector<Block> myBlockchain;
    int distMax = 9999;
    while(myTransactionsVector.size()>0){
        myBlockchain.push_back(newBlock(myBlockchain, myTransactionsVector, distMax));
        std::cout <<" help help help help" << std::endl;
   }


}
std::vector<string> MerkleTree(std::vector<string> myTransaction){
    string result;
    std::vector<string> TransactionTemp;
    //int temp;
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
    if(TransactionTemp.size() == 1) return TransactionTemp;
    MerkleTree(TransactionTemp);

}
Block newBlock(std::vector<Block> myBlockchain, std::vector<Transactions> myTransactionsVector, int & distMax){
    Block myBlock;
    string previousBlock;
    previousBlock.resize(64);

    if(myBlockchain.size() < 1) std::fill(previousBlock.begin(), previousBlock.end(), '0');
    else{
        previousBlock = myBlockchain[myBlockchain.size()-1].getCurrentBlock();
        myBlock.setPreviousBlock(previousBlock);
       // std::cout<< "pp2" << std::endl;

    }
    std::uniform_int_distribution<std::mt19937::result_type> distribution(0,distMax);
    //std::vector<Transactions> myTransactionsVectorCopy(myTransactionsVector);

    int tempTxval;
    std::vector<Transactions> tempTransactionBlock;
    string tempTxHashCheck;
    if(myTransactionsVector.size()>=100) {
        while (tempTransactionBlock.size() < 100 && myTransactionsVector.size()!=0) {
            tempTxval = distribution(gen);
            tempTxHashCheck = myTransactionsVector[tempTxval].getSender()+myTransactionsVector[tempTxval].getReceiver()+std::to_string(myTransactionsVector[tempTxval].getValue()) +std::to_string(myTransactionsVector[tempTxval].getDate());
                   Hashish(tempTxHashCheck);
                 //  std::find(myTransactionsVector.begin(), myTransactionsVector.end(), myUser)
                    if(tempTxHashCheck == myTransactionsVector[tempTxval].getHash()) tempTransactionBlock.push_back(myTransactionsVector[tempTxval]);

            myTransactionsVector.erase(myTransactionsVector.begin() + tempTxval);
        //    std::cout << tempTransactionBlock.size() << " " << myTransactionsVector.size() <<  std::endl;
            distMax--;
        }
    }
    else{
        while (tempTransactionBlock.size() != myTransactionsVector.size()) {
            tempTxval = distribution(gen);
            tempTransactionBlock.push_back(myTransactionsVector[tempTxval]);
            myTransactionsVector.erase(myTransactionsVector.begin() + tempTxval);
            //std::cout<< "pp" << std::endl;
        }
    }
    myBlock.setTransactionBlock(tempTransactionBlock);

    std::vector<string> myTransactionsHashes;
    for(int k = 0; k < tempTransactionBlock.size(); k++){
        myTransactionsHashes.push_back(tempTransactionBlock[k].getHash());
    }

    std::vector<string> MerkleHash;
    MerkleHash = MerkleTree(myTransactionsHashes);
    myBlock.setMerkleHash(MerkleHash[0]);
    myBlock.setTimestamp(std::time(nullptr));

    string MainBlockHash;
    uintmax_t Nonce =0;
    string diffTarget = "000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF";
    myBlock.setDifficultyTarget(diffTarget);
    MainBlockHash = myBlock.getPreviousBlock()+std::to_string(myBlock.getTimestamp())+myBlock.getDifficultyTarget()+myBlock.getMerkleHash()+ myBlock.getVersion();
    string TempBlockHash;
    do{
        TempBlockHash = MainBlockHash +std::to_string(Nonce);
        Hashish(TempBlockHash);
        Nonce++;
    }while(TempBlockHash>myBlock.getDifficultyTarget());

    myBlock.setCurrentBlock(TempBlockHash);
    myBlock.setNonce(Nonce);
    std::cout << TempBlockHash << " " << Nonce << std::endl;

    return myBlock;
}