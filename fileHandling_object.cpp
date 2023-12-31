/* Author : Bharat Rathor. */
// playlist in which this program is done!.
//  https://www.youtube.com/playlist?list=PLLYz8uHU480i1pSs2w_Vzp671ea95PeWY
/*
    File opening Modes :
    1. ios::in    : input/read something form file.
    2. ios::out   : output/write something to file.
    3. ios::app   : append to the existing file.
    4. ios::ate   : update to the existing file .
                    some special operation perform in this mode.
    5. ios::binary: file opened as binary mode.
*/
#include <iostream> // cin |cout.
#include <cstring>  //strcmp().
#include <fstream>  //filehandling.
#include <cstdio>   //rename() | remove().
#include <colors.h> //colors.
#include <iomanip>  //setw().

using namespace std;

// This is a simple Book class .
class Book
{
private:
    unsigned int bookId;
    unsigned int price;
    char bookTitle[20];
    void print_all_records() const
    {
        cout << left;
        cout << "| " << setw(5) << bookId << "\t\t| " << setw(20) << bookTitle << "\t|\t " << setw(7) << price << "  |" << endl;
    }

public:
    // Default constructor.
    Book()
    {
        bookId = 0;
        strcpy(bookTitle, "No Titile");
        price = 0;
    }

    // Parameterized constructor.
    Book(size_t bookid, const char *title, size_t book_price)
    {
        bookId = bookid;
        strcpy(bookTitle, title);
        price = book_price;
    }

    // Function to take book data.
    void setBookData()
    {
        cout << cyan << bold << italic << "Enter BookId : " << colorReset;
        cin >> bookId;
        cin.ignore(); // clear data buffer.
        cout << cyan << bold << italic << "Enter BookTitle : " << colorReset;
        cin.getline(bookTitle, 19); // entered data in bookTitle
        cout << cyan << bold << italic << "Enter price : " << colorReset;
        cin >> price;
    }

    // Function to show book data.
    void showBookData()
    {
        cout << endl
             << red << bold;
        cout << "---------------------------------" << endl;
        cout << "BookId    : " << bookId << endl;
        cout << "BookTitle : " << bookTitle << endl;
        cout << "Price     : " << price << endl;
        cout << "---------------------------------" << endl
             << colorReset;
    }

    // store bookdata to the file.
    int storeBook()
    {
        if (bookId == 0 and price == 0)
        {
            cout << red underline << "Book data is not initilized!!" << endl
                 << colorReset;
            return 0;
        }
        else
        {
            ofstream fout;
            fout.open("Bookfile.dat", ios::app | ios::binary);

            // type casted this cuz write function takes const char* | sizeof *this cuz deref this(object)
            fout.write((char *)this, sizeof(*this));
            fout.close();
            return 1;
        }
    }

    // input file to read the books record.
    void viewAllBooks()
    {
        ifstream fin("Bookfile.dat", ios::in | ios::binary);
        if (not fin.is_open())
        {
            cout << red << underline << "File is not found!" << endl
                 << colorReset;
        }
        else
        {
            cout << endl
                 << red << bold
                 << "-----------------------------------------------------------" << endl
                 << "|";
            cout << cyan << bold << "BOOK ID \t\tBOOK TITILE\t\tBOOK PRICE" << colorReset;
            cout << red << bold "|" << endl
                 << "-----------------------------------------------------------" << endl;
            fin.read((char *)this, sizeof(*this));
            while (not fin.eof())
            {
                // showBookData();
                print_all_records();
                // repeating till eof()
                fin.read((char *)this, sizeof(*this));
            }
            cout << red << bold
                 << "-----------------------------------------------------------" << colorReset << endl;
            fin.close();
        }
    }

    bool searchBook(char *find)
    {
        bool isfound = false;
        ifstream fin("Bookfile.dat", ios::in | ios::binary);
        if (not fin.is_open())
        {
            cout << "File is not found!" << endl;
        }
        else
        {
            fin.read((char *)this, sizeof(*this));
            while (not fin.eof())
            {
                if (strcmp(find, bookTitle) == 0)
                {
                    cout << green bold << " __________________________________" << endl;
                    cout << "|__________Book is found___________|" << endl
                         << colorReset;
                    showBookData();
                    isfound = true;
                }
                fin.read((char *)this, sizeof(*this));
            }
            if (not isfound)
            {
                cout << red underline << "Record is not found !!" << endl
                     << colorReset;
            }
        }
        fin.close();
        return isfound;
    }

