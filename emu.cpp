// Name: Piyush Ghegade
// Roll No: 2301AI52
//--------------------------------------------------------------------------//
#include <bits/stdc++.h>
using namespace std;

#define NUMBER_OF_INSTRUCTION_SET 19

int Memory[1 << 24]; // It is the main memory of code
vector<string> code; // This stores the code
vector<string> mnem(NUMBER_OF_INSTRUCTION_SET); // Contain all mnemonics
int A, B, PC, SP;
int index, cont, exec;
array<int, 2> change;
vector<int> out_mem(12,10); 


void instructionSetArchitecture()
{
    cout << "OpMachineCode Mnemonic Operand" << endl
         << "0              ldc      value " << endl
         << "1              adc      value " << endl
         << "2              ldl      value " << endl
         << "3              stl      value " << endl
         << "4              ldnl     value " << endl
         << "5              stnl     value " << endl
         << "6              add            " << endl
         << "7              sub            " << endl
         << "9              shr            " << endl
         << "10             adj      value " << endl
         << "11             a2sp           " << endl
         << "12             sp2a           " << endl
         << "13             call     offset" << endl
         << "14             return         " << endl
         << "15             brz      offset" << endl
         << "16             brlz     offset" << endl
         << "17             br       offset" << endl
         << "18             HALT           " << endl;
    return;
}

// Functions to perform as per Mnemonic

void SelectingInstructionSet(int opcode,int value){
    if(opcode == 0){
        B = A;
        A = value;
        return;       
    }
    else if(opcode == 1){
        A += value;
        return;  
    }
    else if(opcode == 2){
        exec = 1;
        B = A;
        A = Memory[SP + value];
        change = {SP + value, 0};
        return;  
    }
    else if(opcode == 3){
        exec = 2;
        change = {Memory[SP + value], A};
        Memory[SP + value] = A;
        A = B;
        return;  
    }
    else if(opcode == 4){
        exec = 1;
        A = Memory[A + value];
        change = {SP + value, 0};
        return;  
    }
    else if(opcode == 5){
        exec = 1;
        A = Memory[A + value];
        change = {SP + value, 0};
        return;  
    }
    else if(opcode == 6){
        A = B + A;
        return;  
    }
    else if(opcode == 7){
        A = B - A;
        return;  
    }
    else if(opcode == 8){
        A = B << A;
        return;  
    }
    else if(opcode == 9){
        A = B >> A;
        return;  
    }
    else if(opcode == 10){
        SP = SP + value;
        return;  
    }
    else if(opcode == 11){
        SP = A;
        A = B;
        return;  
    }
    else if(opcode == 12){
        B = A;
        A = SP;
        return;  
    }
    else if(opcode == 13){
        B = A;
        A = PC;
        PC = PC + value;
        return;  
    }
    else if(opcode == 14){
        PC = A;
        A = B;
        return;  
    }
    else if(opcode == 15){
        bool a = (A==0);
        if (!a)
        {
            return;
        }
        else
        {
            PC = PC + value;
        }
        return;  
    }
    else if(opcode == 16){
        bool a = (A>=0);
        if (!a)
        {
            return;
        }
        else
        {
            PC = PC + value;
        }
        return;  
    }
    else if(opcode == 17){
        PC = PC + value;
        return;  
    }
    return;
}
int ldc(int value)
{
    B = A;
    A = value;
    return 1;
}
int adc(int value)
{
    A += value;
    return 2;
}
int ldl(int offset)
{
    exec = 1;
    B = A;
    A = Memory[SP + offset];
    change = {SP + offset, 0};
    return 3;
}
int stl(int offset)
{
    exec = 2;
    change = {Memory[SP + offset], A};
    Memory[SP + offset] = A;
    A = B;
    return 4;
}
int ldnl(int offset)
{
    exec = 1;
    A = Memory[A + offset];
    change = {SP + offset, 0};
    return 5;
}
int stnl(int offset)
{
    exec = 2;
    change = {Memory[A + offset], B};
    Memory[A + offset] = B;
    return 6;
}
int add(int off)
{
    A = B + A;
    return 7;
}
int sub(int off)
{
    A = B - A;
    return 8;
}
int shl(int off)
{
    A = B << A;
    return 9;
}
int shr(int off)
{
    A = B >> A;
    return 10;
}
int adj(int value)
{
    SP = SP + value;
    return 11;
}
int a2sp(int off)
{
    SP = A;
    A = B;
    return 12;
}
int sp2a(int off)
{
    B = A;
    A = SP;
    return 13;
}
int call(int offset)
{
    B = A;
    A = PC;
    PC = PC + offset;
    return 14;
}
int ret(int offset)
{
    PC = A;
    A = B;
    return 15;
}
int brz(int offset)
{
    bool a = (A==0);
    if (!a)
    {
        return 16;
    }
    else
    {
        PC = PC + offset;
    }
    return 16;
}
int brlz(int offset)
{
    bool a = (A>=0);
    if (!a)
    {
        return 17;
    }
    else
    {
        PC = PC + offset;
    }
    return 17;
}
int br(int offset)
{
    PC = PC + offset;
    return 18;
}

