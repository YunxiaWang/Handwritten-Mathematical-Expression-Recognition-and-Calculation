/*
 * calculation.cpp
 *
 *  Created on: 2015/12/7
 *      Author: Wang
 */


#include <iostream>
#include <string>
using namespace std;

char expressionLine[7];
//const char * expressionToParse = "3*2+4*1+(4+9)*6";
const char * expressionToParse = new char;

string readChar(int a[], int n){
	for (int i = 0; i < n; i++){
		if (a[i] <= 9 && a[i] >= 0) //read digit
			expressionLine[i] = a[i];
		if (a[i] == 10) // read 10 for +
			expressionLine[i] = '+';
		if (a[i] == 11) //read 11 for -
			expressionLine[i] = '-';
		if (a[i] == 12) //read 12 for *
			expressionLine[i] = '*';
	}
	string parse(expressionLine);
	return parse;
}

char peek() {
    return *expressionToParse;
}

char get() {
    return *expressionToParse++;
}

int expression();

int number() { //read number
    int result = get() - '0';
    while (peek() >= '0' && peek() <= '9') {
        result = 10*result + get() - '0';
    }
    return result;
}

int factor() { //process expression
    if (peek() >= '0' && peek() <= '9')
        return number();
    else if (peek() == '(') {
        get(); // '('
        int result = expression();
        get(); // ')'
        return result;
    }
    else if (peek() == '-') {
        get();
        return -factor();
    }
    return 0; // error
}

int term() { //calculate * & /
    int result = factor();
    while (peek() == '*' || peek() == '/')
        if (get() == '*')
            result *= factor();
        else
            result /= factor();
    return result;
}

int expression() { //process expression and calculate
    int result = term();
    while (peek() == '+' || peek() == '-')
        if (get() == '+')
            result += term();
        else
            result -= term();
    return result;
}

/* give array value to char *
 * A constant char array: char *new = tmp.c_str();
 * Or you need to modify new char array and constant is not ok: char *new = &tmp[0];
 * string tmp(tab); tab is an array
 */

int caculation(int a[], int n) {
	string parseChar = readChar(a, n); //read in array of recognized digits
	cout<<parseChar<<'/n';
	*expressionToParse = parseChar.c_str();
    int result = expression(); // calculate
    cout<<result<<'/n';
    return 0;
}

int main(){
	int a[7] = {1,2,10,5,12,7};
	caculation(a,7);
	return 0;
}
