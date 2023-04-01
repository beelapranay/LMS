#include <iostream>
#include<winsock.h>
#include<windows.h>
#include<sstream>
#include<ctime>
#include<string>
#include<mysql.h>
#include<cstring>
#include<conio.h>
#include<cstring>
#include<map>
using namespace std;


int users =0;
int count_attempt=0;
int curr_user=0;
int user_no=0;
map <string,string> issued;
string loggedin_user="";

class user
{
private:
string username;
string password;
public:
void adduser()
{
	MYSQL *conn;
	conn = mysql_init(0);
	conn = mysql_real_connect(conn ,"localhost" ,"admin1", "admin1", "librarybook" ,0,NULL,0);
	if(conn)
        cout<<"Connected"<<endl;

    else cout<<"Not Connected!"<<endl;

	cout<<"Username: ";
	cin>>username;
	cout<<"Password: ";
	cin>>password;

	int qstate =0;
	stringstream ss;
	ss<<"INSERT INTO users(username,password) VALUES('"<<username<<"','"<<password<<"')";
	string query =  ss.str();
        const char* q= query.c_str();
        qstate = mysql_query(conn,q);
        if(qstate==0)
            cout<<" \t \t \t \t \t You are now a member of this library!"<<endl;
        else cout<<"Registration Failed!"<<endl;

	users++;
}

void no_users()
{

   if(loggedin_user!="admin")
    {
        cout<<" \t \t \t \t \t Route not authorized! Only ADMIN can view all the user(s) details"<<endl;
        return ;
    }

	cout<<"Existing users are: "<<endl;
	MYSQL* conn;
	MYSQL_ROW row;
	MYSQL_RES* res;
	conn = mysql_init(0);
	conn = mysql_real_connect(conn ,"localhost" ,"admin1", "admin1", "librarybook" ,0,NULL,0);
	if(conn)
    {
        int qstate = mysql_query(conn,"SELECT username FROM users");

        if(!qstate)
        {
            res = mysql_store_result(conn);
            int i=1;
            while(row = mysql_fetch_row(res))
                {
                    cout<<i<<"."<<row[0]<<endl;
                    i++;
                }
        }
    }


}

void login()
{
	if(curr_user==1)
	{
	cout<<" \t \t \t \t \t Already Logged In!";
	return;
	}
	if(count_attempt==3)
	{
		cout<<endl<<endl;
		cout<<" \t \t \t \t \t Please wait to try again!";
		exit(0);
	}
	cout<<"Enter Username and Password to Log In"<<endl;
	string user_name;
	string pass_word;

	cout<<"Username: ";
	cin>>user_name;
	cout<<"Password: ";
	cin>>pass_word;
	cin.ignore();
	system("cls");


	MYSQL* conn;
	MYSQL_ROW row;
	MYSQL_RES* res;
	conn = mysql_init(0);
	conn = mysql_real_connect(conn ,"localhost" ,"admin1", "admin1", "librarybook" ,0,NULL,0);
	if(conn)
    {
        int qstate = mysql_query(conn,"SELECT username,password FROM users");
        bool flag=false;
        if(!qstate)
        {
            res = mysql_store_result(conn);
            while(row = mysql_fetch_row(res))
                {
                if(row[0]==user_name && row[1]==pass_word)
                {
                        loggedin_user = row[0];
                        cout<<" \t \t \t \t \t \t Log In Successful! Username - "<< loggedin_user<<endl;
                        curr_user=1;
                        flag=true;
                        break;
                }
                }
                if(!flag)
                {
                    cout<<" \t \t \t \t \t \t Incorrect Username/Password"<<endl;
                    cout<<" \t \t \t \t \t \t Please try again!"<<endl;
                    cin.ignore();
                    system("cls");
                    count_attempt++;
                    login();
                }


        }
        else
        {
            cout<<" \t \t \t \t \t No account is registered!";
        }

    }
}


};
class book : public user
{
	long long int bno;
	string bookname;
	string aname;
	string default_issued = "None";
	public:

		void addbook()
		{
			if(loggedin_user=="")
			{
				cout<<" \t \t \t \t \t Please Log In!"<<endl;
				return ;
			}
            if(loggedin_user!="admin")
			{
				cout<<" \t \t \t \t \t Route not authorized! Only ADMIN can add new books!!!"<<endl;
				return ;
			}

			cout<<"UID of the book: ";
			cin>>bno;
            cin.ignore();
			cout<<"Name of the book: ";
			cin>>bookname;
            cin.ignore();
			cout<<"Author of the book: ";
			cin>>aname;
            string nul="";
			MYSQL* conn;
			conn = mysql_init(0);
			conn = mysql_real_connect(conn ,"localhost" ,"admin1", "admin1", "librarybook" ,0,NULL,0);
			stringstream ss;
			ss<<"INSERT INTO book(BookNo,Book_Name,Author,Issued_By) VALUES('"<<bno<<"','"<<bookname<<"','"<<aname<<"','"<<default_issued<<"')";
			int qstate=0;
			string query =  ss.str();
            const char* q= query.c_str();
            qstate = mysql_query(conn,q);
            if(qstate==0)
            {
            cout<<" \t \t \t \t \t Book has been added successfully!"<<endl;
            return;
            }
            else
            {
                cout<<" \t \t \t \t \t System Error!!!"<<endl;
                return;
            }

		}

