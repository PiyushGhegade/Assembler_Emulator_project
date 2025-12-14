// Name: Piyush Ghegade
// Roll No: 2301AI52
//--------------------------------------------------------------------------//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#define INFORMATION 1
#define WARNING 2
#define ERROR 3
int error_code=0;
/* Define constants for max label length, max instructions, etc. */
#define MAX_LABELS 100
#define MAX_LINE_LENGTH 100
#define MAX_LENGTH_OF_INSTRUCTION 20
#define MAX_LENGTH_OF_OPREND 20
#define MAX_LENTH_OF_ERROR 100
#define MAX_TOKEN_LENGTH 100

/* Structure to hold label information */
typedef struct
{
    char label[20]; /* The label name */
    int address;    /* The address associated with the label */
    int used;       /* Indicates if the label has been used */
    int isData;  /* Flag to indicate if it's a `DATA` label*/
    int value;   /* Value to store for data entries*/

} Label;

typedef struct
{
    char token[MAX_TOKEN_LENGTH]; /* A generic name for a string-like entity */
    int id;                       /* An ID that could be for indexing or referencing */
    char type;                     /* Type identifier, potentially for categorizing tokens */
    char isTemporary;              /* Flag to indicate if it's a temporary item */
    bool reserved;                 /* Unused field, potentially reserved for future use */

} TokenInfo;

typedef struct
{
    char mnemonic[20]; /* The name of the instruction (e.g., "ldc", "br") */
    int ret;           /* The opcode for the instruction (e.g., 0x00 for "ldc") */
} Instruction;


Instruction instructionTable[] = {
    {"ldc", 0},{"adc", 1},{"ldl", 2},{"stl", 3},
    {"ldnl", 4},{"stnl", 5},{"add", 6},{"sub", 7},     
    {"shl", 8},{"shr", 9},{"adj", 10},{"a2sp", 11},  
    {"sp2a", 12},{"call", 13},{"return", 14},{"brz", 15},   
    {"brlz", 16},{"br", 17},{"HALT", 18},   
};


Label labelTable[MAX_LABELS];
int labelCount = 0; /* Number of labels */
int halt_present=0;
int machine_code_no=0;

char *findLabelByAddress(int address)
{
    int i = 0;
    char *result = NULL;

    /* Complex loop structure with condition check */
    while (i < labelCount)
    {
        /* Check address condition inside a switch statement */
        switch (labelTable[i].address == address)
        {
            case 1:
                result = labelTable[i].label; /* Set result to found label */
                i = labelCount; /* Terminate loop early by setting i beyond the limit */
                break;
            default:
                i++;
                break;
        }
    }
    
    return result; /* Return the found label or NULL if none matched */
}

/* Logging function*/
void GivingMessageToLogFile(int level, const char* message) {
        const char* level_str;  /* Variable to store log level as string */
    FILE* log_file = fopen("logFile.log", "a");  /* Attempt to open the log file in append mode */
    
    /* Check if the log file opened successfully */
    if (log_file == NULL) {
        printf("Failed to open log file.\n");  /* Print error if file opening fails */
        return;  /* Exit function due to failed file access */
    }

    /* Assign appropriate string to represent the log level */
    const char *temp_str1 = "INFORMATION";
    const char *temp_str2 = "WARNING";
    const char *temp_str3 = "ERROR";
    const char *temp_str4 = "UNKNOWN";
    
    if (level == INFORMATION) {
        level_str = temp_str1;
    } else if (level == WARNING) {
        level_str = temp_str2;
    } else if (level == ERROR) {
        level_str = temp_str3;
    } else {
        level_str = temp_str4;
    }

    /* Write the log entry to the file, preceded by the log level and followed by the message */
    const char* log_format = " %s: %s";
    fprintf(log_file, log_format, level_str, message);

    /* Closing the log file to ensure changes are saved */
    fclose(log_file);  /* Close the log file after writing is complete */

}

int findLabelAddress(char *label, int line)
{
    /* Search for the label in the label table */
    char err_str[100];
    int i = 0;
    int foundAddress = -1;

    while (i < labelCount && foundAddress == -1)
    {
        switch (strcmp(labelTable[i].label, label) == 0)
        {
            case 1:
                labelTable[i].used = 10;
                foundAddress = labelTable[i].address;
                break;
        }
        i++;
    }

    /* Check if the label was not found and log an error */
    if (foundAddress == -1)
    {
        sprintf(err_str, "Error: Undefined label '%s' at line -- '%d'\n", label, line);
        GivingMessageToLogFile(ERROR, err_str);
        error_code = 1;
    }

    return foundAddress;
}

