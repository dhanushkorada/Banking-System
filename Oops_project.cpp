#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <string>
#include <fstream>

using namespace std;

class Account;
class CreditCard;
class DebitCard;
class Loan;
class Investment;
class Insurance;
class ATM;

class Account {
private:
    string accountNumber = "";
    string accountHolderName;
    string password;
    string email;
    string address;
    string phone;
protected:
    double balance = 0;
public:
    Account(){
        std::cout << "\nHorizon Bank Database Connected\n";
        this->generate_acc_no();
    }

    void generate_acc_no(){
        srand(time(NULL));  // seed the random number generator with current time
        for (int i = 0; i < 11; i++) {
            accountNumber.push_back('0' + (rand() % 10));  // append a random digit (0-9) to the password
        }
        std::cout << "Your account Number is : " << accountNumber << std::endl;
    }

    
    void setphoneNumber(string num) {
        phone = num;
    }
    void setemail(string em) {
        email = em;
    }
    const string& getemail() {
        return email;
    }
    void setAddress(string add) {
        address = add;
    }
    const string& getAddress() {
        return address;
    }
    const string& getAccountNumber() const {
        return accountNumber;
    }

    const string& getAccountHolderName() const {
        return accountHolderName;
    }
    
    const string& getphoneNumber() const{
        return phone;
    }

    const string& getPassword() const {
        return password;
    }

    double getBalance() const {
        return balance;
    }

    void setPassword() {
        std::cout << "Enter your Password : ";
        cin >> password;
    }

    void setAccountHolderName(const string& newAccountHolderName) {
        accountHolderName = newAccountHolderName;
    }
    
    const virtual void displayAccountInfo() const {
        std::cout << endl << "Account Number: " << accountNumber << endl;
        std::cout << "Account Holder: " << accountHolderName << endl;
        std::cout << "Balance: $" << balance << endl;
        std::cout << "Phone Number : " << phone << endl;
        std::cout << "Email Address : " << email << endl;
        std::cout << "Address : " << address << endl;
    }

    virtual void deposit(double amount) {
        balance += amount;
        std::cout << "Deposit of $" << amount << " successful." << endl;
        update_transaction_history(amount, "dep");
    }

    virtual bool withdraw(double amount) {
        if (amount <= balance) {
            balance -= amount;
            std::cout << "Withdrawal of $" << amount << " successful." << endl;
            update_transaction_history(amount,"with");
            return true;
        } else {
            std::cout << "Insufficient funds." << endl;
            return false;
        }
    }
    void update_transaction_history(double amount, string ch) {
        ofstream of(accountNumber.c_str(),ios::app);
        if(ch == "dep") {
            of << "Deposited to account " << accountNumber << "  \t\t" << amount <<"\t\t" << balance <<endl;
        } else if(ch == "with") {
            of << "Withdrawn from account " << accountNumber << "\t\t\t" << amount << "\t" << balance << endl;
        }
    }
    void print_transaction_history() {
	    ifstream in(accountNumber.c_str());
	    string line;
	    while(! in.eof()) {
		    getline(in,line);
		    cout << line <<endl;
    	}
	    in.close();
    }
};

class SavingsAccount : public Account {
private:
    double interestRate = 2.7;
public:
    SavingsAccount(){
        std::cout << "Savings Account Created";
    }

    double getInterestRate() const {
        return interestRate;
    }

    const virtual void displayAccountInfo() const override {
        Account::displayAccountInfo();
        std::cout << "Account Type: Savings Account" << endl;
        std::cout << "Interest Rate: " << interestRate << "%" << endl;
    }

    virtual void deposit(double amount) override {
        Account::deposit(amount);
        double currentBalance = getBalance();
        balance = currentBalance + (interestRate / 100) * amount;
        Account::deposit((interestRate / 100) * amount);
    }
};

class CheckingAccount : public Account {
private:
    double overdraftLimit;
public:
    CheckingAccount(){
        std::cout << "Checking Account Created";
    }
   
    double getOverdraftLimit() const {
        return overdraftLimit;
    }

    const virtual void displayAccountInfo() const override {
        Account::displayAccountInfo();
        std::cout << "Account Type: Checking Account" << endl;
        std::cout << "Overdraft Limit: $" << overdraftLimit << endl;
    }

