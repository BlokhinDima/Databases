#include <string>
#include <iostream>
#include <utility>
#include <regex>
#include <exception>
#include <pqxx/pqxx>

#include "clients_db.h"

namespace databases
{
	ClientsDB::ClientsDB(const std::string& user, const std::string& password)
	{
		try
		{
			conn = std::make_unique<pqxx::connection>(
				"dbname=" + dbname +
				" user=" + user +
				" password=" + password +
				" host=" + host +
				" port=" + port);

			createTables();
		}
		catch (const std::exception& e)
		{
			throw e;
		}
	}


	void ClientsDB::createTables()
	{
		try
		{
			createClientsInfoTable();
			createClientsPhonesTable();
		}
		catch (const std::exception& e)
		{
			throw e;
		}
	}


	void ClientsDB::createClientsInfoTable()
	{
		try
		{
			std::string query =
				"CREATE TABLE IF NOT EXISTS clients_info"
				"(id SERIAL primary key, "
				"name VARCHAR(60) not null, "
				"surname VARCHAR(60) not null, "
				"email VARCHAR(60) );";

			pqxx::transaction tx{ *conn };
			tx.exec(query);
			tx.commit();
		}
		catch (const std::exception& e)
		{
			throw e;
		}
	}


	void ClientsDB::createClientsPhonesTable()
	{
		try
		{
			std::string query =
				"CREATE TABLE IF NOT EXISTS clients_phones"
				"(client_id integer references clients_info(id) ON DELETE CASCADE, "
				"phone_number VARCHAR(60) not null, "
				"constraint clients_phones_pk primary key (client_id, phone_number) );";

			pqxx::transaction tx{ *conn };
			tx.exec(query);
			tx.commit();
		}
		catch (const std::exception& e)
		{
			throw e;
		}
	}


	void ClientsDB::addClient(
		const std::string& name, 
		const std::string& surname, 
		const std::string& email)
	{
		try
		{
			isEmailValid(email);

			pqxx::transaction tx{ *conn };

			std::string query =
				"INSERT INTO clients_info (id, name, surname, email)"
				"VALUES (DEFAULT, '" + tx.esc(name) + "', '" + tx.esc(surname) + "', '" + tx.esc(email) + 
				"') RETURNING id; ";

			pqxx::result result = tx.exec(query);
			tx.commit();
		}
		catch (const std::exception& e)
		{
			throw e;
		}
	}


	void ClientsDB::deleteClient(const std::string& id)
	{
		try
		{
			pqxx::transaction tx{ *conn };

			std::string query =
				"DELETE FROM clients_info "
				"WHERE id = " + tx.esc(id) + ";";

			tx.exec(query);
			tx.commit();
		}
		catch (const std::exception& e)
		{
			throw e;
		}
	}


	pqxx::result ClientsDB::findClient(
		const ClientInfoFields field, 
		const std::string& search_value) const
	{
		try
		{
			pqxx::transaction tx{ *conn };
			std::string query = 
				"SELECT id, name, surname, email, phone_number FROM clients_info ci "
				"LEFT JOIN clients_phones cp ON cp.client_id = ci.id "
				"WHERE ";

			switch (field)
			{
			case ClientInfoFields::NAME:
				query += "name='";
				break;

			case ClientInfoFields::SURNAME:
				query += "surname='";
				break;

			case ClientInfoFields::EMAIL:
				query += "email='";
				break;

			case ClientInfoFields::PHONE_NUMBER:
				query += "phone_number='";
				break;

			default:
				throw std::invalid_argument("Unknown table field!");
			}

			query += search_value + "';";
			return tx.exec(query);
		}
		catch (const std::exception& e)
		{
			throw e;
		}
	}


	void ClientsDB::updateClientInfo(
		const std::string& id, 
		ClientInfoFields field,
		const std::string& new_value)
	{
		try
		{
			if (isClientIDExist(id))
			{
				pqxx::transaction tx{ *conn };
				std::string query = "UPDATE clients_info SET ";

				switch (field)
				{
				case ClientInfoFields::NAME:
					query += "name='";
					break;

				case ClientInfoFields::SURNAME:
					query += "surname='";
					break;

				case ClientInfoFields::EMAIL:
					query += "email='";
					break;

				default:
					throw std::invalid_argument("Invalid field!");
				}

				query += new_value + "' "
					"WHERE id = " + tx.esc(id) + ";";

				tx.exec(query);
				tx.commit();
			}
			else
			{
				throw std::invalid_argument("Client with ID = " + id + " is not exist!");
			}
		}
		catch (const std::exception& e)
		{
			throw e;
		}
	}


