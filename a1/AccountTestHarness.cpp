#include <iostream>
#include <map>
#include <string>
#include <stdlib.h>


using namespace std;

    
//***********************
// Account Number ADT
//***********************

class AccountNo {                       /* Value range: 0001-9999 */
public:
    AccountNo ();                       /* New account number                          */
    explicit AccountNo ( int number );  /* Existing account number (for lookups)       */
    int number () const;                /* Accessor -- integer value of account number */
private:
    int number_;
    static int next_;
    static int const maxVal_ = 9999;
};


// initialization of static data member to the lowest legal value
int AccountNo::next_ = 1;


// constructor -- constructs a new unique account number
AccountNo::AccountNo () {
	if ( next_ > maxVal_ )
        exit (1);
    number_ = next_++;
}

// constructor -- converts an integer into an account number
// REQUIRES:  number corresponds to an existing account number
AccountNo::AccountNo ( int number ) {
	if ( (number > maxVal_) || (number < 1) )
		exit(1);
	number_ = number;
}


// accessor - returns account number value of object
int AccountNo::number() const {
	return number_;
}


// comparison operators
bool operator== (const AccountNo &a, const AccountNo &b) {
	return a.number() == b.number();
}

bool operator!= (const AccountNo &a, const AccountNo &b) {
	return !(a==b);
}

bool operator< (const AccountNo &a, const AccountNo &b) {
	return a.number() < b.number();
}

bool operator<= (const AccountNo &a, const AccountNo &b) {
	return (a<b) || (a==b);
}

bool operator> (const AccountNo &a, const AccountNo &b) {
	return !(a<=b);
}

bool operator>= (const AccountNo &a, const AccountNo &b) {
	return !(a<b);
}


// streaming operators
istream& operator>> ( istream &sin, AccountNo &a ) {
	int number;
	sin >> number;
	a = AccountNo(number);
	
	return sin;
}

ostream& operator<< ( ostream &sout, const AccountNo &a ) {
	sout.setf(ios::internal);
	sout.width(4);
	sout.fill('0');
	sout << a.number();
	
	return sout;
}


//*******************
// Account
//*******************

class Account {
public:                                             // PUBLIC interface of Account
	explicit Account ( const AccountNo& );          // constructor
	virtual ~Account() {}                           // destructor
	int balance () const;                           // accessor - returns balance as an integer
	virtual void call ( int duration ) = 0;         // records information about a call (e.g., duration of call in minutes)
	virtual void bill () = 0;                       // decrements balance by monthly fee and the cost of using extra minutes
	void pay (int amount);                          // increments balance by amount paid
	virtual void print() const;                     // prints information about the account (e.g., account number, balance, minutes used this month
private:                                            // PRIVATE data members
    const AccountNo& account_no_;                   // holds the account no. attached to the account
    int balance_;                                   // holds the current balance of the account
protected:                                          // PROTECTED interface for use by derived classes
    void setBalance(int balance);                   // mutator - sets the balance
};


//************************************************************************
//* YOUR IMPLENTATION OF ACCOUNT, CHEAP_ACCOUNT, EXPENSIVE_ACCOUNT HERE 
//************************************************************************
// Implementation of Account
// Account Contructor
Account::Account (const AccountNo& account_no) : account_no_(account_no) {
    balance_ = 0;
}

// accessor for Account balance
int Account::balance() const {
    return balance_;
}

// prints the Account information
void Account::print() const {
    string dollar_sign = "$"; // get it??
    int pos_balance = balance_;
    if (balance_ < 0) {
        dollar_sign = "-$"; // now you should get the joke
        pos_balance = (-1) * balance_;
    }
        
    cout << "  Account Number = " << account_no_ << endl;
    cout << "  Balance = " << dollar_sign << pos_balance << endl;
}

// pays the amounts towards the Account balance
void Account::pay(int amount) {
    balance_ += amount;
}

// mutator for the Account balance for use by derived classes
void Account::setBalance(int balance) {
    balance_ = balance;
}

// Cheap Account
class CheapAccount : public Account {
public:                                         // PUBLIC interface of CheapAccount
    explicit CheapAccount( const AccountNo& );  // constructor
    void call(int duration);                    // records duration of a call in minutes
    void bill();                                // decrements balance by monthly fee and the const of using extra minutes
    void print() const;                         // prints information about the account
private:                                        // PRIVATE data members of CheapAccount
    int minutes_;                               // holds the number of minutes used in the current billing cycle
    static int const maxMins_ = 250;            // holds the maximum minutes before overages
    static int const monthlyBill_ = 25;         // holds the monthly bill
};

// Cheap Account implementation
// constructor
CheapAccount::CheapAccount (const AccountNo& acc_no) : Account(acc_no) {
    minutes_ = 0;
}

// prints Cheap Account information
void CheapAccount::print() const {
    cout << "CheapAccount:" << endl;
    Account::print();
    cout << "  Minutes = " << minutes_ << endl;
}

