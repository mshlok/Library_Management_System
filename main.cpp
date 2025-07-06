#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <vector>
#include <string>
#include <conio.h>

using namespace std;

string getCurrentDateTimeString()
{
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm local_time = *std::localtime(&now_c);
    std::ostringstream oss;
    oss << std::put_time(&local_time, "%Y %m %d %H %M %S");
    return oss.str();
}

int calculateDaysPassed(const std::string& dateTimeStr)
 {
    std::tm given_date = {};
    std::istringstream ss(dateTimeStr);
    ss >> given_date.tm_year >> given_date.tm_mon >> given_date.tm_mday>> given_date.tm_hour >> given_date.tm_min >> given_date.tm_sec;
    given_date.tm_year -= 1900;
    given_date.tm_mon -= 1;
    std::time_t given_time = std::mktime(&given_date);
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    double seconds_diff = std::difftime(now_c, given_time);
    return seconds_diff / (60 * 60 * 24);
}

class Books
{
public:
    string title, author, publisher, status, year,ISBN;
    int lineno;

    Books(string isbn)
    {
        ISBN=isbn;
        ifstream book("book.txt");
        int check=0;
        int isbn_check=0;
        string line;
        while(getline(book,line))
        {
            check++;
            if(line==ISBN)
            {
                lineno=check;
                isbn_check=1;
                break;
            }
        }
        if(!isbn_check)
            cout<<"This book does not exist in library";
        getline(book,title);
        getline(book,author);
        getline(book,publisher);
        getline(book,year);
        getline(book,status);
        book.close();
    }

    void change_book_status_to(string Status)
    {
        ifstream book_rd("book.txt");
        vector<string> lines;
        string line;
        int check=0;
        while (getline(book_rd, line))
        {
            check++;

            if (check==lineno+5)
            {
                lines.push_back(Status);

            }
            else
                lines.push_back(line);
        }
        book_rd.close();

        ofstream books_wr("book.txt");
        for (const auto &l : lines)
        {
            books_wr << l << "\n";
        }
        books_wr.close();
        status=Status;

    }
};


class Account
{
public:
    int lineno;
    string UserID;
    virtual int Verifyuser()=0;
    int curr_borr;
protected:
    string password;
};

class StudAccount: public Account
{

public:

    StudAccount(string userid,string pass)
    {
        UserID=userid;
        password=pass;
    }

    int Verifyuser()
    {
        ifstream student("a1.txt");

        int check=0;
        string line;
        while(getline(student,line))
        {
            check++;
            if(line==UserID)
            {
                lineno=check;
                getline(student,line);
                if(line==password)
                    {
                        getline(student,line);
                        getline(student,line);
                        curr_borr=stoi(line);
                        return 1;
                    }
                else
                {
                    cout<<"\nIncorrect Password\n";
                    return 0;
                }
            }
        }
        student.close();
        cout<<"\nIncorrect UserID\n";
        return 0;
    }

    void showAccountDetails()
    {
        ifstream student("a1.txt");
        string line;

        for(int i=0;i<lineno+3;i++)
        {
           getline(student,line);

        }
        cout<<"UserID : "<<UserID<<endl;
        cout<<"Currently Borrowed Books :";
        int n1=stoi(line);
        cout<<" "<<n1<<endl;
        if(n1>0)
            cout<<"ISBN\tBorrowed on"<<endl;
        for(int i=0;i<n1;i++)
        {
            getline(student,line);
            cout<<line<<"\t";
            getline(student,line);
            cout<<line<<"\n";
            getline(student,line);
        }
        cout<<"Fine : Rs"<<showfine()<<endl;
        int n2;
        getline(student,line);
        getline(student,line);
        getline(student,line);
        getline(student,line);
        n2=stoi(line);
        cout<<"History"<<endl;
        if(n2>0)
            cout<<"ISBN"<<endl;
        else
            cout<<"None"<<endl;
        for(int i=0;i<n2;i++)
        {
            getline(student,line);
            cout<<line<<"\n";
        }
    }

