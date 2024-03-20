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

QPIDStatus QPIDMessaging::qpidSendMessage(std::string message, std::string exchange, std::string replyAddress) {
    QPIDSender qpidSender(mBroker);
    INFO_LOG("[ %s ] Sending message: %s, on exchange: %s, with reply address: %s!", __func__, message.c_str(), exchange.c_str(), replyAddress.c_str());
    return qpidSender.sendMessage(exchange, message, replyAddress);
}

QPIDStatus QPIDMessaging::qpidSendInvalidMessage(std::string exchange) {
    QPIDSender qpidSender(mBroker);
    INFO_LOG("[ %s ] Sending invalid message on exchange: %s!", __func__, exchange.c_str());
    return qpidSender.sendMessage(exchange, INVALID_MESSAGE);
}

QPIDStatus QPIDMessaging::qpidReceiveMessages(std::string exchange, qpidReceiveCallback callback) {
    QPIDStatus ret = QPIDStatus::QPID_ERROR;
    if (nullptr != callback) {
        if (activeReceivers.find(exchange) != activeReceivers.end()) {
            INFO_LOG("[ %s ] Already receiving on exchange: %s!", __func__, exchange.c_str());
            ret = QPIDStatus::QPID_ALREADY;
        } else {
            INFO_LOG("[ %s ] Starting receiver on exchange: %s!", __func__, exchange.c_str());
            std::shared_ptr<QPIDReceiver> qpidReceiver = std::make_shared<QPIDReceiver>(mBroker);
            ret = qpidReceiver->receiveMessages(exchange, callback);
            if (ret == QPIDStatus::QPID_SUCCESS) {
                activeReceivers.emplace(exchange, qpidReceiver);
            }
        }
    }

    return ret;
}

QPIDStatus QPIDMessaging::qpidReceiveStop(std::string exchange) {
    QPIDStatus ret = QPIDStatus::QPID_ERROR;
    if (activeReceivers.find(exchange) == activeReceivers.end()) {
        INFO_LOG("[ %s ] Receiver on exchange %s doesn't exist!", __func__, exchange.c_str());
        ret = QPIDStatus::QPID_INVALID_PARAM;
    } else {
        INFO_LOG("[ %s ] Stopping receiver on exchange: %s!", __func__, exchange.c_str());
        ret = activeReceivers[exchange]->receiveStop();
        activeReceivers.erase(exchange);
    }

    return ret;
}

}