// records the duration of a call in minutes
void CheapAccount::call(int duration) {
    minutes_ += duration;
}

// bills the account for the current cycle
void CheapAccount::bill() {
    int bill = monthlyBill_;
    if (minutes_ > maxMins_) {
        bill += minutes_ - maxMins_;
    }
    int new_balance = balance();
    new_balance -= bill;
    setBalance(new_balance);
    minutes_ = 0;
}

//Expensive Account
class ExpensiveAccount : public Account {
public:                                         // PUBLIC interface of ExpensiveAccount
    explicit ExpensiveAccount(const AccountNo&);// constructor
    void call(int duration);                    // empty function to override base's pure virtual function
    void bill();                                // decrements balance by monthly fee
    void print() const;                         // prints information about the account
private:
    static int const monthlyBill = 80;          // holds the monthly bill
};

// Expensive Account implementation
// constructor
ExpensiveAccount::ExpensiveAccount (const AccountNo& acc_no) : Account(acc_no) { }

// prints Expensive Account information
void ExpensiveAccount::print() const {
    cout << "ExpensiveAccount:" << endl;
    Account::print();
}

// empty call function for overriding the pure virtual function
void ExpensiveAccount::call(int duration) { }

// bills the account for the current cycle
void ExpensiveAccount::bill() {
    int new_balance = balance();
    new_balance -= monthlyBill;
    setBalance(new_balance);
}

//************************************************************************
//  Helper variables and functions for test harness
//************************************************************************

//  test-harness operators
enum Op { NONE, NewE, NewC, Balance, Call, Bill, Pay, PrintAll };


//  converts a one-character input comment into its corresponding test-harness operator
Op convertOp( string opStr ) {
    switch( opStr[0] ) {
        case 'E': return NewE; 
        case 'C': return NewC;
        case 'b': return Balance;
        case 'c': return Call;
        case 'B': return Bill;
        case 'p': return Pay;
        case 'P': return PrintAll;
        default: {
            cerr << "Invalid operation " << opStr << endl;
            return NONE;
        }
    }
}


// Reads a number from cin and finds the corresponding Account
// REQUIRES: the next word to read from cin is an integer
// RETURNS: a pointer to a found account.  Otherwise, returns NULL
Account* findAccount( map<AccountNo,Account*>  &accounts) {
    int num;
    cin >> num;
    AccountNo act( num );
    map<AccountNo,Account*>::iterator iter;
    iter = accounts.find( act );
    if ( iter == accounts.end() ) {
        cerr << "Invalid Account Number!" << endl;
        return NULL;
    }
    return iter->second;
}
    

//*******************
// main()
//*******************

int main () {
    cout << "Test harness for family of phone-service accounts:" << endl << endl;
    map<AccountNo, Account*> accounts;
	
    cout << "Command: ";
    string command;
    cin >> command;
	
    Op op = convertOp(command);
	
    while ( !cin.eof() ) {
        switch ( op ) {
                /* Constructors */
            case NewE: {
                AccountNo act;
                Account* p = new ExpensiveAccount( act );
                accounts.insert( pair<AccountNo,Account*>( act, p ) );
                p->print();
                break;
            }
            case NewC: {
                AccountNo act;
                Account* p = new CheapAccount( act );
                accounts.insert( pair<AccountNo,Account*>( act, p ) );
                p->print();
                break;
            }
				
                /* Accessors */
            case Balance: {
                Account* p = findAccount( accounts );
                if ( p!= NULL)
                    cout << "Value of balance data member is: " << p->balance() << endl;
                break;
            }
                
                /* Phone Service Operations */
            case Call: {
                Account* p = findAccount( accounts );
                if ( p != NULL ) {
                    int duration;
                    cin >> duration;
                    p->call( duration );
                }
                break;
            }
                
                
            case Bill: {
                map<AccountNo,Account*>::iterator iter;
                map<AccountNo,Account*>::iterator end = accounts.end();
                for ( iter = accounts.begin(); iter != end; iter++ ) {
                    (iter->second)->bill();
                }
                break;
            }
                
                
            case Pay: {
                Account* p = findAccount( accounts );
                if (p != NULL ) {
                    int amt;
                    cin >> amt;
                    p->pay(amt);
                }
                break;
            }
                
                
                /* Print Accounts */
            case PrintAll: {
                map<AccountNo,Account*>::iterator iter;
                map<AccountNo,Account*>::iterator end = accounts.end();
                for ( iter = accounts.begin(); iter != end; iter++ ) {
                    (iter->second)->print();
                }
                break;
            }
            default: {
                break;
            }
        } // switch 
		
        cout << endl << "Command: ";
        cin >> command;
        op = convertOp(command);
		
    } // while cin OK
	
    map<AccountNo,Account*>::iterator iter;
    map<AccountNo,Account*>::iterator end = accounts.end();
    for ( iter = accounts.begin(); iter != end; iter++ ) 
      delete iter->second;

    return 0;
}
