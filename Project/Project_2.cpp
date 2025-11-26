#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <conio.h>
#include <fstream>
#include <iomanip>
#include <limits>
#include <direct.h> 
#include <windows.h> 
#include <functional>
#include <sys/stat.h>
#include <sys/types.h>

using namespace std;

const string FILE_ADMIN = "Administrators.txt";
const string FILE_EMPLOYEES = "Employees.txt";
const string DIR_INFO = "Employees_Information";
const string DEFAULT_PASS = "111111";

enum Color {
	BLACK = 0, BLUE = 1, GREEN = 2, CYAN = 3, RED = 4, MAGENTA = 5, BROWN = 6,
    LIGHTGRAY = 7, DARKGRAY = 8, LIGHTBLUE = 9, LIGHTGREEN = 10, LIGHTCYAN = 11,
    LIGHTRED = 12, LIGHTMAGENTA = 13, YELLOW = 14, WHITE = 15
};

void setColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

// Check xem đã tồn tại hay chưa
void ensureDirectoryExists() {
    struct stat info;
    if(stat(DIR_INFO.c_str(), &info) != 0) {
        _mkdir(DIR_INFO.c_str());
    }
}

// Hàm chuẩn hóa tên
string standardizeNames(string strName) {
	stringstream ss(strName);
	string strWord, strRes = "";
	while(ss>>strWord) {
		strWord[0] = toupper(strWord[0]);
		for(size_t i = 1; i < strWord.size(); i++) {
			strWord[i] = tolower(strWord[i]);
		}
		strRes += strWord + " ";
	}
	if(!strRes.empty()) {
		strRes.pop_back();
	}
	return strRes;
}

// Xóa khoảng trắng thừa đầu cuối
string trim(const string& strInput) {
	size_t iFirst = strInput.find_first_not_of(" \t\r\n");
    if (string::npos == iFirst) return "";
    size_t iLast = strInput.find_last_not_of(" \t\r\n");
    return strInput.substr(iFirst, (iLast - iFirst + 1));
}

// Hàm kiểm tra số điện thoại
bool isValidPhoneNumber(string strPhone) {
	if(strPhone.length() != 10) {
		return false;
	}
	for(char c : strPhone) {
		if(!isdigit(c)) {
			return false;
		}
	}
	return true;
}

// Hàm kiểm tra Email
bool isValidGmail(string strEmail) {
	string suffix = "@gmail.com";
	if(strEmail.length() <= suffix.length()) {
		return false;
	}
	size_t pos = strEmail.rfind(suffix);
	if(pos == string::npos) {
		return false;
	}
	return pos == (strEmail.length() - suffix.length());
}

// Chuẩn hóa mật khẩu *
string getHiddenPassword() {
	string strPassword = "";
	char cCh;
	while(true) {
		cCh = _getch();
		if(cCh == 13) { // Enter
			break;
		} else if(cCh == 8) { // Backspace
			if(!strPassword.empty()) {
				cout<<"\b \b";
				strPassword.pop_back();
			}
		} else {
			strPassword.push_back(cCh);
			cout<<"*";
		}
	}
	cout<<endl;
	return strPassword;
}

template<typename T>
T getInput(string strPrompt) {
	T tValue;
	while(true) {
		setColor(LIGHTCYAN);
		cout<<strPrompt;
		setColor(WHITE);
		if(cin>>tValue) {
			string strRemain;
			getline(cin, strRemain);
			return tValue;
		} else {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			setColor(LIGHTRED);
			cout<<"Du lieu khong hop le, vui long nhap lai"<<endl;
		}
	}
}

class User {
protected:
	string _strUsername;
    string _strPassword;
public:
	User(string strUser = "", string strPass = "") : _strUsername(strUser), _strPassword(strPass) {}
    virtual ~User() {}

    string getUsername() const { 
		return _strUsername; 
	}
    string getPassword() const { 
		return _strPassword; 
	}
    void setUsername(string strUser) {
		_strUsername = strUser; 
	}
    void setPassword(string strPass) { 
		_strPassword = strPass; 
	}
};

