#include<time.h>
#include<windows.h>
#include "hashmap.h"

unsigned int IDNUM = 1000;         // current employee ID number for new employees

/*---------------------------------Helper functions-------------------------------------*/
// gets the current time as a formatted string
void getTime(char* tme){
    time_t t = time(NULL);                              // initialize time t
    struct tm tm = *localtime(&t);                      // get current time
    int iHour = 0;                                      
    int iMinute = 0;
    if(tm.tm_hour < 12){                                // if the hour < 12
        iHour = tm.tm_hour;
        iMinute = tm.tm_min;
    }
    else if(tm.tm_hour == 12 || tm.tm_hour == 24){      // if the hour == 12 or hour == 24
        iHour = 12;                                     
        iMinute = tm.tm_min;
    }
    else{                                               // if the hour is > 12 and < 24
        iHour = tm.tm_hour % 12;                    
        iMinute = tm.tm_min;   
    }
    if(tm.tm_min < 10){                                 // check if the minute value < 10 
        if(tm.tm_hour < 12 || tm.tm_hour == 24){        // hour is in the AM
            sprintf(tme, "%d:0%d AM", iHour, iMinute);  // format time
        }
        else{                                           // hour is in the PM
            sprintf(tme, "%d:0%d PM", iHour, iMinute);  // format time
        }
    }
    else{
        if(tm.tm_hour < 12 || tm.tm_hour == 24){        // if the hour is AM
            sprintf(tme, "%d:%d AM", iHour, iMinute);   // format time
        }
        else{                                           // if the hour is PM
            sprintf(tme, "%d:%d PM", iHour, iMinute);   // format time 
        }
    }
}
/*--------------------------------End of helper functions----------------------------------*/

/*-----------------------------Insert and delete functions--------------------------------*/
// adds employee to employee array
int addEmployee(char name[], float wage){
    employee *emp = malloc(sizeof(employee));   // create a new employee, allocate size for the employee
    strcpy(emp->name, name);                    // set name data for the new employee
    emp->wage = wage;                           // set wage data for the new employee
    emp->number = IDNUM;                        // set the ID number for the new employee
    strcpy(emp->clockIn, "0");                  // set the clockin time as "0" 
    strcpy(emp->clockOut, "0");                 // set the clockout time as "0"
    IDNUM++;                                    // increment the ID number
    return insert(emp);                         // call insert to insert the employee              
}

// deletes employee from employee array
int deleteEmployee(int ID){
    return removeEmp(ID);   // call removeEmp to remove the employee from the table
}
/*--------------------------End of insert and delete functions--------------------------*/

/*--------------------------Pay and clockin/clockout functions--------------------------*/
// gets the wage for an employee
float getWage(int ID){
    return pay(ID);         // call pay to get the pay for the employee 
}

// updates clockin time for an employee
int clockIn(int ID){
    char tme[9] = "";               // initalize empty string for time
    getTime(tme);                   // call getTime to get the current time
    return setClockIn(ID, tme);     // call setClockIn with ID and tme to clockin employee at the current time
}

// updates clockout time for an employee
int clockOut(int ID){
    char tme[9] = "";               // initalize empty string for time
    getTime(tme);                   // call getTime to get the current time
    return setClockOut(ID, tme);    // call setClockOut with ID and tme to clockout employee at the current time 
}
/*----------------------------End of pay and clockin/clockout functions-------------------------------*/

/*------------------------------------Sort and display functions--------------------------------------*/
// sorts employee array by name
void sortByName(){
    sortName();             // call sortName to sort by name and display data
}

// sorts employee array by wage
void sortByWage(){
    sortWage();             // call sortWage to sort by wage and display data
}

// sorts employee array by ID
void sortByID(){
    sortID();               // call sortID to sort by ID and display data
}

// prints employee array
void printEmployees(){
    printTable();           // prints the current table of employees
}
/*-------------------------------End of sort and display functions-----------------------------------*/

