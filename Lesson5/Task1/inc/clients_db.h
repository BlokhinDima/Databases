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
		ClientsDB(const ClientsDB&) = delete;
		~ClientsDB() { conn->close(); };
		ClientsDB& operator=(const ClientsDB&) = delete;
		void addClient(const std::string& name, const std::string& surname, const std::string& email);
		void deleteClient(const std::string& id);
		pqxx::result findClient(const ClientInfoFields field, const std::string& search_value) const;
		void addPhoneNumber(const std::string& client_id, const std::string& phone_number);
		void updateClientInfo(const std::string& id, ClientInfoFields filed, const std::string& new_value);
		void deletePhoneNumber(const std::string& phone_number);
		pqxx::result getClientsInfo() const;

	private:
		std::string dbname = "Clients";
		std::string host = "localhost";
		std::string port = "5432";
		std::unique_ptr<pqxx::connection> conn;

		void createTables();
		void createClientsInfoTable();
		void createClientsPhonesTable();
		bool ClientsDB::isClientIDExist(const std::string& id) const;
		bool isEmailExist(const std::string& email) const;
		bool isEmailFormatCorrect(const std::string& email) const;
		void isEmailValid(const std::string& email) const;
		bool isPhoneNumberExist(const std::string& phone_number) const;
		bool isPhoneNumberFormatCorrect(const std::string& phone_number) const;
		void isPhoneNumberValid(const std::string& phone_number) const;
	};
}