class Employee : public User {
private:
	string _strName;
    string _strAddress;
    string _strPhonenumber;
    string _strEmail;
public:
	Employee(string strUser = "", string strPass = "", string strName = "", string strAddr = "", string strPhone = "", string strEmail = "")
    	: User(strUser, strPass), _strName(strName), _strAddress(strAddr), _strPhonenumber(strPhone), _strEmail(strEmail) {}

	string getName() const { 
		return _strName; 
	}
    string getAddress() const { 
		return _strAddress; 
	}
    string getPhonenumber() const { 
		return _strPhonenumber; 
	}
    string getEmail() const { 
		return _strEmail; 
	}

    void setName(string strName) { 
		_strName = strName; 
	}
    void setAddress(string strAddr) { 
		_strAddress = strAddr; 
	}
    void setPhonenumber(string strPhone) { 
		_strPhonenumber = strPhone; 
	}
    void setEmail(string strEmail) { 
		_strEmail = strEmail; 
	}

	friend ostream& operator<<(ostream& os, const Employee& e) {
		os<<left<<setw(15)<<e._strUsername<<setw(25) << e._strName<<setw(15)<<e._strPhonenumber<<setw(25)<<e._strEmail<<setw(20)<<e._strAddress;
        return os;
	}

};

template <class T>
class LinkedList {
private:
	struct Node {
		T data;
		Node* next;
		Node(T val) : data(val), next(nullptr) {}
	};
	Node* _phead;

public:
	LinkedList() : _phead(nullptr) {}

	~LinkedList() {
        clear();
    }

	void clear() {
		Node* pCurrent = _phead;
		while(pCurrent != nullptr) {
			Node* pNext = pCurrent->next;
			delete pCurrent;
			pCurrent = pNext;
		}
		_phead = nullptr;
	}

	void pushBack(T value) {
		Node* pNewNode = new Node(value);
		if(!_phead) {
			_phead = pNewNode;
		} else {
			Node* pTemp = _phead;
			while(pTemp->next) {
				pTemp = pTemp->next;
			}
			pTemp->next = pNewNode;
		}
	}

	void forEach(function<void(T)> action) {
		Node* p = _phead;
		while(p) {
			action(p->data);
			p = p->next;
		}
	}

	T find(function<bool(T)> predicate) {
		Node* p = _phead;
		while(p) {
			if(predicate(p->data)) {
				return p->data;
			}
			p = p->next;
		}
		return nullptr;
	}

	bool exists(function<bool(T)> predicate) {
		Node* p = _phead;
		while(p) {
			if(predicate(p->data)) {
				return true;
			}
			p = p->next;
		}
		return false;
	}

	bool removeIf(function<bool(T)> predicate) {
		if(!_phead) {
			return false;
		}
		if(predicate(_phead->data)) {
			Node* pTemp = _phead;
			_phead = _phead->next;
			delete pTemp;
			return true;
		}
		Node* pCur = _phead;
		Node* pPrev = nullptr;
		while(pCur) {
			if(predicate(pCur->data)) {
				pPrev->next = pCur->next;
				delete pCur;
				return true;
			}
			pPrev = pCur;
			pCur = pCur->next;
		}
		return false;
	}

	bool isEmpty() const {
		return _phead == nullptr;
	}
};

class EmployeeManager {
private:
	LinkedList<Employee*> _listEmployees;

	void writeDetailFile(Employee* pEmp) {
		ensureDirectoryExists();
		string strFilename = DIR_INFO + "/" + pEmp->getUsername() + ".txt";
		ofstream fout(strFilename);
		if(fout.is_open()) {
			fout<<pEmp->getName()<<endl;
			fout<<pEmp->getAddress()<<endl;
			fout<<pEmp->getPhonenumber()<<endl;
			fout<<pEmp->getEmail()<<endl;
			fout.close();
		}
	}

	void deleteDetailFile(string strUsername) {
		string strFilename = DIR_INFO + "/" + strUsername + ".txt";
		if(remove(strFilename.c_str()) == 0) {
			setColor(GREEN);
			cout<<"Da xoa file thong tin chi tiet: "<<strFilename<<endl;
			setColor(WHITE);
		}
	}

public:
	EmployeeManager() {
        loadFromFile();
    }

	~EmployeeManager() {
    	_listEmployees.forEach([](Employee* e) {
        	delete e;
        });
    }