int (*call_func[])(int) = {ldc, adc, ldl, stl, ldnl, stnl, add, sub, shl, shr, adj, a2sp, sp2a, call, ret, brz, brlz, br};

// Helping functions
// This function converts decimal to hex
string decToHex(unsigned int num)
{
    stringstream ss;
    string s;
    ss << hex << num;
    string hexStr = ss.str();
    reverse(hexStr.begin(), hexStr.end());

    int len = hexStr.size();
    while (len < 8)
    {
        hexStr.push_back('0');
        len++;
    }

    reverse(hexStr.begin(), hexStr.end());
    return hexStr;
}


bool checkFileType(string nameOfFile)
{
    int nameLength = nameOfFile.length();
    if(nameLength == 0){
        cout<<"Invalid File name"<<endl;
    }
    string extension = ".o";
    int extLength = extension.length();

    bool isLongEnough = (nameLength >= extLength);
    bool hasCorrectExtension = (nameOfFile.substr(nameLength - extLength) == extension);

    if (isLongEnough && hasCorrectExtension)
    {
        return true;
    }
    return false;
}


void showMem()
{
// Prints the memory in set of 4
int k = 0;
int len = code.size();
bool hasMore = (k < len);

while (hasMore)
{
    cout << decToHex(k) << " ";
    bool innerCondition = true;

    for (int j = k; innerCondition; j++)
    {
        cout << decToHex(Memory[j]) << " ";
        innerCondition = (j < min(k + 4, len) - 1);
    }

    k = k + 4;
    cout << endl;

    hasMore = (k < len);
}

return;

}

// Shows value stored in the registers at any point of time
void showReg()
{
    cout << "Value in PC: " << decToHex(PC + 1) << endl;
    cout << "Value in SP: " << decToHex(SP) << endl;
    cout << "Value in A: " << decToHex(A) << endl;
    cout << "Value in B: " << decToHex(B) << endl;
    return;
}

void HALTFOUND(int cont){
        cout << "HALT found" << '\n';
        cout << cont << " statements were executed total" << '\n';
}

// This runs the code or perform operations
bool runCode(int oper, int times = (1 << 25))
{
    int iter = 0;
    bool withinBounds = (times > 0 && PC < code.size());

    for (; withinBounds; times--)
    {
        cont++;
        bool segFault = (PC >= code.size() || cont > static_cast<int>(3e7));
        if (segFault)
        {
            cout << "ERROR !!! Segmentation Fault" << '\n';
            return false;
        }
        string cur = code[PC];
        int op = stoi(cur.substr(6, 2), nullptr, 16); // Hexadecimal Opcode
        bool haltFound = (op == 18);

        if (haltFound)
        {
            HALTFOUND(cont);
            return true;
        }

        int val = stoi(cur.substr(0, 6), nullptr, 16); // First 6 byte operand
        bool isNegative = (val >= (1 << 23));
        if (isNegative)
        {
            val -= (1 << 24);
        }

        bool zeroTimes = (times == 0);
        if (zeroTimes)
        {
            showReg();
        }

        int a = (call_func[op])(val);

        // SelectingInstructionSet(op,val);

        bool writeOperation = (oper == 2 && exec == 2);
        bool readOperation = (oper == 1 && exec == 1);

        if (writeOperation)
        {
            cout << "Writing memory[" << decToHex(PC) << "], from " << decToHex(change[0]) << " to " << decToHex(change[1]) << '\n';
            exec = 0;
        }
        else if (readOperation)
        {
            cout << "Reading memory[" << decToHex(PC) << "], has value: " << decToHex(change[0]) << endl;
            exec = 0;
        }

        PC++;
        index++;
        withinBounds = (times > 0 && PC < code.size());
    }

    return true;
}


