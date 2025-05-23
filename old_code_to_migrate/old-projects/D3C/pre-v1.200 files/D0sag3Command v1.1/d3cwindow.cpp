#include <string>
#include "d3cwindow.h"
#include "ui_d3cwindow.h"
#include "D3CEncrypt.h"
#include "D3CEncryptPW.h"
#include "D3CPrompt.h"
#include "D3CConversation.h"
#include "D3CHelp.h"
#include "D3CMath.h"
#include "D3CProgramConstants.h"
#include "D3CConvert.h"
#include <fstream>
#include <sstream>


std::D3CProgramConstants program;

D3CWindow::D3CWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::D3CWindow){
    ui->setupUi(this);
    QString text = "<P><b><FONT COLOR='#008bb1'>";
    text.append("D0sag3 Command version " + program.getProgramVersion());
    text.append("</P></b>");
    ui->D3CHomeTextTitle->setText(text);
}

D3CWindow::~D3CWindow(){
    delete ui;
}

void D3CWindow::hideWindowItems(){
    hideHome();
    hideEncryption();
    hideAdvanced();
    hideHelp();
    hideMassConversion();
    hideChangeLog();
    hideConversionUnits();
    hideKinematics1D();
    hideEncryptionPW();
    ui->editorBox->hide();
}

void D3CWindow::hideHome(){
    ui->homeText->hide();
    ui->D3CHomeTextTitle->hide();
}

void D3CWindow::showConversionUnits(){
    ui->conversionUnitTitle->show();
    ui->ConversionUnitText->show();
}

void D3CWindow::hideConversionUnits(){
    ui->conversionUnitTitle->hide();
    ui->ConversionUnitText->hide();
}

void D3CWindow::showChangeLog(){
    ui->D3CChangeLogText->show();
    ui->D3CChangeLogTitle->show();
}

void D3CWindow::hideChangeLog(){
    ui->D3CChangeLogText->hide();
    ui->D3CChangeLogTitle->hide();
}

void D3CWindow::hideEncryption(){
    ui->encryptionTitle->hide();
    ui->encryptButton->hide();
    ui->decryptButton->hide();
    ui->encryptText->hide();
    ui->decryptText->hide();
    ui->cryptResetButton->hide();
}

void D3CWindow::hideAdvanced(){
    ui->D3CPromptText->hide();
    ui->D3CPromptTitle->hide();
    ui->D3CPromptLineEnter->hide();
    ui->D3CPromptEnter->hide();
    ui->D3CPromptProgress->hide();
    ui->D3CPromptLineEnter->hide();
    ui->D3CPromptText->clear();
    ui->D3CPromptLineEnter->clear();
    ui->D3CPromptReset->hide();
}

void D3CWindow::hideMassConversion(){
    ui->massConvertTitle->hide();
    ui->massamu->hide();
    ui->massamuText->hide();
    ui->masscarat->hide();
    ui->masscaratText->hide();
    ui->massConvertButton->hide();
    ui->massdram->hide();
    ui->massdramText->hide();
    ui->massdwt->hide();
    ui->massdwtText->hide();
    ui->massg->hide();
    ui->massgText->hide();
    ui->massgr->hide();
    ui->massgrText->hide();
    ui->masslb->hide();
    ui->masslbText->hide();
    ui->masslcwt->hide();
    ui->masslcwtText->hide();
    ui->masslqtr->hide();
    ui->masslqtrText->hide();
    ui->massLT->hide();
    ui->massLTText->hide();
    ui->massmg->hide();
    ui->massmgText->hide();
    ui->massoz->hide();
    ui->massozText->hide();
    ui->massscwt->hide();
    ui->massscwtText->hide();
    ui->massst->hide();
    ui->massstText->hide();
    ui->massST->hide();
    ui->massSTText->hide();
    ui->masstlb->hide();
    ui->masstlbText->hide();
    ui->masstonne->hide();
    ui->masstonneText->hide();
    ui->masstoz->hide();
    ui->masstozText->hide();
    ui->massug->hide();
    ui->massugText->hide();
    ui->masskg->hide();
    ui->masskgText->hide();
    ui->massMT->hide();
    ui->massMTText->hide();
    ui->convertMassReset->hide();
    ui->convertMassFunctions->hide();
    ui->ConvertImage->hide();
}

