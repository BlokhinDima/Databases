#pragma once

#include <utility>
#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/backend/Postgres.h>

#include "book.h"
#include "publisher.h"
#include "sale.h"
#include "shop.h"
#include "stock.h"


namespace databases
{
	class BooksServiceDB
	{
	public:
		BooksServiceDB(const std::string& user, const std::string& password);
		BooksServiceDB(const BooksServiceDB&) = delete;
		~BooksServiceDB() {};
		BooksServiceDB& operator=(const BooksServiceDB&) = delete;
		void createTables();
		void addBook(const std::string& title, const std::string& publisher_id);
		void addPublisher(const std::string& name);
		void addShop(const std::string& name);
		
		void addStockEntry(
			const std::string& book_id, 
			const std::string& shop_id, 
			int count);

		void addSaleEntry(
			int price, 
			const std::string& date, 
			const std::string& stock_id, 
			int count);

		std::vector<std::string> findShopsHasPublisherBooks(const std::string& publisher_name);

	private:
		std::string dbname = "BooksService";
		std::string host = "localhost";
		std::string port = "5432";
		std::unique_ptr<Wt::Dbo::Session> session;

		std::unique_ptr<tables::Publisher> publisher;
		std::unique_ptr<tables::Book> book;
		std::unique_ptr<tables::Sale> sale;
		std::unique_ptr<tables::Shop> shop;
		std::unique_ptr<tables::Stock> stock;
	};
}