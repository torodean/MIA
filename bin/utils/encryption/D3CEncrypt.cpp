//============================================================================
// Name        : D3CEncrypt.cpp
// Author      : Antonius Torode
// Copyright   : This file can be used under the conditions of Antonius' 
//				 General Purpose License (AGPL).
// Created on  : September 24, 2014
// Description : A old encryption program (d0s1) re-adapted for use with MIA.
//============================================================================

#include <iostream> //for troubleshooting.
#include <utility>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <cmath>

// Include the associated header file.
#include "D3CEncrypt.hpp"

using std::vector;
using std::string;
using std::stringstream;
using std::ostringstream;
using std::cout;
using std::endl;

//Constructor for the D3CEncrypt class.
D3CEncrypt::D3CEncrypt() : stringLength (0), vectorSize (0), digit (0), powerofDigit (0), 
		powerofTen (0), devise (0), random7bitNum (0), random7bitbinary (0), encodedVec (0)
{
    //Chuck Norris doesn’t use web standards as the web will conform to him.
}

//Deconstructor for D3CEncrypt class.
D3CEncrypt::~D3CEncrypt() 
{
    //Chuck Norris finished World of Warcraft.
}

//Converts a string to a vector.
vector<int> D3CEncrypt::stringToVector(string a) 
{ 
    stringLength = a.size();
    vector<int> vector(stringLength);
	
	//converts string one into an array of integers.
    for (int i = 0; i < stringLength; i++) { 
        vector[i] = a[i];
    }
    return vector;
}

//Converts each term in the vector array to a binary representation of that term.
vector<int> D3CEncrypt::binaryVector(vector<int> inputVector)
{ 
    vectorSize = inputVector.size();
    for(int i=0;i<vectorSize;i++)
	{
        digit = inputVector[i];
        inputVector[i]=0;
        for(int x=6;x>=0;x--)
		{
            powerofDigit = pow(2,x);
            powerofTen = pow(10, x);
            devise=digit/powerofDigit;
            if(devise == 1)
			{
                inputVector[i]+=powerofTen;
                digit-=powerofDigit;
            }
        }
    }

    return inputVector;
}

//Creates a random number between 0-127.
int D3CEncrypt::random7bit()
{	
    random7bitNum = rand()% 128;
    return random7bitNum;
}

//Changes a number from base 10 format to a binary look but still in base 
//10 so that addition in base ten can be used on the binary number.
int D3CEncrypt::numberToBinary(int num) 
{ 
    random7bitbinary=0;
    for (int x = 6; x >= 0; x--) 
	{
        powerofDigit = pow(2, x);
        powerofTen = pow(10, x);
        devise = num / powerofDigit;
        if (devise == 1) 
		{
            random7bitbinary += powerofTen;
            num -= powerofDigit;
        }
    }
    return random7bitbinary;
}

//Converts an integer to a custom a11b12 formatted number. 
//the first digit is in base 11 and the second in base 12.
string D3CEncrypt::a11b12(int x)
{	
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
    if(x/12<=9)
	{
        num = (x/12);
        convertA << num;
        tens = convertA.str();
        //tens = (x/12);
    }
	else if(x/12 == 10)
	{
        tens = "A";
    }
	else if(x/12 == 11)
	{
        tens = "B";
    }
    remaind = (x-(x/12)*12)%12;
    if(remaind <= 9)
	{
        num = remaind;
        convertB << num;
        ones = convertB.str();
    }
	else if(remaind == 10)
	{
        ones = "A";
    }
	else if(remaind == 11)
	{
        ones = "B";
    }
    a11b12string.replace(0,1,tens);
    a11b12string.replace(1,1,ones);
    ending:
    return a11b12string;
}

//Converts a vector<string> (each position holding a string of length 9) to one combined string.
string D3CEncrypt::stringVectorToString(vector<string> a)
{	
	//sets the integet of size to the size of the inputted vector.
    int size = a.size(); 
	
	//creates a variable called factor that is of type long. 
	//It is long because it is possible for large inputs that 9 times the size of the vector 
	//(the length of the output vector) maye be larger than the maximum int size.
    long factor; 
	
	//Creates a string of total to put the final return into.
    string total; 
	
	//resizes the total string to 9 times the length of the original input because each 
	//character will be represented by 9 digits after being crypted.
    total.resize(9*size); 
	
	//Creates a temporary string to be used later.
    string tempText; 
	
	//resizes the tempText string to 9 because it will be used to hold a single character as it's encrypted value.
    tempText.resize(9); 

    for(int i=0;i<size;i++)
	{
		
		//Sets the tempText variable to the first part of the vector a.
        tempText=a[i]; 
		
		//Sets the long factor to 9 times i to determine which digit of the vector we are working on.
        factor = 9*i; 
        for(int x=0;x<9;x++){
			//sets the total string created earlier to the appropriate corresponding value of the vector a.
            total[factor+x] = tempText[x]; 
        }
    }
    return total; //Returns the total string.
}

