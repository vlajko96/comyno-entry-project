#include <QPIDMessaging.h>

namespace qpidMessaging {

QPIDMessaging::QPIDMessaging(std::string broker) {
    mBroker = broker;
}

QPIDMessaging::~QPIDMessaging() {
    for (auto& receiver : activeReceivers) {
        receiver.second->receiveStop();
    }
    activeReceivers.clear();
}

QPIDStatus QPIDMessaging::qpidSendMessage(std::string topic, std::string message, std::string replyAddress) {
    QPIDSender qpidSender(mBroker);
    std::cout << "Sending message " << message << " on topic " << topic << std::endl;
    return qpidSender.sendMessage(topic, message, replyAddress);
}

QPIDStatus QPIDMessaging::qpidSendInvalidMessage(std::string topic) {
    QPIDSender qpidSender(mBroker);
    std::cout << "Sending invalid message on topic " << topic << std::endl;
    return qpidSender.sendMessage(topic, INVALID_MESSAGE);
}

QPIDStatus QPIDMessaging::qpidReceiveMessages(std::string topic, qpidReceiveCallback callback) {
    QPIDStatus ret = QPIDStatus::QPID_ERROR;
    if (nullptr != callback) {
        if (activeReceivers.find(topic) != activeReceivers.end()) {
            std::cout << "Already receiving on topic " << topic << std::endl;
            ret = QPIDStatus::QPID_ALREADY;
        } else {
            std::cout << "Starting receiver on topic " << topic << std::endl;
            std::shared_ptr<QPIDReceiver> qpidReceiver = std::make_shared<QPIDReceiver>(mBroker);
            ret = qpidReceiver->receiveMessages(topic, callback);
            if (ret == QPIDStatus::QPID_SUCCESS) {
                activeReceivers.emplace(topic, qpidReceiver);
            }
        }
    }

    return ret;
}

QPIDStatus QPIDMessaging::qpidReceiveStop(std::string topic) {
    QPIDStatus ret = QPIDStatus::QPID_ERROR;
    if (activeReceivers.find(topic) == activeReceivers.end()) {
        std::cout << "Receiver on topic " << topic << " doesn't exist" << std::endl;
        ret = QPIDStatus::QPID_ALREADY;
    } else {
        std::cout << "Stopping receiver on topic " << topic << std::endl;
        ret = activeReceivers[topic]->receiveStop();
        activeReceivers.erase(topic);
    }

    return ret;
}

}