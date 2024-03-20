#include <PGConnector.h>

namespace pgClient {

std::shared_ptr<pqxx::connection> PGConnector::openConnection(std::string database) {
    return std::make_shared<pqxx::connection>(database);
}

std::shared_ptr<pqxx::work> PGConnector::createTransaction(pqxx::connection& connection) {
    return std::make_shared<pqxx::work>(connection);
}

pqxx::result PGConnector::executeQuery(pqxx::work& transaction, std::string query) {
    return transaction.exec(query);
}

void PGConnector::prepareStatement(pqxx::connection& connection, std::string name, std::string query) {
    connection.prepare(name, query);
}

void PGConnector::executePrepared(pqxx::work& transaction, std::string name) {
    transaction.exec_prepared(name);
}

void PGConnector::commitTransaction(pqxx::work& transaction) {
    transaction.commit();
}

std::string PGConnector::quoteString(pqxx::work& transaction, std::string str) {
    return transaction.quote(str);
}

bool PGConnector::resultIsEmpty(pqxx::result& result) {
    return result.empty();
}

int PGConnector::resultGetFirstInt(pqxx::result& result) {
    return result[0][0].as<int>();
}

std::vector<Message> PGConnector::resultGetAll(pqxx::result& result) {
    std::vector<Message> messageArray;
    for (const auto& row : result) {
        int messageID = row[0].as<int>();
        std::string payload = row[1].as<std::string>();
        std::string timestamp = row[2].as<std::string>();
        messageArray.emplace_back(Message{messageID, payload, timestamp});
        DEBUG_LOG("[ %s ] Read message with ID: %d, payload: %s, timestamp: %s!", __func__, messageID, payload.c_str(), timestamp.c_str());
    }
    return messageArray;
}

}
