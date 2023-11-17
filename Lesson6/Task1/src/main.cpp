#include <iostream>
#include <utility>
#include <string>
#include <Wt/Dbo/backend/Postgres.h>

#include "books_service_db.h"
#include "shop.h"


void fillDatabase(databases::BooksServiceDB& db)
{
	try
	{
		db.addPublisher("EKSMO");
		db.addPublisher("PITER");
		db.addPublisher("AST");

		db.addShop("Bukvoed");
		db.addShop("Garage Bookshop");
		db.addShop("Book House");
		db.addShop("Best Book Shop");
		db.addShop("Literator");

		db.addBook("Lord of The Rings", "1");
		db.addBook("Harry Potter", "2");
		db.addBook("Witcher", "3");

		db.addStockEntry("1", "1", 10);
		db.addStockEntry("2", "1", 8);
		db.addStockEntry("3", "1", 6);

		db.addStockEntry("1", "2", 3);
		db.addStockEntry("2", "2", 8);
		db.addStockEntry("3", "2", 6);

		db.addStockEntry("1", "3", 10);
		db.addStockEntry("2", "3", 4);

		db.addStockEntry("1", "4", 3);

		db.addSaleEntry(100, "2023-11-06", "2", 1);
		db.addSaleEntry(200, "2023-12-11", "3", 1);
		db.addSaleEntry(350, "2023-12-16", "5", 1);
	}
	catch (const Wt::Dbo::Exception& e)
	{
		throw e;
	}
}


void printShopsHasPublisher(databases::BooksServiceDB& db, std::string& publisher_name)
{
		auto shops = db.findShopsHasPublisherBooks(publisher_name);

		std::cout << "Publisher " + publisher_name + " presented in following shops:\n";

		if (shops.size() == 0)
		{
			std::cout << "No shops found!\n";
		}
		else
		{
			for (const auto& shop : shops) {
				std::cout << shop << std::endl;
			}
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

		databases::BooksServiceDB books_service_db(username, password);

		fillDatabase(books_service_db);

		std::cout << "Enter publisher name: ";
		std::string publisher_name;
		std::cin >> publisher_name;

		printShopsHasPublisher(books_service_db, publisher_name);

		std::cin.get();
		std::cin.get();
	}
	catch (const Wt::Dbo::Exception& e)
	{
		std::cout << e.what() << std::endl;
		std::cin.get();
		std::cin.get();
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
		std::cin.get();
		std::cin.get();
	}
}
