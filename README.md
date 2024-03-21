# comyno-entry-project

## How to setup environment

1. `sudo apt install docker`
2. `sudo apt install docker-compose`
3. `newgrp docker` (if group doesn't already exist)
4. `sudo usermod -aG docker $USER`
5. `sudo chmod 777 /var/run/docker.sock` (this is optional, if you still have "couldn't connect to docker daemon" error)
6. `make build` (this will take 10-15 mins, so bring a coffee ;P)
7. `make start p=<absolute_path_to_project>/project-source`

Note: After you start the container, postgresql database and qpid broker will be started automatically

Note: If you need, you can have multiple shells attached to container, after you start container just use `make attach`

### Database

New postgresql user will be created (user: "comyno", password: "comyno") and new database will be created (database: "comynoDB"). You can connect to database using command `psql -U comyno -d comynoDB`. Also, new tables will be created:

```sql
CREATE TABLE Exchanges (
    ID SERIAL PRIMARY KEY,
    exchange_name VARCHAR(255) UNIQUE,
    exchange_type VARCHAR(255)
);

CREATE TABLE Messages (
    ID SERIAL PRIMARY KEY,
    payload VARCHAR(255),
    exchange_id INTEGER NULL,
    created_at TIMESTAMP WITH TIME ZONE DEFAULT NOW(),
    FOREIGN KEY (exchange_id) REFERENCES Exchanges(ID)
);
```

### Broker

2 queues and 2 topics will be added ("send-queue", "reply-queue", "send-topic", "reply-topic") to qpid broker so we can use them for testing (more can be added using qpid-config command). Also, these queues and topics will be added to "Exchanges" table in database.

    cmake: compiler settings
    log: custom logger
    pg-client: postgres client library and unit tests
    qpid-messaging: qpid client library and unit tests
    test: interactive tests

We have commands.sh script which has following targets:

    ./commands.sh build - builds postgres and qpid client libraries (libqpidmessagingcomyno.so and libpgclientcomyno.so) and puts them in ./output/lib folder, builds unit tests (test-qpid and test-pgclient) and puts them in ./output/bin folder, builds interactive test (test-cm) and puts him in ./output/bin folder
    ./commands.sh clean - cleans build and output folders
    ./commands.sh rebuild - cleans then builds
    ./commands.sh run_target test - run unit tests
    ./commands.sh run_target generate_coverage - generates code coverage report and puts it in ./output/coverage_report folder

## QPID library
```cpp
/**
* @brief Constructor for QPIDMessaging.
* 
* Initializes messaging with the specified broker.
* 
* @param broker The address of the QPID broker. Defaults to "localhost:5672".
*/
QPIDMessaging(std::string broker = "localhost:5672");

/**
* @brief Destructor for QPIDMessaging.
* 
* Cleans up resources used by the QPIDMessaging instance.
*/
virtual ~QPIDMessaging();

/**
* @brief Sends a message to the specified exchange with an optional reply address.
* 
* @param message The message to be sent.
* @param exchange The exchange to send the message to.
* @param replyAddress An optional reply address for the message. Defaults to an empty string.
* @return QPIDStatus indicating the result of the operation:
*         QPID_SUCCESS if the message is sent successfully.
*         QPID_ERROR on a failure to send the message.
*/
QPIDStatus qpidSendMessage(std::string message, std::string exchange, std::string replyAddress = "");

/**
* @brief Sends an invalid message to the specified exchange. Primarily for testing error handling.
* @param exchange The exchange to which the invalid message will be sent.
* @return QPIDStatus indicating the result of the operation:
*         QPID_SUCCESS if the message is sent successfully.
*         QPID_ERROR on a failure during the operation.
*/
QPIDStatus qpidSendInvalidMessage(std::string exchange);

/**
* @brief Starts receiving messages from the specified exchange.
* @param exchange The exchange from which messages will be received.
* @param callback The callback function to be called for each received message.
* @return QPIDStatus indicating the result of the operation:
*         QPID_SUCCESS if the receiver is successfully started.
*         QPID_ALREADY if a receiver for the exchange is already running.
*         QPID_ERROR on a failure to start receiving messages.
*/
QPIDStatus qpidReceiveMessages(std::string exchange, qpidReceiveCallback callback);

/**
* @brief Stops receiving messages from the specified exchange.
* @param exchange The exchange for which message receiving will be stopped.
* @return QPIDStatus indicating the result of the operation:
*         QPID_SUCCESS if the receiver is successfully stopped.
*         QPID_ERROR on a failure to stop receiving messages.
*         QPID_INVALID_PARAM if no receiver exists for the specified exchange.
*/
QPIDStatus qpidReceiveStop(std::string exchange);
```
## Postgres library
```cpp
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
*         PG_SUCCESS: Message successfully persisted.
*         PG_ERROR: Error occurred persisting the message.
*/
PGStatus pgMessagePersist(std::string message, std::string exchange = "");

/**
* @brief Queries messages from the database.
* @param callback The callback to handle queried messages.
* @param exchange The exchange to query messages from. Optional.
* @return PGStatus indicating the result of the operation. Possible values:
*         PG_SUCCESS: Messages successfully queried.
*         PG_ERROR: Error occurred querying the messages.
*/
PGStatus pgMessageQuery(pgSelectCallback callback, std::string exchange = "");
```
## Interactive tests
# qpid
```markdown
item_qpid_send_message - send selected message to selected exchange
item_qpid_send_message_with_reply_address - send selected message to selected exchange with attaching reply address on which you will expect to get reply
item_qpid_send_invalid_message - send invalid message to selected exchange
item_qpid_receive_messages - start listening on selected exchange
item_qpid_receive_messages_and_reply - start listening on selected exchange and answer with selected message
item_qpid_receive_messages_and_persist - start listening on selected exchange and persist received messages
item_qpid_receive_stop - stop receiving on selected exchange
item_qpid_send_message_8_times - send selected message 8 times (stress test)
item_qpid_receive_message_4_times - start receiving on all 4 existing exchanges (stress test)
item_qpid_receive_stop_4_times - stop receiving on all 4 existing exchanges (stress test)
```
# postgres
```markdown
item_pgclient_persist_message - persist message in database without exchange_id
item_pgclient_persist_message_on_exchange - persist message in database with exchange_id
item_pgclient_query_exchange - query messages associated with selected exchange
item_pgclient_read_all_messages - query all messages
item_pgclient_persist_message_4_times - persist selected message 4 times (stress test)
item_pgclient_read_all_messages_4_times - query all messages 4 times (stress test)
```
