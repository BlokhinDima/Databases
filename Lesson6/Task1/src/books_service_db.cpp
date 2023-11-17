#include <string>
#include <vector>
#include <utility>
#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/backend/Postgres.h>

#include "books_service_db.h"
#include "book.h"
#include "publisher.h"
#include "sale.h"
#include "shop.h"
#include "stock.h"


namespace databases
{
	BooksServiceDB::BooksServiceDB(const std::string& user, const std::string& password)
	{
		try
		{
			std::string connectionString =
				"host=" + host +
				" port=" + port +
				" dbname=" + dbname +
				" user=" + user +
				" password=" + password;

			auto postgres = std::make_unique<Wt::Dbo::backend::Postgres>(connectionString);
			session = std::make_unique<Wt::Dbo::Session>();
			(*session).setConnection(std::move(postgres));

			createTables();
		}
		catch (const Wt::Dbo::Exception& e)
		{
			throw e;
		}
	}


	void BooksServiceDB::createTables()
	{
		try
		{
			(*session).mapClass<tables::Publisher>("publisher");
			(*session).mapClass<tables::Book>("book");
			(*session).mapClass<tables::Sale>("sale");
			(*session).mapClass<tables::Shop>("shop");
			(*session).mapClass<tables::Stock>("stock");
			(*session).createTables();
		}
		catch (const Wt::Dbo::Exception& e) 
		{
			std::cout << "Tables already created!: " << e.what() << std::endl;
		}
	}


	void BooksServiceDB::addBook(const std::string& title, const std::string& publisher_id)
	{
		try
		{
			Wt::Dbo::Transaction transaction{ *session };
			std::unique_ptr<tables::Book> book{ new tables::Book() };
			book->title = title;
			Wt::Dbo::ptr<tables::Book> bookPtr = (*session).add(std::move(book));
			Wt::Dbo::ptr<tables::Publisher> publisherPtr = (*session).find<tables::Publisher>().where("id = ?").bind(publisher_id);
			bookPtr.modify()->publisher = publisherPtr;
			transaction.commit();
		}
		catch (const Wt::Dbo::Exception& e)
		{
			throw e;
		}
	}


	void BooksServiceDB::addPublisher(const std::string& name)
	{
		try
		{
			Wt::Dbo::Transaction transaction{ *session };
			std::unique_ptr<tables::Publisher> publisher{ new tables::Publisher() };
			publisher->name = name;
			(*session).add(std::move(publisher));
			transaction.commit();
		}
		catch (const Wt::Dbo::Exception& e)
		{
			throw e;
		}
	}


	void BooksServiceDB::addShop(const std::string& name)
	{
		try
		{
			Wt::Dbo::Transaction transaction{ *session };
			std::unique_ptr<tables::Shop> shop{ new tables::Shop() };
			shop->name = name;
			(*session).add(std::move(shop));
			transaction.commit();
		}
		catch (const Wt::Dbo::Exception& e)
		{
			throw e;
		}
	}


	void BooksServiceDB::addStockEntry(
		const std::string& book_id,
		const std::string& shop_id,
		int count)
	{
		try
		{
			Wt::Dbo::Transaction transaction{ *session };
			std::unique_ptr<tables::Stock> stock{ new tables::Stock() };
			stock->count = count;
			Wt::Dbo::ptr<tables::Stock> stockPtr = (*session).add(std::move(stock));

			Wt::Dbo::ptr<tables::Book> bookPtr = (*session).find<tables::Book>().where("id = ?").bind(book_id);
			stockPtr.modify()->book = bookPtr;

			Wt::Dbo::ptr<tables::Shop> shopPtr = (*session).find<tables::Shop>().where("id = ?").bind(shop_id);
			stockPtr.modify()->shop = shopPtr;

			transaction.commit();
		}
		catch (const Wt::Dbo::Exception& e)
		{
			throw e;
		}
	}


	void BooksServiceDB::addSaleEntry(
		int price,
		const std::string& date,
		const std::string& stock_id,
		int count)
	{
		try
		{
			Wt::Dbo::Transaction transaction{ *session };
			std::unique_ptr<tables::Sale> sale{ new tables::Sale() };
			sale->price = price;
			sale->date_sale = date;
			sale->count = count;
			Wt::Dbo::ptr<tables::Sale> salePtr = (*session).add(std::move(sale));

			Wt::Dbo::ptr<tables::Stock> stockPtr = (*session).find<tables::Stock>().where("id = ?").bind(stock_id);
			salePtr.modify()->stock = stockPtr;

			transaction.commit();
		}
		catch (const Wt::Dbo::Exception& e)
		{
			throw e;
		}
	}


	std::vector<std::string> BooksServiceDB::findShopsHasPublisherBooks(const std::string& publisher_name)
	{
		Wt::Dbo::Transaction transaction{ *session };

		Wt::Dbo::collection<Wt::Dbo::ptr<tables::Shop>> query_result =
			(*session).query<Wt::Dbo::ptr<tables::Shop>>("select s from shop s")
			.leftJoin("stock st on s.id = st.shop_id")
			.leftJoin("book bk on bk.id = st.book_id")
			.leftJoin("publisher pb on bk.publisher_id = pb.id")
			.where("pb.name = ?").bind(publisher_name);

		
		std::vector<std::string> shops;

		for (const Wt::Dbo::ptr<tables::Shop>& shop : query_result) {
			shops.push_back(shop->name);
		}
		
		transaction.commit();
		
		return shops;
	
	}
}
