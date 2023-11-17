#pragma once

#include <string>
#include <Wt/Dbo/Dbo.h>


namespace tables
{
	class Stock;

	class Sale
	{
	public:
		int price = 0;
		std::string date_sale = "";
		int count = 0;
		Wt::Dbo::ptr<Stock> stock;

		template<class Action>
		void persist(Action& a)
		{
			Wt::Dbo::field(a, price, "price");
			Wt::Dbo::field(a, date_sale, "date_sale");
			Wt::Dbo::belongsTo(a, stock, "stock");
			Wt::Dbo::field(a, count, "count");

		}
	};
}