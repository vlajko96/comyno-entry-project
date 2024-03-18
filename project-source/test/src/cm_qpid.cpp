#include "test_utils.hpp"
#include "test_cm.hpp"

qpidMessaging::QPIDMessaging qpidMessagingClient{};
std::unordered_map<std::string, std::string> topicReply;

void receiveCallback(std::string message, std::string topic, std::string replyAddress) {
	std::cout << "Message received: " << message << " on topic:" << topic << "with reply address: " << replyAddress << std::endl;
}

void receiveAndReplyCallback(std::string message, std::string topic, std::string replyAddress) {
	std::cout << "Message received: " << message << " on topic: " << topic << " with reply address: " << replyAddress << std::endl;
    if (replyAddress != "") {
        if (message == "Send me an invalid!") {
            std::cout << "Sending invalid message to topic: " << replyAddress << std::endl;
            qpidMessagingClient.qpidSendMessage(replyAddress, INVALID_MESSAGE);
        } else {
            std::cout << "Replying with message: " << topicReply[topic] << "to topic " << replyAddress << std::endl;
            qpidMessagingClient.qpidSendMessage(replyAddress, message);
        }
    } else {
        std::cout << "Reply address empty!" << std::endl;
    }
}

void item_qpid_send_message(void)
{
    std::cout << "qpid_send_message" << std::endl;
    std::string topic;
    std::string message;
    std::cout << "Enter topic: ";
    std::cin >> topic;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Enter message: ";
    std::getline(std::cin, message);

    if (qpidMessaging::QPIDStatus::QPID_SUCCESS == qpidMessagingClient.qpidSendMessage(topic, message)) {
        std::cout << "Message sent successfully!";
    } else {
        std::cout << "Failed to send the message!";
    }
}

void item_qpid_send_message_with_reply_address(void)
{
    std::cout << "qpid_send_message" << std::endl;
    std::string topic;
    std::string message;
    std::string replyAddress;
    std::cout << "Enter topic: ";
    std::cin >> topic;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Enter message: ";
    std::getline(std::cin, message);
    std::cout << "Enter reply address: ";
    std::cin >> replyAddress;

    if (qpidMessaging::QPIDStatus::QPID_SUCCESS == qpidMessagingClient.qpidSendMessage(topic, message, replyAddress)) {
        std::cout << "Message sent successfully!";
    } else {
        std::cout << "Failed to send the message!";
    }
}

void item_qpid_send_invalid_message(void)
{
    std::cout << "qpid_send_invalid_message" << std::endl;
    std::string topic;
    std::cout << "Enter topic: ";
    std::cin >> topic;

    if (qpidMessaging::QPIDStatus::QPID_SUCCESS == qpidMessagingClient.qpidSendInvalidMessage(topic)) {
        std::cout << "Message sent successfully!";
    } else {
        std::cout << "Failed to send the message!";
    }
}

void item_qpid_receive_messages(void)
{
    std::cout << "qpid_receive_messages" << std::endl;
    std::string topic;
    std::cout << "Enter topic: ";
    std::cin >> topic;

    if (qpidMessaging::QPIDStatus::QPID_SUCCESS == qpidMessagingClient.qpidReceiveMessages(topic, receiveCallback)) {
        std::cout << "Receiver run successfully!";
    } else {
        std::cout << "Failed to run receiver!";
    }
}

void item_qpid_receive_messages_and_reply(void)
{
    std::cout << "qpid_receive_messages" << std::endl;
    std::string topic;
    std::string replyMessage;
    std::cout << "Enter topic: ";
    std::cin >> topic;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Enter reply message: ";
    std::getline(std::cin, replyMessage);
    topicReply.emplace(topic, replyMessage);

    if (qpidMessaging::QPIDStatus::QPID_SUCCESS == qpidMessagingClient.qpidReceiveMessages(topic, receiveAndReplyCallback)) {
        std::cout << "Receiver run successfully!";
    } else {
        std::cout << "Failed to run receiver!";
    }
}

void item_qpid_receive_stop(void)
{
    std::cout << "qpid_receive_stop" << std::endl;
    std::string topic;
    std::cout << "Enter topic: ";
    std::cin >> topic;

    if (qpidMessaging::QPIDStatus::QPID_SUCCESS == qpidMessagingClient.qpidReceiveStop(topic)) {
        std::cout << "Receiver stopped successfully!";
        topicReply.erase(topic);
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
    {"item_qpid_receive_stop", item_qpid_receive_stop},
};

t_module_t cm_qpid_module =
{
    "qpid messaging client",
    T_ARRAY_SIZE(cm_qpid_items),
    cm_qpid_items
};
