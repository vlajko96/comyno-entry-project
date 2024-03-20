#include "test_utils.hpp"
#include "test_cm.hpp"

static qpidMessaging::QPIDMessaging qpidMessagingClient{};
static pgClient::PGClient postgresClient{};
std::unordered_map<std::string, std::string> exchangeReply;

void receiveCallback(std::string message, std::string exchange, std::string replyAddress) {
	std::cout << "Message received: " << message << " on exchange:" << exchange << " with reply address: " << replyAddress << std::endl;
}

void receiveAndReplyCallback(std::string message, std::string exchange, std::string replyAddress) {
	std::cout << "Message received: " << message << " on exchange: " << exchange << " with reply address: " << replyAddress << std::endl;
    if (replyAddress != "") {
        if (message == "Send me an invalid!") {
            std::cout << "Sending invalid message to exchange: " << replyAddress << std::endl;
            qpidMessagingClient.qpidSendMessage(INVALID_MESSAGE, replyAddress);
        } else {
            std::cout << "Replying with message: " << exchangeReply[exchange] << " on exchange: " << replyAddress << std::endl;
            qpidMessagingClient.qpidSendMessage(message, replyAddress);
        }
    } else {
        std::cout << "Reply address empty!" << std::endl;
    }
}

void receiveAndPersistCallback(std::string message, std::string exchange, std::string replyAddress) {
	std::cout << "Message received: " << message << " on exchange: " << exchange << " with reply address: " << replyAddress << std::endl;
    if (pgClient::PGStatus::PG_SUCCESS == postgresClient.pgMessagePersist(message, exchange)) {
        std::cout << "Message persisted successfully!";
    } else {
        std::cout << "Failed to persist the message!";
    }
}

void item_qpid_send_message(void)
{
    std::cout << "qpid_send_message" << std::endl;
    std::string exchange;
    std::string message;
    std::cout << "Enter exchange: ";
    std::cin >> exchange;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Enter message: ";
    std::getline(std::cin, message);

    if (qpidMessaging::QPIDStatus::QPID_SUCCESS == qpidMessagingClient.qpidSendMessage(message, exchange)) {
        std::cout << "Message sent successfully!";
    } else {
        std::cout << "Failed to send the message!";
    }
}

void item_qpid_send_message_with_reply_address(void)
{
    std::cout << "qpid_send_message_with_reply_address" << std::endl;
    std::string exchange;
    std::string message;
    std::string replyAddress;
    std::cout << "Enter exchange: ";
    std::cin >> exchange;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Enter message: ";
    std::getline(std::cin, message);
    std::cout << "Enter reply address: ";
    std::cin >> replyAddress;

    if (qpidMessaging::QPIDStatus::QPID_SUCCESS == qpidMessagingClient.qpidSendMessage(message, exchange, replyAddress)) {
        std::cout << "Message sent successfully!";
    } else {
        std::cout << "Failed to send the message!";
    }
}

void item_qpid_send_invalid_message(void)
{
    std::cout << "qpid_send_invalid_message" << std::endl;
    std::string exchange;
    std::cout << "Enter exchange: ";
    std::cin >> exchange;

    if (qpidMessaging::QPIDStatus::QPID_SUCCESS == qpidMessagingClient.qpidSendInvalidMessage(exchange)) {
        std::cout << "Message sent successfully!";
    } else {
        std::cout << "Failed to send the message!";
    }
}

void item_qpid_receive_messages(void)
{
    std::cout << "qpid_receive_messages" << std::endl;
    std::string exchange;
    std::cout << "Enter exchange: ";
    std::cin >> exchange;

    if (qpidMessaging::QPIDStatus::QPID_SUCCESS == qpidMessagingClient.qpidReceiveMessages(exchange, receiveCallback)) {
        std::cout << "Receiver run successfully!";
    } else {
        std::cout << "Failed to run receiver!";
    }
}

void item_qpid_receive_messages_and_reply(void)
{
    std::cout << "qpid_receive_messages_and_reply" << std::endl;
    std::string exchange;
    std::string replyMessage;
    std::cout << "Enter exchange: ";
    std::cin >> exchange;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Enter reply message: ";
    std::getline(std::cin, replyMessage);
    exchangeReply.emplace(exchange, replyMessage);

    if (qpidMessaging::QPIDStatus::QPID_SUCCESS == qpidMessagingClient.qpidReceiveMessages(exchange, receiveAndReplyCallback)) {
        std::cout << "Receiver run successfully!";
    } else {
        std::cout << "Failed to run receiver!";
    }
}

void item_qpid_receive_messages_and_persist(void)
{
    std::cout << "qpid_receive_messages_and_persist" << std::endl;
    std::string exchange;
    std::cout << "Enter exchange: ";
    std::cin >> exchange;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (qpidMessaging::QPIDStatus::QPID_SUCCESS == qpidMessagingClient.qpidReceiveMessages(exchange, receiveAndPersistCallback)) {
        std::cout << "Receiver run successfully!";
    } else {
        std::cout << "Failed to run receiver!";
    }
}

void item_qpid_receive_stop(void)
{
    std::cout << "qpid_receive_stop" << std::endl;
    std::string exchange;
    std::cout << "Enter exchange: ";
    std::cin >> exchange;

    if (qpidMessaging::QPIDStatus::QPID_SUCCESS == qpidMessagingClient.qpidReceiveStop(exchange)) {
        std::cout << "Receiver stopped successfully!";
        exchangeReply.erase(exchange);
    } else {
        std::cout << "Failed to stop receiver!";
    }
}