    void add_bk_to_curr_borr(Books b)
    {
        ifstream ac_rd("a1.txt");
        vector<string> lines;
        string line;
        int check=0;
        while (getline(ac_rd, line))
        {
            check++;

            if (check==lineno+3)
            {
                int n=stoi(line);
                n=n+1;
                string s=to_string(n);
                lines.push_back(s);
                lines.push_back(b.ISBN);
                lines.push_back(getCurrentDateTimeString());
                lines.push_back("0");
            }
            else
                lines.push_back(line);
        }
        ac_rd.close();

        ofstream ac_wr("a1.txt");
        for (const auto &l : lines)
        {
            ac_wr << l << "\n";
        }
        ac_wr.close();
    }


    void move_bk_from_curr_to_his(Books b)
    {
        int fine=0;
        ifstream ac_rd("a1.txt");
        vector<string> lines;
        string line;
        int check=0;
        while (getline(ac_rd, line))
        {
            check++;
            if (check==lineno+3)
            {
                int n=stoi(line);
                string s=to_string(n-1);
                lines.push_back(s);
                for(int i=0;i<n;i++)
                {
                    getline(ac_rd,line);
                    if(line!=b.ISBN)
                    {
                        lines.push_back(line);
                        getline(ac_rd,line);
                        lines.push_back(line);
                        getline(ac_rd,line);
                        lines.push_back(line);
                    }
                    else
                    {
                        getline(ac_rd,line);
                        int days=calculateDaysPassed(line);
                        getline(ac_rd,line);
                        int paid_days=stoi(line);
                        if(days>15)
                            fine=fine+(days-15-paid_days)*10;
                    }
                }
                getline(ac_rd,line);
                lines.push_back(line);
                getline(ac_rd,line);
                fine=fine+stoi(line);
                line=to_string(fine);
                lines.push_back(line);
                getline(ac_rd,line);
                lines.push_back(line);
                getline(ac_rd,line);
                n=stoi(line);
                line=to_string(n+1);
                lines.push_back(line);
                lines.push_back(b.ISBN);
            }
            else
                lines.push_back(line);
        }
        ac_rd.close();

        ofstream ac_wr("a1.txt");
        for (const auto &l : lines)
        {
            ac_wr << l << "\n";
        }
        ac_wr.close();
    }

    int showfine()
    {
        ifstream student("a1.txt");
        string line;
        int fine=0;
        for(int i=0;i<lineno+3;i++)
        {
            getline(student,line);
        }

        int no_curr_bor=stoi(line);

        for(int i=0;i<no_curr_bor;i++)
        {
            getline(student,line);
            getline(student,line);
            int days=calculateDaysPassed(line);
            getline(student,line);
            int paid_days=stoi(line);
            if(days>15)
                fine=fine+(days-15-paid_days)*10;
        }
        getline(student,line);
        getline(student,line);
        fine=fine+stoi(line);
        student.close();
        return fine;
    }

    void cleardues()
    {
        ifstream ac_rd("a1.txt");
        vector<string> lines;
        string line;

        for(int i=0;i<lineno+3;i++)
        {
            getline(ac_rd, line);
            lines.push_back(line);

        }

        int n=stoi(line);
        for(int i=0;i<n;i++)
        {
            getline(ac_rd, line);
            lines.push_back(line);
            getline(ac_rd, line);
            lines.push_back(line);
            int days=calculateDaysPassed(line);
            getline(ac_rd, line);
            line=to_string(days-15);
            lines.push_back(line);
        }

        getline(ac_rd, line);
        lines.push_back(line);
        getline(ac_rd, line);
        lines.push_back("0");

        while (getline(ac_rd, line))
        {
            lines.push_back(line);
        }
        ac_rd.close();

        ofstream ac_wr("a1.txt");
        for (const auto &l : lines)
        {
            ac_wr << l << "\n";
        }
        ac_wr.close();
    }

};

class FacultyAccount: public Account
{

public:

    FacultyAccount(string userid,string pass)
    {
        UserID=userid;
        password=pass;
    }

