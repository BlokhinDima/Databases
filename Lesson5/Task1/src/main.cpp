#include <iostream>
#include <string>
#include <exception>

#include "clients_db.h"


void printClientInfoFieldsNames()
{
	std::cout << std::endl;
	std::cout << std::left << std::setw(5) << "ID"
		<< std::left << std::setw(20) << "Name"
		<< std::left << std::setw(20) << "Surname"
		<< std::left << std::setw(20) << "Email"
		<< std::left << std::setw(20) << "Phone Number"
		<< std::endl;
}


void printClientsInfo(databases::ClientsDB& clients_db)
{
	auto result = clients_db.getClientsInfo();

	printClientInfoFieldsNames();

	int const num_rows = result.size();
	for (int rownum = 0; rownum < num_rows; ++rownum)
	{
		pqxx::row const row = result[rownum];
		int const num_cols = row.size();
		for (int colnum = 0; colnum < num_cols; ++colnum)
		{
			pqxx::field const field = row[colnum];
			int width = 5;

			if (colnum != 0)
			{
				width = 20;
			}

			std::cout << std::left << std::setw(width) << field.c_str();
		}

		std::cout << std::endl;
	}
}


void printQueryResult(pqxx::result& result)
{
	for (const auto& row : result)
	{
		for (auto const& field : row) std::cout << field.c_str() << '\t';
		std::cout << std::endl;
	}
}


void printMenu()
{
	std::cout << std::endl;
	std::cout << "1. Show Clients Info" << std::endl;
	std::cout << "2. Add client" << std::endl;
	std::cout << "3. Delete client" << std::endl;
	std::cout << "4. Find Client" << std::endl;
	std::cout << "5. Add Client Phone Number" << std::endl;
	std::cout << "6. Delete Client Phone Number" << std::endl;
	std::cout << "7. Change Client Info" << std::endl;
	std::cout << "0. Exit" << std::endl;
	std::cout << std::endl;
}


void printTableFiledsMenu()
{
	std::cout << std::endl;
	std::cout << "1. Change Client Name" << std::endl;
	std::cout << "2. Change Client Surname" << std::endl;
	std::cout << "3. Change Client Email" << std::endl;
	std::cout << "0. Exit" << std::endl;
	std::cout << std::endl;
}


void printSearchMenu()
{
	std::cout << std::endl;
	std::cout << "1. Search Clients By Name" << std::endl;
	std::cout << "2. Search Clients By Surname" << std::endl;
	std::cout << "3. Search Clinent By Email" << std::endl;
	std::cout << "4. Search Clinent By Phone Number" << std::endl;
	std::cout << "0. Exit" << std::endl;
	std::cout << std::endl;
}


int handleInput()
{
	int choice = 0;
	std::cout << "Enter value: ";
	std::cin >> choice;
	while (std::cin.fail())
	{
		std::cout << std::endl << "Wrong input! Try again." << std::endl;
		std::cout << "Enter value: ";
		std::cin.clear();
		std::cin.ignore(INT_MAX, '\n');
		std::cin >> choice;
	}
	return choice;
}


void handleClientAddition(databases::ClientsDB& clients_db)
{
	try
	{
		std::cout << "Enter client name: ";
		std::string name;
		std::cin >> name;

		std::cout << "Enter client surname: ";
		std::string surname;
		std::cin >> surname;

		std::cout << "Enter client email: ";
		std::string email;
		std::cin >> email;

		clients_db.addClient(name, surname, email);
		std::cout << "New client added!\n";
		printClientsInfo(clients_db);
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}


void handleClientDeletion(databases::ClientsDB& clients_db)
{
	try
	{
		std::cout << "Enter ID to delete client: ";
		std::string id;
		std::cin >> id;
		clients_db.deleteClient(id);
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}


void handleClientSearch(databases::ClientsDB& clients_db)
{
	try
	{
		printSearchMenu();
		auto choice = handleInput();

		std::cout << "Enter search string: ";
		std::string search_string;
		std::cin >> search_string;

		auto field = static_cast<databases::ClientsDB::ClientInfoFields>(choice - 1);
		auto query_result = clients_db.findClient(field, search_string);
		std::cout << std::endl << "Search result: " << std::endl;
		printClientInfoFieldsNames();
		printQueryResult(query_result);
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}


void handleClientInfoUpdate(databases::ClientsDB& clients_db)
{
	try
	{
		printTableFiledsMenu();
		auto choice = handleInput();

		std::cout << "Enter client ID: ";
		std::string id;
		std::cin >> id;

		std::cout << "Enter new value: ";
		std::string new_value;
		std::cin >> new_value;

		auto field = static_cast<databases::ClientsDB::ClientInfoFields>(choice - 1);
		clients_db.updateClientInfo(id, field, new_value);
		std::cout << "Client ID = " + id + " info updated!\n";
		printClientsInfo(clients_db);
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}


void handlePhoneNumberAddition(databases::ClientsDB& clients_db)
{
	try
	{
		std::cout << "Enter client ID: ";
		std::string id;
		std::cin >> id;

		std::cout << "Enter phone number: ";
		std::string phone_number;
		std::cin >> phone_number;

		clients_db.addPhoneNumber(id, phone_number);
		std::cout << "Phone number " + phone_number + " added to client with ID = " + id << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}


void handlePhoneNumberDeletion(databases::ClientsDB& clients_db)
{
	try 
	{
		std::cout << "Enter phone number to delete: ";
		std::string phone_number;
		std::cin >> phone_number;

		clients_db.deletePhoneNumber(phone_number);
		std::cout << "Phone number " + phone_number + " deleted or not exist in table! " << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}


int main()
{
	try
	{
		std::cout << "Enter database username: ";
		std::string username;
		std::cin >> username;

		std::cout << "Enter username password: ";
		std::string password;
		std::cin >> password;

		databases::ClientsDB clients_db(username, password);

		for (;;)
		{
			printMenu();
			auto choice = handleInput();

			switch (choice)
			{
			case 1:
				printClientsInfo(clients_db);
				break;

			case 2:
				handleClientAddition(clients_db);
				break;

			case 3:
				handleClientDeletion(clients_db);
				break;

			case 4:
				handleClientSearch(clients_db);
				break;

			case 5:
				handlePhoneNumberAddition(clients_db);
				break;

			case 6:
				handlePhoneNumberDeletion(clients_db);
				break;

			case 7:
				handleClientInfoUpdate(clients_db);
				break;

			case 0:
				return 0;

			default:
				break;
			}
		}
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
		std::cin.get();
		std::cin.get();
	}
}