    virtual bool withdraw(double amount) override {
        if (amount <= balance + overdraftLimit) {
            balance -= amount;
            std::cout << "Withdrawal of $" << amount << " successful." << endl;
            return true;
        } else {
            std::cout << "Withdrawal limit exceeded." << endl;
            return false;
        }
    }
};

class CreditCard : public Account {
private:
    double creditLimit;
    double interestRate;
public:
    CreditCard(){
        std::cout << "C Card Created";
    }
    double getCreditLimit() const {
        return creditLimit;
    }

    double getInterestRate() const {
        return interestRate;
    }

    const virtual void displayAccountInfo() const override {
        Account::displayAccountInfo();
        std::cout << "Account Type: Credit Card" << endl;
        std::cout << "Credit Limit: $" << creditLimit << endl;
        std::cout << "Interest Rate: " << interestRate << "%" << endl;
    }

    virtual void deposit(double amount) override {
        balance -= amount;
        std::cout << "Credit of $" << amount << " successful." << endl;
    }

    virtual bool withdraw(double amount) override {
        if (amount <= balance + creditLimit) {
            balance += amount;
            std::cout << "Payment of $" << amount << " successful." << endl;
            return true;
        } else {
            std::cout << "Payment limit exceeded." << endl;
            return false;
        }
    }
};

class DebitCard : public Account {
private:
    double dailyWithdrawalLimit;
    string pin;
public:
    DebitCard(){
        std::cout << "D Card Created";
    }
    double getDailyWithdrawalLimit() const {
        return dailyWithdrawalLimit;
    }

    const string& getPin() const {
        return pin;
    }

    const virtual void displayAccountInfo() const override {
        Account::displayAccountInfo();
        std::cout << "Account Type: Debit Card" << endl;
        std::cout << "Daily Withdrawal Limit: $" << dailyWithdrawalLimit << endl;
    }

    bool verifyPin(const string& enteredPin) const {
        return enteredPin == pin;
    }

    virtual bool withdraw(double amount) override {
        if (amount <= balance && amount <= dailyWithdrawalLimit) {
            balance -= amount;
            std::cout << "Withdrawal of $" << amount << " successful." << endl;
            return true;
        } else {
            std::cout << "Withdrawal limit exceeded." << endl;
            return false;
        }
    }
};

class Loan : public Account {
private:
    double interestRate;
    vector<Account*> loans;
    string benefiter;
    string phone_number;
    string loan_no = "" ;
    double amount;
public:
    Loan(){
        std::cout << "Loan Creation Started\n";
    }

    void addAccount(Account* account) {
        loans.push_back(account);
        for (const auto& account : loans) {
            benefiter = account->getAccountHolderName();
            phone_number = account->getphoneNumber();
        }
    }

    const void generateloanno(){     
        srand(time(NULL));
        for (int i = 0; i < 11; i++) {
            loan_no.push_back('0' + (rand() % 10));
        }
        cout << "Your Loan Number is : " << loan_no << endl;
    }

    double getInterestRate() const {
        return interestRate;
    }

    void setInterestRate(double amount) {
    	amount = amount;
    	if(amount >= 100000) {
    		interestRate = 0.2;
		} else{
			interestRate = 0.4;
		}
	}
    virtual void displayloanInfo() const {
        std::cout << "Loan Number: " << loan_no << endl;
        std::cout << "Benefiter : " << benefiter << endl;
        std::cout << "Loan value(Amount): $" << amount << endl;
        std::cout << "Interest rate: " << interestRate << endl;
        std::cout << "Registered Phone Number : " << phone_number << endl;
    }

    virtual void deposit(double amount) override {
        balance -= amount;
        std::cout << "Loan payment of $" << amount << " successful." << endl;
    }

    virtual bool withdraw(double amount) override {
        std::cout << "Cannot withdraw from a loan account." << endl;
        return false;
    }
};

class Investment : public Account {
private:
    double interestRate;
public:
    Investment(){
        std::cout << "Inv Created";
    }
    double getInterestRate() const {
        return interestRate;
    }