void D3CWindow::showMassConversion(){
    ui->massConvertTitle->show();
    ui->massamu->show();
    ui->massamuText->show();
    ui->masscarat->show();
    ui->masscaratText->show();
    ui->massConvertButton->show();
    ui->massdram->show();
    ui->massdramText->show();
    ui->massdwt->show();
    ui->massdwtText->show();
    ui->massg->show();
    ui->massgText->show();
    ui->massgr->show();
    ui->massgrText->show();
    ui->masslb->show();
    ui->masslbText->show();
    ui->masslcwt->show();
    ui->masslcwtText->show();
    ui->masslqtr->show();
    ui->masslqtrText->show();
    ui->massLT->show();
    ui->massLTText->show();
    ui->massmg->show();
    ui->massmgText->show();
    ui->massoz->show();
    ui->massozText->show();
    ui->massscwt->show();
    ui->massscwtText->show();
    ui->massst->show();
    ui->massstText->show();
    ui->massST->show();
    ui->massSTText->show();
    ui->masstlb->show();
    ui->masstlbText->show();
    ui->masstonne->show();
    ui->masstonneText->show();
    ui->masstoz->show();
    ui->masstozText->show();
    ui->massug->show();
    ui->massugText->show();
    ui->masskg->show();
    ui->masskgText->show();
    ui->massMT->show();
    ui->massMTText->show();
    ui->convertMassReset->show();
    ui->convertMassFunctions->show();
    ui->ConvertImage->show();
}

void D3CWindow::showHome(){
    ui->homeImage->show();
    ui->homeText->show();
    ui->D3CHomeTextTitle->show();
}

void D3CWindow::showEncryption(){
    ui->encryptionTitle->show();
    ui->encryptButton->show();
    ui->decryptButton->show();
    ui->encryptText->show();
    ui->decryptText->show();
    ui->cryptResetButton->show();
}

void D3CWindow::showEncryptionPW(){
    ui->encryptionTitlePW->show();
    ui->encryptButtonPW->show();
    ui->decryptButtonPW->show();
    ui->encryptTextPW->show();
    ui->decryptTextPW->show();
    ui->cryptResetButtonPW->show();
    ui->encryptPW->show();
    ui->encryptPWLabel->show();
}

void D3CWindow::hideEncryptionPW(){
    ui->encryptionTitlePW->hide();
    ui->encryptButtonPW->hide();
    ui->decryptButtonPW->hide();
    ui->encryptTextPW->hide();
    ui->decryptTextPW->hide();
    ui->cryptResetButtonPW->hide();
    ui->encryptPW->hide();
    ui->encryptPWLabel->hide();
}

void D3CWindow::showAdvanced(){
    ui->D3CPromptText->show();
    ui->D3CPromptTitle->show();
    ui->D3CPromptLineEnter->show();
    ui->D3CPromptEnter->show();
    ui->D3CPromptProgress->show();
    ui->D3CPromptLineEnter->show();
    ui->D3CPromptText->setText(program.getD3CPromptStart());
    ui->D3CPromptReset->show();
}

void D3CWindow::showHelp(){
    ui->helpText->show();
    ui->helpTitle->show();
}

void D3CWindow::hideHelp(){
    ui->helpText->hide();
    ui->helpTitle->hide();
}

void D3CWindow::on_actionHome_triggered(){
    hideWindowItems();
    showHome();
}

void D3CWindow::on_actionExit_triggered(){
    qApp->quit();
}

void D3CWindow::on_actionD0s_triggered(){
    hideWindowItems();
    showEncryption();
}

void D3CWindow::on_actionD3C_prompt_triggered(){
    hideWindowItems();
    showAdvanced();
}

void D3CWindow::on_actionAvailable_Functions_triggered(){
    hideWindowItems();
    showHelp();
}

void D3CWindow::on_actionMass_triggered(){
    hideWindowItems();
    showMassConversion();
}

bool cryptingMessage = false;
bool decryptingMessage = false;
bool multiplication = false;
bool multiplication2 = false;
bool addition = false;
bool addition2 = false;
bool subtraction = false;
bool subtraction2 = false;
bool palindrome = false;
bool digitSum = false;
bool numberOfFactors = false;
bool isPrime = false;
bool primeN = false;
bool isTriangleNumber = false;
bool collatz = false;
bool latticePaths = false;
bool latticePaths2 = false;
bool primeFactors = false;
bool consecutiveDigitProduct = false;
bool consecutiveDigitProduct2 = false;
std::string intOne, intTwo;
long integerOne;

void D3CWindow::primePopulateProgressBar(int percent){
    ui->D3CPromptProgress->setValue(percent);
    ui->D3CPromptProgress->update();
}