//Encrypts the characters of a vector array with a random number.
string D3CEncrypt::cryptCharsNoRand(vector<int> inputVector)
{ 
    bool fix=false;
	
	//sets the Int of vectorSize to the size of the input.
    vectorSize = inputVector.size(); 
	
	//Creates a vector of type strings named encoded. 
	//It is set to the vectorSize and will be used to store the encoded vectors once they are combined with the random numbers.
    vector<string> encoded (vectorSize); 
    vector<int> randomVector;
    int randomNum;
	
	//reserves enought space within randomVector to place all of the 
	//random valiues to be combined with the string vector encoded later.
    randomVector.reserve(vectorSize); 

    for(int i=0;i<vectorSize; i++)
	{
		//in this variation, the random number is instead set to just 49.
        randomNum = 49; 

		//stores the random number that was generated earlier within a vector randomVector 
		//that will contain all of the random numbers in the order that they are generated.
        randomVector[i] = randomNum; 

		//converts the random number generated to a binary form but keeps it in base 
		//10 so it can be added to the character for part of the crypt process.
        random7bitbinary = numberToBinary(randomNum); 

		//adds the random number in binary form to the original character in binary form, giving a base 3 number.
        inputVector[i]+=random7bitbinary; 
    }
    string encodedPart, encode = "000000000";
    encodedPart.reserve(2);
    encoded.reserve(vectorSize+2);
    for(int i=0;i<vectorSize; i++)
	{
        encodedPart = a11b12(randomVector[i]);

        encodedVec = inputVector[i];

        if(encodedVec < 1000000)
            fix = true;

        stringstream ss;
        ss << encodedVec;
        encode = ss.str();

        encode.resize(9);

        //In some cases, the anscii code generates a binary vector without the proper length 
		//of 6 (100000) which causes errors when trying to set the 7'th and 8th position. 
		//The below code will fix that by setting the first digit in the string to 0.
        if(fix)
		{
            for(int y=6;y>0;y--)
			{
                encode[y]=encode[y-1];
            }
            encode[0]='0';
            fix=false;
        }

		//set's the last 2 digits in encode equal to the a11b12 form of the random number generated for the specific character being coded.
        for(int x=7;x<9;x++)
		{ 
            encode[x] = encodedPart[x-7];
        }

        encoded[i]=encode;
    }
    string finalEncode, outputVector;
    finalEncode = stringVectorToString(encoded);
    outputVector = finalEncode;
    return outputVector;
}

//Encrypts the characters of a vector array with a random number.
string D3CEncrypt::cryptChars(vector<int> inputVector){ 
    bool fix=false;
	
	//sets the Int of vectorSize to the size of the input.
    vectorSize = inputVector.size(); 
	
	//Creates a vector of type strings named encoded. 
	//It is set to the vectorSize and will be used to store the encoded vectors once they are combined with the random numbers.
    vector<string> encoded (vectorSize); 
    vector<int> randomVector;
    int randomNum;
	
	//reserves enought space within randomVector to place all of the
	//random valiues to be combined with the string vector encoded later.
    randomVector.reserve(vectorSize); 

    for(int i=0;i<vectorSize; i++)
	{
		//creates a random number from 0-127 and stores it in the variable randomNum;
        randomNum=random7bit(); 
		
		//stores the random number that was generated earlier within a vector randomVector 
		//that will contain all of the random numbers in the order that they are generated.
        randomVector[i] = randomNum; 
		
		//converts the random number generated to a binary form but keeps it in base 
		//10 so it can be added to the character for part of the crypt process.
        random7bitbinary = numberToBinary(randomNum); 
		
		//adds the random number in binary form to the original 
		//character in binary form, giving a base 3 number.
        inputVector[i]+=random7bitbinary; 
    }
    string encodedPart, encode = "000000000";
    encodedPart.reserve(2);
    encoded.reserve(vectorSize+2);
    for(int i=0;i<vectorSize; i++)
	{
        encodedPart = a11b12(randomVector[i]);

        encodedVec = inputVector[i];

        if(encodedVec < 1000000)
            fix = true;

        stringstream ss;
        ss << encodedVec;
        encode = ss.str();

        encode.resize(9);

        //In some cases, the anscii code generates a binary vector without the proper length 
		//of 6 (100000) which causes errors when trying to set the 7'th and 8th position. 
		//The below code will fix that by setting the first digit in the string to 0.
        if(fix)
		{
            for(int y=6;y>0;y--)
			{
                encode[y]=encode[y-1];
            }
            encode[0]='0';
            fix=false;
        }

		//set's the last 2 digits in encode equal to the a11b12 form of the random number generated for the specific character being coded.
        for(int x=7;x<9;x++){ 
            encode[x] = encodedPart[x-7];
        }

        encoded[i]=encode;
    }
    string finalEncode, outputVector;
    finalEncode = stringVectorToString(encoded);
    outputVector = finalEncode;
    return outputVector;
}