    const virtual void displayAccountInfo() const override {
        Account::displayAccountInfo();
        std::cout << "Account Type: Investment" << endl;
        std::cout << "Interest Rate: " << interestRate << "%" << endl;
    }

    virtual void deposit(double amount) override {
        balance += amount;
        std::cout << "Investment deposit of $" << amount << " successful." << endl;
    }

    virtual bool withdraw(double amount) override {
        if (amount <= balance) {
            balance -= amount;
            std::cout << "Investment withdrawal of $" << amount << " successful." << endl;
            return true;
        } else {
            std::cout << "Withdrawal limit exceeded." << endl;
            return false;
        }
    }
};

class ATM  : public Account{
private:
    vector<Account*> accounts;
    int ast;
public:
    ATM(){
        std::cout << "Atm Created";
    }  
    void addAccount(Account* account) {
        accounts.push_back(account);
    }

    void displayAccounts() const {
        for (const auto& account : accounts) {
            std::cout << "Account number: " << account->getAccountNumber() << ", balance: " << account->getBalance() << endl;
        }
    }

    void withdrawFromAccount(string accountNumber, double amount) {
        for (auto& account : accounts) {
            if (account->getAccountNumber() == accountNumber) {
                account->withdraw(amount);
                std::cout << "Withdrawal of $" << amount << " from account " << accountNumber << " successful!" << endl;
                return;
            }
        }
        std::cout << "Error: Account with account number " << accountNumber << " not found!" << endl;
    }

    void depositIntoAccount(string accountNumber, double amount) {
        for (auto& account : accounts) {
            if (account->getAccountNumber() == accountNumber) {
                account->deposit(amount);
                std::cout << "Deposit of $" << amount << " into account " << accountNumber << " successful!" << endl;
                return;
            }
        }
        std::cout << "Error: Account with account number " << accountNumber << " not found!" << endl;
    }
};

class Insurance {
private:
    string policyNumber = "";
    string policyHolderName;
    vector<Account*> policies;
    string phone_number;
    int ast;

protected:    
    double policyAmount;
public:    
    void addAccount(Account* account) {
        policies.push_back(account);
        for (const auto& account : policies) {
            policyHolderName = account->getAccountHolderName();
            phone_number = account->getphoneNumber();
        }
    }
    Insurance(){
        std::cout << "Insurance Policy Registration Started\n";
    }
    const string& getPolicyNumber() const {
        return policyNumber;
    }
    const void generatepolicyno(){     
        srand(time(NULL));
        for (int i = 0; i < 11; i++) {
            policyNumber.push_back('0' + (rand() % 10));
        }
        cout << "Your Policy Number is : " << policyNumber << endl;
    }

    const string& getPolicyHolderName() const {
        return policyHolderName;
    }

    double getPolicyAmount() const {
        return policyAmount;
    }

    void setPolicyAmount(double amount) {
        policyAmount = amount;
    }

    virtual void displayPolicyInfo() const {
        std::cout << "Policy Number: " << policyNumber << endl;
        std::cout << "Policy Holder: " << policyHolderName << endl;
        std::cout << "Policy Amount: $" << policyAmount << endl;
        std::cout << "Registered Phone Number : " << phone_number << endl;
    }

    virtual double calculatePremium() const {
        return 0.0;
    }
};

class LifeInsurance : public Insurance {
private:
    int age;
    char gender;
public:
   LifeInsurance(){
        std::cout << "Inv Created";
    }

    int getAge() const {
        return age;
    }

    char getGender() const {
        return gender;
    }

    virtual void displayPolicyInfo() const override {
        Insurance::displayPolicyInfo();
        std::cout << "Policy Type: Life Insurance" << endl;
        std::cout << "Policy Holder Age: " << age << endl;
        std::cout << "Policy Holder Gender: " << gender << endl;
    }

    virtual double calculatePremium() const override {
        double premium = age * policyAmount / 1000;
        if (gender == 'F') {
            premium *= 0.9; // Female customers get 10% discount
        }
        return premium;
    }
};

class CarInsurance : public Insurance {
private:
    string carModel;
    int carYear;
public:
    CarInsurance(){
        std::cout << "Ins Created";
    }
    const string& getCarModel() const {
        return carModel;
    }