void D3CWindow::on_D3CPromptEnter_clicked(){
    QString function = ui->D3CPromptLineEnter->text();
    ui->D3CPromptText->moveCursor(QTextCursor::End);
    ui->D3CPromptText->insertPlainText("\n");
    ui->D3CPromptText->insertPlainText(function);
    ui->D3CPromptLineEnter->clear();


    std::D3CPrompt prompt;
    std::D3CEncrypt crypt;
    std::D3CMath math;
    int variable = prompt.caseDecider(function.toStdString());
    std::string output;

    if(cryptingMessage){
        variable = 1;
    }else if(decryptingMessage){
        variable = 2;
    }else if(multiplication || multiplication2){
        variable = 3;
    }else if(addition || addition2){
        variable = 4;
    }else if(subtraction || subtraction2){
        variable = 9;
    }else if(palindrome){
        variable = 12;
    }else if(digitSum){
        variable = 13;
    }else if(numberOfFactors){
        variable = 14;
    }else if(isPrime){
        variable = 10;
    }else if(isTriangleNumber){
        variable = 15;
    }else if(collatz){
        variable = 21;
    }else if(latticePaths){
        variable = 22;
    }else if(primeN){
        variable = 16;
    }else if(primeFactors){
        variable = 19;
    }else if (consecutiveDigitProduct || consecutiveDigitProduct2){
        variable = 20;
    }

    switch( variable ){
    case 1: //cryptingMessage
    {
        if(!cryptingMessage){
            ui->D3CPromptText->insertPlainText("\nEnter message to Crypt: ");
            cryptingMessage = true;
        }else{
            output = crypt.Crypt(function.toStdString());
            QString Qtext = QString::fromStdString(output);
            ui->D3CPromptText->insertPlainText("\nd0sag3 Crypt: \n" + Qtext);
            cryptingMessage = false;
        }
        break;
    }
    case 2: //decryptingMessage
    {
        if(!decryptingMessage){
            ui->D3CPromptText->insertPlainText("\nEnter message to Crypt: ");
            decryptingMessage = true;
        }else{
            output = crypt.DeCrypt(function.toStdString());
            QString Qtext = QString::fromStdString(output);
            ui->D3CPromptText->insertPlainText("\nMessage: \n" + Qtext);
            decryptingMessage = false;
        }
        break;
    }
    case 3: //multiplication || multiplication2
    {
        if(!multiplication){
            ui->D3CPromptText->insertPlainText("\n--Multiplication of Integers of any size--\nEnter first integer to multiply: ");
            multiplication = true;
        }else if(!multiplication2){
            intOne = function.toStdString();
            ui->D3CPromptText->insertPlainText("\nEnter second integer to multiply: ");
            multiplication2 = true;
        }else{
            intTwo = function.toStdString();
            output = math.multiplyTwoStrings(intOne, intTwo);
            QString Qtext = QString::fromStdString(output);
            ui->D3CPromptText->insertPlainText("\nInteger product: " + Qtext);
            multiplication = false;
            multiplication2 = false;
            ui->D3CPromptText->insertPlainText("\nEnter Command: ");
        }
        break;
    }
    case 4: //addition || addition2
    {
        if(!addition){
            ui->D3CPromptText->insertPlainText("\n--Addition of integers of any size--\nEnter first integer to add: ");
            addition = true;
        }else if(!addition2){
            intOne = function.toStdString();
            ui->D3CPromptText->insertPlainText("\nEnter second integer to add: ");
            addition2 = true;
        }else{
            intTwo = function.toStdString();
            output = math.addTwoStrings(intOne, intTwo);
            QString Qtext = QString::fromStdString(output);
            ui->D3CPromptText->insertPlainText("\nInteger sum: " + Qtext);
            addition = false;
            addition2 = false;
            ui->D3CPromptText->insertPlainText("\nEnter Command: ");
        }
        break;
    }
    case 5:
    {
        qApp->quit();
    }
    case 6:
    {
        std::D3CHelp help;
        output = help.helpRunner();
        QString Qtext = QString::fromStdString("\n" + output);
        ui->D3CPromptText->insertPlainText(Qtext);
        ui->D3CPromptText->insertPlainText("\nEnter Command:");
        break;
    }
    case 7:
    {
        ui->D3CPromptText->insertPlainText("\nKinematics not yet programmed into the GUI version");
//        D3CKinematics.kinematicsRunner();
        break;
    }
    case 8:
    {
          ui->D3CPromptText->insertPlainText("\nThis feature is not yet finished in the GUI version");
//        D3CConvert.convertRunner();
        break;
    }
    case 9: //subtraction || subtraction2
    {
        if(!subtraction){
            ui->D3CPromptText->insertPlainText("\n--Subtraction of Integers of any size--\nEnter first Integer: ");
            subtraction = true;
        }else if(!subtraction2){
            intOne = function.toStdString();
            ui->D3CPromptText->insertPlainText("\nEnter second Integer to subtract from first: ");
            subtraction2 = true;
        }else{
            intTwo = function.toStdString();
            output = math.subtractTwoStrings(intOne, intTwo);
            QString Qtext = QString::fromStdString(output);
            ui->D3CPromptText->insertPlainText("\nInteger product: " + Qtext);
            subtraction = false;
            subtraction2 = false;
            ui->D3CPromptText->insertPlainText("\nEnter Command: ");
        }
        break;
    }
    case 10: //isPrime
    {
        if(!isPrime){
            ui->D3CPromptText->insertPlainText("\nEnter a positive Integer: ");
            isPrime = true;
        }else{
            output = function.toStdString();
            long longInput = atol(output.c_str());
            if(math.isPrime(longInput)){
                ui->D3CPromptText->insertPlainText("\nThe number you entered is prime.");
            }else{
                 ui->D3CPromptText->insertPlainText("\nThe number you entered is not prime.");
            }
            isPrime = false;
            ui->D3CPromptText->insertPlainText("\nEnter Command: ");
        }
        break;
    }
    case 11: //help extended
    {
        std::D3CHelp help;
        output = help.helpRunnerExtended();
        QString Qtext = QString::fromStdString("\n" + output);
        ui->D3CPromptText->insertPlainText(Qtext);
        ui->D3CPromptText->insertPlainText("\nEnter Command:");
        break;
    }
    case 12: //palindrome
    {
        if(!palindrome){
            ui->D3CPromptText->insertPlainText("\nEnter a positive integer: ");
            palindrome = true;
        }else{
            output = function.toStdString();
            long longInput = std::atol(output.c_str());
            if(math.isPalindrome(longInput)){
               ui->D3CPromptText->insertPlainText("\nThe number you entered is palindrome.");
            }else{
               ui->D3CPromptText->insertPlainText("\nThe number you entered is not palindrome.");
            }
            ui->D3CPromptText->insertPlainText("\nEnter Command:");
            palindrome = false;
        }
        break;
    }
    case 13: //digitSum
    {
        if(!digitSum){
            ui->D3CPromptText->insertPlainText("\nEnter a positive Integer of any length: ");
            digitSum = true;
        }else{
            output = function.toStdString();
            ui->D3CPromptText->insertPlainText("Digit sum: " + math.sumOfDigits(output));
            digitSum = false;
            ui->D3CPromptText->insertPlainText("\nEnter Command:");
        }
        break;
    }
    case 14: //numberOfFactors
    {
        if(!numberOfFactors){
            ui->D3CPromptText->insertPlainText("\nEnter a positive Integer: ");
            numberOfFactors = true;
        }else{
            output = function.toStdString();
            long longInput = std::atol(output.c_str());
            int factors = math.numberOfFactors(longInput);

            std::stringstream strstream;
            strstream << factors;
            strstream >> output;

            ui->D3CPromptText->insertPlainText("\nNumber of Factors: " + QString::fromStdString(output));
            numberOfFactors = false;
            ui->D3CPromptText->insertPlainText("\nEnter Command:");
        }
        break;
    }
    case 15: //isTriangleNumber
    {
        if(!isTriangleNumber){
            ui->D3CPromptText->insertPlainText("\nEnter a positive Integer: ");
            isTriangleNumber = true;
        }else{
            output = function.toStdString();
            long longInput = atol(output.c_str());
            if(math.isTriangleNumber(longInput)){
                ui->D3CPromptText->insertPlainText("\nThe number you entered is a triangle number.");
            }else{
                 ui->D3CPromptText->insertPlainText("\nThe number you entered is not a triangle number.");
            }
            ui->D3CPromptText->insertPlainText("\nEnter Command: ");
            isTriangleNumber = false;
        }
        break;
    }
    case 16: //primeN
    {
        if(!primeN){
            ui->D3CPromptText->insertPlainText("\nEnter a positive integer: ");
            primeN = true;
        }else{
            output = function.toStdString();
            long longInput = std::atol(output.c_str());
            long longOutput = math.primeNumberN(longInput);
            primeN = false;
            ui->D3CPromptText->insertPlainText("\nPrime number " + QString::number(longInput) + ": " + QString::number(longOutput));
            ui->D3CPromptText->insertPlainText("\nEnter Command: ");
        }
        break;
    }
    case 17: //primeNpopulate
    {
        ui->D3CPromptText->insertPlainText("May cause a program crash, fix not yet programmed.");
        math.primeNumberNpopulate();
        break;
    }
    case 18: //primeNerase
	{
        math.primeNumberNerase();
        break;
    }
    case 19: //prime factors
    {
        if(!primeFactors){
            ui->D3CPromptText->insertPlainText("\nEnter a positive integer: ");
            primeFactors = true;
        }else{
            output = function.toStdString();
            long longInput = std::atol(output.c_str());

            ui->D3CPromptText->insertPlainText("\nThe prime factors are: \n");
            for (long i = 1; i <= (longInput + 1) / 2; i++) {
                if (math.isPrime(i) == true && longInput % i == 0){
                    ui->D3CPromptText->insertPlainText(QString::number(i) + " ");
                }
            }
            ui->D3CPromptText->insertPlainText("\nEnter Command: ");
        }
        break;
    }
    case 20:
    {
        if(!consecutiveDigitProduct){
            ui->D3CPromptText->insertPlainText("\nEnter a positive integer of any size: ");
            consecutiveDigitProduct = true;
        }else if(!consecutiveDigitProduct2){
            intOne = function.toStdString();
            ui->D3CPromptText->insertPlainText("\nEnter the number of consecutive numbers you would like to find the largest product of: ");
            consecutiveDigitProduct2 = true;
        }else{
            output = function.toStdString();
            long longInput = std::atol(output.c_str());
            long longOut = math.largestProductOfXConsecutiveNumbers(intOne, longInput);
            ui->D3CPromptText->insertPlainText("\nThe largest product of " + QString::number(longInput) + " numbers is " + QString::number(longOut));
            consecutiveDigitProduct = false;
            consecutiveDigitProduct2 = false;
            ui->D3CPromptText->insertPlainText("\nEnter Command: ");
        }
        break;
    }
    case 21: //collatz
	{
		{
          if(!collatz){
                ui->D3CPromptText->insertPlainText("\nEnter a starting number (positive integer): ");
                collatz = true;
            }else{
                output = function.toStdString();
                long longInput = atol(output.c_str());
                ui->D3CPromptText->insertPlainText("\n" + function + " ");
                output = math.productCollatzSequence(longInput);
                QString Qtext = QString::fromStdString(output);
                ui->D3CPromptText->insertPlainText(Qtext);
                ui->D3CPromptText->insertPlainText("\nCollatz sequence complete.");
                collatz = false;
            }
        }
        break;
	}
    case 22: //latticePaths
    {
        if(!latticePaths){
            ui->D3CPromptText->insertPlainText("\nEnter grid size in horizontal direction: ");
            latticePaths = true;
        }else if(!latticePaths2){
            integerOne = std::atol(function.toStdString().c_str());
            ui->D3CPromptText->insertPlainText("\nEnter grid size in virtical direction: ");
            latticePaths2 = true;
        }else{
            long integerTwo = std::atol(function.toStdString().c_str());
            long outputLong = math.latticePathsOfSquare(integerOne, integerTwo);

            std::stringstream strstream;
            strstream << outputLong;
            strstream >> output;

            ui->D3CPromptText->insertPlainText("\nThe total lattice paths are " + QString::fromStdString(output));
            latticePaths = false;
            latticePaths2 = false;
        }
        break;
    }
    default:
        std::D3CConversation converse;
        if(converse.isTerm(function.toStdString())){
            std::string text = converse.reply(function.toStdString());
            QString Qtext = QString::fromStdString("\n" + text);
            ui->D3CPromptText->insertPlainText(Qtext);
        }else{
            ui->D3CPromptText->insertPlainText("\nYou entered an invalid command: " + function);
            ui->D3CPromptText->insertPlainText("\nEnter Command: ");

        }
        break;
    }
//    QScrollBar *scroll = ui->D3CPromptText->verticalScrollBar();
//    scroll->setValue(scroll->maximum());
}