void InsertLabelToTable(const char *label, int address, int isdata, int value, int line_num) {
    /* Verify if the label is already in the symbol table */
    char err_str[100];
    int i;
    int isDuplicate = 0;

    /* Loop through the label table to check for duplicates, with an extra condition */
    for (i = 0; i < labelCount && !isDuplicate; i++) {
        int match = !(strcmp(labelTable[i].label, label) != 0); 
        
        /* Check if a duplicate is found */
        if (match) {
            isDuplicate = 1; 
            sprintf(err_str, "Error: Duplicate label '%s' found at line -- '%d'\n", label, line_num);
            GivingMessageToLogFile(ERROR, err_str);  
            error_code = 1;
        }
    }

    /* Only add the label if no duplicate was found */
    if (!isDuplicate) {
        /* Store the label and its associated details into the symbol table */
        strcpy(labelTable[labelCount].label, label);  // Copy label name to table
        labelTable[labelCount].address = address;     // Store label's address
        labelTable[labelCount].isData = isdata;       // Store data flag for label
        labelTable[labelCount].value = value;         // Store label value
        labelCount++;  // Increment label count
    }
}

#define MAX_LINE_LENGTH 100

/* Function to get the opcode for a given instruction mnemonic */
int getOperationcode(char *mnemonic) {
    int i = 0;
    int foundOpcode = -1; 
    
    /* Iterate through the instruction table and check for matching mnemonic */
    for (i = 0; i < 19 && foundOpcode == -1; i++) {  
        int isMatch = !strcmp(instructionTable[i].mnemonic, mnemonic); 

        /* Switch to handle matching condition */
        switch (isMatch) {
            case 1:
                foundOpcode = instructionTable[i].ret;  
                return instructionTable[i].ret;  // Return corresponding opcode if found
            default:
                break;
        }
    }

    /* Return -1 if mnemonic is not found in the table */
    return (foundOpcode == -1) ? -1 : foundOpcode;
}

/* Function to remove leading spaces from the start of a string */
void left_strip(char *str) {
    int i = 0, j = 0;
    int isSpaceChar = 0;  // Unnecessary variable to indicate if a character is a space

    /* Use a for loop to bypass all initial spaces */
    for (; isspace(str[i]); i++) {
        isSpaceChar = 1;  // Redundant use, just indicates space was encountered
    }

    /* Move remaining characters to the beginning of the string */
    for (; str[i] != '\0'; i++, j++) {
        str[j] = str[i];
    }

    /* Conditionally null-terminate the string */
    switch (str[j] != '\0') {
        case 1:
            str[j] = '\0';
            break;
        default:
            break;
    }
}

int error_check(char *operand, char *instruction) {
    char err_str[100];
    int operandLength = strlen(operand);  // Unnecessary variable to hold operand length
    int isLdc = !strcmp(instruction, "ldc");  // Use unnecessary variables to indicate instruction type
    int isAdd = !strcmp(instruction, "add");

    /* Check for 'ldc' instruction */
    switch (isLdc) {
        case 1:  // if instruction is 'ldc'
            if (!(operandLength > 0)) {  // Rewritten to check if operand is empty
                sprintf(err_str, "Missing operand for 'ldc'\n");  // Error for missing operand
                GivingMessageToLogFile(ERROR, err_str);
                error_code = 1;
            }
            break;
        default:
            break;
    }

    /* Check for 'add' instruction */
    switch (isAdd) {
        case 1:
            if (!!(operandLength > 0)) {  // Rewritten to check if operand is present (using double negation)
                sprintf(err_str, "Unexpected operand for 'add'\n");  // Error for unexpected operand
                GivingMessageToLogFile(ERROR, err_str);
                error_code = 1;
            }
            break;
        default:
            break;
    }

    return 0;  // No errors found, return success
}

void strip_comments_and_whitespace(char *line) {
    int length = strlen(line);
    int adjustedLength = length;  // Unnecessary variable to hold the modified length
    char *semicolonPosition = strchr(line, ';');
    int hasComment = 0;           // Unnecessary flag to indicate if a comment is found

    /* Check for semicolon to strip comments */
    switch (semicolonPosition != NULL) {
        case 1:
            *semicolonPosition = '\0';
            hasComment = 1;       // Set flag indicating a comment was found
            break;
        default:
            hasComment = 0;       // No comment found
            break;
    }

    /* Remove trailing whitespace using a for loop */
    for (int i = adjustedLength - 1; i >= 0; i--) {
        int isSpace = isspace((unsigned char)line[i]);
        switch (isSpace) {
            case 1:
                line[i] = '\0';
                adjustedLength--;  // Update adjusted length
                break;
            default:
                i = -1;           // Stop loop once non-whitespace is found
                break;
        }
    }
}

