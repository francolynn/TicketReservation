//Author: Franco Lynn, Sriram Vaandakar, Noel Johnson, Jayden Brown
//Date:4/29/2017
//Description: This program will allow users to view seating availability in multiple auditoriums
//			   book seats, and view a financial report. The majority of work in this program is done through doubly linked lists.
//Overloaded operators used:
//LinkedList += operator: Lines 158, 469, 653
//LinkedList [] operator: Lines 448, 450, 518, 520, 523, 576, 582, 597, 624, 647-649, 676, 678, 701
//ResNode input << operator: Not used
//ResNode output >> operator: 704, 701

#include<iostream>
#include<iomanip>
#include<string>
#include<sstream>
#include<fstream>
#include<cmath>
#include <cstdlib>
#include "LinkedList.h"
#include "BaseNode.h"
#include "ResNode.H"

using namespace std;

struct Tickets
{
    //total amounts for sales report
    int adult1 = 0, adult2 = 0, adult3 = 0,
        senior1 = 0, senior2 = 0, senior3 = 0,
        child1 = 0, child2 = 0, child3 = 0;

    //ticket prices
    int adultPrice = 7, seniorPrice = 5, childPrice = 3;
};

void openFile(fstream&, const char&);
void openFileInt(fstream&, int);
void openFile2(ofstream&, const char&);
char userInterface();
char mainMenu();
char auditoriumMenu();
void showSeats(const char&);
void getSeats(const char&, LinkedList&, int&, int&, int&, int&);
bool validateAndParse(int&, const string&, const int&, const int&);
void reserve(const char&, LinkedList&, int&, bool&);
bool bestAvailable(const char&, LinkedList&, LinkedList&, const int&);
void fillList(LinkedList&, const char);
void updateLists(LinkedList&, LinkedList&, const int&);
void updateFiles(const char&, LinkedList&);
void ticketData(const char&, Tickets&, const int&, const int&, const int&);
void salesReport(const Tickets&);
void printList(LinkedList&);

