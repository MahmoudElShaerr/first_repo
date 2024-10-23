#include <iostream>
#include <fstream>
#include <map>
using namespace std;
#define Min_Balance 500

class InsufficientFunds{};

class Account
{
private:
	long accountnumber;
	string firstname;
	string lastname;
	float balance;
	static long Nextaccountnumber;
public:
	Account() {}
	Account(string fname, string lname, float balance);
	long getAccno() { return accountnumber; }
	string getFirstname() { return firstname; }
	string getLastname() { return lastname; }
	float getBalance() { return balance; }
	void Deposit(float amount);
	void Withdraw(float amount);
	static void setLastaccountnumber(long accountnumber);
	static long getLastaccountnumber();
	friend ofstream& operator<<(ofstream& ofs, Account& acc);
	friend ifstream& operator>>(ifstream& ifs, Account& acc);
	friend ostream& operator<<(ostream& out, Account& acc);
};
long Account::Nextaccountnumber = 0;

class Bank
{
private:
	map<long, Account>accounts;
public:
	Bank();
	Account Openaccount(string fname, string lname, float balance);
	Account BalanceEnquire(long accountnumber);
	Account Deposit(long accountnumber, float amount);
	Account Withdraw(long accountnumber, float amount);
	void CloseAccount(long accountnumber);
	void ShowAllaccounts();
	~Bank();
};

int main()
{
	Bank b;
	Account acc;
	int choice;
	string fname, lname;
	long accountnumber;
	float balance, amount;
	
	cout << "Welcome to our Banking System" << endl;
	do
	{
		cout << "Select one option below " << endl;
		cout << "Open Account choose: 1 " << endl;
		cout << "Balance Enquire choose: 2 " << endl;
		cout << "Deposit choose: 3 " << endl;
		cout << "Withdraw choose:  4" << endl;
		cout << "Close an account choose: 5 " << endl;
		cout << "Show all accounts choose:6 " << endl;
		cout << "Quit choose: 7 " << endl;
		cout << "Enter Your choice: ";
		cin >> choice;
		cout << endl;
		switch (choice)
		{
		case 1:
			cout << "Enter your First name: ";
			cin >> fname;
			cout << "Enter your Last name: ";
			cin >> lname;
			cout << "Enter your Balance: ";
			cin >> balance;
			acc = b.Openaccount(fname, lname, balance);
			cout << "Congradulation Account is created" << endl;
			cout << acc;
			cout << endl;
			break;
		case 2:
			cout << "Enter Account number: ";
			cin >> accountnumber;
			acc = b.BalanceEnquire(accountnumber);
			cout << endl << "Your Account details: " << endl;
			cout << acc;
			cout << endl;
			break;
		case 3:
			cout << "Enter Account number: ";
			cin >> accountnumber;
			cout << "Enter Balance: ";
			cin >> amount;
			acc = b.Deposit(accountnumber, amount);
			cout << endl << "Amount is Deposited" << endl;
			cout << acc;
			cout << endl;
			break;
		case 4:
			cout << "Enter account number: ";
			cin >> accountnumber;
			cout << "Enter balance: ";
			cin >> amount;
			try 
			{
				acc = b.Withdraw(accountnumber, amount);
				cout << endl;
				cout << "Amount withdrawn" << endl;
				cout << acc;
				cout << endl;
			}
			catch (InsufficientFunds& ex)
			{
				cout << "Error Insufficient Funds!!! Please try again " << endl;
			}
			break;
		case 5:
			cout << "Enter Account number: ";
			cin >> accountnumber;
			b.CloseAccount(accountnumber);
			cout << endl;
			break;
		case 6:
			b.ShowAllaccounts();
			break;
		case 7:
			break;
		default:
			cout << "Enter valid number" << endl;
			exit(0);
		}
	} while (choice != 7);

	return 0;
}

Account::Account(string fname, string lname, float balance)
{
	Nextaccountnumber++;
	accountnumber = Nextaccountnumber;
	firstname = fname;
	lastname = lname;
	this->balance = balance;
}
void Account::Deposit(float amount)
{
	balance += amount;
}
void Account::Withdraw(float amount)
{
	if (balance - amount < Min_Balance)
		throw InsufficientFunds();
	balance -= amount;
}
void Account::setLastaccountnumber(long accountnumber)
{
	Nextaccountnumber = accountnumber;
}
long Account:: getLastaccountnumber()
{
	return Nextaccountnumber;
}
ofstream& operator<<(ofstream& ofs, Account& acc)
{
	ofs << acc.accountnumber << endl;
	ofs << acc.firstname << endl;
	ofs << acc.lastname << endl;
	ofs << acc.balance << endl;
	return ofs;
}


ifstream& operator>>(ifstream& ifs, Account& acc)
{
	ifs >> acc.accountnumber;
	ifs >> acc.firstname;
	ifs >> acc.lastname;
	ifs >> acc.balance;
	return ifs;
}

ostream& operator<<(ostream& out, Account& acc)
{
	cout << "Account number is: " << acc.getAccno() << endl;
	cout << "First name is: " << acc.getFirstname() << endl;
	cout << "Last name is: " << acc.getLastname() << endl;
	cout << "Balance: " << acc.getBalance() << endl;
	
	return out;
}


Bank::Bank()
{
	Account account;
	ifstream ifs;
	ifs.open("NewBank_Data");
	if (!ifs)
		// cout << "Error in opening the file" << endl;
		return;
	while (!ifs.eof())
	{
		ifs >> account;
		accounts.insert(pair <long, Account>(account.getAccno(), account));
	}
	Account::setLastaccountnumber(account.getAccno());
	ifs.close();
}
Account Bank::Openaccount(string fname, string lname, float balance)
{
	ofstream ofs;
	Account account(fname, lname, balance);
	accounts.insert(pair <long, Account>(account.getAccno(), account));
	ofs.open("NewBank_Data", ios::trunc);
	map <long, Account> :: iterator itr;
	for (itr = accounts.begin();itr != accounts.end();itr++)
	{
		ofs << itr->second;
	}
	ofs.close();
	return account;
}
Account Bank::BalanceEnquire(long accountnumber)
{
	map<long, Account>::iterator itr = accounts.find( accountnumber);
	return itr->second;
}
Account Bank::Deposit(long accountnumber, float amount)
{
	map<long, Account>::iterator itr = accounts.find(accountnumber);
	itr->second.Deposit(amount);
	return itr->second;
}
Account Bank::Withdraw(long accountnumber, float amount)
{
	map<long, Account>::iterator itr = accounts.find(accountnumber);
	itr->second.Withdraw(amount);
	return itr->second;
}
void Bank::CloseAccount(long accountnumber)
{
	map<long, Account>::iterator itr = accounts.find(accountnumber);
	accounts.erase(accountnumber);
	cout << "Account is deleted" << endl;
}
void Bank::ShowAllaccounts()
{
	map<long, Account>::iterator itr;
	for (itr = accounts.begin();itr != accounts.end();itr++)
	{
		cout << "Account: " << itr->first << endl << itr->second << endl;
	}
}
Bank::~Bank()
{
	ofstream ofs;
	ofs.open("NewBank_Data", ios::trunc);
	map<long, Account > ::iterator itr;
	for (itr = accounts.begin();itr != accounts.end();itr++)
	{
		ofs << itr->second;
	}
	ofs.close();
}