int is_name_line(const char *line) {
    const char *start = line;
    int hasLeadingWhitespace = 0;  // Unnecessary flag to track leading whitespace

    /* Skip leading whitespace using a for loop */
    for (int i = 0; *line != '\0' && i == 0; ) {
        switch (isspace((unsigned char)*line)) {
            case 1:
                hasLeadingWhitespace = 1;
                line++;
                break;
            default:
                i = 1;  // Stop loop once non-whitespace character is found
                break;
        }
    }
    start = line;  // Start of the alphanumeric section

    int foundAlnum = 0;  // Unnecessary variable to indicate alphanumeric characters

    /* Traverse alphanumeric characters */
    for (int j = 0; *line != '\0' && j == 0; ) {
        switch (isalnum((unsigned char)*line)) {
            case 1:
                foundAlnum = 1;
                line++;
                break;
            default:
                j = 1;  // Stop loop on non-alphanumeric character
                break;
        }
    }

    /* Check for the colon character at the end of the alphanumeric section */
    int hasColon = (line != start && *line == ':') ? 1 : 0;

    switch (hasColon) {
        case 1:
            line++;  // Move past the colon if present
            break;
        default:
            return 0;
    }

    /* Skip any additional whitespace */
    for (int k = 0; *line != '\0' && k == 0; ) {
        switch (isspace((unsigned char)*line)) {
            case 1:
                line++;
                break;
            default:
                k = 1;  // Stop loop once non-whitespace character is found
                break;
        }
    }

    /* Final check for comment or end of line */
    int isNameLine = (*line == ';' || *line == '\0') ? 1 : 0;
    switch (isNameLine) {
        case 1:
            return 1;
        default:
            return 0;
    }
}

void rstrip(char *str) {
    int length = strlen(str);
    int adjustedLength = length;  // Unnecessary variable to track length
    int whitespaceFlag = 0;       // Unnecessary variable to indicate whitespace found

    /* Loop backward through the string to remove trailing whitespace */
    for (int i = length - 1; i >= 0; i--) {
        /* Check if the current character is whitespace */
        switch (isspace((unsigned char)str[i])) {
            case 1:
                adjustedLength = i;  // Update adjustedLength to current position
                whitespaceFlag = 1;  // Set flag indicating whitespace
                break;
            default:
                whitespaceFlag = 0;  // Reset flag if non-whitespace found
                i = -1;              // Force exit from loop
                break;
        }
    }

    /* Use adjustedLength to set the new null terminator */
    str[adjustedLength] = '\0';
}

int detectInfiniteLoop(FILE *file, int line_num) {
    char err_str[100];
    char line[MAX_LINE_LENGTH];
    int address = 0;

    /* Rewind file to the beginning and initialize address and line number */
    rewind(file);
    address = 0;
    line_num = 0;

    /* Loop over each line in the file using a for loop */
    for (; fgets(line, sizeof(line), file) != NULL; line_num++) {
        char instruction[MAX_LENGTH_OF_INSTRUCTION] = {0};
        char operand[MAX_LENGTH_OF_OPREND] = {0};
        char label[20] = {0};
        int op;
        char *lab = NULL;

        /* Strip comments, whitespace, and adjust the line format */
        strip_comments_and_whitespace(line);
        left_strip(line);
        rstrip(line);

        /* Parse label, instruction, and operand based on line format */
        int tokensParsed = 0;
        tokensParsed = sscanf(line, "%19[^:]: %19s %19s", label, instruction, operand);
        if (tokensParsed < 3) tokensParsed = sscanf(line, "%19s %19s", instruction, operand);
        if (tokensParsed < 2) tokensParsed = sscanf(line, "%19[^:]:", label);

        /* Check if line contains a name line to adjust address */
        switch (is_name_line(line)) {
            case 1:
                address--;
                break;
            default:
                break;
        }

        /* Check for a branch instruction and possible infinite loop */
        switch (strcmp(instruction, "br") == 0) {
            case 1:
                op = operand[0] - '0';
                lab = findLabelByAddress(op);

                int targetAddress = (lab == NULL)
                                    ? findLabelAddress(operand, line_num)
                                    : findLabelAddress(lab, line_num);

                /* Check if branch address is equal to current address */
                if (targetAddress == address) {
                    sprintf(err_str, " Infinite loop detected at address %d (branching to itself).\n", address);
                    GivingMessageToLogFile(WARNING, err_str);
                    return 1;
                }
                break;
            default:
                break;
        }

        /* Increment the address for each new line */
        address++;
    }

    return 0;
}