    int getCarYear() const {
        return carYear;
    }

    virtual void displayPolicyInfo() const override {
        Insurance::displayPolicyInfo();
        std::cout << "Policy Type: Car Insurance" << endl;
        std::cout << "Car Model: " << carModel << endl;
        std::cout << "Car Year: " << carYear << endl;
    }

    virtual double calculatePremium() const override {
        double premium = policyAmount * 0.05; // Premium is 5% of policy amount
        if (carYear < 2010) {
            premium *= 1.2; // Older cars are more expensive to insure
        }
        return premium;
    }
};

int main(){
    int response = 0;
    vector <Account *> Acc(100, nullptr);
    int no_of_acc = 0;
    
    while(response != 3){
        std::cout << "Welcome to Horizon Bank\nA world of oppurtunities at your doorstep\nPress 1 to login\nPress 2 to Create a New Account\nPress 3 to Exit\n";
        cin >> response;
        if(response == 1){
            if(no_of_acc == 0){
                std::cout << "No accounts in the Database\n";
                continue;
            }
            string acc_no_test;
            int temp = -1;
            int attempts = 3;
            int flag = 0;
            std::cout << "Enter your 11 digit Account number : " ;
            cin >> acc_no_test;
            for(int i = 0; i < no_of_acc ; i++){
                temp = i;
                if(Acc[i]->getAccountNumber() == acc_no_test){
                    flag = 1;
                    break;
                }
            }
            if(flag == 0)
                std::cout << "\nInvalid Account Number\nAccount Number not found\n";
            else{
                while(attempts){
                    string pass;
                    std::cout << "Enter your password : \n";
                    cin >> pass;
                    string s2 = Acc[temp]->getPassword();
                    int choice = 0;
                    if(s2 == pass){
                        while(choice != 10){
                            std::cout << "Welcome, " << Acc[temp]->getAccountHolderName() << "." << endl;
                            std::cout << "Your current balance is $" << Acc[temp]->getBalance() << "." << endl;
                            std::cout << "Please select an option: " << endl;
                            std::cout << "1. Deposit" << endl;
                            std::cout << "2. Withdraw" << endl;
                            std::cout << "3. Check balance" << endl;
                            std::cout << "4. View transaction history" << endl;
                            std::cout << "5. ATM services" << endl;
                            std::cout << "6. Insurance services" << endl;
                            std::cout << "7. Loan services" << endl;
                            std::cout << "8. Investment services" << endl;
                            std::cout << "9. Transaction" << endl;
                            std::cout << "10. Exit" << endl;
                            cin >> choice;
                                switch (choice) {
                                case 1:
                                    double depositAmount;
                                    std::cout << "Enter deposit amount: ";
                                    cin >> depositAmount;
                                    Acc[temp]->deposit(depositAmount);
                                    continue;
                                case 2:
                                    double withdrawAmount;
                                    std::cout << "Enter withdraw amount: ";
                                    cin >> withdrawAmount;
                                    Acc[temp]->withdraw(withdrawAmount);
                                    continue;
                                case 3:
                                    std::cout << "Your current balance is $" << Acc[temp]->getBalance() << "." << endl;
                                    continue;
                                case 4:
                                    Acc[temp]->print_transaction_history();
                                    continue;
                                case 5:
                                    { 
                                        ATM myATM;
                                        myATM.addAccount(Acc[temp]);
                                        myATM.displayAccounts();
                                        continue;
                                    }
                                case 6:
                                    {
                                        double amount;
                                        Insurance myInsurance;
                                        myInsurance.addAccount(Acc[temp]);
                                        cout << "Enter the amount you want to create a insurance policy for : ";
                                        cin >> amount;
                                        if(amount > Acc[temp]->getBalance())
                                            cout << "Insufficient funds to create required Insurance Policy\n";
                                        Acc[temp]->withdraw(amount);
                                        myInsurance.setPolicyAmount(amount);
                                        myInsurance.generatepolicyno();
                                        cout << "Policy successfully created\nHere is the policy information\n";
                                        myInsurance.displayPolicyInfo();
                                        continue;
                                    }
                                case 7:
                                    {
                                        double amount;
                                    	Loan myloan;
                                    	myloan.addAccount(Acc[temp]);
                                    	cout << "Enter the loan amount required: ";
                                    	cin >> amount;
                                    	if(amount > (Acc[temp]->getBalance()) * 0.5) {
                                    		cout << "Your balance is less than half the loan amount\n Sorry we cannot provide a loan : ";
                                    		continue;
										}
										Acc[temp]->deposit(amount);
                                        cout << "Loan successfully sanctioned\n";
										myloan.setInterestRate(amount);
										myloan.generateloanno();
                                    	myloan.displayloanInfo();
                                        continue;
                                    }
                                case 8:
                                    {
                                        Investment myInvestment;
                                        myInvestment.displayAccountInfo();
                                        continue;
                                    }
                                case 9:
                                    {
                                        string temp_acc;
                                        int temp1,flag1 = 0;
                                        double bal = Acc[temp]->getBalance();
                                        cout << "Your Balance is : " << bal << endl;
                                        cout << "Enter the account number you want to send money to : ";
                                        cin >> temp_acc;
                                        for(int i = 0; i < no_of_acc ; i++){
                                            temp1 = i;
                                            if(Acc[i]->getAccountNumber() == temp_acc){
                                                flag1 = 1;
                                                break;
                                            }
                                        }
                                        if(flag1 == 1 && temp1 != temp){
                                            int amt,x;
                                            cout << Acc[temp1]->getAccountHolderName() << endl;
                                            cout << "Is this the account you want to send to : ";
                                            cin >> x;
                                            if(x == 0){
                                                cout << "Transaction terminated\n";
                                                continue;
                                            }
                                            cout << "Enter the amount you want to send : ";
                                            cin >> amt;
                                            if(amt <= bal){
                                                Acc[temp]->withdraw(amt);
                                                Acc[temp1]->deposit(amt);
                                                cout << "Transaction Successfull\n";
                                                cout << "Your balance is : " << Acc[temp]->getBalance() << endl;
                                                continue;
                                            }
                                            else{
                                                cout << "Funds not sufficient\nTransaction failed\n";
                                                continue;
                                            }
                                        }
                                        else if(temp1 == temp){
                                            cout << "Transaction to same account not possible";
                                            continue;
                                        }
                                        else{
                                            cout << "Account Number not found\n";
                                            continue;
                                        }
                                    }
                                case 10:
                                    goto endmenu;
                                default:
                                    std::cout << "Invalid option selected." << endl;
                                    continue;
                            }
                        }
                    }
                    else{
                        attempts--;
                        if (attempts > 0) {
                            std::cout << "Invalid login. You have " << attempts << " attempts remaining." << endl;
                        } 
                        else {
                           std::cout << "Invalid login. Your account is locked. Contact customer service for assistance." << endl;
                           goto endmenu;
                        }
                    }  
                }
            }
        }
        else if(response == 2){
            string name;
            int dob;
            char sex;
            string acc_no;
            int pass;
            string ph_no;
            string email;
            string address;
            std::cout << "\nEnter your name : ";
            cin >> name;
            std::cout << "\nEnter your Date of Birth (yyyymmdd): ";
            cin >> dob;

            if(dob >= 20050400){
                std::cout << "\nYou are not eligible for creating an account due to age constraint\n";
                continue;
            }

            std::cout << "\nEnter your phone number : ";
            cin >> ph_no;
            cout << "Please provide your email address : ";
            cin >> email;
            cout << "Please provide you address : ";
            cin >> address;
            std::cout << "\nAccount creation in progress";
            Acc[no_of_acc] = new Account;
            Acc[no_of_acc]->setAccountHolderName(name);
            Acc[no_of_acc]->setPassword();
            Acc[no_of_acc]->setphoneNumber(ph_no);
            Acc[no_of_acc]->setAddress(address);
            Acc[no_of_acc]->setemail(email);
            std::cout << "\nAccount created Successfully";
            Acc[no_of_acc]->displayAccountInfo();
            no_of_acc++;
            std::cout << "Now you can login using these details.\n";
        }
        endmenu:;
    }
    for (auto it = Acc.begin(); it != Acc.end(); ++it) {
        delete *it;
    }
}