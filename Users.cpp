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
    int maxNonce = 100000;

    std::vector<int> txDeletionBlock;
    std::vector<Block> blockCandidates;
    int b= 0;
    for(b = 0; b < 5; b++){
       blockCandidates.push_back(newBlock(myBlockchain, myTransactionsVector,txDeletionBlock, distMax, maxNonce));
    }
    int maxBlockDistribution = 5;
    std::uniform_int_distribution<std::mt19937::result_type> distribution(0,maxBlockDistribution);
    int randomBlock = distribution(gen);
    Block minedBlock;
    bool notmined = true;
    while(notmined) {
        for (b = 0; b < 5; b++) {
            minedBlock = mineBlock(blockCandidates[randomBlock], maxNonce);
            if (minedBlock.getNonce() !=-1) {
                myBlockchain.push_back(minedBlock);
                notmined = false;
                break;
            }
            }
        maxNonce *=2;
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
Block newBlock(std::vector<Block> myBlockchain, std::vector<Transactions> myTransactionsVector, std::vector<int> txDeletionBlock, int & distMax, int maxNonce){
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

    std::vector<Transactions> tempTransactionBlock;
    blockBodyGeneration(distMax, myTransactionsVector,tempTransactionBlock, txDeletionBlock );

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


    //std::cout << TempBlockHash << " " << Nonce << std::endl;

    return myBlock;
}
Block mineBlock(Block myBlock, int maxNonce){
    string TempBlockHash;
    string MainBlockHash;
    int Nonce = 0;
    MainBlockHash = myBlock.getPreviousBlock()+std::to_string(myBlock.getTimestamp())+myBlock.getDifficultyTarget()+myBlock.getMerkleHash()+ myBlock.getVersion();
    do{
        TempBlockHash = MainBlockHash +std::to_string(Nonce);
        Hashish(TempBlockHash);
        Nonce++;
    }while(TempBlockHash>myBlock.getDifficultyTarget() && Nonce <=maxNonce);
    if(TempBlockHash<myBlock.getDifficultyTarget()) {
        myBlock.setCurrentBlock(TempBlockHash);
        myBlock.setNonce(Nonce);
    }
    else myBlock.setNonce(-1);
    return myBlock;
}
void blockBodyGeneration(int distMax, std::vector<Transactions> myTransactionsVector, std::vector<Transactions> tempTransactionBlock, std::vector<int> txDeletionVector){
    std::uniform_int_distribution<std::mt19937::result_type> distribution(0,distMax);
    //std::vector<Transactions> myTransactionsVectorCopy(myTransactionsVector);
    int tempTxval;
    //std::vector<Transactions> tempTransactionBlock;
    std::vector<Transactions> myTransactionsVectorCopy(myTransactionsVector);
    string tempTxHashCheck;
    //std::vector<int> txDeletionVector;
    if(myTransactionsVectorCopy.size()>=100) {
        while (tempTransactionBlock.size() < 100 && myTransactionsVectorCopy.size()!=0) {
            tempTxval = distribution(gen);
            tempTxHashCheck = myTransactionsVectorCopy[tempTxval].getSender()+myTransactionsVectorCopy[tempTxval].getReceiver()+std::to_string(myTransactionsVectorCopy[tempTxval].getValue()) +std::to_string(myTransactionsVectorCopy[tempTxval].getDate());
            Hashish(tempTxHashCheck);
            //  std::find(myTransactionsVector.begin(), myTransactionsVector.end(), myUser)
            if(tempTxHashCheck == myTransactionsVectorCopy[tempTxval].getHash()) tempTransactionBlock.push_back(myTransactionsVectorCopy[tempTxval]);
            myTransactionsVectorCopy.erase(myTransactionsVectorCopy.begin()+tempTxval);
            txDeletionVector.push_back(tempTxval);
            //    std::cout << tempTransactionBlock.size() << " " << myTransactionsVector.size() <<  std::endl;
            distMax--;
        }
    }
    else {
        while (tempTransactionBlock.size() != myTransactionsVectorCopy.size()) {
            tempTxval = distribution(gen);
            tempTransactionBlock.push_back(myTransactionsVectorCopy[tempTxval]);
            myTransactionsVectorCopy.erase(myTransactionsVectorCopy.begin() + tempTxval);
            txDeletionVector.push_back(tempTxval);

            //std::cout<< "pp" << std::endl;
        }
    }
};