	void addEmployee(Employee* pEmp) {
		_listEmployees.pushBack(pEmp);
		saveToFile();
		writeDetailFile(pEmp);
	}

	bool isUsernameExist(string strUsername) {
		return _listEmployees.exists([&](Employee* e) {
        	return e->getUsername() == strUsername;
    	});
	}

	Employee* getEmployeeByUsername(string strUsername) {
		return _listEmployees.find([&](Employee* e) {
        	return e->getUsername() == strUsername;
    	});
	}

	void deleteEmployee(string strUsername) {
		bool bDeleted = _listEmployees.removeIf([&](Employee* e) {
            if (e->getUsername() == strUsername) {
                delete e;
                return true;
            }
            return false;
        });

		if(bDeleted) {
			saveToFile();
			deleteDetailFile(strUsername);
			setColor(GREEN);
			cout<<"Da xa nhan vien thanh cong"<<endl;
			setColor(WHITE);
		} else {
			setColor(RED);
			cout<<"Khong tin thay username nay"<<endl;
			setColor(WHITE);
		}
	}

	void updateEmployeeInfo() {
		string strUsername;
		cout<<"Nhap username can cap nhat: ";
		getline(cin, strUsername);
		strUsername = trim(strUsername);

		Employee* pEmp = getEmployeeByUsername(trim(strUsername));
		if(!pEmp) {
			setColor(RED);
			cout<<"Khong tim thay nhan vien voi username: "<<strUsername<<endl;
			setColor(WHITE);
			return;
		}

		setColor(YELLOW);
		cout<<"--- Thong tin hien tai ---"<<endl;
		cout << *pEmp << endl;
		setColor(WHITE);

		string strInput;
		cout<<"--- Cap nhat thong tin (an Enter neu giu nguyen) ---"<<endl;

		cout<<"Ho ten ("<<pEmp->getName()<<"): "; 
		getline(cin, strInput);
		if(!strInput.empty()) {
			pEmp->setName(standardizeNames(strInput));
		}

		cout<<"Dia chi ("<<pEmp->getAddress()<<"): "; 
		getline(cin, strInput);
		if(!strInput.empty()) {
			pEmp->setAddress(strInput);
		}

		while(true) {
			cout<<"SDT (" << pEmp->getPhonenumber()<<"): ";
			getline(cin, strInput);

			if(strInput.empty()) {
				break;
			}

			if(isValidPhoneNumber(trim(strInput))) {
				pEmp->setPhonenumber(trim(strInput));
				break;
			} else {
				setColor(RED);
				cout<<"So dien thoai phai co dung 10 chu so: "<<endl;
				setColor(WHITE);
			}
		}

		while(true) {
			cout<<"Email (" << pEmp->getEmail() << "): ";
			getline(cin, strInput);

			if (strInput.empty()) {
                break; 
            }

			if(isValidGmail(trim(strInput))) {
				pEmp->setEmail(trim(strInput));
				break;
			} else {
				setColor(RED);
				cout<<"Email phải co duoi @gmail.com";
				setColor(WHITE);
			}
		}

		saveToFile();
        writeDetailFile(pEmp);
		setColor(GREEN);
		cout<<"Cap nhat thanh cong"<<endl;
		setColor(WHITE);
	}

	void showAll() {
		if(_listEmployees.isEmpty()) {
			setColor(YELLOW);
			cout<<"Danh sach rong"<<endl;
			setColor(WHITE);
			return;
		}
		setColor(CYAN);
		cout<<left<<setw(15)<<"USERNAME"<<setw(25)<<"HO TEN"<<setw(15)<<"SDT"<<setw(25)<<"EMAIL"<<setw(20)<<"DIA CHI"<<endl;
		cout<<string(100, '-')<<endl;
		setColor(WHITE);

		_listEmployees.forEach([](Employee* e) {
            cout<<*e<<endl;
        });
	}

