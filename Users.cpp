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
    std::cout << HashedKey << " ";
    oss.clear();
    oss.str("");
    string final;
    final =Hashish(HashedKey);
    std::cout << final << std::endl;

    myUser.public_key = HashedKey;
    return myUser;
}
void MainFunction(){
    std::vector<User> userVector;
    User Person;
    userVector.reserve(1000);
    for(int i = 0; i < 1000; i++){
        Person = GenerateUser(i);
        userVector.push_back(Person);
    }
 /*   for(int j = 0; j < 1000; j++){
        for(int j1 =0;j1<1000;j1++){
          if(userVector[j].public_key == userVector[j1].public_key && j != j1)  std::cout << "sutampa " << j << " " << j1 << std::endl;
        }
    }*/
}