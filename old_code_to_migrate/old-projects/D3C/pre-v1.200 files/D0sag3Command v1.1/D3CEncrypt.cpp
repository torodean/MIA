/*
 * D3CEncrypt.cpp
 *
 *  Created on: Sep 24, 2014
 *      Author: Antonius Torode
 */

#include <vector>
#include <sstream>
#include <cstdlib>
#include <math.h>
#include "D3CEncrypt.h"

namespace std {

D3CEncrypt::D3CEncrypt() : stringLength (0), vectorSize (0), digit (0), powerofDigit (0), powerofTen (0), devise (0), random7bitNum (0), random7bitbinary (0), encodedVec (0){
    // TODO Auto-generated constructor stub
}

vector<int> D3CEncrypt::stringToVector(string a) { //Converts a string to a vector
    stringLength = a.size();
    vector<int> vector(stringLength);
//	cout << "vector: ";
    for (int i = 0; i < stringLength; i++) { //converts string one into an array of integers.
        vector[i] = a[i];
//		cout << vector[i];
    }
//	cout << endl << endl;
    return vector;
}

vector<int> D3CEncrypt::binaryVector(vector<int> inputVector){ //Converts each term in the vector array to a binary representation of that term
    vectorSize = inputVector.size();
    for(int i=0;i<vectorSize;i++){
        digit = inputVector[i];
        inputVector[i]=0;
//		cout << "baseTwoVector[" << i << "]: ";
        for(int x=6;x>=0;x--){
            powerofDigit = pow(2,x);
//			cout << "powerofDigit: " << powerofDigit << endl;
            powerofTen = pow(10, x);
//			cout << "powerofTen: " << powerofTen << endl;
            devise=digit/powerofDigit;
            if(devise == 1){
                inputVector[i]+=powerofTen;
                digit-=powerofDigit;
            }
        }
//		cout << inputVector[i];
//		cout << endl;
    }

    return inputVector;
}

int D3CEncrypt::random7bit(){ //creates a random number between 0-127
    random7bitNum = rand()% 128;
    return random7bitNum;
}

int D3CEncrypt::numberToBinary(int num) { //changes a number from base 10 format to a binary look but still in base 10 so that addition in base ten can be used on the binary number.
    random7bitbinary=0;
    for (int x = 6; x >= 0; x--) {
        powerofDigit = pow(2, x);
//		cout << "powerofDigit: " << powerofDigit << endl;
        powerofTen = pow(10, x);
//		cout << "powerofTen: " << powerofTen << endl;
        devise = num / powerofDigit;
        if (devise == 1) {
            random7bitbinary += powerofTen;
            num -= powerofDigit;
        }
    }
    return random7bitbinary;
}

string D3CEncrypt::a11b12(int x){ //converts an integer to a custom a11b12 formatted number. the first digit is in base 11 and the second in base 12.
//	cout << "~~program reaches a11b12()~~" << endl;
    int remaind, num;
    string ones, tens, a11b12string;
    ostringstream convertA, convertB;
    if(x<9){
        a11b12string.replace(0,1,"0");
        num = x;
        convertA << num;
        ones = convertA.str();
        a11b12string.replace(1,1,ones);
        goto ending;
    }
    if(x/12<=9){
        num = (x/12);
        convertA << num;
        tens = convertA.str();
        //tens = (x/12);
    }else if(x/12 == 10){
        tens = "A";
    }else if(x/12 == 11){
        tens = "B";
    }
//	cout << "tens: \t\t" << tens << endl;
    remaind = (x-(x/12)*12)%12;
//	cout << "remaind: \t" << remaind << endl;
    if(remaind <= 9){
        num = remaind;
        convertB << num;
        ones = convertB.str();
//		ones = x%12;
    }else if(remaind == 10){
        ones = "A";
    }else if(remaind == 11){
        ones = "B";
    }
//	cout << "ones: \t\t" << ones << endl;
    a11b12string.replace(0,1,tens);
    a11b12string.replace(1,1,ones);
    ending:
    return a11b12string;
}

string D3CEncrypt::stringVectorToString(vector<string> a){ //converts a vector<string> (each position holding a string of length 9) to one combined string.
    int size = a.size(); //sets teh integet of size to the size of the inputted vector.
//	cout << "size: \t\t\t" << size << endl;
    long factor; //creates a variable called factor that is of type long. It is long because it is possible for large inputs that 9 times the size of the vector (the length of the output vector) maye be larger than the maximum int size.
    string total; //Creates a string of total to put the final return into.
    total.resize(9*size); //resizes the total string to 9 times the length of the original input because each character will be represented by 9 digits after being crypted.
//	cout << "total size: \t\t" << total.size() << endl;
    string tempText; //Creates a temporary string to be used later.
    tempText.resize(9); //resizes the tempText string to 9 because it will be used to hold a single character as it's encrypted value.

    for(int i=0;i<size;i++){
        tempText=a[i]; //Sets the tempText variable to the first part of the vector a.
        factor = 9*i; //Sets the long factor to 9 times i to determine which digit of the vector we are working on.
        for(int x=0;x<9;x++){
            total[factor+x] = tempText[x]; //sets the total string created earlier to the appropriate corresponding value of teh vector a.
        }
    }
    return total; //Returns the total string.
}

string D3CEncrypt::cryptCharsNoRand(vector<int> inputVector){ //Encrypts the characters of a vector array with a random number
//	cout << endl << "~~program reaches CryptChars()~~" << endl;
    bool fix=false;
    vectorSize = inputVector.size(); //sets the Int of vectorSize to the size of the input.
    vector<string> encoded (vectorSize); //Creates a vector of type strings named encoded. It is set to the vectorSize and will be used to store the encoded vectors once they are combined with the random numbers.
    vector<int> randomVector;
    int randomNum;
    randomVector.reserve(vectorSize); //reserves enought space within randomVector to place all of the random valiues to be combined with the string vector encoded later.
//	cout << "VectorSize: " << vectorSize << endl;
//	cout << endl;

    for(int i=0;i<vectorSize; i++){
        randomNum = 49; //in this variation, the random number is instead set to just 49.

        randomVector[i] = randomNum; //stores the random number that was generated earlier within a vector randomVector that will contain all of the random numbers in the order that they are generated.

        random7bitbinary = numberToBinary(randomNum); //converts the random number generated to a binary form but keeps it in base 10 so it can be added to the character for part of the crypt process.

        inputVector[i]+=random7bitbinary; //adds the random number in binary form to the original character in binary form, giving a base 3 number.

    }
    string encodedPart, encode = "000000000";
    encodedPart.reserve(2);
//	cout << "~~program reaches CryptCombine()~~" << endl;
    encoded.reserve(vectorSize+2);
    for(int i=0;i<vectorSize; i++){
        encodedPart = a11b12(randomVector[i]);
//		cout << "randomVector[" << i << "]: \t" << randomVector[i] << endl;
//		cout << "encodedPart: \t\t" << encodedPart << endl;

        encodedVec = inputVector[i];

        if(encodedVec < 1000000){
            fix = true;
        }

//		cout << "encodedVec: \t\t" << encodedVec << endl;
        stringstream ss;
        ss << encodedVec;
        encode = ss.str();

        encode.resize(9);

        //In some cases, the anscii code generates a binary vector without the proper length of 6 (100000) which causes errors when trying to set the 7'th and 8th position. The below code will fix that by setting the first digit in the string to 0.
        if(fix == true){
            for(int y=6;y>0;y--){
                encode[y]=encode[y-1];
            }
            encode[0]='0';
            fix=false;
        }

//		cout << "encode: \t\t" << encode << endl;
//		cout << "encode[7][8]: \t\t";

        for(int x=7;x<9;x++){ //set's the last 2 digits in encode equal to the a11b12 form of the random number generated for the specific character being coded.
            encode[x] = encodedPart[x-7];
//			cout << encode[x];
        }

//		cout << endl;
//		cout << "encode: \t\t" << encode << endl;

        encoded[i]=encode;
//		cout << "Encoded[" << i << "]: \t\t" << encoded[i] << endl;
    }
    string finalEncode, outputVector;
    finalEncode = stringVectorToString(encoded);
//	cout << "finalEncode: " << finalEncode << endl;
    outputVector = finalEncode;
//	cout << "outputVector: " << outputVector << endl;
    return outputVector;
}

string D3CEncrypt::cryptChars(vector<int> inputVector){ //Encrypts the characters of a vector array with a random number
//	cout << endl << "~~program reaches CryptChars()~~" << endl;
    bool fix=false;
    vectorSize = inputVector.size(); //sets the Int of vectorSize to the size of the input.
    vector<string> encoded (vectorSize); //Creates a vector of type strings named encoded. It is set to the vectorSize and will be used to store the encoded vectors once they are combined with the random numbers.
    vector<int> randomVector;
    int randomNum;
    randomVector.reserve(vectorSize); //reserves enought space within randomVector to place all of the random valiues to be combined with the string vector encoded later.
//	cout << "VectorSize: " << vectorSize << endl;
//	cout << endl;

    for(int i=0;i<vectorSize; i++){
        randomNum=random7bit(); //creates a random number from 0-127 and stores it in the variable randomNum;
//		cout << "randomNum: \t\t" << randomNum << endl;
        randomVector[i] = randomNum; //stores the random number that was generated earlier within a vector randomVector that will contain all of the random numbers in the order that they are generated.
//		cout << "randomVector[" << i << "]: \t" << randomVector[i] << endl;
        random7bitbinary = numberToBinary(randomNum); //converts the random number generated to a binary form but keeps it in base 10 so it can be added to the character for part of the crypt process.
//		cout << "random7bitbinary: \t" << random7bitbinary << endl;;
//		cout << "inputVector[" << i << "] before: \t" << inputVector[i] << endl;
        inputVector[i]+=random7bitbinary; //adds the random number in binary form to the original character in binary form, giving a base 3 number.
//		cout << "inputVector[" << i << "] after: \t" << inputVector[i] << endl;
//		cout << endl;
    }
    string encodedPart, encode = "000000000";
    encodedPart.reserve(2);
//	cout << "~~program reaches CryptCombine()~~" << endl;
    encoded.reserve(vectorSize+2);
    for(int i=0;i<vectorSize; i++){
        encodedPart = a11b12(randomVector[i]);
//		cout << "randomVector[" << i << "]: \t" << randomVector[i] << endl;
//		cout << "encodedPart: \t\t" << encodedPart << endl;

        encodedVec = inputVector[i];

        if(encodedVec < 1000000){
            fix = true;
        }

//		cout << "encodedVec: \t\t" << encodedVec << endl;
        stringstream ss;
        ss << encodedVec;
        encode = ss.str();

        encode.resize(9);

        //In some cases, the anscii code generates a binary vector without the proper length of 6 (100000) which causes errors when trying to set the 7'th and 8th position. The below code will fix that by setting the first digit in the string to 0.
        if(fix == true){
            for(int y=6;y>0;y--){
                encode[y]=encode[y-1];
            }
            encode[0]='0';
            fix=false;
        }

//		cout << "encode: \t\t" << encode << endl;
//		cout << "encode[7][8]: \t\t";

        for(int x=7;x<9;x++){ //set's the last 2 digits in encode equal to the a11b12 form of the random number generated for the specific character being coded.
            encode[x] = encodedPart[x-7];
//			cout << encode[x];
        }

//		cout << endl;
//		cout << "encode: \t\t" << encode << endl;

        encoded[i]=encode;
//		cout << "Encoded[" << i << "]: \t\t" << encoded[i] << endl;
    }
    string finalEncode, outputVector;
    finalEncode = stringVectorToString(encoded);
//	cout << "finalEncode: " << finalEncode << endl;
    outputVector = finalEncode;
//	cout << "outputVector: " << outputVector << endl;
    return outputVector;
}

string D3CEncrypt::CryptNoRand(string input){ //Encrypts a string
    string cryptedVector;
    vector<int> baseTwoVector, inputVector;

    inputVector = stringToVector(input);
    baseTwoVector = binaryVector(inputVector);
    cryptedVector = cryptCharsNoRand(baseTwoVector);

    return cryptedVector;
}

string D3CEncrypt::Crypt(string input){ //Encrypts a string
    string cryptedVector;
    vector<int> baseTwoVector, inputVector;
//	cout <<endl << "~~program reaches Crypt()~~" << endl;
//	cout << "original input: " << input << endl << endl;
    inputVector = stringToVector(input);
//	cout << "inputVector size: " << inputVector.size() << endl;
    baseTwoVector = binaryVector(inputVector);
//	cout << "baseTwoVector size: " << baseTwoVector.size() << endl;
    cryptedVector = cryptChars(baseTwoVector);
//	cout << "cryptedVector size: " << cryptedVector.size() << endl;

    return cryptedVector;
}

vector<string> D3CEncrypt::cryptedStringToVector(string a) { //Converts a string after being encrypted using crypt() to a vector seperating each character into it's own position.
    stringLength = a.size()/9;
    long fact;
    string temp;
    temp.resize(9);
    vector<string> vector(stringLength);
    for(int i=0;i<stringLength;i++){
        fact = i*9;
        for(int x=0;x<9;x++){
            temp[x] = a[x+fact];
//			cout << "temp[" << x << "]: " << temp[x] << endl;
        }
//		cout << "temp: " << temp << endl;
        vector[i] = temp;
//		cout << "vector[" << i << "]: " << vector[i] << endl;
    }
//	cout << "finished cryptedStringToVector()" << endl;
    return vector;
}

vector<string> D3CEncrypt::seperateRandom(vector<string> input){
//	cout << "~~Code reached seperateRandom()~~" << endl;
    vector<string> seperatedRandoms;
    vectorSize = input.size();
    string temp="00", temp2, tempchar1, tempchar2;
    temp.resize(2);
    temp2.resize(9);
    seperatedRandoms.resize(vectorSize);

    for(int i=0;i<vectorSize;i++){
        temp2=input[i];
//		cout << "temp2: " << temp2 << endl;
        tempchar1 = temp2[7];
        tempchar2 = temp2[8];
        temp.replace(0,1,tempchar1);
        temp.replace(1,1,tempchar2);
//		cout << "temp: " << temp << endl;
        seperatedRandoms[i] = temp;
//		cout << "seperatedRandoms[" << i << "]: " << seperatedRandoms[i] << endl;
    }
    return seperatedRandoms;
}

vector<int> D3CEncrypt::vectorStringToInt(vector<string> a){
    vectorSize = a.size();
    vector<int> intVec (vectorSize);
    int tempInt;
    string tempString;

    for(int i=0;i<vectorSize;i++){
        tempString = a[i];
        for(int x=0;x<7;x++){
            tempInt = atoi(tempString.c_str());
        }
        intVec[i] = tempInt;
//		cout << "intVec[" << i << "]: " << intVec[i] << endl;
    }
    return intVec;
}

vector<int> D3CEncrypt::seperateBinary(vector<string> input){
//	cout << "~~Code reached seperateBinary()~~" << endl;
    vectorSize = input.size();
    vector<string> trimmed (vectorSize);
    vector<int> seperatedChars (vectorSize);
    string temp;

    for(int i=0;i<vectorSize;i++){
        temp=input[i];
        temp.resize(7);
//		cout << "temp: " << temp << endl;
        trimmed[i] = temp;
//		cout << "trimmed[" << i << "]: " << trimmed[i] << endl;
    }

    seperatedChars = vectorStringToInt(trimmed);
    return seperatedChars;
}

int D3CEncrypt::a11b12toReg(string a11b12){
//	cout << "~~Code reached a11b12toReg()~~" << endl;
    string charOne, charTwo;
    int intOne, intTwo, reg=0;

    charOne = a11b12[0];
//	cout << "charOne: " << charOne << endl;
    charTwo = a11b12[1];
//	cout << "charTwo: " << charTwo << endl;

    if(charOne == "A"){
        reg+=120;
    } else {
        intOne = atoi(charOne.c_str());
        reg+=12*intOne;
    }
    if(charTwo == "A"){
        reg+=10;
    }else if(charTwo == "B"){
        reg+=11;
    }else{
        intTwo = atoi(charTwo.c_str());
        reg+=intTwo;
    }

    return reg;
}

vector<int> D3CEncrypt::converta11b12vecToReg(vector<string> a){
//	cout << "~~Code reached converta11b12vecToReg()~~" << endl;
    vectorSize = a.size();
    vector<int> randomVector (vectorSize);
    string temp;
    int randomNum;

//	cout << "yo" << endl;
    for(int i=0;i<vectorSize;i++){
        temp = a[i];
//		cout << "temp: " << temp << endl;
        randomNum = a11b12toReg(temp);
//		cout << "randomNum: " << randomNum << endl;
        randomVector[i] = randomNum;
    }
    return randomVector;
}

vector<int> D3CEncrypt::numberVectorToBinaryVector(vector<int> a){
    vectorSize=a.size();
    vector<int> binaryRandomVector (vectorSize);
    int temp;

    for(int i=0;i<vectorSize;i++){
        temp = a[i];
        binaryRandomVector[i] = numberToBinary(temp);
//		cout << "binaryRandomVector[" << i << "]: " << binaryRandomVector[i] << endl;
    }
    return binaryRandomVector;
}

vector<int> D3CEncrypt::DeCryptChars(vector<int> a, vector<int> b){
    vectorSize = a.size();
    vector<int> deCryptedBinaryVector (vectorSize);

    for(int i=0;i<vectorSize;i++){
        deCryptedBinaryVector[i] = a[i] - b[i];
//		cout << "deCryptedBinaryVector[" << i << "]: " << deCryptedBinaryVector[i] << endl;
    }
    return deCryptedBinaryVector;
}

vector<int> D3CEncrypt::binaryVectorToASCII(vector<int> a){
    vectorSize = a.size();
    vector<int> deCryptedIntVector (vectorSize);
    int digit, character, power, twoPower;
    for(int i=0;i<vectorSize;i++){
        character = 0;
        digit = a[i];
//		cout << "digit: " << digit << endl;
        for(int x=6;x>=0;x--){
            power = pow(10,x);
//			cout << "power: " << power << endl;
            twoPower = pow(2, x);
//			cout << "twoPower: " << twoPower << endl;
            character += ((digit/power)%10)*(twoPower);
        }
//		cout << "Character: " << character << endl;
        deCryptedIntVector[i] = character;
//		cout << "deCryptedIntVector[" << i << "]: " << deCryptedIntVector[i] << endl;
    }
    return deCryptedIntVector;
}

string D3CEncrypt::intVectorToString(vector<int> a){
    vectorSize = a.size();
    string deCryptedVector;
    deCryptedVector.resize(vectorSize);
    int character;
    for(int i=0;i<vectorSize;i++){
        character = a[i];
        deCryptedVector[i] = character;
    }
    return deCryptedVector;
}

string D3CEncrypt::DeCrypt(string input){
    string deCryptedVector;
    vector<string> inputVector, seperatedRandomVector;
    vector<int> seperatedcharacterVector, baseTenRandomVector, randomVector, randomBinaryVector,deCryptedBinaryVector, deCryptedIntVector;
//	cout << "original input: " << input << endl << endl;
//	cout <<endl << "~~program reaches DeCrypt()~~" << endl;
    inputVector = cryptedStringToVector(input);
    seperatedRandomVector = seperateRandom(inputVector);
    seperatedcharacterVector = seperateBinary(inputVector);
    randomVector = converta11b12vecToReg(seperatedRandomVector);
    randomBinaryVector = numberVectorToBinaryVector(randomVector);
    deCryptedBinaryVector = DeCryptChars(seperatedcharacterVector,randomBinaryVector);

    deCryptedIntVector = binaryVectorToASCII(deCryptedBinaryVector);
    deCryptedVector = intVectorToString(deCryptedIntVector);

    return deCryptedVector;
}


D3CEncrypt::~D3CEncrypt() {
    // TODO Auto-generated destructor stub
}

}/* namespace std */

