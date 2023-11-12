#pragma once

#include <string>
#include <utility>
#include <pqxx/pqxx>

namespace databases
{
	class ClientsDB
	{
	public:

		enum class ClientInfoFields
		{
			NAME,
			SURNAME,
			EMAIL,
			PHONE_NUMBER,
		};

		ClientsDB(const std::string& user, const std::string& password);
		~ClientsDB() { conn->close(); };
		void addClient(const std::string& name, const std::string& surname, const std::string& email);
		void deleteClient(const std::string& id);
		pqxx::result findClient(const ClientInfoFields field, const std::string& search_value);
		void addPhoneNumber(const std::string& client_id, const std::string& phone_number);
		void updateClientInfo(const std::string& id, ClientInfoFields filed, const std::string& new_value);
		void deletePhoneNumber(const std::string& phone_number);
		pqxx::result getClientsInfo();

	private:
		std::string dbname = "Clients";
		std::string host = "localhost";
		std::string port = "5432";
		std::unique_ptr<pqxx::connection> conn;

		void createTables();
		void createClientsInfoTable();
		void createClientsPhonesTable();
		bool ClientsDB::isClientIDExist(const std::string& id);
		bool isEmailExist(const std::string& email);
		bool isEmailFormatCorrect(const std::string& email);
		void isEmailValid(const std::string& email);
		bool isPhoneNumberExist(const std::string& phone_number);
		bool isPhoneNumberFormatCorrect(const std::string& phone_number);
		void isPhoneNumberValid(const std::string& phone_number);
	};
}