void pass1(FILE *inputFile)
{
    int a;
    int line_num = 0;
    char line[MAX_LINE_LENGTH]; /* Buffer to hold each line of the input file */
    int address = 0;            /* Start memory address at 0 */
    char *labelEnd;
    char *comment;
    char operand[MAX_LENGTH_OF_OPREND] = {0};     /* Buffer to hold the operand */
    char instruction[MAX_LENGTH_OF_INSTRUCTION] = {0}; /* Buffer to hold the instruction */
    char err_str[MAX_LENTH_OF_ERROR];
    
    /* Read each line from the input file */
    while (fgets(line, sizeof(line), inputFile))
    {
        line_num++;
        
        /* Skip lines that are empty or start with ';' */
        bool co = *line == '\n' || *line == ';';
        if (co) continue;
        
        labelEnd = strchr(line, ':'); /* Check for the presence of a label */
        comment = strchr(line, ':');
        a = 1;

        if (comment)
        {
            while (*(comment + 1) == ' ') comment++;
            
            a = (*(comment + 1) == ';' || *(comment + 1) == '\n') ? 0 : a;
        }

        /* Process label if it exists */
        switch (labelEnd != NULL)
        {
            case 1:
                *labelEnd = '\0'; /* Remove the colon from the label */
                
                /* Check if label starts with a number (invalid) */
                switch (isdigit(line[0]) != 0)
                {
                    case 1:
                        sprintf(err_str, "Invalid label name '%s' at line -- %d\n", line, line_num);
                        GivingMessageToLogFile(ERROR, err_str);
                        error_code = 1; /* Give error code as 1 if label name starts with a number */
                        break;
                }

                /* Skip spaces after label */
                while (*(labelEnd + 1) == ' ') labelEnd++;
                
                sscanf(labelEnd, "%s %s", instruction, operand); /* Parse instruction and operand */
                
                /* Process instructions */
                switch (strcmp(instruction, "SET") == 0 || strcmp(instruction, "data") == 0)
                {
                    case 1:
                        {
                            int value = strtol(operand, NULL, 0); /* Convert operand to integer */
                            int isData = (strcmp(instruction, "data") == 0) ? 1 : 0;
                            
                            /* Add label to symbol table with the given value or current address */
                            InsertLabelToTable(line, (isData ? address : value), isData, value, line_num);
                            memset(instruction, 0, sizeof(instruction));
                            address++;
                        }
                        continue;

                    default:
                        /* Default label behavior with current address */
                        InsertLabelToTable(line, address, 0, 0, line_num);
                        
                        /* Adjust address if there's only a comment */
                        address = (a == 0) ? address - 1 : address;
                }
                labelEnd += 1;
                break;

            default:
                /* Check for comments at the beginning of a line */
                {
                    int i = 0;
                    while (isspace(line[i])) i++; /* Skip leading whitespace */
                    
                    if (line[i] == ';')
                    {
                        address--; /* Adjust address for comments */
                    }
                }
        }
        /* Increment address counter (each instruction is 1 memory unit) */
        address++;
    }
}

unsigned int shift_int_max_left(int shift_count) {
    // Cast INT_MAX to unsigned int and shift left by shift_count
    unsigned int result = (unsigned int)__INT_MAX__ << shift_count;
    
    // Mask the result to keep only the lower 32 bits
    result &= 0xFFFFFFFF;
    return result;
}