	void loadFromFile() {
		ifstream fin(FILE_EMPLOYEES);
		if(!fin.is_open()) {
			return;
		}
		string strLine;
		while(getline(fin, strLine)) {
			if(strLine.empty()) {
				continue;
			}
			
			// Loại bỏ BOM
			if(strLine.size() >= 3 && (unsigned char)strLine[0] == 0xEF) {
				strLine = strLine.substr(3);
			}

			stringstream ss(strLine);
			string strSegment;
			vector<string> vecFields;
			while(getline(ss, strSegment, ',')) {
				vecFields.push_back(trim(strSegment));
			}

			if(vecFields.size() == 6) {
				Employee* pEmp = new Employee(vecFields[0], vecFields[1], vecFields[2], vecFields[3], vecFields[4], vecFields[5]);
                _listEmployees.pushBack(pEmp);
			}
		}
		fin.close();
	}

	void saveToFile() {
		ofstream fout(FILE_EMPLOYEES);
		_listEmployees.forEach([&](Employee* e) {
			fout<<e->getUsername()<< ","<<e->getPassword()<<","<<e->getName()<<","<<e->getAddress()<<","<<e->getPhonenumber()<<","<<e->getEmail()<<endl;
		});
		fout.close();
	}

	Employee* login(string strUser, string strPass) {
		return _listEmployees.find([&](Employee* e) {
            return e->getUsername() == strUser && e->getPassword() == strPass;
        });
	}

	void showMyInfoFromFile(string strUsername) {
		string strFilename = DIR_INFO + "/" + strUsername + ".txt";
		ifstream fin(strFilename);
		if(!fin.is_open()) {
			setColor(RED);
			cout<<"Khong tim thay file thong tin chi tiet ("<<strFilename<<")"<<endl;
			setColor(WHITE);
			return;
		}
		string strLine;
		setColor(CYAN);
		cout<<"--- THONG TIN CUA BAN (tu file "<<strFilename<<") ---"<<endl;
		setColor(WHITE);
		vector<string> vecLabels = {"Ho ten: ", "Dia chi: ", "SDT: ", "Email: "};
		int iIndex = 0;
		while(getline(fin, strLine) && iIndex < 4) {
			setColor(YELLOW);
			cout<<left<<setw(10)<<vecLabels[iIndex]<<strLine<<endl;
			setColor(WHITE);
			iIndex++;
		}
		fin.close();
	}
};

class Application {
private:
	EmployeeManager _manager;
    Employee* _pCurrentUser;

	bool adminLogin() {
		ifstream fin(FILE_ADMIN);
		if(!fin.is_open()) {
			setColor(RED); 
			cout<<"Khong tim thay file"<<FILE_ADMIN<<endl;
			setColor(WHITE);
			return false;
		}
		string strFileUser, strFilePass, strInputUser, strInputPass;
		fin>>strFileUser>>strFilePass;
		fin.close();

		setColor(YELLOW);
		cout<<"***************************"<<endl;
		cout<<"*   DANG NHAP ADMIN       *"<<endl;
		cout<<"***************************"<<endl;
		setColor(WHITE);
		cout<<"User: ";
		getline(cin, strInputUser);
		cout<<"Pin: ";
		strInputPass = getHiddenPassword();

		if(trim(strInputUser) == strFileUser && strInputPass == strFilePass) {
			setColor(GREEN);
			cout<<"Dang nhap Admin thanh cong"<<endl;
			setColor(WHITE);
			return true;
		}

		setColor(RED);
		cout<<"Sai thong tin dang nhap"<<endl;
		setColor(WHITE);
		return false;
	}

