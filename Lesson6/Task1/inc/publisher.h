#pragma once

#include <string>
#include <Wt/Dbo/Dbo.h>


namespace tables
{
	class Book;

	class Publisher
	{
	public:
		std::string name = "";
		Wt::Dbo::collection<Wt::Dbo::ptr<Book>> books;

		template<class Action>
		void persist(Action& a)
		{
			Wt::Dbo::field(a, name, "name");
			Wt::Dbo::hasMany(a, books, Wt::Dbo::ManyToOne, "publisher");
		}
	};
}