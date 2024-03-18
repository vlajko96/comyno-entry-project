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
    INFO_LOG("[ %s ] Sending message: %s, on topic: %s, with reply address: %s!", __func__, message.c_str(), topic.c_str(), replyAddress.c_str());
    return qpidSender.sendMessage(topic, message, replyAddress);
}

QPIDStatus QPIDMessaging::qpidSendInvalidMessage(std::string topic) {
    QPIDSender qpidSender(mBroker);
    INFO_LOG("[ %s ] Sending invalid message on topic: %s!", __func__, topic.c_str());
    return qpidSender.sendMessage(topic, INVALID_MESSAGE);
}

QPIDStatus QPIDMessaging::qpidReceiveMessages(std::string topic, qpidReceiveCallback callback) {
    QPIDStatus ret = QPIDStatus::QPID_ERROR;
    if (nullptr != callback) {
        if (activeReceivers.find(topic) != activeReceivers.end()) {
            INFO_LOG("[ %s ] Already receiving on topic: %s!", __func__, topic.c_str());
            ret = QPIDStatus::QPID_ALREADY;
        } else {
            INFO_LOG("[ %s ] Starting receiver on topic: %s!", __func__, topic.c_str());
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
        INFO_LOG("[ %s ] Receiver on topic %s doesn't exist!", __func__, topic.c_str());
        ret = QPIDStatus::QPID_ALREADY;
    } else {
        INFO_LOG("[ %s ] Stopping receiver on topic: %s!", __func__, topic.c_str());
        ret = activeReceivers[topic]->receiveStop();
        activeReceivers.erase(topic);
    }

    return ret;
}

}