void D3CWindow::on_encryptButton_clicked(){
    std::D3CEncrypt D3Ccrypt;
    QString function = ui->encryptText->toPlainText();
    if(!function.isEmpty()){
        std::string encrypted = D3Ccrypt.Crypt(function.toStdString());
        QString Qencrypted = QString::fromStdString(encrypted);
        ui->decryptText->setPlainText(Qencrypted);
    }
}

void D3CWindow::on_decryptButton_clicked(){
    std::D3CEncrypt D3Ccrypt;
    QString function = ui->decryptText->toPlainText();
    if(!function.isEmpty()){
        std::string decrypted = D3Ccrypt.DeCrypt(function.toStdString());
        QString Qdecrypted = QString::fromStdString(decrypted);
        ui->encryptText->setPlainText(Qdecrypted);
    }
}

void D3CWindow::on_cryptResetButton_clicked()
{
    ui->encryptText->setText("Enter message to encrypt...");
    ui->decryptText->setText("Enter message to decrypt...");
}

void D3CWindow::on_D3CPromptLineEnter_returnPressed()
{
    ui->D3CPromptEnter->click();
    ui->D3CPromptLineEnter->clear();
}

void D3CWindow::on_convertMassReset_clicked()
{
    ui->massamuText->clear();
    ui->masscaratText->clear();
    ui->massdramText->clear();
    ui->massdwtText->clear();
    ui->massgText->clear();
    ui->massgrText->clear();
    ui->masslbText->clear();
    ui->masslcwtText->clear();
    ui->masslqtrText->clear();
    ui->massLTText->clear();
    ui->massmgText->clear();
    ui->massozText->clear();
    ui->massscwtText->clear();
    ui->massstText->clear();
    ui->massSTText->clear();
    ui->masstlbText->clear();
    ui->masstonneText->clear();
    ui->masstozText->clear();
    ui->massugText->clear();
    ui->masskgText->clear();
    ui->massMTText->clear();

    ui->convertMassFunctions->setText("Enter one mass to convert.");
}