int main()
{
    //stores user's menu selections
    char selection;

    //create a obj to hold accumulative ticket data
    Tickets ticketTotals;

    //while user does not choose to exit menu (option 3)
    do
    {
        //counters for ticket data
        int numTickets = 0, adult = 0, senior = 0, child = 0;

        //print menu and get menu selection
        selection = userInterface();

        //if selection corresponds to an auditorium
        if (selection >= '1' && selection <='3')
        {
            //create a linked list that holds all auditorium seats
            LinkedList *List = new LinkedList(nullptr, nullptr);
            //create a linked list for seats that user will request
            LinkedList *ResList = new LinkedList(nullptr, nullptr);

            //true if seats have been reserved, else false
            bool seatsReserved = false;

            //fill the list based on auditorium file
            fillList(*List, selection);

            showSeats(selection);

            //get seat selection information
            getSeats(selection, *ResList, numTickets, adult, senior, child);

            //checks availability and exits, or updates file if seats are available.
            reserve(selection, *ResList, numTickets, seatsReserved);

            //if seats were unavailable
            if (!seatsReserved)
            {
                //delete the list and create a new empty list
                delete ResList;
                ResList = new LinkedList(nullptr, nullptr);

                //find best available seats and if user wants to reserve them
                if (bestAvailable(selection, *List, *ResList, numTickets) == true)
                    reserve(selection, *ResList, numTickets, seatsReserved);
            }

            //make changes to appropriate lists depending on which auditorium tickets were ordered from
            if (seatsReserved == true)
            {
                //increment ticket data
                ticketData(selection, ticketTotals, adult, senior, child);

                //make necessary changes to auditorium lists and files
                updateLists(*List, *ResList, numTickets);
                updateFiles(selection, *List);
            }
            delete ResList;
            delete List;
        }
        if (selection == 'r'){salesReport(ticketTotals);}

    //repeat while user does not elect to exit
    }while(selection != 'x');

    return 0;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//function that makes a linked list for files
void fillList(LinkedList &List, const char auditorium)
{
    fstream file;

    //open and validate file
    openFile(file, auditorium);

    int rowNum = 1;
    string row;     //holds user input
    bool status;    //reservation status of seat
    ResNode *cur;   //maneuver node

    //while there are still rows in the auditorium
    while (!file.eof())
    {
        //read first line of file, and ignore newline char
        getline(file, row, '\r');
        file.ignore();

        //for the number of seats in the row
        for(unsigned int i=0; i<row.length();i++)
        {
            //get seat status
            if(row[i]=='#'){status = true;}
            else {status = false;}

            //create new node and fill member variables
            cur = new ResNode(rowNum, i+1, status, nullptr, nullptr);

            //add node to end of the list
            List+=cur;
        }
        //increment row for next iteration
        rowNum++;
    }
    file.close();
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//function to help debug, will print out whole list
void printList(LinkedList &List)
{
    if (List.getHead() == nullptr)
    {
        cout << "LIST IS EMPTY\n";
        return;
    }
    //counter
    int i = 0;

    //if lsit isnt empty
    while (List[i]!=nullptr)
    {
        cout << i+1 << ". " << List[i]->getRow() << "x" << List[i]->getSeat() << ", ";

        if (List[i]->getPrev() == nullptr){cout << "null, ";}
        else {cout << List[i-1]->getRow() << "x" << List[i-1]->getSeat() << ", ";}

        if (List[i]->getNext() == nullptr){cout << "null, ";}
        else {cout << List[i+1]->getRow() << "x" << List[i+1]->getSeat() << ", ";}

        if (List[i]->getStatus()==true){cout << "#";}
        else {cout << "R";}
        cout << endl;
        i++;
    }
    cout << endl;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

char userInterface()
{
	char selection;                                                     //store user's menu selection

	selection = mainMenu();                                             //display menu and get selection

	if (selection == '1' || selection =='2')
    {
        selection = auditoriumMenu();                                  //print auditorium menu and get selection
        return selection;
    }

	if(selection=='3')                                                  //returns r for view report
        return 'r';

    if (selection == '4')                                               //option to exit
	{
		cout << "\nThank your for using, and have a wonderful day!" << endl;

		return 'x';                                                     //returns -1 if user wishes to exit
	}

	//if this actually returns, there is a big issue in the code
	return 'E';
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//First menu user encounters. If user does not exit. Function will send user to next menu.

char mainMenu()
{
    //variable to store user's menu selection
	string mainSelection;
	const int NUM_OPTIONS = 4;

    //print menu options and instructions
	cout << "Please select from the following menu options:\n\n"
            "1. Reserve Seats\n2. View Auditorium\n3. Ticket Report\n"
            "4. Exit\n\n"
            "To make your selection, enter the corresponding number: ";

    //get selection from user
    getline(cin, mainSelection);

    //validation loop checks first char of input for bounds and data type
	while (mainSelection.length()>((NUM_OPTIONS/10)+1) || mainSelection[0] < '1' || mainSelection[0] > ('0'+NUM_OPTIONS))
	{
	    //print error
		cout << "\nInvalid entry. Please make another selection (1,2,3 or 4): ";
        //get another selection and update loop variable
        getline(cin, mainSelection);
		cout << endl;
	}
    cout << endl;

    //return back to user interface
	return mainSelection[0];
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////-----------------------------------------------------------------------------------------------------------------------------------------
//Function that displays list of auditoriums and gets selection from user. Sends selection value back to main
char auditoriumMenu()
{
    string auditorium;
    const int NUM_AUDITORIUMS = 3;

	//print auditorium menu
    cout << "1. Auditorium 1\n"
            "2. Auditorium 2\n"
            "3. Auditorium 3\n\n"
            "Enter the number corresponding with auditorium to view: ";

	getline(cin, auditorium);

    //validation loop for selection
    while (auditorium.length()>((NUM_AUDITORIUMS/10)+1) || auditorium[0] < '1' || auditorium[0] > ('0'+NUM_AUDITORIUMS))
	{
		cout << "Invalid entry. Please make another selection (1,2 or 3): ";
        getline(cin, auditorium);
		cout << endl;
	}

	cout << endl;

    //returns requested auditorium number to userinterface()
	return auditorium[0];
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////-----------------------------------------------------------------------------------------------------------------------------------------
void showSeats(const char& auditorium)
{
    fstream file;

    openFile(file, auditorium);

    //stores one row of the seat map
    string row;

    //row display header
    int rowNumber = 1;

    getline(file, row);                         //store first row of seats into row
    cout << ' ';                                //space for Row x Column 0x0

    for (unsigned int i=1; i<row.length(); i++)   //for loop to print out column number. The +1 is to account for newline char
        cout << (i%10);                         //mod 10 to print column headers in single digits

    cout << endl;

    file.seekg(0L, ios::beg);                   //go back to beginning of file because first line has been inputted already

    while (!file.eof())                         //loop that prints out entire file or until EOF bit is found
    {
        getline(file, row);                     //get next row
        cout << rowNumber << row << endl;       //print row and row number
        rowNumber++;                            //increment row number
    }
    cout << endl;
    file.close();   //close file
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////-----------------------------------------------------------------------------------------------------------------------------------------
//gets user input for seats
void getSeats(const char& auditorium, LinkedList& ResList, int& numTickets, int& adult, int& senior, int& child)
{
    fstream file;
    openFile(file, auditorium);

    string row1,        //stores first row
           userInput;   //stores user input for validation purposes

    getline(file, row1);            //store first row into row1
    file.seekg(0, ios::end);        //go to end of file

    //formulas
    //file size = (file.tellg()+2);
    //number of columns including endline = (row1.length()+1);
    const int NUM_COLUMNS = row1.length()-1;
    const int NUM_ROWS = (((int)file.tellg()+2) / (int)(row1.length()+1));
    const int TOTAL_SEATS = NUM_COLUMNS*NUM_ROWS;

    //Instructions for User
    cout << "Numbers on the left: Row number\n"
            "Numbers on the top : Seat number\n"
            "Number signs (#):    Available seats\n"
            "Dots ( . ):          Reserved Seats\n\n"
            "Example: The seat on top-left corner is Row 1 and Seat 1\n\n"
            "To purchase a ticket, please enter the following information:\n"<< endl;


    //remaining number of tickets, to avoid prompting for ticket type if all tickets are accounted for
    int remaining;

    //do-while loop that gets ticket types. Will repeat if user does not allocate all tickets
    do
    {
        //get input
        cout << "Total number of tickets (1-" << TOTAL_SEATS<<"): ";
        getline(cin, userInput);

        //validate
        while(validateAndParse(numTickets, userInput, 1, TOTAL_SEATS)==false){
                getline(cin, userInput);}

        //restore remaining amount
        remaining = numTickets;

        //get number of adult tickets
        cout << "Number of Adult Tickets (0-" << remaining<<"): ";
        getline(cin, userInput);

        while(validateAndParse(adult, userInput, 0, remaining)==false){
            getline(cin, userInput);}

        //update remaining tickets
        remaining -= adult;

        //if there are still tickets left
        if (remaining > 0)
        {
            //get number of senior tickets
            cout << "Number of Senior Tickets (0-" << remaining<<"): ";
            getline(cin, userInput);

            while(validateAndParse(senior, userInput, 0, remaining)==false){
                getline(cin, userInput);}

            //update remaining tickets
            remaining -= senior;
        }

        //if there are still tickets left
        if (remaining > 0)
        {
            //get number of child tickets
            cout << "Number of Child Tickets (0-" << remaining<<"): ";
            getline(cin, userInput);

            while(validateAndParse(child, userInput, 0, remaining)==false){
                getline(cin, userInput);}

            //update remaining tickets
            remaining -= child;
        }
        if (remaining > 0)
        {
            cout << "\nError! Every ticket must be assigned a type. Please try again.\n\n";
        }
    }while (remaining != 0);

    //allocate space for the vectors that will hold reservation information
    //row.reserve(numTickets);
    //column.reserve(numTickets);

    cout << "\nPlease select a row and seat for each ticket:\n";

    //flag that signals if user selects the same seat twice
    bool duplicate = false;
    int row = 0;
    int column = 0;

    //for loop that gets row and column data for each ticket and stores them in a linked list of requested seats "ResList"
    for(int i=0; i<numTickets; i++)
    {
        //repeats if user enters the same seat twice
        do
        {
            //change flag back to avoid infinite loop
            duplicate = false;

            cout << "\nTicket " << i+1 << endl;

            //get row and validate
            cout << "Row (1-" << NUM_ROWS << "): ";
            getline(cin, userInput);
            while(validateAndParse(row, userInput, 1, NUM_ROWS)==false){
                getline(cin, userInput);}

            //get column and validate
            cout << "Seat (1-" << NUM_COLUMNS << "): ";
            getline(cin, userInput);
            while(validateAndParse(column, userInput, 1, NUM_COLUMNS)==false){
                getline(cin, userInput);}

            //for loop that checks
            for (int j = 0; ResList[j]!=nullptr && duplicate==false; j++)
            {
                if (ResList[j]->getRow() == row && ResList[j]->getSeat() == column)
                {
                    duplicate = true;
                    cout << "\nError: Cannot select the same seat twice. Please try again.\n";
                }
            }
        }while (duplicate == true);

        //Dynamically allocate node and add to the list
        ResNode *cur = new ResNode(row, column, 0, nullptr, nullptr);
        ResList+=cur;
    }
    cout << endl;

    file.close();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//validates and sets value to variable being prompted for
//function parameters (variable to parse for, user string input, lowest valid value, highest valid value)
bool validateAndParse(int& num, const string& USER_INPUT, const int& MIN, const int& MAX)
{
    //if 0 is a valid input and is selected
    //This block is necessary because iss>>num will store a '0' if attempting to parse a non-int value
    if (MIN == 0 && USER_INPUT == "0")
    {
        num = 0;
        return true;
    }

    //initialize a input string stream object with user input
    istringstream iss(USER_INPUT);

    //modified for loop to validate input
    for (int i = 0; iss; i++)
    {
        //parse for integers
        iss >> num;

        //returns false for numeric bounds, incorrect data type, or multiple inputs separated by white-space
        if (!(num>=MIN && num<=MAX) || num == 0 || i>1)
        {
            cout << "Invalid entry! Please enter a number between (" << MIN << "-" << MAX << "): ";
            return false;
        }
    }
    //entire string is parsed and all input requirements are met
    return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void reserve(const char& auditorium, LinkedList& ResList, int &NUM_TICKETS, bool& seatsReserved)
{
    //open and validate file
    fstream file;
    openFile(file, auditorium);

    int seat, newlineBytes;                         //stores numerical value of seat
    string row1;                                    //stores first row
    getline(file, row1);                            //store 1st row into row1

    file.seekg(0, ios::end);                        //go to end of file


    //file size = (file.tellg()+2);
    const int NUM_COLUMNS = row1.length()-1;

    for (int i=0; ResList[i]!=nullptr; i++)    //for loop repeating for number of tickets
    {
        newlineBytes = (2*(ResList[i]->getRow()-1))-1;

        //calculate row and column data to find seat number
        seat = ((ResList[i]->getRow()-1)*(NUM_COLUMNS)) + (ResList[i]->getSeat());

        //move input pointer to seat location and use static cast to fit parameter requirements
        file.seekg(static_cast<long>(seat+newlineBytes), ios::beg);

        //read status symbol of the seat
        //the calculation added to seat is the byte adjustment for the file. 2 byes per line for Windows OS
        if (file.get() == '.')
        {
            cout<< "Sorry, those seats are unavailable at this time.\n" <<endl; //print error message

            //update reservation flag
            seatsReserved = false;
            file.close();                                                       //clear buffer and close file
            return;                                                             //exit function
        }
    }
    //change reservation flag
    seatsReserved = true;
    cout << "Confirmation: You have purchased " << NUM_TICKETS << " tickets!\n" << endl;
    file.close();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool bestAvailable(const char& auditorium, LinkedList& List, LinkedList& ResList, const int& NUM_TICKETS)
{
    const int NUM_ROWS = List.getTail()->getRow();      //total cols
    const int NUM_COLS = List.getTail()->getSeat();     //total rows
    int MID = 0;            //middle seat of auditorium
    int seat = 0;           //actual numeric seat number
    int bestRow = 0;        //row of best seat
    int bestCol = 0;        //col of best seat
    double curDistance;     //distance of current seat
    double closest = -1;    //closest distance so far

    //if total seats is even, divide by 2
    if ((NUM_ROWS*NUM_COLS) % 2 == 0)
        MID = (NUM_ROWS*NUM_COLS)/2;

    else //if odd, get actual middle
        MID = ((NUM_ROWS*NUM_COLS)/2) + 1;

    //triple for loop that goes through each seat sequentially
    for (int row = 1; row<=NUM_ROWS ; row++)
    {
        for(int col = 1; col<=NUM_COLS-NUM_TICKETS+1; col++)
        {
            //for loop for number of tickets
            for(int k = 0; k<NUM_TICKETS; k++)
            {
                //compute seat number
                seat = NUM_COLS*(row-1) +col;

                //if a reserved seat is encountered, exit this for loop
                if (List[seat+k-1]->getStatus()==false){break;}

                //if a reservable section is encountered
                if (k==NUM_TICKETS-1)
                {
                    //calculate curDistance from first seat of the section to the center of the auditorium using hypotenuse function
                    curDistance = hypot(List[MID-1]->getRow()-row, List[MID-1]->getSeat()-col);

                    //if closest still contains default value, or if it is further than curDistance
                    if (closest == -1 || curDistance < closest)
                    {
                        //set closest equal to curDistance, and set best node members to row and column that has the closest curDistance
                        closest = curDistance;
                        bestRow = row;
                        bestCol = col;
                    }
                    //in case of a tie
                    if (curDistance == closest)
                    {
                        //calculate how many rows apart each seat is from the center row using absolute value and subtraction
                        int curRowDist = abs(row - List[MID]->getRow());
                        int closestRowDist = abs(bestRow - List[MID]->getRow());

                        //if the current seat is in a closer row
                        if (curRowDist<closestRowDist)
                        {
                            //update data for best seats with current data
                            closest = curDistance;
                            bestRow = row;
                            bestCol = col;
                        }
                    }
                }
            }
        }
    }

    //if there were available seats and 'closest' did not retain default value
    if (closest != -1)
    {
        //first seat number of best seat section
        int bestSeat = (bestRow-1)*NUM_COLS + bestCol;

        //Display best available seats to user
        cout << "These are the best available seats for the quantity you selected (Row x Seat format): \n\n";

        //for loop that displays the seats
        for (int i = 0; i<NUM_TICKETS; i++){
            cout << "Seat " << i+1 << ": " << List[bestSeat+i-1]->getRow() << "x" << List[bestSeat+i-1]->getSeat() << endl;}

        string userInput;

        //get input
        cout << "\nWould you like to reserve these seats? (Y/N): ";
        getline(cin, userInput);

        //validate
        while(!cin || userInput.length()>1 || (userInput[0]!='Y' && userInput[0]!='N'))
        {
            cout << "Error: Invalid entry. Please enter either 'Y' or 'N': ";
            getline(cin, userInput);
        }

        //if user wants to reserve the seats
        if (userInput[0] == 'Y')
        {
            //for loop that adds nodes to reservation list
            for (int i = 0; i<NUM_TICKETS; i++)
            {
                //create and fill node
                ResNode *best = new ResNode;
                best->setRow(List[bestSeat+i-1]->getRow());
                best->setSeat(List[bestSeat+i-1]->getSeat());
                best->setStatus(List[bestSeat+i-1]->getStatus());
                best->setNext(nullptr);
                best->setPrev(nullptr);

                //add node to list
                ResList+=best;
            }
            cout << endl;
            return true;
        }
    }
    //return false if user declined, or there were no available seats
    cout << endl;
    return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void updateLists(LinkedList& List, LinkedList &ResList, const int& NUM_TICKETS)
{
    const int NUM_COLS = List.getTail()->getSeat();
    int seat; //numeric value of seat

    //repeats once for each ticket reserved in current iteration of do-while loop
    for (int i = 0; i<NUM_TICKETS; i++)
    {
        //compute seat
        seat = (ResList[i]->getRow()-1)*NUM_COLS + ResList[i]->getSeat();
        //change seat status
        List[seat-1]->setStatus(false);
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//writes new statuses into auditorium files
void updateFiles(const char& auditorium, LinkedList &List)
{
    //open and validate file
    ofstream file;
    //use the openFile function that works with filestream to satisfy overloaded << operater requirements in ResNode class
    openFile2(file, auditorium);

    //comnpute total rows and cols
    const int NUM_ROWS = List.getTail()->getRow();
    const int NUM_COLS = List.getTail()->getSeat();

    //for loop that writes file seat by seat
    for (int row = 0; row < NUM_ROWS; row++)
    {
        for (int col = 0; col < NUM_COLS; col++)
        {
            //overloaded operator use that writes seat status to file
            file << *List[(NUM_COLS*row)+col];
        }
        //do not print an extra endline after last seat
        if (NUM_ROWS-row != 1){file << "\r\n";}
    }
    file.close();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ticketData(const char& auditorium, Tickets& T, const int& adult, const int& senior, const int& child)
{
    //increment appropriate member variables
    if (auditorium == '1')
    {
        T.adult1 += adult;
        T.senior1 += senior;
        T.child1 += child;
    }
    if (auditorium == '2')
    {
        T.adult2 += adult;
        T.senior2 += senior;
        T.child2 += child;
    }
    if (auditorium == '3')
    {
        T.adult3 += adult;
        T.senior3 += senior;
        T.child3 += child;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void salesReport(const Tickets& T)
{
    fstream file;

    //cout << "         1         2         3         4         5\n";
    //cout << "12345678901234567890123456789012345678901234567890123456789\n";
    cout << "**********************************************************" << endl; //separate report from user interface
    //print column headers
    cout << setw(12) << "AUDITORIUM #"
         << setw(6) << "OPEN"
         << setw(10) <<"RESERVED"
         << setw(7) << "ADULT"
         << setw(8) << "SENIOR"
         << setw(7) << "CHILD"
         << setw(9) << "SALES\n"
         << setw(58) << setfill('-') << '-' << setfill(' ')<<endl;


    int r=0,              //counts reserved seats
        e=0;              //counts empty seats

    int totalR= 0,        //total reserved
        totalE= 0;

    //for loop that opens each file and prints information
    for (int i = 1; i<=3; i++)
    {
        //open file corresponding with current loop
        openFileInt(file, i);

        while (!file.eof())
        {
            //status of each seat
            char status = file.get();
            //if seat is open, increment e
            if (status == '#'){e++;}
            //if seat is taken, increment r
            if (status == '.'){r++;}
        }
        //increment total empty and taken seats
        totalR+=r;
        totalE+=e;

        //print sales stats for individual column
        cout<< setw(10) << "AUDITORIUM " << i
            << setw(5)  << e
            << setw(8)  << r;

        //writes data for only the current file in loop
        if (i == 1)
        {
            //compute total sales of auditorium
            int A1Sales = (T.adult1*T.adultPrice)+(T.senior1*T.seniorPrice)+(T.child1*T.childPrice);

        //print totals for each ticket type and sales
        cout<< setw(8)  << T.adult1
            << setw(7)  << T.senior1
            << setw(8)  << T.child1
            << setw(7)  << A1Sales << ".00\n" << endl;
        }
        if (i == 2)
        {
            int A2Sales = (T.adult2*T.adultPrice)+(T.senior2*T.seniorPrice)+(T.child2*T.childPrice);

        cout<< setw(8)  << T.adult2
            << setw(7)  << T.senior2
            << setw(8)  << T.child2
            << setw(7)  << A2Sales << ".00\n" << endl;
        }
        if (i == 3)
        {
            int A3Sales = (T.adult3*T.adultPrice)+(T.senior3*T.seniorPrice)+(T.child3*T.childPrice);

        cout<< setw(8)  << T.adult3
            << setw(7)  << T.senior3
            << setw(8)  << T.child3
            << setw(7)  << A3Sales << ".00\n" << endl;
        }

        //reset counter for next auditorium
        r = 0;
        e = 0;

        file.close();
    }

    //compute totals for the entire theater
    int totalAdult = T.adult1+T.adult2+T.adult3;
    int totalSenior = T.senior1+T.senior2+T.senior3;
    int totalChild = T.child1+T.child2+T.child3;
    int totalSales = (totalAdult*T.adultPrice)+(totalSenior*T.seniorPrice)+(totalChild*T.childPrice);

    //print total sales data
    cout << setw(12) << "TOTAL"
         << setw(5)  << totalE
         << setw(8)  << totalR
         << setw(8)  << totalAdult
         << setw(7)  << totalSenior
         << setw(8)  << totalChild
         << setw(7)  << totalSales << ".00\n";
    cout << "**********************************************************" << endl << endl;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//this function opens files with a switch structure based on a char value
void openFile(fstream &file, const char& auditorium)
{
    //switch structure to determine which file to open based on auditorium number.
    switch(auditorium)
    {
    case '1':
        //opens file to display auditorium 1. File opened in binary for tellg, tellp accuracy
        file.open("a1.txt", ios::in | ios::out | ios::binary);
        break;
    case '2':
        file.open("a2.txt", ios::in | ios::out | ios::binary);
        break;
    case '3':
        file.open("a3.txt", ios::in | ios::out | ios::binary);
        break;
    default:
        break;
    }
     //validate file opening
    if (file.fail())
    {
        //user reaches error page if file opening fails
        cout << "Sorry, we encountered an error. Please try again at a later time." << endl;
        //exit program
        exit (0);
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//this function opens files with a switch structure based on an integer value, used if function needs to work on every file
void openFileInt(fstream &file, int auditorium)
{
    //switch structure to determine which file to open based on auditorium number.
    switch(auditorium)
    {
    case 1:
        //opens file to display auditorium 1. File opened in binary for tellg, tellp accuracy
        file.open("a1.txt", ios::in | ios::out | ios::binary);
        break;
    case 2:
        file.open("a2.txt", ios::in |ios::out | ios::binary);
        break;
    case 3:
        file.open("a3.txt", ios::in |ios::out | ios::binary);
        break;
    default:
        break;
    }
     //validate file opening
    if (file.fail())
    {
        //user reaches error page if file opening fails
        cout << "Sorry, we encountered an error. Please try again at a later time." << endl;
        //exit program
        exit (0);
    }
}

//open files using ofstream specifically
void openFile2(ofstream &file, const char& auditorium)
{
    //switch structure to determine which file to open based on auditorium number.
    switch(auditorium)
    {
    case '1':
        //opens file to display auditorium 1. File opened in binary for tellg, tellp accuracy
        file.open("a1.txt", ios::out | ios::binary);
        break;
    case '2':
        file.open("a2.txt", ios::out | ios::binary);
        break;
    case '3':
        file.open("a3.txt", ios::out | ios::binary);
        break;
    default:
        break;
    }
     //validate file opening
    if (file.fail())
    {
        //user reaches error page if file opening fails
        cout << "Sorry, we encountered an error. Please try again at a later time." << endl;
        //exit program
        exit (0);
    }
}
