#include <PGClient.h>

namespace pgClient {

std::mutex PGClient::mMutex;

PGClient::PGClient(std::string database) : mPool(5) {
    mDatabase = database;
    mPGConnector = std::make_shared<PGConnector>();
}

PGStatus PGClient::pgMessagePersist(std::string message, std::string exchange) {
    PGStatus ret = PGStatus::PG_SUCCESS;
    INFO_LOG("[ %s ] Persisting message: %s, on exchange: %s!", __func__, message.c_str(), exchange.c_str());

    try {
        std::shared_ptr<pqxx::connection> conn = mPGConnector->openConnection(mDatabase);
        std::shared_ptr<pqxx::work> txn = mPGConnector->createTransaction(*conn);

        // Execute the SQL queries
        if (exchange != "") {
            std::string query = "SELECT ID FROM Exchanges WHERE exchange_name = " + mPGConnector->quoteString(*txn, exchange);
            pqxx::result r = mPGConnector->executeQuery(*txn, query);

            if (mPGConnector->resultIsEmpty(r)) {
                ERROR_LOG("[ %s ] Invalid exchange: %s!", __func__, exchange.c_str());
                ret = PGStatus::PG_ERROR;
            } else {
                auto exchangeID = mPGConnector->resultGetFirstInt(r);
                query = "INSERT INTO Messages (payload, exchange_id) VALUES (" + mPGConnector->quoteString(*txn, message) + " , " + std::to_string(exchangeID) + ")";
                pqxx::result r = mPGConnector->executeQuery(*txn, query);
            }
        } else {
            std::string query = "INSERT INTO Messages (payload) VALUES (" + mPGConnector->quoteString(*txn, message) + ")";
            pqxx::result r = mPGConnector->executeQuery(*txn, query);
        }

        mPGConnector->commitTransaction(*txn);
    } catch (const std::exception &e) {
        WARNING_LOG("[ %s ] Exception: %s!", __func__, e.what());
        ret = PGStatus::PG_ERROR;
    }

    return ret;
}

void PGClient::queryExchange(int exchangeID, pgSelectCallback callback) {
    try {
        std::shared_ptr<pqxx::connection> conn = mPGConnector->openConnection(mDatabase);
        std::shared_ptr<pqxx::work> txn = mPGConnector->createTransaction(*conn);

        // Set the transaction isolation level
        mPGConnector->prepareStatement(*conn, "set_iso_level", "SET TRANSACTION ISOLATION LEVEL SERIALIZABLE");
        mPGConnector->executePrepared(*txn, "set_iso_level");

        std::string query = "";

        // Execute the SQL queries
        if (exchangeID != -1) {
            query = "SELECT ID, payload, created_at FROM Messages WHERE exchange_id = " + std::to_string(exchangeID);
        } else {
            query = "SELECT ID, payload, created_at FROM Messages";
        }
        pqxx::result r = mPGConnector->executeQuery(*txn, query);

        std::vector<Message> messageArray = mPGConnector->resultGetAll(r);

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
        std::shared_ptr<pqxx::connection> conn = mPGConnector->openConnection(mDatabase);
        std::shared_ptr<pqxx::work> txn = mPGConnector->createTransaction(*conn);

        // Execute the SQL queries
        if (exchange != "") {
            std::string query = "SELECT ID FROM Exchanges WHERE exchange_name = " + mPGConnector->quoteString(*txn, exchange);
            pqxx::result r = mPGConnector->executeQuery(*txn, query);

            if (mPGConnector->resultIsEmpty(r)) {
                ERROR_LOG("[ %s ] Invalid exchange: %s!", __func__, exchange.c_str());
                ret = PGStatus::PG_ERROR;
            } else {
                auto exchangeID = mPGConnector->resultGetFirstInt(r);
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

#ifdef GTEST
    void PGClient::injectPGConnector(std::shared_ptr<IPGConnector> pgConnector) {
        mPGConnector = pgConnector;
    }
#endif

}