//Encrypts a string.
string D3CEncrypt::CryptNoRand(string input, bool toSquish)
{	
    string cryptedVector;
    vector<int> baseTwoVector, inputVector;

    inputVector = stringToVector(std::move(input));
    baseTwoVector = binaryVector(inputVector);
    cryptedVector = cryptCharsNoRand(baseTwoVector);

	if(toSquish)
	{
		//cryptedVector = squish(cryptedVector);
	}
	
    return cryptedVector;
}

//Encrypts a string.
string D3CEncrypt::Crypt(const string& input, bool toSquish)
{
    string cryptedVector;
    vector<int> baseTwoVector, inputVector;
    inputVector = stringToVector(std::move(input));
    baseTwoVector = binaryVector(inputVector);
    cryptedVector = cryptChars(baseTwoVector);
	
	cout << cryptedVector << endl << endl;
	
	if(toSquish)
	{
		//cryptedVector = squish(cryptedVector);
	}
	
    return cryptedVector;
}

//Converts a string after being encrypted using crypt() to a vector seperating each character into it's own position.
vector<string> D3CEncrypt::cryptedStringToVector(string a) { 
    stringLength = a.size()/9;
    long fact;
    string temp;
    temp.resize(9);
    vector<string> vector(stringLength);
    for(int i=0;i<stringLength;i++)
	{
        fact = i*9;
        for(int x=0;x<9;x++)
		{
            temp[x] = a[x+fact];
        }
        vector[i] = temp;
    }
    return vector;
}

vector<string> D3CEncrypt::seperateRandom(vector<string> input)
{
    vector<string> seperatedRandoms;
    vectorSize = input.size();
    string temp="00", temp2, tempchar1, tempchar2;
    temp.resize(2);
    temp2.resize(9);
    seperatedRandoms.resize(vectorSize);

    for(int i=0;i<vectorSize;i++)
	{
        temp2=input[i];
        tempchar1 = temp2[7];
        tempchar2 = temp2[8];
        temp.replace(0,1,tempchar1);
        temp.replace(1,1,tempchar2);
        seperatedRandoms[i] = temp;
    }
    return seperatedRandoms;
}

vector<int> D3CEncrypt::vectorStringToInt(vector<string> a)
{
    vectorSize = a.size();
    vector<int> intVec (vectorSize);
    int tempInt;
    string tempString;

    for(int i=0;i<vectorSize;i++)
	{
        tempString = a[i];
        for(int x=0;x<7;x++)
		{
            tempInt = atoi(tempString.c_str());
        }
        intVec[i] = tempInt;
    }
    return intVec;
}

vector<int> D3CEncrypt::seperateBinary(vector<string> input)
{
    vectorSize = input.size();
    vector<string> trimmed (vectorSize);
    vector<int> seperatedChars (vectorSize);
    string temp;

    for(int i=0;i<vectorSize;i++)
	{
        temp=input[i];
        temp.resize(7);
        trimmed[i] = temp;
    }

    seperatedChars = vectorStringToInt(trimmed);
    return seperatedChars;
}

