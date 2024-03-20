#include "test_utils.hpp"
#include "test_cm.hpp"

static pgClient::PGClient postgresClient{};

void queryCallback(std::vector<pgClient::Message> messages) {
    if (messages.empty()) {
        std::cout << "No messages received!" << std::endl;
    } else {
        for (const auto& message : messages) {
            std::cout << "Read message with ID: " << message.id << ", payload: " << message.payload << " and timestamp: " << message.timestamp << std::endl;
        }
    }
	
}

void item_pgclient_persist_message(void)
{
    std::cout << "pgclient_persist_message" << std::endl;
    std::string message;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Enter message: ";
    std::getline(std::cin, message);

    if (pgClient::PGStatus::PG_SUCCESS == postgresClient.pgMessagePersist(message)) {
        std::cout << "Message persisted successfully!";
    } else {
        std::cout << "Failed to persist the message!";
    }
}

void item_pgclient_persist_message_on_exchange(void)
{
    std::cout << "pgclient_persist_message_on_exchange" << std::endl;
    std::string exchange;
    std::string message;
    std::cout << "Enter exchange: ";
    std::cin >> exchange;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Enter message: ";
    std::getline(std::cin, message);

    if (pgClient::PGStatus::PG_SUCCESS == postgresClient.pgMessagePersist(message, exchange)) {
        std::cout << "Message persisted successfully!";
    } else {
        std::cout << "Failed to persist the message!";
    }
}

void item_pgclient_query_exchange(void)
{
    std::cout << "pgclient_query_exchange" << std::endl;
    std::string exchange;
    std::cout << "Enter exchange: ";
    std::cin >> exchange;

    if (pgClient::PGStatus::PG_SUCCESS == postgresClient.pgMessageQuery(queryCallback, exchange)) {
        std::cout << "Exchange queried successfully!";
    } else {
        std::cout << "Failed to query the Exchange!";
    }
}

void item_pgclient_read_all_messages(void)
{
    std::cout << "pgclient_read_all_messages" << std::endl;

    if (pgClient::PGStatus::PG_SUCCESS == postgresClient.pgMessageQuery(queryCallback)) {
        std::cout << "Messages read successfully!";
    } else {
        std::cout << "Failed to read messages!";
    }
}

void item_pgclient_persist_message_4_times(void)
{
    std::cout << "pgclient_persist_message_4_times" << std::endl;
    std::string message;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Enter message: ";
    std::getline(std::cin, message);

    if (pgClient::PGStatus::PG_SUCCESS == postgresClient.pgMessagePersist(message)) {
        std::cout << "Message persisted successfully!";
    } else {
        std::cout << "Failed to persist the message!";
    }
    if (pgClient::PGStatus::PG_SUCCESS == postgresClient.pgMessagePersist(message)) {
        std::cout << "Message persisted successfully!";
    } else {
        std::cout << "Failed to persist the message!";
    }
    if (pgClient::PGStatus::PG_SUCCESS == postgresClient.pgMessagePersist(message)) {
        std::cout << "Message persisted successfully!";
    } else {
        std::cout << "Failed to persist the message!";
    }
    if (pgClient::PGStatus::PG_SUCCESS == postgresClient.pgMessagePersist(message)) {
        std::cout << "Message persisted successfully!";
    } else {
        std::cout << "Failed to persist the message!";
    }
}

void item_pgclient_read_all_messages_4_times(void)
{
    std::cout << "pgclient_read_all_messages_4_times" << std::endl;

    if (pgClient::PGStatus::PG_SUCCESS == postgresClient.pgMessageQuery(queryCallback)) {
        std::cout << "Messages read successfully!";
    } else {
        std::cout << "Failed to read messages!";
    }
    if (pgClient::PGStatus::PG_SUCCESS == postgresClient.pgMessageQuery(queryCallback)) {
        std::cout << "Messages read successfully!";
    } else {
        std::cout << "Failed to read messages!";
    }
    if (pgClient::PGStatus::PG_SUCCESS == postgresClient.pgMessageQuery(queryCallback)) {
        std::cout << "Messages read successfully!";
    } else {
        std::cout << "Failed to read messages!";
    }
    if (pgClient::PGStatus::PG_SUCCESS == postgresClient.pgMessageQuery(queryCallback)) {
        std::cout << "Messages read successfully!";
    } else {
        std::cout << "Failed to read messages!";
    }
}

t_item_t cm_pgclient_items[] =
{
    {"item_pgclient_persist_message", item_pgclient_persist_message},
    {"item_pgclient_persist_message_on_exchange", item_pgclient_persist_message_on_exchange},
    {"item_pgclient_query_exchange", item_pgclient_query_exchange},
    {"item_pgclient_read_all_messages", item_pgclient_read_all_messages},
    {"item_pgclient_persist_message_4_times", item_pgclient_persist_message_4_times},
    {"item_pgclient_read_all_messages_4_times", item_pgclient_read_all_messages_4_times},
};

t_module_t cm_pgclient_module =
{
    "postgres client",
    T_ARRAY_SIZE(cm_pgclient_items),
    cm_pgclient_items
};