void main(){
    initTable();                                  // initialize table

    /*-------------File processing, reading data from a file to fill the table-------------------*/
    FILE *f;                                      // create new file pointer
    f = fopen("employees.txt", "r");              // open file for reading
    if(f == NULL){                                // check if file was opened
        printf("File could not be opened.");
    }

    char name[25];                                // employee name
    float wage;                                   // employee wage
    char clockInTime[9];                          // employee clockin time
    char clockOutTime[9];                         // employee clockout time
    char line[100];                               // line array, used for reading lines from file
    int id = 1000;                                // temp ID used for insertion from file 

    while(fgets(line, 100, f)){                   // while there is a line to read
        char*piece;                               // piece variable, used to seperate line
        
        piece = strtok(line, "|");                // seperate line by "|", grab the name of the employee
        strncpy(name, piece, 25);                 // copy name data to name
    
        piece = strtok(NULL, "|");                // seperate line by "|", grab the wage of the employee
        wage = atof(piece);                       // set wage = float value of piece
        
        piece = strtok(NULL, "|");                // seperate line by "|", grab the clockin time of the employee
        strncpy(clockInTime, piece, 9);           // copy clockin data to clockInTime
        
        piece = strtok(NULL, "|");                // seperate line by "|", grab the clockout time of the employee
        strncpy(clockOutTime, piece, 9);          // copy clockout data to clockOutTime
        
        addEmployee(name, wage);                  // insert employee
        setClockIn(id, clockInTime);              // set clockin time for employee
        setClockOut(id, clockOutTime);            // set clockout time for employee
        id++;                                     // increment temp ID
    }

    fclose(f);                                    // close the file
    /*------------------------------End of file processing----------------------------------*/

    /*-----------------------------------Terminal UI----------------------------------------*/
    char input[3];
    while(1){
        printf("--------------------------------------Hello, Welcome to the Dog Dog Inc. Employee Records System--------------------------------------\n");
        printf("| Command List:                                                                                                                   |\n");
        printf("|____________________________________________________________________________________________________________________________________|\n");
        printf("| Command | Description                                                                                     | Entries Required       |\n");
        printf("|_________|_________________________________________________________________________________________________|________________________|\n");
        printf("| IE      | Insert a new employee                                                                           | Requires name and wage |\n");
        printf("| DE      | Delete an existing employee                                                                     | Requires ID number---- |\n");
        printf("| GW      | Prints the pay for an existing employee                                                         | Requires ID number---- |\n");
        printf("| CI      | Updates clockin time for an existing employee                                                   | Requires ID number---- |\n");
        printf("| CO      | Updates clockout time for an existing employee                                                  | Requires ID number---- |\n");
        printf("| VE      | Displays employee records and writes data to empData.txt                                        | ---------------------- |\n");
        printf("| SN      | Displays employee records sorted by name and writes data to empData.txt                         | ---------------------- |\n");
        printf("| SI      | Displays employee records sorted by ID and writes data to empData.txt                           | ---------------------- |\n");
        printf("| SW      | Displays employee records sorted by wage and writes data to empData.txt                         | ---------------------- |\n");
        printf("| ES      | Ends system                                                                                     | ---------------------- |\n");
        printf("______________________________________________________________________________________________________________________________________\n");
        printf("Please enter your command: ");
        scanf("%2s", &input);
        fflush(stdin);
        if(strcmp(input, "ES") == 0){
            printf("_______________________________________________________________________________________________________________________________________\n");
            printf("Ending system, Goodbye!");
            Sleep(1000);
            break;
        }
        if(strcmp(input, "IE") == 0){
            char fname[10];
            char lname[15];
            char name[25];
            char swage[4];
            float wage = 0;
            
            printf("______________________________________________________________________________________________________________________________________\n");
            printf("Enter the employees first name(9 character limit): ");
            scanf("%9s", &fname);
            fflush(stdin);
            
            printf("Enter the employees last name(14 character limit): ");
            scanf("%14s", &lname);
            fflush(stdin);
            
            sprintf(name, "%s %s", fname, lname);
            
            while(1){
                printf("Enter the employee wage(max wage = $999): ");
                scanf("%3s", &swage);
                fflush(stdin);
            
                int check = 0;            
                for(int i = 0; i < 4; i++){
                    if(swage[i] == '\0'){
                        break;
                    }
                    if(isdigit(swage[i]) == 0){
                        check = 1;
                    }
                }
                if(check == 1){
                    printf("Incorrect wage entered\n");
                    continue;
                }
                else{
                    wage = atof(swage);
                    break;
                }
            }


            int check = addEmployee(name, wage);
            printf("______________________________________________________________________________________________________________________________________\n");
            if(check == 0){
                printf("Employee Successfully Inserted, Restarting\n");
                Sleep(1000);
                printf("\n");
                continue;
            }
            else{
                printf("Unsuccessful Insertion, Restarting\n");
                Sleep(1000);
                printf("\n");
                continue;
            }
        }   
        if(strcmp(input, "DE") == 0){
            int ID;
            
            printf("______________________________________________________________________________________________________________________________________\n");
            printf("Enter the employee ID: ");
            scanf("%d", &ID);
            fflush(stdin);
            int check = deleteEmployee(ID);
            
            printf("______________________________________________________________________________________________________________________________________\n");
            if(check == 0){
                printf("Employee Successfully Deleted, Restarting\n");
                Sleep(1000);
                printf("\n");
                continue;
            }
            else{
                printf("Invalid ID, Restarting\n");
                Sleep(1000);
                printf("\n");
                continue;
            }
        }
        if(strcmp(input, "GW") == 0){
            int ID;
            
            printf("______________________________________________________________________________________________________________________________________\n");   
            printf("Enter the employee ID: ");
            fflush(stdin);
            scanf("%d", &ID);
            
            float check = getWage(ID);
            if(check == -1.0){
                printf("______________________________________________________________________________________________________________________________________\n");    
                printf("Invalid ID, Restarting\n");
                Sleep(1000);
                printf("\n");
                continue;
            }
            else{
                printf("______________________________________________________________________________________________________________________________________\n");    
                printf("Employe %d made $%.2f\n", ID, check);
                
                printf("______________________________________________________________________________________________________________________________________\n");    
                char c;

                while(1){
                    printf("To restart program, enter 1: ");
                    scanf("%1s", &c);
                    fflush(stdin);
                    if(c == 49){
                        break;
                    }
                }
                printf("Restarting\n");
                Sleep(1000);
                printf("\n");
                continue;
            }
        }
        if(strcmp(input, "CI") == 0){
            int ID;
            
            printf("______________________________________________________________________________________________________________________________________\n");                
            printf("Enter the employee ID: ");
            scanf("%d", &ID);
            fflush(stdin);
            
            int check = clockIn(ID);
            
            printf("______________________________________________________________________________________________________________________________________\n");    
            if(check == 0){   
                printf("Employee Clockin Successful, Restarting\n");
                Sleep(1000);
                printf("\n");
                continue;
            }
            else{
                printf("Invalid ID, Restarting\n");
                Sleep(1000);
                printf("\n");
                continue;            
            }
        }
        if(strcmp(input, "CO") == 0){
            int ID;
            
            printf("______________________________________________________________________________________________________________________________________\n");    
            printf("Enter the employee ID: ");
            scanf("%d", &ID);
            fflush(stdin);
            
            int check = clockOut(ID);
            
            printf("______________________________________________________________________________________________________________________________________\n");        
            if(check == 0){
                printf("Employee Clockout Successful, Restarting\n");
                Sleep(1000);
                printf("\n");
                continue;            
            }
            else{
                printf("Invalid ID, Restarting\n");
                Sleep(1000);
                printf("\n");
                continue;               
            }
        }
        if(strcmp(input, "VE") == 0){
            printf("______________________________________________________________________________________________________________________________________\n");        
            printf("Raw Data\n");
            
            printEmployees();
            
            printf("\n");
            printf("______________________________________________________________________________________________________________________________________\n");        
            char c;

            while(1){
                printf("To restart program, enter 1: ");
                scanf("%1s", &c);
                fflush(stdin);
                if(c == 49){
                    break;
                }
            }
            printf("Restarting\n");
            Sleep(1000);
            printf("\n");
            continue;               
        }
        if(strcmp(input, "SN") == 0){
            printf("______________________________________________________________________________________________________________________________________\n");        
            printf("Data Sorted by Name\n");
            
            sortByName();
            
            printf("\n");
            printf("______________________________________________________________________________________________________________________________________\n");    
            char c;

            while(1){
                printf("To restart program, enter 1: ");
                scanf("%1s", &c);
                fflush(stdin);
                if(c == 49){
                    break;
                }
            }
            printf("Restarting\n");
            Sleep(1000);
            printf("\n");
            continue;           
        }
        if(strcmp(input, "SI") == 0){
            printf("______________________________________________________________________________________________________________________________________\n");        
            printf("Data Sorted by ID\n");
            
            sortByID();
            
            printf("\n");
            printf("______________________________________________________________________________________________________________________________________\n");    
            char c;

            while(1){
                printf("To restart program, enter 1: ");
                scanf("%1s", &c);
                fflush(stdin);
                if(c == 49){
                    break;
                }
            }
            printf("Restarting\n");
            Sleep(1000);
            printf("\n");
            continue;           
        }
        if(strcmp(input, "SW") == 0){
            printf("______________________________________________________________________________________________________________________________________\n");        
            printf("Data Sorted by Wage\n");
            
            sortByWage();
            
            printf("\n");
            printf("______________________________________________________________________________________________________________________________________\n");    
            char c;

            while(1){
                printf("To restart program, enter 1: ");
                scanf("%1s", &c);
                fflush(stdin);
                if(c == 49){
                    break;
                }
            }
            printf("Restarting\n");
            Sleep(1000);
            printf("\n");
            continue;           
        }
        
        printf("Invalid Command Entered\n");
        Sleep(1000);
        printf("Restarting System\n");
        Sleep(1000);
    }
    /*--------------------------------End of terminal UI-------------------------------------*/

    freeMem();                                    // free memory used by table
}