void D3CWindow::on_actionChange_Log_triggered(){
    hideWindowItems();
    showChangeLog();
    std::ifstream myfile;
    std::string text;
    myfile.open("../build-D0sag3Command-Desktop_Qt_5_3_MinGW_32bit-Release/D3CCL.txt");
    if(myfile.is_open()){
        while(std::getline(myfile, text)){
           ui->D3CChangeLogText->insertPlainText(QString::fromStdString(text) + "\n");
        }
    }
}

void D3CWindow::on_actionConversion_Units_triggered(){
    hideWindowItems();
    showConversionUnits();
    std::D3CConvert convert;
    std::string text = convert.convertHelp("help");
    QString Qtext = QString::fromStdString(text);
    ui->ConversionUnitText->setText(Qtext);
}

bool D3CWindow::convertMultipleEntries(){
    int counter = 0;
    while (counter < 2){
    if(ui->massamuText->text() != NULL){
        counter ++;
    }
    if(ui->masscaratText->text() != NULL){
        counter ++;
    }
    if(ui->massdramText->text() != NULL){
        counter ++;
    }
    if(ui->massdwtText->text() != NULL){
        counter ++;
    }
    if(ui->massgText->text() != NULL){
        counter ++;
    }
    if(ui->massgrText->text() != NULL){
        counter ++;
    }
    if(ui->masslbText->text() != NULL){
        counter ++;
    }
    if(ui->masslcwtText->text() != NULL){
        counter ++;
    }
    if(ui->masslqtrText->text() != NULL){
        counter ++;
    }
    if(ui->massLTText->text() != NULL){
        counter ++;
    }
    if(ui->massmgText->text() != NULL){
        counter ++;
    }
    if(ui->massozText->text() != NULL){
        counter ++;
    }
    if(ui->massscwtText->text() != NULL){
        counter ++;
    }
    if(ui->massstText->text() != NULL){
        counter ++;
    }
    if(ui->massSTText->text() != NULL){
        counter ++;
    }
    if(ui->masstlbText->text() != NULL){
        counter ++;
    }
    if(ui->masstonneText->text() != NULL){
        counter ++;
    }
    if(ui->masstozText->text() != NULL){
        counter ++;
    }
    if(ui->massugText->text() != NULL){
        counter ++;
    }
    if(ui->masskgText->text() != NULL){
        counter ++;
    }
    break;
    }
    if(counter > 1){
        return true;
    }else{
        return false;
    }
}

