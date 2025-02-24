#include <iostream>
#include <fstream>
#include <Windows.h>
using namespace std;

struct User {
    string f_name, l_name, ph_no, address, cnic;
    int amount;
    User* left;
    User* right;
    User(string fname, string lname, string phone, string addr, string cnic_id, int bal)
        : f_name(fname), l_name(lname), ph_no(phone), address(addr), cnic(cnic_id), amount(bal), left(nullptr), right(nullptr) {}
};

// Function to insert a new user into the BST
User* insertUser(User* root, User* newUser) {
    if (!root)
        return newUser;

    if (newUser->f_name < root->f_name)
        root->left = insertUser(root->left, newUser);
    else
        root->right = insertUser(root->right, newUser);

    return root;
}

// Function to search for a user by first name in the BST
User* searchUser(User* root, const string& name) {
    if (!root || root->f_name == name)
        return root;

    if (name < root->f_name)
        return searchUser(root->left, name);

    return searchUser(root->right, name);
}

// Function to find the minimum value node (used in deletion)
User* minValueNode(User* node) {
    User* current = node;
    while (current && current->left != nullptr)
        current = current->left;
    return current;
}

// Function to delete a user from the BST
User* deleteUser(User* root, const string& name) {
    if (!root)
        return root;

    if (name < root->f_name)
        root->left = deleteUser(root->left, name);
    else if (name > root->f_name)
        root->right = deleteUser(root->right, name);
    else {
        if (!root->left) {
            User* temp = root->right;
            delete root;
            return temp;
        } else if (!root->right) {
            User* temp = root->left;
            delete root;
            return temp;
        }

        User* temp = minValueNode(root->right);
        root->f_name = temp->f_name;
        root->right = deleteUser(root->right, temp->f_name);
    }
    return root;
}

// Function to display user information
void displayUser(User* user) {
    if (user) {
        cout << "First Name: " << user->f_name << endl;
        cout << "Last Name: " << user->l_name << endl;
        cout << "Phone No.: " << user->ph_no << endl;
        cout << "Address: " << user->address << endl;
        cout << "CNIC: " << user->cnic << endl;
        cout << "Balance: " << user->amount << endl;
    }
}

// Function to perform in-order traversal (used for sorting)
void inOrder(User* root) {
    if (root) {
        inOrder(root->left);
        displayUser(root);
        inOrder(root->right);
    }
}

// Function to save the BST to a file
void saveToFile(User* root, ofstream& outFile) {
    if (root) {
        outFile << root->f_name << " " << root->l_name << " " << root->ph_no << " "
                << root->address << " " << root->cnic << " " << root->amount << endl;
        saveToFile(root->left, outFile);
        saveToFile(root->right, outFile);
    }
}

// Function to load data from file into BST
User* loadFromFile(ifstream& inFile) {
    User* root = nullptr;
    string fname, lname, phone, address, cnic;
    int amount;

    while (inFile >> fname >> lname >> phone >> address >> cnic >> amount) {
        User* newUser = new User(fname, lname, phone, address, cnic, amount);
        root = insertUser(root, newUser);
    }
    return root;
}

int main() {
    int choice;
    User* root = nullptr;
    ifstream inFile("users.dat");

    if (inFile.is_open()) {
        root = loadFromFile(inFile);
        inFile.close();
    }

    do {
        cout << "1. Open Account\n2. Deposit\n3. Withdraw\n4. Check Balance\n";
        cout << "5. Update Record\n6. Delete Record\n7. Search Record\n8. Display All Users\n9. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            string fname, lname, phone, address, cnic;
            int amount;
            cout << "Enter First Name: "; cin >> fname;
            cout << "Enter Last Name: "; cin >> lname;
            cout << "Enter Phone No.: "; cin >> phone;
            cout << "Enter Address: "; cin >> address;
            cout << "Enter CNIC: "; cin >> cnic;
            cout << "Enter Initial Deposit: "; cin >> amount;

            User* newUser = new User(fname, lname, phone, address, cnic, amount);
            root = insertUser(root, newUser);
        } 
        else if (choice == 2) {
            string fname;
            int deposit;
            cout << "Enter First Name: "; cin >> fname;
            User* user = searchUser(root, fname);
            if (user) {
                cout << "Current Balance: " << user->amount << endl;
                cout << "Enter Deposit Amount: "; cin >> deposit;
                user->amount += deposit;
            } else {
                cout << "User not found!" << endl;
            }
        } 
        else if (choice == 3) {
            string fname;
            int withdraw;
            cout << "Enter First Name: "; cin >> fname;
            User* user = searchUser(root, fname);
            if (user) {
                cout << "Current Balance: " << user->amount << endl;
                cout << "Enter Withdraw Amount: "; cin >> withdraw;
                if (withdraw <= user->amount) {
                    user->amount -= withdraw;
                } else {
                    cout << "Insufficient Balance!" << endl;
                }
            } else {
                cout << "User not found!" << endl;
            }
        } 
        else if (choice == 4) {
            string fname;
            cout << "Enter First Name: "; cin >> fname;
            User* user = searchUser(root, fname);
            if (user) {
                cout << "Balance: " << user->amount << endl;
            } else {
                cout << "User not found!" << endl;
            }
        } 
        else if (choice == 5) {
            string fname;
            cout << "Enter First Name: "; cin >> fname;
            User* user = searchUser(root, fname);
            if (user) {
                cout << "Current Details:" << endl;
                displayUser(user);

                cout << "Update First Name: "; cin >> user->f_name;
                cout << "Update Last Name: "; cin >> user->l_name;
                cout << "Update Phone No.: "; cin >> user->ph_no;
                cout << "Update Address: "; cin >> user->address;
                cout << "Update CNIC: "; cin >> user->cnic;
            } else {
                cout << "User not found!" << endl;
            }
        } 
        else if (choice == 6) {
            string fname;
            cout << "Enter First Name: "; cin >> fname;
            root = deleteUser(root, fname);
        } 
        else if (choice == 7) {
            string fname;
            cout << "Enter First Name: "; cin >> fname;
            User* user = searchUser(root, fname);
            if (user) {
                displayUser(user);
            } else {
                cout << "User not found!" << endl;
            }
        } 
        else if (choice == 8) {
            inOrder(root);
        }

    } while (choice != 9);

    ofstream outFile("users.dat");
    if (outFile.is_open()) {
        saveToFile(root, outFile);
        outFile.close();
    }

    cout << "Thank you for using the banking system!" << endl;
    return 0;  
}