int D3CEncrypt::a11b12toReg(string a11b12)
{
    string charOne, charTwo;
    int intOne, intTwo, reg=0;

    charOne = a11b12[0];
    charTwo = a11b12[1];

    if(charOne == "A")
	{
        reg+=120;
    } 
	else 
	{
        intOne = atoi(charOne.c_str());
        reg+=12*intOne;
    }
    if(charTwo == "A")
	{
        reg+=10;
    }
	else if(charTwo == "B")
	{
        reg+=11;
    }
	else
	{
        intTwo = atoi(charTwo.c_str());
        reg+=intTwo;
    }

    return reg;
}

vector<int> D3CEncrypt::converta11b12vecToReg(vector<string> a)
{
    vectorSize = a.size();
    vector<int> randomVector (vectorSize);
    string temp;
    int randomNum;

    for(int i=0;i<vectorSize;i++)
{
        temp = a[i];
        randomNum = a11b12toReg(temp);
        randomVector[i] = randomNum;
    }
    return randomVector;
}

vector<int> D3CEncrypt::numberVectorToBinaryVector(vector<int> a)
{
    vectorSize=a.size();
    vector<int> binaryRandomVector (vectorSize);
    int temp;

    for(int i=0;i<vectorSize;i++)
	{
        temp = a[i];
        binaryRandomVector[i] = numberToBinary(temp);
    }
    return binaryRandomVector;
}

vector<int> D3CEncrypt::DeCryptChars(vector<int> a, vector<int> b)
{
    vectorSize = a.size();
    vector<int> deCryptedBinaryVector (vectorSize);

    for(int i=0;i<vectorSize;i++)
	{
        deCryptedBinaryVector[i] = a[i] - b[i];
    }
    return deCryptedBinaryVector;
}

vector<int> D3CEncrypt::binaryVectorToASCII(vector<int> a)
{
    vectorSize = a.size();
    vector<int> deCryptedIntVector (vectorSize);
    int val, character, power, twoPower;
    for(int i=0;i<vectorSize;i++){
        character = 0;
        val = a[i];
        for(int x=6;x>=0;x--)
		{
            power = pow(10,x);
            twoPower = pow(2, x);
            character += ((val/power)%10)*(twoPower);
        }
        deCryptedIntVector[i] = character;
    }
    return deCryptedIntVector;
}

string D3CEncrypt::intVectorToString(vector<int> a)
{
    vectorSize = a.size();
    string deCryptedVector;
    deCryptedVector.resize(vectorSize);
    int character;
    for(int i=0;i<vectorSize;i++)
	{
        character = a[i];
        deCryptedVector[i] = character;
    }
    return deCryptedVector;
}

