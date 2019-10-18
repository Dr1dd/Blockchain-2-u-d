#include <iostream>
#include "blockchain.h"
using std::string;


const string &Block::getPreviousBlock() const {
    return previousBlock;
}

void Block::setPreviousBlock(const string &previousBlock) {
    Block::previousBlock = previousBlock;
}

const string &Block::getCurrentBlock() const {
    return currentBlock;
}

void Block::setCurrentBlock(const string &currentBlock) {
    Block::currentBlock = currentBlock;
}

time_t Block::getTimestamp() const {
    return Timestamp;
}

void Block::setTimestamp(time_t Timestamp) {
    Block::Timestamp = Timestamp;
}

const string &Block::getVersion() const {
    return version;
}

void Block::setVersion(const string &version) {
    Block::version = version;
}

const string &Block::getMerkleHash() const {
    return merkleHash;
}

void Block::setMerkleHash(const string &merkelHash) {
    Block::merkleHash = merkelHash;
}

unsigned int Block::getNonce() const {
    return Nonce;
}

void Block::setNonce(unsigned int Nonce) {
    Block::Nonce = Nonce;
}

string Block::getDifficultyTarget() const {
    return difficultyTarget;
}

void Block::setDifficultyTarget(string difficultyTarget) {
    Block::difficultyTarget = difficultyTarget;
}

const std::vector<Transactions> &Block::getTransactionBlock() const {
    return TransactionBlock;
}

void Block::setTransactionBlock(const std::vector<Transactions> &TransactionBlock) {
    Block::TransactionBlock = TransactionBlock;
}

