#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
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
    oss.clear();
    oss.str("");
    string final;
    final =Hashish(HashedKey);

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
         int randomsk =distribution(gen);
        while(coin > userVector[randomsk].balance) coin = dist(gen);


                myTransaction.setSender(userVector[randomsk].public_key);
                myTransaction.setReceiver(userVector[distribution(gen)].public_key);
                myTransaction.setSendersID(randomsk);
                myTransaction.setValue(coin);
                myTransaction.setDate(std::time(nullptr));

                string TransactionHash;
                TransactionHash = myTransaction.getSender() + myTransaction.getReceiver()+ std::to_string(myTransaction.getValue()) + std::to_string(myTransaction.getDate());
                Hashish(TransactionHash);
                myTransaction.setHash(TransactionHash);
    return myTransaction;
}
void MainFunction(){
    std::random_device rand;
    static std::mt19937 generation(rand());
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
        myTransactionsVector.push_back(myTransaction);
        tempTx++;

    }

    std::vector<Block> myBlockchain;
    int distMax;
    int maxNonce = 100000;

    std::vector<int> txDeletionBlock;
    std::vector<std::vector<int>> txDeletionBlockVector;
    std::vector<Block> blockCandidates;
    int b= 0;
    while(myTransactionsVector.size()>0) {
        for (b = 0; b < 5; b++) {
            distMax = myTransactionsVector.size()-1;
            blockCandidates.push_back(newBlock(myBlockchain, myTransactionsVector, userVector, txDeletionBlock, distMax, maxNonce));
            txDeletionBlockVector.push_back(txDeletionBlock);
            txDeletionBlock.clear();
        }
        int maxBlockDistribution = 4;
        std::uniform_int_distribution<std::mt19937::result_type> distribution(0, maxBlockDistribution);
        int randomBlock = distribution(gen);
        std::vector<int> arrayOfRandomBlockIndexes{0, 1, 2, 3, 4};
        std::vector<int> arrayCopy = arrayOfRandomBlockIndexes;
        Block minedBlock;
        bool notmined = true;
        int Nonce = 0;
        while (notmined) {
            for (b = 0; b < 5; b++) {
                minedBlock = mineBlock(blockCandidates[arrayCopy[randomBlock]], maxNonce, Nonce);
                arrayCopy.erase(arrayCopy.begin() + randomBlock);
                std::cout << minedBlock.getNonce() << " - Bloko nonce (jei 0 - nera), " << minedBlock.getCurrentBlock()
                          << std::endl;
                if (minedBlock.getNonce() != 0) {
                    myBlockchain.push_back(minedBlock);
                    for (int c = 0; c < txDeletionBlockVector[b].size(); c++) {
                        myTransactionsVector.erase(myTransactionsVector.begin() + txDeletionBlockVector[b][c]);
                    }
                    txDeletionBlockVector.clear();
                    txDeletionBlock.clear();
                    std::cout << "Blokas buvo iskastas" << std::endl;
                    notmined = false;
                    break;
                }
                maxBlockDistribution--;
                std::uniform_int_distribution<std::mt19937::result_type> distribution(0, maxBlockDistribution);
                randomBlock = distribution(generation);
                std::cout << " 3" << std::endl;
            }
            arrayCopy.clear();
            arrayCopy.resize(5);
            std::copy(arrayOfRandomBlockIndexes.begin(), arrayOfRandomBlockIndexes.end(), arrayCopy.begin());
            Nonce = maxNonce;
            maxNonce *= 2;
            maxBlockDistribution = 4;
            std::uniform_int_distribution<std::mt19937::result_type> distribution(0, maxBlockDistribution);
            randomBlock = distribution(generation);
        }
        blockCandidates.clear();
        std::cout << myTransactionsVector.size() <<  " - Likusiu transakciju skaicius" << std::endl;
    }

}