	void showAdminMenu() {
		if (!adminLogin()) {
			return;
		}
		int iChoice;
		do {
			setColor(CYAN);
			cout<<"*********MENU**********"<<endl;
			cout<<"    1. Them Employee"<<endl;
			cout<<"    2. Xoa Employee"<<endl;
			cout<<"    3. Tim Employee"<<endl;
			cout<<"    4. Cap nhat Employee"<<endl;
			cout<<"    5. Hien thi thong Emplyee"<<endl;
			cout<<"    6. Thoat"<<endl;
			setColor(CYAN);
			cout<<"***********************"<<endl;
			setColor(WHITE);
			iChoice = getInput<int>("Nhap lua chon (1-6): ");

			switch(iChoice) {
				case 1: {
					string strUser, strName, strAddr, strPhone, strEmail;
					setColor(YELLOW);
					cout<<"--- THEM NHAN VIEN ---"<<endl;
					setColor(WHITE);

					cout<<"Nhap Username: ";
					getline(cin, strUser);
					strUser = trim(strUser);

					if(strUser.empty()) {
						setColor(RED);
						cout<<"Username khong duoc de trong"<<endl;
						setColor(WHITE);
						break;
					}

					if(_manager.isUsernameExist(trim(strUser))) {
						setColor(RED);
						cout<<"Username da ton tai, huy thao tac"<<endl;
						setColor(WHITE); 
						break;
					}

					cout<<"Nhap ho ten: ";
					getline(cin, strName);
					strName = trim(strName);
					if(strName.empty()) {
						setColor(RED);
						cout<<"Ho ten khong duoc de trong"<<endl;
						setColor(WHITE);
						break;
					}

					cout<<"Nhap dia chi: ";
					getline(cin, strAddr);
					strAddr = trim(strAddr);

					do{
						cout<<"Nhap SDT(10 so): ";
						getline(cin, strPhone);
						if(!isValidPhoneNumber(trim(strPhone))) {
							setColor(RED);
							cout<<"So dien thoai phai co dung 10 chu so"<<endl;
							setColor(WHITE);
						}
					}while(!isValidPhoneNumber(trim(strPhone)));

					do {
						cout<<"Nhap Email (phai co duoi @gmail.com): ";
						getline(cin, strEmail);

						strEmail = trim(strEmail);

						if(!isValidGmail(strEmail)) {
							setColor(RED);
							cout<<"Email khong hop le"<<endl;
							setColor(WHITE);
						} 
					} while(!isValidGmail(strEmail));

					_manager.addEmployee(new Employee(trim(strUser), DEFAULT_PASS, standardizeNames(strName), strAddr, strPhone, strEmail));
					setColor(GREEN); 
					cout << "Them thanh cong! Mat khau mac dinh la " << DEFAULT_PASS << "\n"; setColor(WHITE);
					break;
				}
				case 2: {
					string strUser; 
					cout<<"Nhap username can xoa: "; 
					getline(cin, strUser);
					_manager.deleteEmployee(trim(strUser));
					break;
				}
				case 3: {
					string strUser;
					cout<<"Nhap username can tim: ";
					getline(cin, strUser);
					Employee* pEmp = _manager.getEmployeeByUsername(trim(strUser));
					if(pEmp) {
						setColor(GREEN);
						cout<<"Tim thay: "<<*pEmp<<endl;
						setColor(CYAN);
						cout<<left<<setw(15)<<"USERNAME"<<setw(25)<<"HO TEN"<<setw(15)<<"SDT"<<setw(25)<<"EMAIL"<<setw(20)<<"DIA CHI"<<endl;
						cout << string(100, '-') << endl;
						setColor(WHITE);
						cout << *pEmp << endl;
					} else {
						setColor(RED);
						cout<<"Khong tim thay"<<endl; 
						setColor(WHITE);
					}
					break;
				}
				case 4: {
					_manager.updateEmployeeInfo();
					break;
				}
				case 5:
					_manager.showAll(); 
                    break;
				case 6:
					setColor(YELLOW);
					cout<<"Dang xuat Admin..."<<endl;
					setColor(WHITE);
					break;
				default:
				setColor(RED);
				cout<<"Lua chon khong hop le"<<endl;
				setColor(WHITE);
			} 
		} while(iChoice != 6);
	}

	void changePass(Employee* pEmp, bool bForced) {
		string strOld, strNew, strConfirm;
		if(bForced) {
			setColor(YELLOW);
			cout<<"\n========================================="<<endl;
			cout<<"BAN DANG NHAP LAN DAU VUI LONG DOI MAT KHAU"<<endl;
			cout<<"========================================="<<endl;
			setColor(WHITE);
		} else {
			cout<<"Nhap mat khau cu: ";
			strOld = getHiddenPassword();
			if(strOld != pEmp->getPassword()) {
				setColor(RED);
				cout<<"Mat khau cu khong dung"<<endl;
				setColor(WHITE);
				return;
			}
		}

		while(true) {
			cout<<"Nhap mat khau moi: ";
			strNew = getHiddenPassword();
			if(strNew.empty()) {
				setColor(RED);
				cout<<"Mat khau khong duoc de trong";
				setColor(WHITE);
				continue;
			}
			if(strNew.length() < 6) {
				setColor(RED);
				cout<<"Mat khau phai co it nhat 6 ky tu"<<endl;
				setColor(WHITE);
				continue;
			}

			cout<<"Xac nhan mat khau moi: ";
			strConfirm = getHiddenPassword();

			if(strNew == strConfirm) {
				pEmp->setPassword(strNew);
                _manager.saveToFile();
				setColor(GREEN);
				cout<<"Doi mat khau thanh cong"<<endl;
				setColor(WHITE);
				break;
			} else {
				setColor(RED);
				cout<<"Mat khau xac nhan khong khop, vui long thu lai"<<endl;
				setColor(WHITE);
			}
		}
	} 

