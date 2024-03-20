#ifndef _PG_CLIENT_H_
#define _PG_CLIENT_H_

#include <iostream>
#include <string>
#include <unordered_map>

#include <PGCommon.h>

namespace pgClient {

/**
 * @class PGClient
 * @brief Handles PostgreSQL database operations.
 */
class PGClient {
 public:
    /**
     * @brief Constructor for PGClient.
     * @param database The database connection string. Defaults to "dbname=comynoDB user=comyno password=comyno hostaddr=127.0.0.1 port=5432".
     */
    PGClient(std::string database = "dbname=comynoDB user=comyno password=comyno hostaddr=127.0.0.1 port=5432");

    /**
     * @brief Destructor for PGClient.
     */
    virtual ~PGClient() = default;

    /**
     * @brief Persists a message in the database.
     * @param message The message to persist.
     * @param exchange The exchange associated with the message. Optional.
     * @return PGStatus indicating the result of the operation. Possible values:
     *         - PG_SUCCESS: Message successfully persisted.
     *         - PG_ERROR: Error occurred persisting the message.
     */
    PGStatus pgMessagePersist(std::string message, std::string exchange = "");

    /**
     * @brief Queries messages from the database.
     * @param callback The callback to handle queried messages.
     * @param exchange The exchange to query messages from. Optional.
     * @return PGStatus indicating the result of the operation. Possible values:
     *         - PG_SUCCESS: Messages successfully queried.
     *         - PG_ERROR: Error occurred querying the messages.
     */
    PGStatus pgMessageQuery(pgSelectCallback callback, std::string exchange = "");

 private:
    std::string mDatabase;
    static std::mutex mMutex;
    pgSelectCallback mSelectCallback;
    ThreadPool mPool;

    void queryExchange(int exchangeID, pgSelectCallback callback);
};

}

#endif  /* _PG_CLIENT_H_ */