    int Verifyuser()
    {
        ifstream faculty("a2.txt");
        int check=0;
        string line;
        while(getline(faculty,line))
        {
            check++;
            if(line==UserID)
            {
                lineno=check;
                getline(faculty,line);
                if(line==password)
                    {
                        getline(faculty,line);
                        getline(faculty,line);
                        curr_borr=stoi(line);
                        return 1;
                    }
                else
                {
                    cout<<"\nIncorrect Password\n";
                    return 0;
                }
            }
        }
        faculty.close();
        cout<<"\nIncorrect UserID\n";
        return 0;
    }

    void showAccountDetails()
    {
        ifstream faculty("a2.txt");
        string line;

        for(int i=0;i<lineno+3;i++)
        {
           getline(faculty,line);

        }
        cout<<"UserID : "<<UserID<<endl;
        cout<<"Currently Borrowed Books :";
        int n1=stoi(line);
        cout<<" "<<n1<<endl;
        if(n1>0)
            cout<<"ISBN\tBorrowed on (YYYY MM DD HH MM SS)"<<endl;
        for(int i=0;i<n1;i++)
        {
            getline(faculty,line);
            cout<<line<<"\t";
            getline(faculty,line);
            cout<<line<<"\n";
        }
        int n2;
        getline(faculty,line);
        getline(faculty,line);
        n2=stoi(line);
        cout<<"History"<<endl;
        if(n2>0)
            cout<<"ISBN"<<endl;
        else
            cout<<"None"<<endl;
        for(int i=0;i<n2;i++)
        {
            getline(faculty,line);
            cout<<line<<"\n";
        }
    }

    void add_bk_to_curr_borr(Books b)
    {
        ifstream ac_rd("a2.txt");
        vector<string> lines;
        string line;
        int check=0;
        while (getline(ac_rd, line))
        {
            check++;
            if (check==lineno+3)
            {
                int n=stoi(line);
                n=n+1;
                string s=to_string(n);
                lines.push_back(s);
                lines.push_back(b.ISBN);
                lines.push_back(getCurrentDateTimeString());
            }
            else
                lines.push_back(line);
        }
        ac_rd.close();

        ofstream ac_wr("a2.txt");
        for (const auto &l : lines)
        {
            ac_wr << l << "\n";
        }
        ac_wr.close();
    }

    void move_bk_from_curr_to_his(Books b)
    {
        ifstream ac_rd("a2.txt");
        vector<string> lines;
        string line;
        int check=0;
        int overdue;
        while (getline(ac_rd, line))
        {
            check++;
            if (check==lineno+3)
            {
                int n=stoi(line);
                string s=to_string(n-1);
                lines.push_back(s);
                for(int i=0;i<n;i++)
                    {
                        getline(ac_rd,line);
                        if(line!=b.ISBN)
                        {
                            lines.push_back(line);
                            getline(ac_rd,line);
                            lines.push_back(line);
                        }
                        else
                        {
                            getline(ac_rd,line);
                            int days=calculateDaysPassed(line);
                            if(days>30)
                                overdue=days-30;
                        }
                    }
                getline(ac_rd,line);
                lines.push_back(line);
                getline(ac_rd,line);
                n=stoi(line);
                line=to_string(n+1);
                lines.push_back(line);
                lines.push_back(b.ISBN);
            }
            else
                lines.push_back(line);
        }
        ac_rd.close();

        ofstream ac_wr("a2.txt");
        for (const auto &l : lines)
        {
            ac_wr << l << "\n";
        }
        ac_wr.close();
        cout<<"The book had been overdue for "<<overdue<<"days."<<endl;
    }

    int max_days()
    {
        ifstream faculty("a2.txt");
        string line;

        for(int i=0;i<lineno+3;i++)
        {
           getline(faculty,line);

        }
        int n=stoi(line);
        int temp_max=0;
        for(int i=0;i<n;i++)
        {
            getline(faculty,line);
            getline(faculty,line);

            if((calculateDaysPassed(line))>temp_max)
                temp_max=calculateDaysPassed(line);
        }
        return temp_max;
    }

};