    bool deleteBook(char *del_book)
    {
        bool isdeleted = false;
        ofstream fout;
        ifstream fin("Bookfile.dat", ios::in | ios::binary);
        if (not fin.is_open())
        {
            cout << red << underline << "File is not found!" << endl
                 << colorReset;
        }
        else
        {
            fout.open("tempfile.dat", ios::out | ios::binary);

            fin.read((char *)this, sizeof(*this));
            while (not fin.eof())
            {
                if (strcmp(del_book, bookTitle))
                {
                    fout.write((char *)this, sizeof(*this));
                }
                else
                {
                    isdeleted = true;
                }
                fin.read((char *)this, sizeof(*this));
            }
        }
        fin.close();
        fout.close();
        // deleting the old file.
        remove("Bookfile.dat");
        // renaming the temp file with old file.
        rename("tempfile.dat", "Bookfile.dat");
        // return true / false.
        return isdeleted;
    }

    bool updateBook(const char *update_it)
    {
        bool isupdate = false;
        fstream file;
        file.open("Bookfile.dat", ios::in | ios::out | ios ::ate | ios::binary);
        // seekg(0)-> set the filePtr at beg. cuz in update mode filePtr points the last byte.
        file.seekg(0);
        file.read((char *)this, sizeof(*this));
        while (not file.eof())
        {
            if (not strcmp(update_it, bookTitle))
            {
                setBookData();
                // moving file_ptr 1 record prev.
                int curr_pos = file.tellp();
                file.seekp(curr_pos - sizeof(*this));
                // writing the current record in file.
                file.write((char *)this, sizeof(*this));
                isupdate = true;
            }

            file.read((char *)this, sizeof(*this));
        }
        file.close();
        // returning true / false.
        return isupdate;
    }
};

int menu()
{
    size_t choice;
    cout << endl
         << yellow << bold;
    cout << "--------Book Management System---------" << endl;
    cout << "    (1). FOR INSERT BOOK RECORD" << endl;
    cout << "    (2). FOR VIEW ALL RECORDS" << endl;
    cout << "    (3). FOR SEARCH RECORDS" << endl;
    cout << "    (4). FOR DELETE RECORDS" << endl;
    cout << "    (5). FOR UPDATE RECORDS" << endl;
    cout << "    (6). FOR EXIT" << endl;
    cout << endl
         << "ENTER YOUR CHOICE : " << colorReset;
    cin >> choice;
    return choice;
}

int main()
{
    Book book;
    char find[20];
    char choice;
    while (true)
    {
        system("cls");
        switch (menu())
        {
        case 1:
            book.setBookData();
            cout << endl;
            cout << grey << bold << "Do you want to store it permanently (y/n) : " << colorReset;
            cin >> choice;
            if (choice == 'y')
            {
                book.storeBook();
                cout << boxYellow << red bold << "Book Stored Successfully!" << endl
                     << colorReset;
            }
            break;
        case 2:
            book.viewAllBooks();
            break;
        case 3:
            cout << cyan << italic << bold "Enter the Book title to search the record : " << colorReset;
            cin.ignore();
            cin.getline(find, sizeof(find) - 1);
            book.searchBook(find);
            break;
        case 4:
            cout << cyan << italic << bold << "Enter the Book title to delete the record : " << colorReset;
            cin.ignore();
            cin.getline(find, sizeof(find) - 1);
            if (book.deleteBook(find))
                cout << boxYellow << red bold << "Record is deleted Successfully!" << endl
                     << colorReset;
            else
                cout << red << underline << "Record deletion Failed!!" << endl
                     << colorReset;
            break;
        case 5:
            cout << cyan << italic << bold "Enter the Book title to update the record : " << colorReset;
            cin.ignore();
            cin.getline(find, sizeof(find) - 1);
            if (book.updateBook(find))
                cout << boxYellow << red bold << "Book updated Successfully!" << endl
                     << colorReset;
            else
                cout << red << underline << "Book updation Failed!!" << endl
                     << colorReset;
            break;
        case 6:
            cout << grey << bold << "[EXIT] Are you sure ? (y/n) : " << colorReset;
            cin >> choice;
            if (choice not_eq 'y')
                continue;
            cout << boxBlue << bold << yellow << "Thanks!! For Using The Application!!" << endl
                 << colorReset;
            system("pause");
            system("cls");
            exit(EXIT_SUCCESS);
            break;
        default:
            cout << grey << bold << "Enter a valid Choice !!" << endl
                 << colorReset;
        }
        system("pause");
    }

    return 0;
}