void item_qpid_send_message_8_times(void)
{
    std::cout << "qpid_send_message_8_times" << std::endl;
    std::string message;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Enter message: ";
    std::getline(std::cin, message);

    if (qpidMessaging::QPIDStatus::QPID_SUCCESS == qpidMessagingClient.qpidSendMessage(message, "send-queue")) {
        std::cout << "Message sent successfully!";
    } else {
        std::cout << "Failed to send the message!";
    }
    if (qpidMessaging::QPIDStatus::QPID_SUCCESS == qpidMessagingClient.qpidSendMessage(message, "send-queue")) {
        std::cout << "Message sent successfully!";
    } else {
        std::cout << "Failed to send the message!";
    }
    if (qpidMessaging::QPIDStatus::QPID_SUCCESS == qpidMessagingClient.qpidSendMessage(message, "send-topic")) {
        std::cout << "Message sent successfully!";
    } else {
        std::cout << "Failed to send the message!";
    }
    if (qpidMessaging::QPIDStatus::QPID_SUCCESS == qpidMessagingClient.qpidSendMessage(message, "send-topic")) {
        std::cout << "Message sent successfully!";
    } else {
        std::cout << "Failed to send the message!";
    }
    if (qpidMessaging::QPIDStatus::QPID_SUCCESS == qpidMessagingClient.qpidSendMessage(message, "reply-queue")) {
        std::cout << "Message sent successfully!";
    } else {
        std::cout << "Failed to send the message!";
    }
    if (qpidMessaging::QPIDStatus::QPID_SUCCESS == qpidMessagingClient.qpidSendMessage(message, "reply-queue")) {
        std::cout << "Message sent successfully!";
    } else {
        std::cout << "Failed to send the message!";
    }
    if (qpidMessaging::QPIDStatus::QPID_SUCCESS == qpidMessagingClient.qpidSendMessage(message, "reply-topic")) {
        std::cout << "Message sent successfully!";
    } else {
        std::cout << "Failed to send the message!";
    }
    if (qpidMessaging::QPIDStatus::QPID_SUCCESS == qpidMessagingClient.qpidSendMessage(message, "reply-topic")) {
        std::cout << "Message sent successfully!";
    } else {
        std::cout << "Failed to send the message!";
    }
}

void item_qpid_receive_message_4_times(void)
{
    std::cout << "qpid_receive_message_4_times" << std::endl;

    if (qpidMessaging::QPIDStatus::QPID_SUCCESS == qpidMessagingClient.qpidReceiveMessages("send-queue", receiveCallback)) {
        std::cout << "Receiver run successfully!";
    } else {
        std::cout << "Failed to run receiver!";
    }
    if (qpidMessaging::QPIDStatus::QPID_SUCCESS == qpidMessagingClient.qpidReceiveMessages("send-topic", receiveCallback)) {
        std::cout << "Receiver run successfully!";
    } else {
        std::cout << "Failed to run receiver!";
    }
    if (qpidMessaging::QPIDStatus::QPID_SUCCESS == qpidMessagingClient.qpidReceiveMessages("reply-queue", receiveCallback)) {
        std::cout << "Receiver run successfully!";
    } else {
        std::cout << "Failed to run receiver!";
    }
    if (qpidMessaging::QPIDStatus::QPID_SUCCESS == qpidMessagingClient.qpidReceiveMessages("reply-topic", receiveCallback)) {
        std::cout << "Receiver run successfully!";
    } else {
        std::cout << "Failed to run receiver!";
    }
}

void item_qpid_receive_stop_4_times(void)
{
    std::cout << "qpid_receive_stop_4_times" << std::endl;

    if (qpidMessaging::QPIDStatus::QPID_SUCCESS == qpidMessagingClient.qpidReceiveStop("send-queue")) {
        std::cout << "Receiver stopped successfully!";
        exchangeReply.erase("send-queue");
    } else {
        std::cout << "Failed to stop receiver!";
    }
    if (qpidMessaging::QPIDStatus::QPID_SUCCESS == qpidMessagingClient.qpidReceiveStop("send-topic")) {
        std::cout << "Receiver stopped successfully!";
        exchangeReply.erase("send-topic");
    } else {
        std::cout << "Failed to stop receiver!";
    }
    if (qpidMessaging::QPIDStatus::QPID_SUCCESS == qpidMessagingClient.qpidReceiveStop("reply-queue")) {
        std::cout << "Receiver stopped successfully!";
        exchangeReply.erase("reply-queue");
    } else {
        std::cout << "Failed to stop receiver!";
    }
    if (qpidMessaging::QPIDStatus::QPID_SUCCESS == qpidMessagingClient.qpidReceiveStop("reply-topic")) {
        std::cout << "Receiver stopped successfully!";
        exchangeReply.erase("reply-topic");
    } else {
        std::cout << "Failed to stop receiver!";
    }
}

t_item_t cm_qpid_items[] =
{
    {"item_qpid_send_message", item_qpid_send_message},
    {"item_qpid_send_message_with_reply_address", item_qpid_send_message_with_reply_address},
    {"item_qpid_send_invalid_message", item_qpid_send_invalid_message},
    {"item_qpid_receive_messages", item_qpid_receive_messages},
    {"item_qpid_receive_messages_and_reply", item_qpid_receive_messages_and_reply},
    {"item_qpid_receive_messages_and_persist", item_qpid_receive_messages_and_persist},
    {"item_qpid_receive_stop", item_qpid_receive_stop},
    {"item_qpid_send_message_8_times", item_qpid_send_message_8_times},
    {"item_qpid_receive_message_4_times", item_qpid_receive_message_4_times},
    {"item_qpid_receive_stop_4_times", item_qpid_receive_stop_4_times},
};

t_module_t cm_qpid_module =
{
    "qpid messaging client",
    T_ARRAY_SIZE(cm_qpid_items),
    cm_qpid_items
};