void D3CWindow::on_massConvertButton_clicked()
{
    if(!convertMultipleEntries()){
        std::D3CConvert convert;
        bool test = false;
        QString number, units;

        while (!test){
        if(ui->massamuText->text() != NULL){
           number = ui->massamuText->text();
           units = "amu";
           test = true;
        }
        if(ui->masscaratText->text() != NULL){
            number = ui->masscaratText->text();
            units = "carat";
            test = true;
        }
        test = true;
        if(ui->massdramText->text() != NULL){
            number = ui->massdramText->text();
            units = "dram";
            test = true;
        }
        if(ui->massdwtText->text() != NULL){
            number = ui->massdwtText->text();
            units = "dwt";
            test = true;
        }
        if(ui->massgText->text() != NULL){
            number = ui->massgText->text();
            units = "g";
            test = true;
        }
        if(ui->massgrText->text() != NULL){
            number = ui->massgrText->text();
            units = "gr";
            test = true;
        }
        if(ui->masslbText->text() != NULL){
            number = ui->masslbText->text();
            units = "lb";
            test = true;
        }
        if(ui->masslcwtText->text() != NULL){
            number = ui->masslcwtText->text();
            units = "lcwt";
            test = true;
        }
        if(ui->masslqtrText->text() != NULL){
            number = ui->masslqtrText->text();
            units = "lqtr";
            test = true;
        }
        if(ui->massLTText->text() != NULL){
            number = ui->massLTText->text();
            units = "LT";
            test = true;
        }
        if(ui->massmgText->text() != NULL){
            number = ui->massmgText->text();
            units = "mg";
            test = true;
        }
        if(ui->massozText->text() != NULL){
            number = ui->massozText->text();
            units = "oz";
            test = true;
        }
        if(ui->massscwtText->text() != NULL){
            number = ui->massscwtText->text();
            units = "scwt";
            test = true;
        }
        if(ui->massstText->text() != NULL){
            number = ui->massstText->text();
            units = "st";
            test = true;
        }
        if(ui->massSTText->text() != NULL){
            number = ui->massSTText->text();
            units = "ST";
            test = true;
        }
        if(ui->masstlbText->text() != NULL){
            number = ui->masstlbText->text();
            units = "tlb";
            test = true;
        }
        if(ui->masstonneText->text() != NULL){
            number = ui->masstonneText->text();
            units = "tonne";
            test = true;
        }
        if(ui->masstozText->text() != NULL){
            number = ui->masstozText->text();
            units = "toz";
            test = true;
        }
        if(ui->massugText->text() != NULL){
            number = ui->massugText->text();
            units = "ug";
            test = true;
        }
        if(ui->masskgText->text() != NULL){
            number = ui->masskgText->text();
            units = "kg";
            test = true;
        }
        }
        std::string unitString = units.toStdString();
        double numberString = number.toDouble();
        ui->convertMassFunctions->clear();
        ui->convertMassFunctions->insertPlainText("Converting: " + number + units + "\n");

        convert.massConversion(numberString, unitString);

        QString mass;
        mass = QString::number(convert.getmassamu());
        ui->massamuText->setText(mass);
        mass = QString::number(convert.getmasscarat());
        ui->masscaratText->setText(mass);
        mass = QString::number(convert.getmassdram());
        ui->massdramText->setText(mass);
        mass = QString::number(convert.getmassdwt());
        ui->massdwtText-> setText(mass);
        mass = QString::number(convert.getmassg());
        ui->massgText-> setText(mass);
        mass = QString::number(convert.getmassgr());
        ui->massgrText-> setText(mass);
        mass = QString::number(convert.getmasslb());
        ui->masslbText-> setText(mass);
        mass = QString::number(convert.getmasslongcwt());
        ui->masslcwtText-> setText(mass);
        mass = QString::number(convert.getmasslongqtr());
        ui->masslqtrText-> setText(mass);
        mass = QString::number(convert.getmassLT());
        ui->massLTText-> setText(mass);
        mass = QString::number(convert.getmassmg());
        ui->massmgText-> setText(mass);
        mass = QString::number(convert.getmassoz());
        ui->massozText-> setText(mass);
        mass = QString::number(convert.getmassshortcwt());
        ui->massscwtText-> setText(mass);
        mass = QString::number(convert.getmassst());
        ui->massstText-> setText(mass);
        mass = QString::number(convert.getmassST());
        ui->massSTText-> setText(mass);
        mass = QString::number(convert.getmasslb());
        ui->masstlbText-> setText(mass);
        mass = QString::number(convert.getmasst());
        ui->masstonneText-> setText(mass);
        mass = QString::number(convert.getmasstroypound());
        ui->masstozText-> setText(mass);
        mass = QString::number(convert.getmassug());
        ui->massugText-> setText(mass);
        mass = QString::number(convert.getmasskg());
        ui->masskgText->setText(mass);
        mass = QString::number(convert.getmassMT());
        ui->massMTText->setText(mass);


    }else{
        ui->convertMassFunctions->insertPlainText("You entered values for multiple units.");
        ui->convertMassFunctions->insertPlainText("Please only enter one mass to convert.");
    }
}