	bool ClientsDB::isClientIDExist(const std::string& id) const
	{
		try
		{
			pqxx::transaction tx{ *conn };

			std::string query =
				"SELECT COUNT(*) FROM clients_info "
				"WHERE id = '" + tx.esc(id) + "';";

			auto count = tx.query_value<int>(query);

			std::cout << count;
			if (count > 0)
				return true;

			return false;
		}
		catch (const std::exception& e)
		{
			throw e;
		}
	}


	void ClientsDB::addPhoneNumber(
		const std::string& client_id, 
		const std::string& phone_number)
	{
		try
		{
			isPhoneNumberValid(phone_number);
			
			pqxx::transaction tx{ *conn };

			std::string query =
				"INSERT INTO clients_phones (client_id, phone_number)"
				"VALUES ('" + tx.esc(client_id) + "', '" + tx.esc(phone_number) + "'); ";

			pqxx::result result = tx.exec(query);
			tx.commit();
		}
		catch (const std::exception& e)
		{
			throw e;
		}
	}


	void ClientsDB::deletePhoneNumber(const std::string& phone_number)
	{
		try
		{
			if (isPhoneNumberFormatCorrect(phone_number) && isPhoneNumberExist(phone_number))
			{
				pqxx::transaction tx{ *conn };

				std::string query =
					"DELETE FROM clients_phones "
					"WHERE phone_number = '" + tx.esc(phone_number) + "';";

				tx.exec(query);
				tx.commit();
			}
		}
		catch (const std::exception& e)
		{
			throw e;
		}
	}


	void ClientsDB::isEmailValid(const std::string& email) const
	{
		if (!isEmailFormatCorrect(email))
			throw std::invalid_argument(
				"Wrong email format!: " + email + " Requred: _____@_____.com");

		if (isEmailExist(email))
			throw std::invalid_argument("Email already exist!: " + email);
	}


	bool ClientsDB::isEmailExist(const std::string& email) const
	{
		try
		{
			pqxx::transaction tx{ *conn };

			std::string query =
				"SELECT COUNT(*) FROM clients_info "
				"WHERE email = '" + tx.esc(email) + "';";

			auto count = tx.query_value<int>(query);

			if (count > 0)
				return true;

			return false;
		}
		catch (const std::exception& e)
		{
			throw e;
		}
	}


	bool ClientsDB::isEmailFormatCorrect(const std::string& email) const
	{
		const std::regex pattern("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
		return std::regex_match(email, pattern);
	}


	void ClientsDB::isPhoneNumberValid(const std::string& phone_number) const
	{
		if (!isPhoneNumberFormatCorrect(phone_number))
			throw std::invalid_argument(
				"Wrong phone number format!: " + phone_number + " Requred: +XXXXXXXXXXX");

		if (isPhoneNumberExist(phone_number))
			throw std::invalid_argument("Phone number already exist!: " + phone_number);
	}


	bool ClientsDB::isPhoneNumberExist(const std::string& phone_number) const
	{
		try
		{
			pqxx::transaction tx{ *conn };

			std::string query =
				"SELECT COUNT(*) FROM clients_phones "
				"WHERE phone_number = '" + tx.esc(phone_number) + "';";

			auto count = tx.query_value<int>(query);

			std::cout << count;
			if (count > 0)
				return true;

			return false;
		}
		catch (const std::exception& e)
		{
			throw e;
		}
	}


	bool ClientsDB::isPhoneNumberFormatCorrect(const std::string& phone_number) const
	{
		const std::regex pattern("[+][0-9]{11}");
		return std::regex_match(phone_number, pattern);
	}

	
	pqxx::result ClientsDB::getClientsInfo() const
	{
		try
		{
			pqxx::transaction tx{ *conn };
			std::string query = 
				"SELECT id, name, surname, email, phone_number FROM clients_info ci "
				"LEFT JOIN clients_phones cp ON cp.client_id = ci.id "
				"GROUP BY id, cp.phone_number "
				"ORDER BY id;";
			return tx.exec(query);
		}
		catch (const std::exception& e)
		{
			throw e;
		}
	}
}