std::vector<string> MerkleTree(std::vector<string> myTransaction){
    string result;
    std::vector<string> TransactionTemp;
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
Block newBlock(std::vector<Block> myBlockchain, std::vector<Transactions> myTransactionsVector, std::vector<User> myUser, std::vector<int> & txDeletionBlock, int & distMax, int maxNonce){
    Block myBlock;
    string previousBlock;
    previousBlock.resize(64);
    if(myBlockchain.size() < 1) std::fill(previousBlock.begin(), previousBlock.end(), '0');
    else{
        previousBlock = myBlockchain[myBlockchain.size()-1].getCurrentBlock();
        myBlock.setPreviousBlock(previousBlock);
    }
    std::uniform_int_distribution<std::mt19937::result_type> distribution(0,distMax);
    std::vector<Transactions> tempTransactionBlock;
    blockBodyGeneration(distMax, myTransactionsVector, myUser, tempTransactionBlock, txDeletionBlock );
    std::vector<string> myTransactionsHashes;
    for(int k = 0; k < tempTransactionBlock.size(); k++){
        myTransactionsHashes.push_back(tempTransactionBlock[k].getHash());
    }
    myBlock.setTransactionBlock(tempTransactionBlock);
    std::vector<string> MerkleHash;

    if(!myTransactionsHashes.empty()){
        if(myTransactionsHashes.size()==1) MerkleHash = myTransactionsHashes; // v0.3
        else MerkleHash = MerkleTree(myTransactionsHashes);
    }

    if(!MerkleHash.empty())
        myBlock.setMerkleHash(MerkleHash[0]);

    myBlock.setTimestamp(std::time(nullptr));

    string MainBlockHash;
    string diffTarget = "00FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF";
    myBlock.setDifficultyTarget(diffTarget);
    if(!MerkleHash.empty()){
        return myBlock;
    }
    else{
        std:: cout << "Nerasta transakciju " << std::endl;
        std::exit(0);
    }
}
Block mineBlock(Block myBlock, int maxNonce, int Nonce){
    string TempBlockHash;
    string MainBlockHash;
    MainBlockHash = myBlock.getPreviousBlock()+std::to_string(myBlock.getTimestamp())+myBlock.getDifficultyTarget()+myBlock.getMerkleHash()+ myBlock.getVersion();
    do{
        TempBlockHash = MainBlockHash +std::to_string(Nonce);
        Hashish(TempBlockHash);
        Nonce++;
    }while(TempBlockHash>myBlock.getDifficultyTarget() && Nonce <=maxNonce);
    if(TempBlockHash<myBlock.getDifficultyTarget()) {
        myBlock.setCurrentBlock(TempBlockHash);
        myBlock.setNonce(Nonce-1);
    }
    else myBlock.setNonce(0);
    return myBlock;
}
void blockBodyGeneration(int distMax, std::vector<Transactions> myTransactionsVector, std::vector<User> myUser, std::vector<Transactions> & tempTransactionBlock, std::vector<int> & txDeletionVector){
    std::uniform_int_distribution<std::mt19937::result_type> distribution(0,distMax);
    int tempTxval;
    std::vector<Transactions> myTransactionsVectorCopy(myTransactionsVector);
    string tempTxHashCheck;
    if(myTransactionsVectorCopy.size()>=100) {
        while (tempTransactionBlock.size() < 100 && myTransactionsVectorCopy.size()!=0) {
            tempTxval = distribution(gen);
            tempTxHashCheck = myTransactionsVectorCopy[tempTxval].getSender()+myTransactionsVectorCopy[tempTxval].getReceiver()+std::to_string(myTransactionsVectorCopy[tempTxval].getValue()) +std::to_string(myTransactionsVectorCopy[tempTxval].getDate());
            Hashish(tempTxHashCheck);
            if(tempTxHashCheck == myTransactionsVectorCopy[tempTxval].getHash() && (myUser[myTransactionsVectorCopy[tempTxval].getSendersID()].balance- myTransactionsVectorCopy[tempTxval].getValue()) >=0) tempTransactionBlock.push_back(myTransactionsVectorCopy[tempTxval]);
            else myTransactionsVector.erase(myTransactionsVector.begin()+tempTxval);
            myTransactionsVectorCopy.erase(myTransactionsVectorCopy.begin()+tempTxval);
            txDeletionVector.push_back(tempTxval);
            distMax--;
        }
    }
    else {
        while (tempTransactionBlock.size() != myTransactionsVectorCopy.size()) {
            tempTxval = distribution(gen);
            tempTransactionBlock.push_back(myTransactionsVectorCopy[tempTxval]);
            myTransactionsVectorCopy.erase(myTransactionsVectorCopy.begin() + tempTxval);
            txDeletionVector.push_back(tempTxval);
        }
    }
};