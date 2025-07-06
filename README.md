# Library_Management_System
This README provides instructions for compilation and execution of the software. It also provides the explanation of the main code and file structure.


Overview:
This is a Library Management System implemented in C++. It allows students, faculty, and librarians to manage book borrowing, returning, and user accounts efficiently. The system reads and writes data to text files to maintain user and book records.

Instruction regarding compilation and execution
1. Compile the code using a C++ compiler:
   g++ -o library_system main.cpp   
2. Run the executable:
   ./library_system   
3. Choose your user type by entering the corresponding number.(1=Student,2=Faculty,3=Librarian).
4. Enter your UserID and Password to log in.
5. Perform operations based on the available options.


File Structure

- `book.txt`: This txt file stores book details in the following format:
		ISBN
		Title 
		Author 
		Publisher 	
		Year 	
		Status(Available/Reserved/Borrowed)

- `a1.txt`: This txt files stores student account information in the following format:
		UserID 
		Password 
		Current No of currently borrowed books(n)  
		(for each book we will have the next 3 lines) 
		isbn of the borrowed book 	
		borrowed on 
		No of days for which fine is already paid 	
		Fine on returned books 
		Rs X History 
		No of books borrowed before
		isbn of each book

- `a2.txt`: This txt files stores faculty account information in the following format:
		UserID 
		Password 
		Current No of currently borrowed books(n)  
		(for each book we will have the next 3 lines) 
		isbn of the borrowed book 	
		borrowed on 
		No of books borrowed before
		isbn of each book

- `a3.txt`: This txt files stores Librarian Account information in the following format:
		UserID
		Password

- `main.cpp`: Contains the program logic

Dependencies
- C++ Standard Library (iostream, fstream, chrono, string, vector, sstream, vector, iomainp)
- Windows-specific library (`conio.h`) for password input masking

Explanation of the main code:

1. Helper Functions

getCurrentDateTimeString(): Returns the current date and time as a formatted string: "YYYY MM DD HH MM SS".
	Uses chrono and ctime libraries.

calculateDaysPassed(const std::string& dateTimeStr): Calculates the number of days passed since a given date.
	Converts the given string (YYYY MM DD HH MM SS) into a tm struct. Uses std::difftime() to get the 	difference between the current date and the provided date.

2. Books Class

Purpose: Represents a book in the library.
Attributes: title, author, publisher, year, status, ISBN, lineno
Constructor: Books(string isbn)
Reads from book.txt to fetch book details based on the given ISBN.
If the book is not found, it prints a message.
Methods:
change_book_status_to(string Status): Updates the book's status in book.txt (e.g., Available/Borrowed).

3. Account Base Class
Purpose: Represents a user account.
Attributes:
UserID, password, lineno(represents the line number of the userid in the txt file), curr_borr(represents number of currently borrowed books)
Methods:
Verifyuser(): Check the user from the txt files.

3.1 StudAccount (Inherits from Account)
Purpose: Represents a student’s account.
Constructor: StudAccount(string userid, string pass)
	Initializes UserID and password.
Methods:
Verifyuser(): Checks the student’s credentials from a1.txt.
showAccountDetails(): Displays borrowed books, fines, and borrowing history.
add_bk_to_curr_borr(Books b): Adds a book to the student’s currently borrowed books list.
move_bk_from_curr_to_his(Books b): Moves a returned book from currently borrowed list to the borrowing history.
showfine(): Calculates the fine based on overdue books.
cleardues(): Clears the outstanding fine.

3.2 FacultyAccount (Inherits from Account)
Purpose: Represents a faculty’s account.
Constructor: FacultyAccount(string userid, string pass)
Initializes UserID and password.
Methods:
Verifyuser(): Checks the faculty’s credentials from a2.txt.
showAccountDetails(): Displays borrowed books and history.
add_bk_to_curr_borr(Books b): Adds a borrowed book to faculty’s record.
move_bk_from_curr_to_his(Books b): Moves a returned book to history.
max_days(): Finds the maximum overdue days for a faculty member.

3.3 LibrarianAccount (Inherits from Account)
Purpose: Represents a librarian’s account.
Constructor: LibrarianAccount(string userid, string pass)
Methods:
Verifyuser(): Checks the librarian’s credentials from a3.txt.

4. User Base Class
Purpose: Represents a general user with UserID.

4.1. Student (Inherits from User)
Purpose: Represents a student user.
Constructor: Student(string userid)
Methods:
borrowBook(Books b, StudAccount A): Allows borrowing a book if no fine and limit (<3 books).
returnBook(Books b, StudAccount A): Allows returning a book.
payfine(StudAccount A): Clears the outstanding fine.

4.2 Faculty (Inherits from User)
Purpose: Represents a faculty user.
Methods:
borrowBook(Books b, FacultyAccount A): Faculty can borrow a book if no overdue books (limit: 5 books).
returnBook(Books b, FacultyAccount A): Returns a book.

4.3 Librarian (Inherits from User)
Purpose: Represents a librarian user.
Methods:
add_book(string title, string author, string publisher, string year, string isbn): Adds a new book to book.txt.
remove_book(string isbn): Removes a book from book.txt.
add_user_stud(string userid, string pass): Adds a new student account to a1.txt.
remove_user_stud(string userid): Removes a student account form a1.txt.
add_user_facu(string userid, string pass): Adds a new faculty account to a2.txt.
remove_user_facu(string userid): Removes a faculty account form a2.txt.

5. main() Function
Purpose: Handles user login and operations.
Steps:
Asks user to select a role:
1 → Student
2 → Faculty
3 → Librarian

Then asks for UserID and password`.

Password is entered using _getch() (hides input with *).
Authenticate the user.

Calls Verifyuser() for respective account type.
Perform actions based on user role:

Student: Borrow, return, view account, clear dues.
Faculty: Borrow, return, view account.
Librarian: Add/remove books, add/remove users.