	void showEmployeeMenu() {
		int iAttempts = 0;
		while(iAttempts < 3) {
			string strUser, strPass;
			cout<<"\n***************************"<<endl;
			cout<<"*  DANG NHAP EMPLOYEE     *"<<endl;
			cout<<"***************************"<<endl;
			setColor(WHITE);
			cout<<"User: ";
			getline(cin, strUser);
			cout<<"Pass: ";
			strPass = getHiddenPassword();

			_pCurrentUser = _manager.login(trim(strUser), strPass);

			if(_pCurrentUser) {
				setColor(GREEN);
				cout<<"Xin chao, "<< _pCurrentUser->getName()<<endl;
				setColor(WHITE);
				if(_pCurrentUser->getPassword() == DEFAULT_PASS) {
					changePass(_pCurrentUser, true);
				}

				int iChoice;
				do {
					setColor(CYAN);
					cout<<"*******MENU EMPLOYEES*******"<<endl;
					cout<<"     1. Xem thong tin tai khoan"<<endl;
					cout<<"     2. Doi password"<<endl;
					cout<<"     3. Thoat"<<endl;
					setColor(CYAN);
					cout<<"****************************"<<endl;
					setColor(WHITE);

					iChoice = getInput<int>("Nhap lua chon: ");

					if(iChoice == 1) {
						_manager.showMyInfoFromFile(_pCurrentUser->getUsername());

					} else if(iChoice == 2) {
						changePass(_pCurrentUser, false);
					} else if(iChoice == 3) {
						setColor(YELLOW);
						cout<<"Dang xuat..."<<endl;
						setColor(WHITE);
					} else {
						setColor(RED);
						cout<<"Lua chon khong hop le"<<endl;
						setColor(WHITE);
					}
				} while(iChoice != 3);

				_pCurrentUser = nullptr;
                return;
			} else {
				setColor(RED);
				cout<<"Sai Username hoac Password"<<endl;
				cout<<"Con "<<(2 - iAttempts)<<" lan thu"<<endl;
				setColor(WHITE);
				iAttempts++;
			}
		}
		setColor(RED);
		cout<<"\n========================================="<<endl;
		cout<<"  BAN DA NHAP SAI 3 LAN"<<endl;
		cout<<"  HE THONG KHOA CHUC NANG DANG NHAP"<<endl;
		cout<<"========================================="<<endl;
		setColor(WHITE);
	}

public:
	void run() {
		ensureDirectoryExists();
		while(true) {
			setColor(YELLOW);
			cout<<"\n============================================="<<endl;
			cout<<"   HE THONG QUAN LY NHAN SU (POPEYES)   "<<endl;
			cout<<"============================================="<<endl;
			setColor(WHITE);
			cout<<"1. Admin (Quan tri vien)"<<endl;
            cout<<"2. Employee (Nhan vien)"<<endl;
            cout<<"3. Thoat chuong trinh"<<endl;
			setColor(YELLOW);
			cout<<"============================================="<<endl;
			setColor(WHITE);

			int iRole = getInput<int>("Nhap vai tro cua ban (1-3): ");
			
			if(iRole == 1) {
				showAdminMenu();
			} else if(iRole == 2) {
				showEmployeeMenu();
			} else if(iRole == 3) {
				cout<<"Ket thuc chuong trinh, tam biet"<<endl;
				break;
			} else {
				cout<<"Lua chon khong hop le"<<endl;
			}
		}
	}
};

int main() {
	SetConsoleOutputCP(65001); 
    Application app;
    app.run();
	
return 0;
}