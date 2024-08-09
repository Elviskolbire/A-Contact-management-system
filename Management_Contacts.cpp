//A contact managing module

//Including all the libraries needed in the program 
#include <iostream>
#include <list>
#include <fstream> 
#include <string>
#include <vector>
#include <sstream>
#include <regex>
#include <algorithm>
using namespace std;
//A class header file that contains the implementations of our class
#include "class_contact.h"

//Declearing all our functions before defining them, to avoid errors  when calling a function below the calling function 
list<contact> add_contact(list<contact> &add);
void view_contact(const list<contact> &Contacts);void save_to_file(list<contact> &con);
void delete_contact(list<contact> &del);
void search_contact(list<contact> &Contacts);
string remove_wide_spaces(const string &name);
void read_file(list<contact> &Contact);
bool validation_e(const string &email);
void contact_backup_file(const string &c_file, const string & b_file );
bool phone_validation(const string& number);

//The implementation details of adding a new Contact to the list.

list<contact> add_contact(list<contact> &add)
{
	string name, email, phone;
	
	/*name: use to take the owner name 
email : use to receive input of the owner 
phone : To take the phone number */
	do
	{
		if (name.empty())
		{
			std::cout << "Enter name: ";
			std ::getline(std::cin, name);
			for (auto &name : name)
			{
				name = std::toupper(name);
			}
			name = remove_wide_spaces(name);
			/* Change the user input to an upper case letter, then any extra spacing between names to a single space.*/
		}
		else
			break;
	} while (name.empty() && std::cout << "\nInvalid name!\n\n");

	do
	{
		do
		{
			//Check phone number validation is true before we can escape the loop and confirm a valid input has been received 
			std::cout << "Enter your contact: ";
			std ::getline(std::cin, phone);
		} while ((phone_validation( phone) == false && std::cout << "\nContact must be 10 digits!!!\n\n"));

	} while (phone.empty() && std::cout << "No Contact entered !\n\n");

	do
	{
		std::cout << "Enter  your email: ";
		std ::getline(std::cin, email);
		for (auto &email : email)
			email = std::tolower(email);
			
	//Check email validation is true before we can escape the loop
	//Display a message if validation is false
	} while ((email.empty() && std::cout << "no email entered!\n\n") || (validation_e(email) == false and std::cout << "\nInvalid email format eg: example@gmail.com\n\n"));
	add.emplace_back(contact(name, email, phone));
	std::cout << "\n"
			  << name << " added successfully\n\n";
			 
			  save_to_file (add);//Save contact to file any time a new Contact is been successfully added

	return add; // returns the current list of Contacts 
}


//The function for viewing our contact list
void view_contact( list<contact> &Contacts)
{
	read_file(Contacts);// loads all the contacts in file
	if (!Contacts.empty())
	{
		std::cout << "******CONTACT LIST*****\n******°°°°°°°°°°°°°°°°°°******\n";
		for (const auto &Contact : Contacts)
		{
			std::cout << "Name: " << Contact.name << "\nPhone number: " << Contact.phone_number << "\nEmail: " << Contact.email << "\n\n";
		} //displays all the contacts using a loop to traverse the list if the list is not empty
	}
	else
		std::cout << "Empty phonebook \n\n";
}

/*Searching for a specific contact using the name of the handler */
void search_contact(list<contact> &Contacts)
{
	string name;
	do
	{
		/* We first check to see if the name is valid, and match the letter to capital letter then compare to check if name is in the contact saved
		*/
		std::cout << "Search by name: ";
		getline(std::cin, name);
		for (auto &name : name)
			name = std::toupper(name);//automatically change characters to upper case letters 
		name = remove_wide_spaces(name);//removes wide spaces between names

	} while (name.empty() && std::cout << "Invalid name!\n\n");
	read_file(Contacts);
	if (!Contacts.empty())
	{	//iterating the list to check for  targeted search
		for (const auto names : Contacts)
		{    
			if (names.name == name)
			{
				std::cout << "Contact found: " << names.name << "\nCONTACT: " << names.phone_number << "\nEmail: " << names.email << "\n\n";
				return;
			}
		}
		std::cout << name << " Not found in contact! \n\n";
	}
	else
		std::cout << "No Contacts \n\n";
}

//To delete a  contact 
void delete_contact(list<contact> &del)
{
	string name;
  read_file();
	
	if (!del.empty())
	{
	do
	{
		//Validation of the name before searching to delete it
		std::cout << "Delete by name: ";
		getline(std::cin, name);
		for (auto &name : name)
			name = std::toupper(name);//Change input characters to upper case. 
		name = remove_wide_spaces(name);//removes wide spaces between names and insert only one single space 

	} while (name.empty() && std::cout << "Invalid name!\n\n");
	read_file(del);

		for (auto dels = del.begin(); dels != del.end(); dels++)
		{
			if (name == dels->name)
			{
				del.erase(dels);//removes the contact if found 
				std::cerr << "Contact deleted\n\n ";
				save_to_file(del); // Update the current file for the changes made.
				return;
			}
		}

		std::cout << "Contact is not in phonebook\n\n";
		return;
	}
	std::cout << "Nothing to delete \n\n";
}

