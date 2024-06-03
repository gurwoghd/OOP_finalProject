#include <iostream>
#include<string>
#include<vector>
using namespace std;

class User {
private:
	string identification;
	string password;
public:
	User(const string& id, const string& pw) : identification(id), password(pw) {}

	string getID() const {
		return identification;
	}

	string getPW() const {
		return password;
	}

};

class UserManager {
private:
	vector<User> users;
public :
	void addNewUser(const string& id, const string& pw) {
		users.push_back(User(id, pw));
	}

	bool checkAll(const string& id, const string& pw) {
		for (const auto& ID : users) {
			if (ID.getID() == id && ID.getPW() == pw) {
				return true;
			}
		}
		return false;
	}
	bool checkID(const string& id) {
		for (const auto& ID : users) {
			if (ID.getID() == id) {
				return true;
			}
		}
		return false;
	}
};

//virtual class Command//
class Command {
public:
	virtual void execute() = 0;
	virtual ~Command() {}
};

//register and login commands have inheritance from Command, polymorphism//
class RegisterCommand : public Command {
private:
	//UserManager is needed in order to add the newly made account into the user vector//
	UserManager& Manager;
public:
	RegisterCommand(UserManager & manager) : Manager(manager) {}
	
	//override execute from virtual class Command, polymorphism//
	void execute() override {
		string ID;
		string PW;
		cout << "\nEnter an ID to use: ";
		cin >> ID;
		cout << "Enter a password to use: ";
		cin >> PW;
		if (!Manager.checkID(ID)) {
			Manager.addNewUser(ID, PW);
			cout << "\nRegister Complete! You are ready to log in!" << endl;
		}
		else {
			cout << "\nID or PW already exists. Try again." << endl;
		}

	}
};

class LoginasAdmin : public Command {
private:
	UserManager& Manager;
public:
	LoginasAdmin(UserManager & manager) : Manager(manager) {}

	//override execute from virtual class Command
	void execute() override {
		string ID;
		string PW;
		string adminID = "admin";
		string adminPW = "admin1234";

		cout << "Enter the admin ID: ";
		cin >> ID;
		cout << "\nEnter the admin PW: ";
		cin >> PW;
		if (ID == adminID && PW == adminPW) {
			cout << "Welcome admin!" << endl;
		}
		else {
			cout << "ID or PW is incorrect." << endl;
		}
	}
};

class LoginasCustomer : public Command {
private:
	UserManager& Manager;
public:
	LoginasCustomer(UserManager & manager) : Manager(manager) {}
	
	//override execute from virtual class Command
	void execute() override {
		string ID;
		string PW;
		cout << "\nEnter your ID: ";
		cin >> ID;
		cout << "Enter your password: ";
		cin >> PW;
		if (Manager.checkAll(ID, PW)) {
			cout << "\nLogin successful, welcome " << ID << endl;
		}
		else {
			cout << "\nID or password is incorrect." << endl;
		}
	}
};

//using the classes made above, the menu displays the login section//
class LoginMenu {
private:
	Command* Register;
	Command* Administer;
	Command* Customer;
public:
	LoginMenu(Command* reg, Command* admin, Command* cus): Register(reg), Administer(admin), Customer(cus) {}

	void display() {
		cout << "Welcome to the Book Recommendation System!" << endl;
		while (true) {
			int selection;
			cout << "\n1. Register" << endl;
			cout << "2. Login(Admin)" << endl;
			cout << "3. Login(Customer)" << endl;
			cout << "4. Exit" << endl;
			cout << "\nSelect an option: ";
			cin >> selection;

			switch (selection) {
			case 1:
				Register->execute();
				break;
			case 2:
				Administer->execute();
				break;
			case 3:
				Customer->execute();
				break;
			case 4:
				cout << "Goodbye, See you next time!" << endl;
				return;
			default:
				cout << "Invalid selection, select another one: ";
			}
		}
	}
};

int main() {
	UserManager Manager;
	
	Command* Register = new RegisterCommand(Manager);
	Command* Administer = new LoginasAdmin(Manager);
	Command* Customer = new LoginasCustomer(Manager);

	LoginMenu LoginMenu(Register, Administer, Customer);

	LoginMenu.display();
	delete Register;
	delete Administer;
	delete Customer;
}