class LibrarianAccount: public Account
{
public:

    LibrarianAccount(string userid,string pass)
    {
        UserID=userid;
        password=pass;
    }
    int Verifyuser()
    {
        ifstream librarian("a3.txt");
        int check=0;
        string line;
        while(getline(librarian,line))
        {
            check++;
            if(line==UserID)
            {
                lineno=check;
                getline(librarian,line);
                if(line==password)
                {
                    return 1;
                }
                else
                {
                    cout<<"\nIncorrect Password\n";
                    return 0;
                }
            }
        }
        librarian.close();
        cout<<"\nIncorrect UserID\n";
        return 0;
    }
};

class User
{
public:
    string UserID;
};

class Student: public User
{
public:
    Student(string userid)
    {
        UserID=userid;
    }

    void borrowBook(Books b, StudAccount A)
    {
        if(A.showfine()==0)
            {
                if(A.curr_borr<3)
                {
                    if(b.status=="Available")
                    {
                        b.change_book_status_to("Borrowed");
                        A.add_bk_to_curr_borr(b);
                        cout<<"Book borrowed successfully.";
                    }
                    else
                        cout<<"You cannot borrow this book. It is "<<b.status<<" by someone else";
                }
                else
                    cout<<"You have already reached the limit of 3 books, return the books in order to borrow a new one.";
            }
        else
            cout<<"Clear your dues first in order to borrow another book.";
    }

    void returnBook(Books b,StudAccount A)
    {
        b.change_book_status_to("Available");
        A.move_bk_from_curr_to_his(b);
        cout<<"Book has been returned successfully";
    }

    void payfine(StudAccount A)
    {
        if(A.showfine()>0)
        {
            int a=A.showfine();
            A.cleardues();
            cout<<"Dues of Rs"<<a<<" has been cleared successfully.";
        }
        else
            cout<<"Fine is already paid.";
    }
};


class Faculty: public User
{
public:
    Faculty(string userid)
    {
        UserID=userid;
    }

    void borrowBook(Books b, FacultyAccount A)
    {
        if(A.max_days()<60)
        {
            if(A.curr_borr<5)
            {
                if(b.status=="Available")
                {
                    b.change_book_status_to("Borrowed");
                    A.add_bk_to_curr_borr(b);
                    cout<<"Book has been borrowed successfully.";
                }
                else
                    cout<<"You cannot borrow this book. It is "<<b.status<<" by someone else";
            }
            else
                cout<<"You have already reached the limit of 5 books, return the books in order to borrow a new one.";
        }
        else
            cout<<"Return the overdue book first to borrow another book";
    }

    void returnBook(Books b,FacultyAccount A)
    {
        b.change_book_status_to("Available");
        A.move_bk_from_curr_to_his(b);
        cout<<"Book has been returned successfully.";
    }
};

class Librarian: public User
{
public:
    Librarian(string userid)
    {
        UserID=userid;
    }

    void add_book(string title,string author, string publisher, string year,string isbn)
    {
        ifstream bk_rd("book.txt");
        vector<string> lines;
        string line;
        lines.push_back(isbn);
        lines.push_back(title);
        lines.push_back(author);
        lines.push_back(publisher);
        lines.push_back(year);
        lines.push_back("Available");
        lines.push_back(" ");

        while (getline(bk_rd, line))
        {
            lines.push_back(line);
        }
        bk_rd.close();

        ofstream bk_wr("book.txt");
        for (const auto &l : lines)
        {
            bk_wr << l << "\n";
        }
        bk_wr.close();
        cout<<"Book has been added successfully";
    }

    void remove_book(string isbn)
    {
        ifstream bk_rd("book.txt");
        vector<string> lines;
        string line;
        while (getline(bk_rd, line))
        {
            if(line==isbn)
            {
                for(int i=0;i<6;i++)
                    getline(bk_rd, line);
            }
            else
                lines.push_back(line);
        }
        bk_rd.close();

        ofstream bk_wr("book.txt");
        for (const auto &l : lines)
        {
            bk_wr << l << "\n";
        }
        bk_wr.close();
        cout<<"Book has been removed successfully";
    }

