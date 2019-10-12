#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <chrono>
#include <random>
#include <cmath>
#include "blockchain.h"

User GenerateUser(int i){
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 generator(seed);
    std::uniform_int_distribution<std::mt19937::result_type> distribution(100,1000000);
    User myUser;
    myUser.name = "Vardas:" + std::to_string(i);
    myUser.public_key= ""; // pridėti
    myUser.balance = distribution(generator);
    std::cout << seed << std::endl;
    return myUser;
}
void MainFunction(){
    std::vector<User> userVector;

    userVector.reserve(1000);
    for(int i = 0; i < 1000; i++){
        userVector.push_back(GenerateUser(i));
    }
    for(int j = 0; j < 1000; j++){
        //std::cout << userVector[j].name << " " << userVector[j].balance << std::endl;
    }
}