void D3CWindow::on_D3CPromptReset_clicked()
{
    hideWindowItems();
    showAdvanced();
}

void D3CWindow::showKinematics1D(){
    ui->kinematics1DTitle->show();
    ui->Kinematics1D->show();
    ui->Kinematics1DResetButton->show();
    ui->Kinematics1DSolveButton->show();
    ui->Kinematics1DText->show();
    ui->Kinematics1D_2->show();
    ui->Kinematics1D_3->show();
    ui->Kinematics1D_4->show();
    ui->Kinematics1D_5->show();
    ui->Kinematics1D_6->show();
    ui->Kinematics1D_7->show();
    ui->Kinematics1D_8->show();
    ui->Kinematics1D_9->show();
    ui->Kinematics1D_10->show();
    ui->Kinematics1D_11->show();
    ui->Kinematics1D_12->show();
    ui->Kinematics1D_13->show();
    ui->Kinematics1D_14->show();
    ui->Kinematics1D_15->show();
    ui->Kinematics1D_16->show();
    ui->Kinematics1D_17->show();
    ui->Kinematics1D_18->show();
    ui->Kinematics1D_19->show();
    ui->Kinematics1D_20->show();
    ui->Kinematics1D_21->show();
    ui->Kinematics1D_22->show();
    ui->Kinematics1D_23->show();
    ui->Kinematics1D_24->show();
    ui->ConvertImage->show();
}