void pass2(FILE *inputFile, FILE *listfile)
{
    int lineIndex = 0;
    char buffer[MAX_LINE_LENGTH];                  // Buffer for each line
    int addr = 0;                                  // Starting memory address
    int machinecode;
    FILE *outputFile = fopen("machineCode.o", "wb");  // Output file for machine code
    int index = 0;
    char errorMessage[100];
    
    for (; fgets(buffer, sizeof(buffer), inputFile) != NULL; ++lineIndex)  // Changed to for loop
    {
        char instr[20] = {0};                      // Store instruction
        char oprnd[20] = {0};                      // Operand as string
        int opcode = 0, oprndVal = 0;              // Opcode and operand value
        char *cmt;
        char *trimmed;
        char *labelLocation;
        int generatedCode;
        char *label;
        
        left_strip(buffer);                        // Remove leading whitespace

        cmt = strchr(buffer, ';');                 // Locate comments
        bool hasComment = (cmt != NULL);           
        if (hasComment)
        {
            *cmt = '\0';                           // Nullify comment portion
        }
        trimmed = strtok(buffer, "\n\r\t");        // Trim trailing whitespace

        bool isTrimmedEmpty = (trimmed == NULL || strlen(trimmed) == 0); // Additional variable for condition
        if (isTrimmedEmpty) continue;              // Skip empty lines

        labelLocation = strchr(trimmed, ':');      // Check for label
        bool hasLabel = (labelLocation != NULL);   // Additional variable for condition
        if (hasLabel)
        {
            fprintf(listfile, "%08x          %s\n", addr, trimmed);
            trimmed = labelLocation + 1;

            while (*trimmed == ' ' || *trimmed == '\t') trimmed++;
            bool trimmedEmpty = (*trimmed == '\0');
            if (trimmedEmpty) continue;
        }

        sscanf(trimmed, "%s %s", instr, oprnd);    // Parse instruction and operand
        bool isHalt = (strcmp(instr, "HALT") == 0);
        if (isHalt) halt_present = 1;

        bool isData = (strcmp(instr, "data") == 0); // Check if it is a data directive
        if (isData)
        {
            oprndVal = strtol(oprnd, NULL, 0);
            fprintf(listfile, "%08x %08x data 0x%x\n", addr, oprndVal, oprndVal);
            addr++;
            continue;
        }

        bool isSet = (strcmp(instr, "SET") == 0);
        if (isSet)
        {
            addr++;
            continue;
        }

        opcode = getOperationcode(instr);                // Get opcode
        bool unknownOpcode = (opcode == -1 || opcode > 19); // Extra condition variable
        if (unknownOpcode)
        {
            sprintf(errorMessage, "Unknown instruction '%s' at line %d.\n", instr, lineIndex);
            GivingMessageToLogFile(ERROR, errorMessage);
            error_code = 1;
        }

        error_check(oprnd, instr);
        
        bool isBranch = (strcmp(instr, "call") == 0 || strcmp(instr, "brz") == 0 || 
                         strcmp(instr, "br") == 0 || strcmp(instr, "brlz") == 0); // Check for branch instructions
        if (isBranch)
        {
            oprndVal = findLabelAddress(oprnd, lineIndex) - addr - 1;
            generatedCode = (opcode & 0xFF) | (oprndVal << 8);
            if(strcmp(instr, "call") == 0 || strcmp(instr, "brz") == 0 || strcmp(instr, "br") == 0 || strcmp(instr, "brlz") == 0) label = findLabelByAddress(oprndVal + addr + 1);
            fprintf(listfile, "%08x %08x %s %s\n", addr, generatedCode, instr, label);
            addr += 1;
            fwrite(&generatedCode, sizeof(int), 1, outputFile);
            machine_code_no += 1;
            continue;
        }

        bool hasOprnd = (strlen(oprnd) > 0);     // Check if operand is non-empty
        if (hasOprnd)
        {
            bool isLabel = isalpha(oprnd[0]);
            if (isLabel)
            {
                oprndVal = findLabelAddress(oprnd, lineIndex);
            }
            else
            {
                char *endptr;
                oprndVal = strtol(oprnd, &endptr, 0);
                bool invalidNumber = (*endptr != '\0'); 
                if (invalidNumber)
                {
                    sprintf(errorMessage, "Invalid number '%s' at line %d.\n", oprnd, lineIndex);
                    GivingMessageToLogFile(ERROR, errorMessage);
                    error_code = 1;
                }
            }
        }
        else
        {
            oprndVal = 0x7FFFFFFF;
        }

        bool hasMaxOperand = (oprndVal == 0x7FFFFFFF); // Check if operand is max int
        if (hasMaxOperand)
        {
            generatedCode = (opcode & 0xFF);
            fwrite(&generatedCode, sizeof(int), 1, outputFile);
            machine_code_no++;
        }
        else if(!hasMaxOperand)
        {
            generatedCode = (opcode & 0xFF) | (oprndVal << 8);
            fwrite(&generatedCode, sizeof(int), 1, outputFile);
            machine_code_no++;
        }

        label = findLabelByAddress(oprndVal);
        bool labelExists = (label != NULL);
        if (labelExists)
        {
            fprintf(listfile, "%08x %08x %s %s\n", addr, generatedCode, instr, label);
        }
        else
        {
            if (oprndVal != 0x7FFFFFFF)
            {
                fprintf(listfile, "%08x %08x %-4s 0x%x\n", addr, generatedCode, instr, oprndVal);
            }
            else
            {
                fprintf(listfile, "%08x %08x %-4s\n", addr, generatedCode, instr);
            }
        }
        addr++;
    }

    index = 0;
    while (index < labelCount)  
    {
        bool isUnused = (labelTable[index].used != 10); 
        if (isUnused)
        {
            sprintf(errorMessage, "Unused label found: %s at line %d\n", labelTable[index].label, lineIndex);
            GivingMessageToLogFile(WARNING, errorMessage);
        }
        index++;
    }



    bool hasHaltMissing = (halt_present == 0); 
    if (hasHaltMissing)
    {
        int haltCode = (18 & 0xFF) | shift_int_max_left(8);
        sprintf(errorMessage, "Halt not found\n");
        GivingMessageToLogFile(WARNING, errorMessage);
        fprintf(listfile, "%08x %08x %-4s\n", addr, haltCode, "HALT");
        fwrite(&haltCode, sizeof(int), 1, outputFile);
        addr++;
        machine_code_no++;
    }

    index = 0;
    while (index < labelCount)  // Changed from for loop to while loop
    {
        bool isDataLabel = (labelTable[index].isData == 1); // Check for data labels
        if (isDataLabel)
        {
            machinecode = labelTable[index].value;
            printf("%x.", machinecode);
            fwrite(&machinecode, sizeof(int), 1, outputFile);
            machine_code_no++;
        }
        index++;
    }

    fclose(outputFile); // Close output file
}



