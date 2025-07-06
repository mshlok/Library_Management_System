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

## Key Features
- **Role-based access control** (Student, Faculty, Librarian)
- **Book management** (add/remove/update)
- **User account management**
- **Borrow/return operations**
- **Fine calculation** for overdue books
- **File I/O persistence** for all data

## Class Structure

### Helper Functions
- `getCurrentDateTimeString()`  
  Returns current date/time as "YYYY MM DD HH MM SS" string
- `calculateDaysPassed(const std::string& dateTimeStr)`  
  Calculates days passed since given date

### Books Class
**Purpose**: Represents a book in the library  
**Attributes**:
- title, author, publisher, year
- status, ISBN, lineno

**Methods**:
- `Books(string isbn)` (constructor)
- `change_book_status_to(string Status)`

### Account Hierarchy (Base Class)
**Attributes**:
- UserID, password
- lineno (file line number)
- curr_borr (current borrowed count)

#### 1. StudAccount (Student)
**Methods**:
- `Verifyuser()` (checks a1.txt)
- `showAccountDetails()`
- `add_bk_to_curr_borr(Books b)`
- `move_bk_from_curr_to_his(Books b)`
- `showfine()`, `cleardues()`

#### 2. FacultyAccount (Faculty)
**Methods**:
- `Verifyuser()` (checks a2.txt)
- `max_days()` (calculates max overdue)

#### 3. LibrarianAccount (Librarian)
**Methods**:
- `Verifyuser()` (checks a3.txt)

### User Hierarchy (Base Class)
**Attributes**: UserID

#### 1. Student
**Methods**:
- `borrowBook(Books b, StudAccount A)`
- `returnBook(Books b, StudAccount A)`
- `payfine(StudAccount A)`

#### 2. Faculty
**Methods**:
- `borrowBook(Books b, FacultyAccount A)`
- `returnBook(Books b, FacultyAccount A)`

#### 3. Librarian
**Methods**:
- `add_book()`/`remove_book()`
- `add_user_stud()`/`remove_user_stud()`
- `add_user_facu()`/`remove_user_facu()`

## Main Program Flow
1. Role selection (1-Student, 2-Faculty, 3-Librarian)
2. UserID/password authentication (hidden input)
3. Role-specific operations:
   - **Students**: Borrow/return, view account, clear dues
   - **Faculty**: Borrow/return, view account
   - **Librarians**: Manage books/users