void D3CWindow::hideKinematics1D(){
    ui->kinematics1DTitle->hide();
    ui->Kinematics1D->hide();
    ui->Kinematics1DResetButton->hide();
    ui->Kinematics1DSolveButton->hide();
    ui->Kinematics1DText->hide();
    ui->Kinematics1D_2->hide();
    ui->Kinematics1D_3->hide();
    ui->Kinematics1D_4->hide();
    ui->Kinematics1D_5->hide();
    ui->Kinematics1D_6->hide();
    ui->Kinematics1D_7->hide();
    ui->Kinematics1D_8->hide();
    ui->Kinematics1D_9->hide();
    ui->Kinematics1D_10->hide();
    ui->Kinematics1D_11->hide();
    ui->Kinematics1D_12->hide();
    ui->Kinematics1D_13->hide();
    ui->Kinematics1D_14->hide();
    ui->Kinematics1D_15->hide();
    ui->Kinematics1D_16->hide();
    ui->Kinematics1D_17->hide();
    ui->Kinematics1D_18->hide();
    ui->Kinematics1D_19->hide();
    ui->Kinematics1D_20->hide();
    ui->Kinematics1D_21->hide();
    ui->Kinematics1D_22->hide();
    ui->Kinematics1D_23->hide();
    ui->Kinematics1D_24->hide();
}

void D3CWindow::on_action1D_Kinematics_triggered(){
    hideWindowItems();
    showKinematics1D();
}

void D3CWindow::on_actionHide_Windows_triggered(){
    hideWindowItems();
    ui->ConvertImage->show();
}

void D3CWindow::on_Kinematics1DResetButton_clicked()
{
    ui->Kinematics1DText->setText("Enter the known values above.");
    ui->Kinematics1D->clear();
    ui->Kinematics1D_2->clear();
    ui->Kinematics1D_3->clear();
    ui->Kinematics1D_4->clear();
    ui->Kinematics1D_5->clear();
    ui->Kinematics1D_6->clear();
    ui->Kinematics1D_7->clear();
    ui->Kinematics1D_8->clear();
    ui->Kinematics1D_9->clear();
    ui->Kinematics1D_10->clear();
    ui->Kinematics1D_11->clear();
    ui->Kinematics1D_12->clear();


}

void D3CWindow::on_actionD0s1_triggered(){
    hideWindowItems();
    showEncryptionPW();
}

void D3CWindow::on_encryptButtonPW_clicked()
{
    D3CEncryptPW encrypt;
    QString function = ui->encryptTextPW->toPlainText(), functionPW = ui->encryptPW->text();
    if(functionPW.isEmpty()){
        functionPW = "$@d0sag3_EmptyPW.fail_error555$$&**&()";
    }
    if(!function.isEmpty()){
        std::string encrypted = encrypt.EncryptPW(function.toStdString(), functionPW.toStdString());
        QString Qencrypted = QString::fromStdString(encrypted);
        ui->decryptTextPW->setPlainText(Qencrypted);
    }
}

void D3CWindow::on_cryptResetButtonPW_clicked()
{
    ui->encryptTextPW->setText("Enter message to encrypt...");
    ui->decryptTextPW->setText("Enter message to decrypt...");
    ui->encryptPW->clear();
}



void D3CWindow::on_decryptButtonPW_clicked()
{
    D3CEncryptPW encrypt;
    QString function = ui->decryptTextPW->toPlainText(), functionPW = ui->encryptPW->text();
    if(functionPW.isEmpty()){
        functionPW = "$@d0sag3_EmptyPW.fail_error555$$&**&()";
    }
    if(!function.isEmpty()){
        std::string decrypted = encrypt.DecryptPW(function.toStdString(), functionPW.toStdString());
        QString Qdecrypted = QString::fromStdString(decrypted);
        ui->encryptTextPW->setPlainText(Qdecrypted);
    }
}
