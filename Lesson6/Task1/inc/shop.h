#pragma once

#include <string>
#include <Wt/Dbo/Dbo.h>


namespace tables
{
	class Stock;

	class Shop
	{
	public:
		std::string name = "";
		Wt::Dbo::collection<Wt::Dbo::ptr<Stock>> stocks;

		template<class Action>
		void persist(Action& a)
		{
			Wt::Dbo::field(a, name, "name");
			Wt::Dbo::hasMany(a, stocks, Wt::Dbo::ManyToOne, "shop");
		}
	};
}
