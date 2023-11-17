#pragma once

#include <string>
#include <Wt/Dbo/Dbo.h>


namespace tables
{
	class Book;
	class Shop;
	class Sale;

	class Stock
	{
	public:
		int count = 0;
		Wt::Dbo::ptr<Book> book;
		Wt::Dbo::ptr<Shop> shop;
		Wt::Dbo::collection<Wt::Dbo::ptr<Sale>> sales;

		template<class Action>
		void persist(Action& a)
		{
			Wt::Dbo::belongsTo(a, book, "book");
			Wt::Dbo::belongsTo(a, shop, "shop");
			Wt::Dbo::field(a, count, "count");
			Wt::Dbo::hasMany(a, sales, Wt::Dbo::ManyToOne, "stock");
		}
	};
}
