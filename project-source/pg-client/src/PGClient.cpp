#include <PGClient.h>

namespace pgClient {

std::mutex PGClient::mMutex;

PGClient::PGClient(std::string database) : mPool(5) {
    mDatabase = database;
}

PGStatus PGClient::pgMessagePersist(std::string message, std::string exchange) {
    PGStatus ret = PGStatus::PG_SUCCESS;
    INFO_LOG("[ %s ] Persisting message: %s, on exchange: %s!", __func__, message.c_str(), exchange.c_str());

    try {
        pqxx::connection conn(mDatabase);
        pqxx::work txn(conn);

        // Execute the SQL queries
        if (exchange != "") {
            std::string query = "SELECT ID FROM Exchanges WHERE exchange_name = " + txn.quote(exchange);
            pqxx::result r = txn.exec(query);

            if (r.empty()) {
                ERROR_LOG("[ %s ] Invalid exchange: %s!", __func__, exchange.c_str());
                ret = PGStatus::PG_ERROR;
            } else {
                auto exchangeID = r[0][0].as<int>();
                query = "INSERT INTO Messages (payload, exchange_id) VALUES (" + txn.quote(message) + " , " + txn.quote(exchangeID) + ")";
                pqxx::result r = txn.exec(query);
            }
        } else {
            std::string query = "INSERT INTO Messages (payload) VALUES (" + txn.quote(message) + ")";
            pqxx::result r = txn.exec(query);
        }

        txn.commit();
    } catch (const std::exception &e) {
        WARNING_LOG("[ %s ] Exception: %s!", __func__, e.what());
        ret = PGStatus::PG_ERROR;
    }

    return ret;
}

void PGClient::queryExchange(int exchangeID, pgSelectCallback callback) {
    try {
        pqxx::connection conn(mDatabase);
        pqxx::work txn(conn);

        // Set the transaction isolation level
        conn.prepare("set_iso_level", "SET TRANSACTION ISOLATION LEVEL SERIALIZABLE");
        txn.exec_prepared("set_iso_level");

        std::string query = "";

        // Execute the SQL queries
        if (exchangeID != -1) {
            query = "SELECT ID, payload, created_at FROM Messages WHERE exchange_id = " + txn.quote(exchangeID);
        } else {
            query = "SELECT ID, payload, created_at FROM Messages";
        }
        pqxx::result r = txn.exec(query);

        std::vector<Message> messageArray;
        for (const auto& row : r) {
            int messageID = row[0].as<int>();
            std::string payload = row[1].as<std::string>();
            std::string timestamp = row[2].as<std::string>();
            messageArray.emplace_back(Message{messageID, payload, timestamp});
            DEBUG_LOG("[ %s ] Read message with ID: %d, payload: %s, timestamp: %s!", __func__, messageID, payload.c_str(), timestamp.c_str());
        }
        mMutex.lock();
        callback(messageArray);
        mMutex.unlock();
    } catch (const std::exception &e) {
        WARNING_LOG("[ %s ] Exception: %s!", __func__, e.what());
    }
}

PGStatus PGClient::pgMessageQuery(pgSelectCallback callback, std::string exchange) {
    PGStatus ret = PGStatus::PG_SUCCESS;
    INFO_LOG("[ %s ] Querying messages on exchange: %s!", __func__, exchange.c_str());

    try {
        pqxx::connection conn(mDatabase);
        pqxx::work txn(conn);

        // Execute the SQL queries
        if (exchange != "") {
            std::string query = "SELECT ID FROM Exchanges WHERE exchange_name = " + txn.quote(exchange);
            pqxx::result r = txn.exec(query);

            if (r.empty()) {
                ERROR_LOG("[ %s ] Invalid exchange: %s!", __func__, exchange.c_str());
                ret = PGStatus::PG_ERROR;
            } else {
                auto exchangeID = r[0][0].as<int>();
                mPool.enqueue(std::bind(&PGClient::queryExchange, this, exchangeID, callback));
            }
        } else {
            mPool.enqueue(std::bind(&PGClient::queryExchange, this, -1, callback));
        }
    } catch (const std::exception &e) {
        WARNING_LOG("[ %s ] Exception: %s!", __func__, e.what());
        ret = PGStatus::PG_ERROR;
    }

    return ret;
}

}