bool runEmu()
{
    // Interactive emulator run
    string choice,option;
    cout << "Enter a command or 0 to exit:" << endl;
    cin >> choice;
    option = choice;
    if(option[0] != '0'){
        if(option[0] < 'a' && option[0] > 'e'){
            cout<<"Invalid choice"<<endl;
        }
    }
    map<string, int> choiceList;
    choiceList["a"] = 1;
    choiceList["b"] = 2;
    choiceList["c"] = 3;
    choiceList["d"] = 4;
    choiceList["e"] = 5;
    choiceList["0"] = 6;
    
    int choiceNum = choiceList[choice];
    
    bool isIsa = (choiceNum == 1);
    bool isRunOneLine = (choiceNum == 2);
    bool isRunComplete = (choiceNum == 3);
    bool isShowReg = (choiceNum == 4);
    bool isShowMem = (choiceNum == 5);
    bool isExit = (choiceNum == 6);

    if (isIsa)
    {
        instructionSetArchitecture();
    }
    else if (isRunOneLine)
    {
        cout << "Running One Line at a time" << endl;
        return runCode(0, 1);
    }
    else if (isRunComplete)
    {
        cout << "Running Complete file" << endl;
        return runCode(0);
    }
    else if (isShowReg)
    {
        showReg();
    }
    else if (isShowMem)
    {
        showMem();
    }
    else if (isExit)
    {
        code.clear();
        mnem.clear();
        out_mem.clear();
        if(PC != 0) PC = 0;
        if(SP != 0) SP = 0;
        if(A != 0) A = 0;
        if(B != 0) B = 0;
        index = 0;
        cont = 0;
        exec = 0;
        exit(0);
    }
    else
    {
        cout << "Enter correct instruction" << '\n';
    }

    return true;
}


int main()
{
    mnem = {
    "ldc", "adc", "ldl", "stl", "ldnl", "stnl", "add", "sub",
    "shl", "shr", "adj", "a2sp", "sp2a", "call", "return",
    "brz", "brlz", "br", "HALT"
    };
    // Input function
    string fileInput;
    cout << "Welcome to Emulator" << '\n';
    cout << "Please Enter file name: (Ex: machineCode.o) " << '\n';
    cin >> fileInput;
    if (checkFileType(fileInput) != true)
    {
        cout << "!!! ERROR !!! Invalid File Format , Please enter .o file only" << '\n';
        exit(0);
    }
    int pos = 0;
    ifstream file(fileInput, ios::in | ios::binary);
    unsigned int read;
    if(pos) return 0;
    while (file.read((char *)&read, sizeof(int)))
    {
        Memory[pos++] = read;
        code.push_back(decToHex(read));
    }

    // Instruction Message
    cout << "Following functions can be executed :" << '\n';
    cout << "-> Show instruction set using : a" << '\n';
    cout << "-> Emulate code one line at time using instruction: b" << '\n';
    cout << "-> Emulate till the end using instruction: c" << '\n';
    cout << "-> Show registers, PC and SP values: d" << '\n';
    cout << "-> Memory Dump using instruction: e" << '\n';
    while (true)
    {
        runEmu();
    }
        code.clear();
        mnem.clear();
        out_mem.clear();
        if(PC != 0) PC = 0;
        if(SP != 0) SP = 0;
        if(A != 0) A = 0;
        if(B != 0) B = 0;
        index = 0;
        cont = 0;
        exec = 0;
    return 0;
}