int main(int argc, char *argv[])
{
    FILE *inputFile, *listfile, *logfile;

    // printf("%s\n", argv[0]);
    printf("******************************************************* MY PROJECT STARTS FROM HERE ******************************************************************\n");
    printf("WELCOME TO ASSEMBLER\n");
    printf("Type in './asm [Filename].asm' format\n");
    printf("Example : './asm test1.asm'\n");
    printf("******************************************************************************************************************************************************\n");
    /* Argument count validation */
    switch (argc == 2)
    {
        case 0:
            // printf("%d\n", argc);
            // printf("Usage: %s <assembly file>\n", argv[0]);
            return 1; /* Error code exit */
        default:
            break;
    }

    /* Open input assembly file */
    inputFile = fopen(argv[1], "r");
    switch (inputFile != NULL)
    {
        case 0:
            perror("Error opening file");
            return 1;
        default:
            logfile = fopen("logFile.log", "w");
            printf("LOG FILE IS GENERATED AS logFile.log\n");
            GivingMessageToLogFile(INFORMATION, "LOG CODE GENERATED logFile.log\n");
            fclose(logfile);
            break;
    }

    /* First pass to collect labels and addresses */
    pass1(inputFile);
    rewind(inputFile); /* Rewind to beginning for the second pass */

    /* Open listing file */
    listfile = fopen("listfiles.txt", "w");
    switch (listfile != NULL)
    {
        case 0:
            perror("Error opening file");
            fclose(inputFile);
            return 1;
        default:
            break;
    }

    /* Second pass to generate machine code */
    pass2(inputFile, listfile);
    detectInfiniteLoop(inputFile, 0);

    /* Close files and conditionally remove or log output files */
    fclose(inputFile);
    fclose(listfile);

    if (error_code != 0)
    {
        switch (remove("listfiles.txt") + remove("machineCode.o"))
        {
            case -1:
                perror("Error removing files");
                break;
            default:
                GivingMessageToLogFile(WARNING, "MACHINE CODE NOT GENERATED\n");
                GivingMessageToLogFile(WARNING, "LISTING FILE NOT GENERATED\n");
                printf("MACHINE CODE NOT GENERATED\n");
                printf("LISTING FILE NOT GENERATED\n");
                break;
        }
    }
    else
    {
        GivingMessageToLogFile(INFORMATION, "MACHINE CODE GENERATED machineCode.obj\n");
        GivingMessageToLogFile(INFORMATION, "LISTING FILE GENERATED listfiles.txt\n");
        printf("MACHINE CODE GENERATED in machineCode.obj\n");
        printf("LISTING FILE GENERATED in listfiles.txt\n");
    }

    return 0;
}