    void add_user_stud(string userid,string pass)
    {
        ifstream ac_rd("a1.txt");
        vector<string> lines;
        string line;
        lines.push_back(userid);
        lines.push_back(pass);
        lines.push_back("Current");
        lines.push_back("0");
        lines.push_back("Fine");
        lines.push_back("0");
        lines.push_back("History");
        lines.push_back("0");
        lines.push_back(" ");

        while (getline(ac_rd, line))
        {
            lines.push_back(line);
        }
        ac_rd.close();

        ofstream ac_wr("a1.txt");
        for (const auto &l : lines)
        {
            ac_wr << l << "\n";
        }
        ac_wr.close();
        cout<<"Student has been added successfully";
    }

    void remove_user_stud(string userid)
    {
        ifstream ac_rd("a1.txt");
        vector<string> lines;
        string line;
        int check_user=0;
        while (getline(ac_rd, line))
        {
            if(line==userid)
            {
                check_user=1;
                getline(ac_rd, line);
                getline(ac_rd, line);
                getline(ac_rd, line);
                int n=stoi(line);
                for(int i=0;i<3*n;i++)
                {
                    getline(ac_rd, line);
                }
                getline(ac_rd, line);
                getline(ac_rd, line);
                getline(ac_rd, line);
                getline(ac_rd, line);
                n=stoi(line);
                for(int i=0;i<n+2;i++)
                {
                    getline(ac_rd, line);
                }
            }
            lines.push_back(line);
        }
        ac_rd.close();

        ofstream ac_wr("a1.txt");
        for (const auto &l : lines)
        {
            ac_wr << l << "\n";
        }
        ac_wr.close();
        if(check_user)
            cout<<"Student has been removed successfully";
        else
            cout<<"No student exists with this user name";
    }

    void add_user_facu(string userid,string pass)
    {
        ifstream ac_rd("a2.txt");
        vector<string> lines;
        string line;
        lines.push_back(userid);
        lines.push_back(pass);
        lines.push_back("Current");
        lines.push_back("0");
        lines.push_back("History");
        lines.push_back("0");
        lines.push_back(" ");

        while (getline(ac_rd, line))
        {
            lines.push_back(line);
        }
        ac_rd.close();

        ofstream ac_wr("a2.txt");
        for (const auto &l : lines)
        {
            ac_wr << l << "\n";
        }
        ac_wr.close();
        cout<<"Faculty has been added successfully";
    }

    void remove_user_facu(string userid)
    {
        ifstream ac_rd("a2.txt");
        vector<string> lines;
        string line;
        int check_user=0;

        while (getline(ac_rd, line))
        {
            if(line==userid)
            {
                check_user=1;
                getline(ac_rd, line);
                getline(ac_rd, line);
                getline(ac_rd, line);
                int n=stoi(line);
                for(int i=0;i<2*n;i++)
                {
                    getline(ac_rd, line);
                }
                getline(ac_rd, line);
                getline(ac_rd, line);
                n=stoi(line);
                for(int i=0;i<n+2;i++)
                {
                    getline(ac_rd, line);
                }
            }
            lines.push_back(line);
        }
        ac_rd.close();

        ofstream ac_wr("a2.txt");
        for (const auto &l : lines)
        {
            ac_wr << l << "\n";
        }
        ac_wr.close();
        if(check_user)
            cout<<"Faculty has been removed successfully";
        else
            cout<<"No faculty exists with this user name";
    }


};