//Decrypts a string encrypted with D3Crypt.
string D3CEncrypt::DeCrypt(string input, bool squish)
{
    string deCryptedVector;
	
	if(squish)
		input = expand(input);
	
    vector<string> inputVector, seperatedRandomVector;
    vector<int> seperatedcharacterVector, baseTenRandomVector, randomVector, randomBinaryVector,deCryptedBinaryVector, deCryptedIntVector;
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

//Shortens the encrypted message created by the d0s1 encryption.
string D3CEncrypt::squish(string input)
{
	string output;
	long size = input.size();
	string loopString;
	
	for (long i=0; i<size ; i++)
	{
		for(int j=0;j<3;j++){
			loopString += input[i+j];
			//cout << i << "(" << j << ")" << ": " << loopString << endl; //for troubleshooting.
		}
		
		if(loopString == "000"){
			output += "q";
		} else if(loopString == "001"){
			output += "p";
		} else if(loopString == "002"){
			output += "M";
		} else if(loopString == "010"){
			output += "U";
		} else if(loopString == "011"){
			output += "o";
		} else if(loopString == "012"){
			output += "s";
		} else if(loopString == "020"){
			output += "r";
		} else if(loopString == "021"){
			output += "L";
		} else if(loopString == "022"){
			output += "T";
		} else if(loopString == "100"){
			output += "a";
		} else if(loopString == "101"){
			output += "N";
		} else if(loopString == "102"){
			output += "n";
		} else if(loopString == "110"){
			output += "x";
		} else if(loopString == "111"){
			output += "#";
		} else if(loopString == "112"){
			output += "f";
		} else if(loopString == "120"){
			output += "c";
		} else if(loopString == "121"){
			output += "w";
		} else if(loopString == "122"){
			output += "V";
		} else if(loopString == "200"){
			output += "v";
		} else if(loopString == "201"){
			output += "E";
		} else if(loopString == "202"){
			output += "D";
		} else if(loopString == "210"){
			output += "m";
		} else if(loopString == "211"){
			output += "e";
		} else if(loopString == "212"){
			output += "u";
		} else if(loopString == "220"){
			output += "$";
		} else if(loopString == "221"){
			output += "b";
		} else if(loopString == "222"){
			output += "@";
		} else if(loopString == "003"){
			output += "K";
		} else if(loopString == "013"){
			output += "t";
		} else if(loopString == "023"){
			output += "l";
		} else if(loopString == "033"){
			output += "W";
		} else if(loopString == "103"){
			output += "P";
		} else if(loopString == "113"){
			output += "J";
		} else if(loopString == "123"){
			output += "I";
		} else if(loopString == "133"){
			output += "y";
		} else if(loopString == "203"){
			output += "d";
		} else if(loopString == "213"){
			output += "!";
		} else if(loopString == "223"){
			output += "k";
		} else if(loopString == "233"){
			output += "C";
		} else if(loopString == "303"){
			output += "z";
		} else if(loopString == "313"){
			output += "O";
		} else if(loopString == "323"){
			output += "g";
		} else if(loopString == "333"){
			output += "F";
		} else if(loopString == "004"){
			output += "j";
		} else if(loopString == "014"){
			output += "Z";
		} else if(loopString == "024"){
			output += "G";
		} else if(loopString == "034"){
			output += "X";
		} else if(loopString == "044"){
			output += "H";
		} else if(loopString == "104"){
			output += "h";
		} else if(loopString == "114"){
			output += "Q";
		} else if(loopString == "124"){
			output += "R";
		} else if(loopString == "134"){
			output += "S";
		} else if(loopString == "144"){
			output += "i";
		} else if(loopString == "204"){
			output += "Y";
		} else if(loopString == "214"){
			output += "K";
		} else if(loopString == "224"){
			output += "%";
		} else if(loopString == "234"){
			output += "^";
		} else if(loopString == "244"){
			output += "&";
		} else if(loopString == "304"){
			output += "*";
		} else if(loopString == "314"){
			output += "(";
		} else if(loopString == "324"){
			output += ")";
		} else if(loopString == "334"){
			output += "-";
		} else if(loopString == "344"){
			output += "_";
		} else if(loopString == "404"){
			output += "=";
		} else if(loopString == "414"){
			output += "+";
		} else if(loopString == "424"){
			output += "[";
		} else if(loopString == "434"){
			output += "]";
		} else if(loopString == "444"){
			output += ":";
		} else if(loopString == "005"){
			output += "'";
		} else if(loopString == "015"){
			output += ";";
		} else if(loopString == "025"){
			output += "<";
		} else if(loopString == "035"){
			output += ",";
		} else if(loopString == "045"){
			output += ">";
		} else if(loopString == "055"){
			output += ".";
		} else if(loopString == "505"){
			output += "/";
		} else if(loopString == "515"){
			output += "?";
		} else if(loopString == "525"){
			output += "|";
		} else if(loopString == "535"){
			output += "~";
		} else {
			output += loopString[0];
			i-=2;
		}
		
		//cout << output << endl; //for troubleshooting.	
		loopString = "";
		i+=2;
	}
	
	return output;
}

//Shortens the encrypted message created by the d0s1 encryption.
string D3CEncrypt::expand(string input){
	string output;
	long size = input.size();
	
	for (long i=0; i<size ; i++){		
		if(input[i] =='q'){
			output += "000";
		} else if(input[i] == 'p'){
			output += "001";
		} else if(input[i] == 'M'){
			output += "002";
		} else if(input[i] == 'U'){
			output += "010";
		} else if(input[i] == 'o'){
			output += "011";
		} else if(input[i] == 's'){
			output += "012";
		} else if(input[i] == 'r'){
			output += "020";
		} else if(input[i] == 'L'){
			output += "021";
		} else if(input[i] == 'T'){
			output += "022";
		} else if(input[i] == 'a'){
			output += "100";
		} else if(input[i] == 'N'){
			output += "101";
		} else if(input[i] == 'n'){
			output += "102";
		} else if(input[i] == 'x'){
			output += "110";
		} else if(input[i] == '#'){
			output += "111";
		} else if(input[i] == 'f'){
			output += "112";
		} else if(input[i] == 'c'){
			output += "120";
		} else if(input[i] == 'w'){
			output += "121";
		} else if(input[i] == 'V'){
			output += "122";
		} else if(input[i] == 'v'){
			output += "200";
		} else if(input[i] == 'E'){
			output += "201";
		} else if(input[i] == 'D'){
			output += "202";
		} else if(input[i] == 'm'){
			output += "210";
		} else if(input[i] == 'e'){
			output += "211";
		} else if(input[i] == 'u'){
			output += "212";
		} else if(input[i] == '$'){
			output += "220";
		} else if(input[i] == 'b'){
			output += "221";
		} else if(input[i] == '@'){
			output += "222";
		} else if(input[i] == 'K'){
			output += "003";
		} else if(input[i] == 't'){
			output += "013";
		} else if(input[i] == 'l'){
			output += "023";
		} else if(input[i] == 'W'){
			output += "033";
		} else if(input[i] == 'p'){
			output += "103";
		} else if(input[i] == 'J'){
			output += "113";
		} else if(input[i] == 'I'){
			output += "123";
		} else if(input[i] == 'y'){
			output += "133";
		} else if(input[i] == 'd'){
			output += "203";
		} else if(input[i] == '!'){
			output += "213";
		} else if(input[i] == 'k'){
			output += "223";
		} else if(input[i] == 'C'){
			output += "233";
		} else if(input[i] == 'z'){
			output += "303";
		} else if(input[i] == 'O'){
			output += "313";
		} else if(input[i] == 'g'){
			output += "323";
		} else if(input[i] == 'F'){
			output += "333";
		} else if(input[i] == 'j'){
			output += "004";
		} else if(input[i] == 'Z'){
			output += "014";
		} else if(input[i] == 'G'){
			output += "024";
		} else if(input[i] == 'X'){
			output += "034";
		} else if(input[i] == 'H'){
			output += "044";
		} else if(input[i] == 'h'){
			output += "104";
		} else if(input[i] == 'Q'){
			output += "114";
		} else if(input[i] == 'R'){
			output += "124";
		} else if(input[i] == 'S'){
			output += "134";
		} else if(input[i] == 'i'){
			output += "144";
		} else if(input[i] == 'Y'){
			output += "204";
		} else if(input[i] == 'K'){
			output += "214";
		} else if(input[i] == '%'){
			output += "224";
		} else if(input[i] == '^'){
			output += "234";
		} else if(input[i] == '&'){
			output += "244";
		} else if(input[i] == '*'){
			output += "304";
		} else if(input[i] == '('){
			output += "314";
		} else if(input[i] == ')'){
			output += "324";
		} else if(input[i] == '-'){
			output += "334";
		} else if(input[i] == '_'){
			output += "344";
		} else if(input[i] == '='){
			output += "404";
		} else if(input[i] == '+'){
			output += "414";
		} else if(input[i] == '['){
			output += "424";
		} else if(input[i] == ']'){
			output += "434";
		} else if(input[i] == ':'){
			output += "444";
		} else if(input[i] == '\''){
			output += "005";
		} else if(input[i] == ';'){
			output += "015";
		} else if(input[i] == '<'){
			output += "025";
		} else if(input[i] == ','){
			output += "035";
		} else if(input[i] == '>'){
			output += "045";
		} else if(input[i] == '.'){
			output += "055";
		} else if(input[i] == '/'){
			output += "505";
		} else if(input[i] == '?'){
			output += "515";
		} else if(input[i] == '|'){
			output += "525";
		} else if(input[i] == '~'){
			output += "535";
		}else {
			output += input[i];
		}
		//cout << output << endl; //for troubleshooting.	
	}
	
	return output;
}

void D3CEncrypt::d0s1CryptRunner()
{
    cout << "..." << endl;
    cout << "...Please enter a string to encrypt: " << endl;
    cout << endl;

    string input, output;
    getline(std::cin,input);
    output = Crypt(input, true);

    cout << endl;
    cout << "...The encrypted output is: " << endl;
    cout << endl;
    cout << output << endl;
}

void D3CEncrypt::d0s1DeCryptRunner()
{
    cout << "..." << endl;
    cout << "...Please enter a string to de-crypt: " << endl;
    cout << endl;

    string input, output;
    getline(std::cin,input);
    output = DeCrypt(input, true);

    cout << endl;
    cout << "...The de-crypted output is: " << endl;
    cout << endl;
    cout << output << endl;
}
