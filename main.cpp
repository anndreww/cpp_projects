#include<iostream>
#include<algorithm>
#include<cassert>
using namespace std;

#define max_books 10
#define max_users 10

struct book {
    int id;
    string name;
    int quantity;
    int borrowers;
    int users_with_book[max_users]{}, len;

    book() {
        quantity = borrowers = len = 0;
        id = -1;
        name = " ";
    }

    void read() {
        cout << "Enter ID, Name, Quantity: ";
        cin >> id >> name >> quantity;
    }

    bool borrow(int user_id) {
        if (quantity - borrowers == 0)
            return false;
        users_with_book[len++] = user_id;
        ++borrowers;
        return true;
    }

    void return_copy() {
        assert(borrowers > 0);
        --borrowers;
    }

    bool has_prefix(string prefix) {
        if (name.size() < prefix.size())
            return false;

        for (int i = 0; i < prefix.size(); ++i) {
            if (prefix[i] != name[i])
                return false;
        }
        return true;
    }
    void print() {
        cout << "\nID - " << id << ", Name - " << name << ", Quantity - "
             << quantity << ", Borrow - " << borrowers;
    }
};

bool cmp_book_by_name(book &a, book &b) {
    return a.name < b.name;
}

bool cmp_book_by_id(book &a, book &b) {
    return a.id < b.id;
}

struct user {
    int id;
    string name;
    int user_borrowed_books[max_books];
    int len;

    user() {
        name = " ";
        len = 0;
        id = -1;
    }

    void read() {
        cout << "Enter user name and ID: ";
        cin >> name >> id;
    }

    void borrow(int book_id) {
        user_borrowed_books[len++] = book_id;
    }
    void return_copy(int book_id) {
        bool removed = false;
        for (int i = 0; i < len; ++i) {
            if (user_borrowed_books[i] == book_id) {
                for (int j = i; j < len - 1; ++j)
                    user_borrowed_books[j] = user_borrowed_books[j + 1];
                removed = true;
                --len;
                break;
            }
        }
        if (!removed)
            cout << "User " << name << " never borrowed this book" << "\n";
    }

    bool is_borrowed(int book_id) {
        for (int i = 0; i < len; ++i) {
            if (book_id == user_borrowed_books[i])
                return true;
        }
        return false;
    }

    void print() {
        sort(user_borrowed_books, user_borrowed_books + len);

        cout << "\nUser " << name << " borrowed next books: ";
        for (int i = 0; i < len; ++i)
            cout << user_borrowed_books[i] << " ";
        cout << "\n";
    }
};

struct library_system {
    int total_books;
    book books[max_books];
    int total_users;
    user users[max_users];

    library_system() {
        total_books = total_users = 0;
    }

    void run() {
        while (true) {
            int number = list();

            switch (number) {
                case 1:
                    add_book();
                    break;
                case 2:
                    search_books_by_prefix();
                    break;
                case 3:
                    print_who_borrowed_book_by_name();
                    break;
                case 4:
                    list_books_by_id();
                    break;
                case 5:
                    list_books_by_name();
                    break;
                case 6:
                    add_user();
                    break;
                case 7:
                    user_borrow_book();
                    break;
                case 8:
                    user_return_book();
                    break;
                case 9:
                    print_users();
                    break;
                default:
                    return;
            }
        }
    }

    int list() {
        int number = -1;

        cout << "\n1 - Add a book\n";
        cout << "2 - Search books by prefix\n";
        cout << "3 - Users who borrowed\n";
        cout << "4 - Books by ID\n";
        cout << "5 - Books by name\n";
        cout << "6 - Add a user\n";
        cout << "7 - Borrow a book\n";
        cout << "8 - Return a book\n";
        cout << "9 - Users\n";
        cout << "10 - Exit\n";

        while (number == -1) {

            cout << "\nEnter a number: ";
            cin >> number;

            if (!(1 <= number && number <= 10)) {
                cout << "Undefined. Try again\n";
                number = -1;
            }
        }
        return number;
    }

    void add_book() {
        book var;
        var.read();
        for (int i = 0; i < total_books; ++i) {
            if (books[i].id == var.id || books[i].name == var.name) {
                books[i].quantity += var.quantity;
                return;
            }
        }
        books[total_books++] = var;
    }

    void search_books_by_prefix() {
        cout << "Enter the prefix: ";
        string prefix;
        cin >> prefix;

        int cnt = 0;
        for (int i = 0; i < total_books; ++i) {
            if (books[i].has_prefix(prefix)){
                cout << books[i].name << "\n";
                ++cnt;
            }
        }

        if(!cnt)
            cout<<"No books founded\n";
    }

    void add_user() {
        users[total_users++].read();
    }

    int bookIndexByName(string name) {
        for (int i = 0; i < total_books; ++i) {
            if (name == books[i].name)
                return i;
        }
        return -1;
    }

    int userIndexByName(string name) {
        for (int i = 0; i < total_users; ++i) {
            if (name == users[i].name)
                return i;
        }
        return -1;
    }

    bool userNameAndBookName(int &user_index, int &book_index, int trials = 3) {
        string user_name;
        string book_name;

        while (trials--) {
            cout << "Enter user name and book name: ";
            cin >> user_name >> book_name;

            user_index = userIndexByName(user_name);

            if (user_index == -1) {
                cout << "Invalid user name. Try again\n";
                continue;
            }
            book_index = bookIndexByName(book_name);

            if (book_index == -1) {
                cout << "Invalid book name. Try again\n";
                continue;
            }
            return true;
        }
        cout << "You did several trials! Try later.";
        return false;
    }

    void user_borrow_book() {
        int user_index, book_index;
        // Prompt for input user name and book name
        if (!userNameAndBookName(user_index, book_index))
            return;

        int user_id = users[user_index].id;
        int book_id = books[book_index].id;

        if (!books[book_index].borrow(user_id))
            cout << "No more books available to borrow\n";
        else
            users[user_index].borrow(book_id);
    }

    void user_return_book() {
        int user_index, book_index;
        // Prompt for input user name and book name
        if (!userNameAndBookName(user_index, book_index))
            return;

        int book_id = books[book_index].id;
        books[book_index].return_copy();
        users[user_index].return_copy(book_id);
    }

    void list_books_by_id() {
        sort(books, books + total_books, cmp_book_by_id);

        for (int i = 0; i < total_books; ++i)
            books[i].print();
        cout << "\n";
    }

    void list_books_by_name() {
        sort(books, books + total_books, cmp_book_by_name);

        for (int i = 0; i < total_books; ++i)
            books[i].print();
        cout << "\n";
    }

    void print_users() {
        for (int i = 0; i < total_users; ++i)
            users[i].print();
    }

    void print_who_borrowed_book_by_name() {
        string book_name;
        cout << "Enter book name: ";
        cin >> book_name;

        int book_index = bookIndexByName(book_name);

        if (book_index == -1) {
            cout << "Invalid book name.\n";
            return;
        }
        int book_id = books[book_index].id;

        if (books[book_index].borrowers == 0) {
            cout << "No borrowed copies\n";
            return;
        }

        for (int i = 0; i < total_users; ++i) {
            if (users[i].is_borrowed(book_id))
                cout << users[i].name << "\n";
        }

    }
};

int main() {

    library_system library;
    library.run();

    return 0;
} 