int main()
{
    cout<<"Welcome to IITK Library"<<endl;
    cout<<"Enter \n1 for Student \n2 for Faculty \n3 for Librarian\n";
    int n;
    cin>>n;
    cout<<"Enter UserID \n";
    string UserID;
    string pass;
    cin>>UserID;
    cout<<"Enter Password\n";
            char ch;
            while ((ch = _getch()) != '\r')
            {
                if (ch == '\b')
                {
                    if (!pass.empty())
                    {
                        pass.pop_back();
                        cout << "\b \b";
                    }
                }
                else
                {
                    pass.push_back(ch);
                    std::cout << '*';
                }
            }

    if(n==1)
    {
        StudAccount Stud(UserID,pass);
        if(Stud.Verifyuser()==1)
        {
            cout<<"\nSuccessfully Logged in.\n";
            Student s1(UserID);
            cout<<"Enter \n1 to borrow a book \n2 to return the book \n3 to show account details \n4 to clear dues\n";
            int x;
            string isbn;
            cin>>x;
            if(x==1)
            {
                cout<<"Enter ISBN\n";
                cin>>isbn;
                Books b1(isbn);
                s1.borrowBook(b1,Stud);
            }
            else if(x==2)
            {
                cout<<"Enter ISBN\n";
                cin>>isbn;
                Books b1(isbn);
                s1.returnBook(b1,Stud);
            }
            else if(x==3)
            {
                Stud.showAccountDetails();
            }
            else if(x==4)
            {
                s1.payfine(Stud);
            }
            else
                cout<<"Please enter the number from the given numbers";

        }
        else
        {
            cout<<"Try Again";
        }
    }

    else if(n==2)
    {
        FacultyAccount fac(UserID,pass);
        if(fac.Verifyuser()==1)
        {
            cout<<"\nSuccessfully Logged in.\n";
            Faculty f1(UserID);
            cout<<"Enter \n1 to borrow a book \n2 to return the book \n3 to show account details\n";
            int x;
            string isbn;
            cin>>x;
            if(x==1)
            {
                cout<<"Enter ISBN\n";
                cin>>isbn;
                Books b1(isbn);
                f1.borrowBook(b1,fac);
            }
            else if(x==2)
            {
                cout<<"Enter ISBN\n";
                cin>>isbn;
                Books b1(isbn);
                f1.returnBook(b1,fac);
            }
            else if(x==3)
            {
                fac.showAccountDetails();
            }
            else
                cout<<"No function exists corresponding to the input number. Try again by entering the correct no.";
        }
        else
        {
                cout<<"Try again";
        }

    }
    else
    {
        LibrarianAccount Lib(UserID,pass);
        if(Lib.Verifyuser()==1)
        {
            cout<<"\nSuccessfully Logged in.\n";
            Librarian lib(UserID);
            cout<<"Enter \n1 to add a book \n2 to remove a book \n3 to add a user \n4 to remove a user\n";
            int x;
            cin>>x;
            if(x==1)
            {
                string title, author, publisher,year,ISBN;
                cout<<"Enter title\n";
                cin>>title;
                cout<<"Enter author\n";
                cin>>author;
                cout<<"Enter publisher\n";
                cin>>publisher;
                cout<<"Enter year\n";
                cin>>year;
                cout<<"Enter ISBN\n";
                cin>>ISBN;
                lib.add_book(title, author, publisher,year,ISBN);
            }
            else if(x==2)
            {
                string isbn;
                cout<<"Enter ISBN\n";
                cin>>isbn;
                lib.remove_book(isbn);
            }
            else if(x==3)
            {
                int y;
                string userid,pass;
                cout<<"Enter \n1 to add Student \n2 to add faculty\n";
                cin>>y;
                cout<<"UserID\n";
                cin>>userid;
                cout<<"password\n";
                cin>>pass;
                if(y==1)
                    lib.add_user_stud(userid,pass);
                else
                    lib.add_user_facu(userid,pass);
            }
            else if(x==4)
            {
                int y;
                string userid;
                cout<<"Enter \n1 to remove Student \n2 to remove faculty\n";
                cin>>y;
                cout<<"UserID\n";
                cin>>userid;
                if(y==1)
                    lib.remove_user_stud(userid);
                else
                    lib.remove_user_facu(userid);
            }
            else
                cout<<"No command exists corresponding to the given number, enter correct number";
        }
        else
        {
            cout<<"Try Again";
        }
    }

    return 0;
}