		bool searchbook(string no)
		{
			if(loggedin_user=="")
			{
				 cout<<" \t \t \t \t \t Please Log In!"<<endl;
				 return false;
			}

			bool flag=false;
			MYSQL* conn;
            MYSQL_ROW row;
            MYSQL_RES* res;
            conn = mysql_init(0);
            conn = mysql_real_connect(conn ,"localhost" ,"admin1", "admin1", "librarybook" ,0,NULL,0);
            if(conn)
            {
            int qstate = mysql_query(conn,"SELECT BookNo,Book_Name,Author from book");
            if(!qstate)
            {
                res = mysql_store_result(conn);
                while(row = mysql_fetch_row(res))
                {
                        if(row[0]== no)
                        {
                            cout<<"The book details are: "<<endl;
                            cout<<row[0]<<"  "<<row[1]<<"  "<<row[2]<<"  "<<row[3]<<endl;
                            flag=true;
                            return true;
                        }
                }
            }
            else cout<<"Failed"<<endl;
            if(!flag)
                {
                    cout<<"Book has not been found!"<<endl;
                    return false;
                }
            }
		}

		void allbooks()
		{
			MYSQL* conn;
            MYSQL_ROW row;
            MYSQL_RES* res;
            conn = mysql_init(0);
            conn = mysql_real_connect(conn ,"localhost" ,"admin1", "admin1", "librarybook" ,0,NULL,0);
            if(conn)
            {
            int qstate = mysql_query(conn,"SELECT BookNo,Book_Name,Author,Issued_By from book");
            if(!qstate)
            {
                res = mysql_store_result(conn);
                while(row = mysql_fetch_row(res))
                {
                        cout<<" \t \t \t \t \t "<<row[0]<<" "<<row[1]<<"  "<<row[2]<<endl;
                }

            }
            }
            else cout<<"System Error!!!"<<endl;
		}

	void issue()
	{
		if(loggedin_user=="")
		{
			cout<<"Please Log In!"<<endl;
			return ;
		}
		string bno;
		cout<<" \t Enter the number of the book to be issued: "<<endl;
		cin>>bno;
		bool found = searchbook(bno);
		if(found)
		{
            MYSQL* conn;
            MYSQL_ROW row;
            MYSQL_RES* res;
            conn = mysql_init(0);
            conn = mysql_real_connect(conn ,"localhost" ,"admin1", "admin1", "librarybook" ,0,NULL,0);
            stringstream ss;
            int qstate=0;
            if(conn)
            {
            int qstate = mysql_query(conn,"SELECT BookNo, Book_Name, Author, Issued_By from book");
            if(!qstate)
            {
                res = mysql_store_result(conn);
                while(row = mysql_fetch_row(res))
                {
                        if(bno==row[0])
                        {
                            ss<<"UPDATE book SET Issued_By=('"<<loggedin_user<<"')";
                            string query =  ss.str();
                              const char* q= query.c_str();
                              qstate = mysql_query(conn,q);
                              if(qstate==0){
                                cout<<" \t \t \t \t \t Book has been issued successfully!"<<endl;
                                cout<<" \t \t \t The book has to be returned within a week of issuing."<<endl;
                              }
                              else cout<<"Failed"<<endl;
                              issued.insert({loggedin_user, bno});
                              return;
                        }
                }

            }
		}
    }
	else cout<<" \t \t \t \t \t Book has not been found!"<<endl;

}
	void deposit()
	{
		map<string ,string>::iterator itr;
		for(itr=issued.begin();itr!=issued.end();itr++)
		{
			if(itr->first==loggedin_user)
			{
			issued.erase(loggedin_user);
			cout<<" \t \t \t \t \t Book has been returned successfully!";
			return ;
			}
		}
	}

	void print_user()
	{
        map<string ,string>:: iterator itr;
        bool flag=false;

        for(itr=issued.begin();itr!=issued.end();itr++)
        {
            if(itr->first==loggedin_user ){
                cout<<" \t \t \t \t \t Current user - "<<itr->first<<"   has been issued book number:   "<<itr->second;
                flag=true;
            }

        }

        if(!flag)
            cout<<" \t \t \t \t \t "<<loggedin_user<<" User is logged out/No books have been issued!";
            cout<<endl;
	}

};


int main()
{
	cout<<" \t \t \t \t \t Library Management System "<<endl<<endl<<endl<<endl;
	user u1;
	int x;
	int choice=0;
	book b1;

	do
	{
		cout<<endl;
		cout<<"Menu:"<<endl;
		cout<<"1. Login"<<endl;
		cout<<"2. Register"<<endl;
		cout<<"3. Show all Books"<<endl;
		cout<<"4. Search a Book"<<endl;
		cout<<"5. Issue a Book"<<endl;
		cout<<"6. Return a Book"<<endl;
		cout<<"7. Get all the Users"<<endl;
		cout<<"8. Add a Book"<<endl;
		cout<<"9. Issued Books"<<endl;
		cout<<"0 to exit \n"<<endl;
		cin>>x;
	string no2;

	switch(x)
	{
		case 1 : u1.login();
				 break;
		case 2 : u1.adduser();
				 u1.login();
				 break;
		case 3 : b1.allbooks();
				 break;
		case 4 : cout<<"Enter book number: ";
				 cin>>no2;
				 b1.searchbook(no2);
		case 5 : b1.issue();
				 break;
		case 6 : b1.deposit();
				 break;
		case 7 : u1.no_users();
				 break;
				 break;
		case 8 : b1.addbook();
				 break;
		case 9 : b1.print_user();
				 break;
		case 0 : choice=1;

	}
	}
	while(choice==0);
}