//Save contact details to a 📂
void save_to_file(list<contact> &con)
{
	/* Any time we want to save new info to our file, we backup the previous data to prevent data lost incase of any unforseen circumstances */
	contact_backup_file("Contacts.txt", "backup_contact.txt");	
	std::vector<contact> vec(con.begin(), con.end());
	//Sort function from the standard library to sort the list by temporarily storing the data in a vector passing in  begin(), end() iterators and a lamba function to our sort function 
	std::sort(vec.begin(), vec.end(), [](const contact &a, contact &b) { return a.name < b.name; });
	std::ofstream file;
	
	file.open("Contacts.txt" );//Open the file for writing data eg. "Contacts.txt"

	if (file.is_open()) // Making sure the file is opened 
	{
		for (const auto cons : vec)
		{
			file << "Name: " << cons.name << "\nPhone number: " << cons.phone_number << "\nEmail: " << cons.email << "\n\n";
		}

		file.close();//Closes the file after reading 
	}
	else
	{ // throws an error message if the file is unable to open ;
		std::cerr << "Unable to open file! \n";
	}
}

//Reads  from file/Loads data from file 📂
void read_file(list<contact> &Contact)
{
	ifstream file("Contacts.txt");
	std::string name, phone, email, line;
	if (!file)
	{
		std::cerr << "Unable to open file" << std::endl;//Throws an error if file fails to open
		return;
	}
Contact.clear(); // Clears the current list before loading every  data from file back to the list
	while (std::getline(file, line))
	{
		/*This loop Reads everything on file so  I  makes sure the name, phone, email indicators are removed from the strings when reading each line of the files to prevent 
		reading incorrect strings and appending to list  using find() to catch the prefix string and substr() to delete the string*/
		if (line.find("Name: ") == 0)
		{
			name = line.substr(6);
		}
		else if (line.find("Phone number: ") == 0)
		{
			phone = line.substr(14);
		}
		else if (line.find("Email: ") == 0)
		{
			email = line.substr(7);
			Contact.emplace_back(name, email, phone);			//Appends the exact name,email and phone number to the contact List 
		}
	}
	file.close();//close file after reading/loading 
}

//This function removes wide spaces between words and insert only one space. it takes one param: name 
string remove_wide_spaces(const string &name)
{
	std::istringstream istring(name);//automatically graps the all the string
	string word, results;
	while (istring >> word) // Extract each word and stores at the word variable 
	{
		if (!(results.empty()))
			results += ' '; // Appends a single space if a word is been extracted
		results += word; // finally append the word to results 
	}
	return results; // return results with a single spacing between each word
}

//A boolean function to check for email validation using regular expressions  #regex library 
bool validation_e(const string &email)
{
	//Param: Take one  constant email parameter 
	const std::regex pattern(R"(^[a-zA-Z0-9.!#$%&'*+/=?^_`{|}~-]+@[a-zA-Z0-9](?:[a-zA-Z0-9-]{0,61}[a-zA-Z0-9])?(?:\.[a-zA-Z0-9](?:[a-zA-Z0-9-]{0,61}[a-zA-Z0-9])?)*$)");
	return std::regex_match(email, pattern);//returns true if the input email  meets all conditions in the regular expression
}

//void Backed up function to store files in a different file
void contact_backup_file(const string &c_file, const string & b_file )
{
	ifstream current_file(c_file, ios::binary);
	ofstream backed_file(b_file, ios::binary);
	if (backed_file && current_file)
	{
	 backed_file << current_file.rdbuf();//Reads and write to the backed_file 
	}
	
}

//boolean function to check phone number validation 
bool phone_validation(const string& number)
{
	regex number_pattern(R"(^\d{10}$)");
	//regex_match() to match the input number with the expression 
	return regex_match(number, number_pattern);//returns true if  number is valid or false if number does not meet requirements
}

int main()
{
	string name, phone, email;
	
	std::ifstream file("Contacts.txt");
	
	list<contact> lis;//List object
	int choice;
	
	/*I am using an infinite loop to repeat all the code in the while block until the user decides to quit the program .  switch statements is been use to serve user an opportunity to perform an action at a time.
	
	cin.ignore() will clear and ignore the input buffer incase unintended characters are entered,  this will prevent the program from crashing or running into errors. 
	*/
	while (true) 
	{
		std::cout << "1. Add contact to phonebook\n";
		std::cout << "2. View contact in phonebook\n";
		std::cout << "3. Search contact in phonebook\n";
		std::cout << "4. Delete contact in phonebook\n";
		std::cout << "5. Exit app\n";
		std::cout << "Chose your choice: ";
		std::cin >> choice;
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "\n";
		switch (choice)
		{
		case 1:
		add_contact(lis);
			break;
			
		case 2:

			view_contact(lis);
			break;
		case 3:
			search_contact(lis);
			break;

		case 4:
			delete_contact(lis);
			break;

		case 5:
			std::cout << "@@@@EXITING PROGRAM @@@@\n";
			return 0;

		default:
			std::cerr << "Wrong choice!!\n\n";
		}
	}
}