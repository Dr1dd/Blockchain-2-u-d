#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <cmath>
#include <vector>
#include "blockchain.h"

using std::string;
using std::cout;
using std::endl;

string Hashish(string &a){
    int sz=0;
    string b;
    string first;
    int hashSize = 32;
    int sk2 = 0;
    int val;
    sz = a.size();
    int fullSum = 1;

    auto start = std::chrono::high_resolution_clock::now();
    for(std::string::size_type j= 0; j < a.size(); j++){
        fullSum += a[j] * j;
    }
    if(sz>0){
        if(a.size()<=hashSize){
            b.resize(hashSize);
            for(std::string::size_type i = 0; i < hashSize; i++){
                if(i == 0 ) {
                    first = a[i];
                    first = ToHex(first, false);
                    b[i] = (first[0]  * sz * a[i] *a[sz-1] *fullSum) %127;
                    if(b[i] == 0) b[i] = (first[0]  *a[sz-1] * sz * a[i] *fullSum) %113;
                    val = int(b[i]);
                    valueCheck(val, b, a, i);
                }
                if(i < sz && i > 0){
                    if (sk2 == sz) sk2 =0;
                    b[i] = (b[i-1] * a[i] *a[i-1] * b[sk2] *fullSum) %127;
                    if(int(b[i]) == 0){
                        b[i] = (b[i-1] * a[i] *a[i-1] * b[sk2] *fullSum) %113;
                    }
                    val = int(b[i]);
                    valueCheck(val, b, a, i);
                    sk2++;
                }
                if(i>=sz){
                    if (sk2 == sz) sk2 =0;
                    b[i] = (b[i-1] * b[sk2] * a[i%sz] *fullSum) %127;
                    if(b[i] == 0) b[i] = (b[i-1] * a[i%sz] * b[sk2] *fullSum) %113;
                    val = int(b[i]);
                    valueCheck(val, b, a, i);
                    sk2++;
                }
            }
            b = ToHex(b, false);
        }
        if(sz>hashSize){
            b.resize(hashSize);
            b= Compress(b, a, hashSize);
            for(std::string::size_type i = 0; i < hashSize; i++){
                if(i != 0) {
                    b[i] = (sz*b[i-1]*i *a[i] *a[a.size()-1]) %127;
                    if(b[i] == 0) b[i] = (sz*b[i-1]*i*a[i]*a[a.size()-1]) %113;
                    val = int(b[i]);
                    valueCheck(val, b, a, i);
                }
                else{
                    b[i] = (sz*b[0]*a[0]*a[a.size()-1]) %127;
                    if(b[i] == 0) b[i] = (sz*b[0]*a[0]*a[a.size()-1]) %113;
                    val = int(b[i]);
                    valueCheck(val, b, a, i);
                }
            }
            b = ToHex(b, false);
        }
        a = b;
    }
    int n = a.length();
    int g = 0;
    while(g < 5){
        for(int m = 0; m < n/2; m+=2){
            std::swap(a[m+g], a[m+1+g]);
        }
        g++;
    }
    return a;
}
string valueCheck(int & val, string & b, string a, int i ){
    while (val < 0 || val < 16 || val ==0){
        if(val <0) {
            if((-b[i]%127) != 0) b[i] = (-b[i]+i)%127;
            else b[i] = (-b[i]+i)%113;
            val = int(b[i]);
        }
        if(0 < val && val < 16){
            b[i] = (b[i]+15)*2+i;
            val = int(b[i]);
        }
        if(val == 0){
            if(i != 0){
                b[i] = (b[i-1]*a.size()+a[0]+i)%127;
                if(b[i] == 0) b[i] = (b[i-1]*a.size()+a[0]+i)%113;
                val = int(b[i]);
            }
            else {
                b[i] = (127*a.size()+a[0]+i)%127;
                if(b[i] == 0) b[i] = (b[i-1]*a.size()+a[0]+i)%113;
                val = int(b[i]);
            }
        }
    }
    return b;
}
string Compress(string &b, string a, int hashSize){
    int skaicius = 0;
    int sum = 0;
    int gSum = 0;
    int j = 0;
    int daliklis = a.length()/hashSize;
    for(std::string::size_type v = 0; v < a.size(); v++){
        gSum += a[v];
    }
    for(std::string::size_type i = 0; i < a.size(); i++){
        sum = sum + a[i];
        if(skaicius == daliklis){
            if(j !=0){
                b[j] = (sum / daliklis*gSum *a[i] *a[i-1]*a[a.size()-1]*b[j-1]) %127;
                if(b[j] == 0) b[j] = (sum / daliklis*gSum *a[i] *a[i-1] *a[a.size()-1]*b[j-1]) %113;
            }
            else b[j] = (sum / daliklis*gSum *a[i] *a[i-1]*a[a.size()-1]) %127;
            skaicius = 0;
            sum = 0;
            j++;
        }
        skaicius++;
    }
    return b;
}
string ToHex(const string& s, bool upper_case /* = true */)
{
    std::ostringstream ret;
    for (string::size_type i = 0; i < s.length(); ++i)
        ret << std::hex << std::setw(2) << (upper_case ? std::uppercase : std::nouppercase) << (int)s[i];

    return ret.str();
}
