#include <QPIDReceiver.h>

namespace qpidMessaging {

std::mutex QPIDReceiver::mMutex;

QPIDReceiver::QPIDReceiver(std::string broker, std::shared_ptr<IQPIDBrokerConnector> brokerConnector) : mBrokerConnector{brokerConnector} {
    mConnection = mBrokerConnector->createConnection(broker);
    mReceivingActive = false;
}

QPIDReceiver::~QPIDReceiver() {
    if (mReceivingActive) {
        mReceivingActive = false;
        mBrokerConnector->closeConnection(mConnection);
    }
    if (mMessageReceiverThreadFunction.joinable())
        mMessageReceiverThreadFunction.join();
}

void QPIDReceiver::waitForMessages() {
    try {
        while (mReceivingActive) { // Loop to keep listening for messages
            qpid::messaging::Message message = mBrokerConnector->receiverFetch(mReceiver);
            mMutex.lock();
            DEBUG_LOG("[ %s ] Received message: %s!", __func__, message.getContent().c_str());
            mReceiveCallback(message.getContent(), mExchange, message.getReplyTo().getName());
            mMutex.unlock();
            mBrokerConnector->sessionAcknowledge(mSession);
        }
    } catch (const std::exception& e) {
        WARNING_LOG("[ %s ] Exception: %s!", __func__, e.what());
        if (mBrokerConnector->connectionIsOpen(mConnection))
            mBrokerConnector->closeConnection(mConnection);
    }
}


QPIDStatus QPIDReceiver::receiveMessages(std::string exchange, qpidReceiveCallback callback) {
    QPIDStatus ret = QPIDStatus::QPID_SUCCESS;

    try {
        mBrokerConnector->openConnection(mConnection);
        mSession = mBrokerConnector->createSession(mConnection);
        mExchange = exchange;
        mReceiver = mBrokerConnector->createReceiver(mSession, mExchange);

        mReceiveCallback = callback;
        mReceivingActive = true;
        DEBUG_LOG("[ %s ] Starting receiver thread on exchange: %s!", __func__, exchange.c_str());
        mMessageReceiverThreadFunction = std::thread(&QPIDReceiver::waitForMessages, this);
    } catch (const std::exception& e) {
        WARNING_LOG("[ %s ] Exception: %s!", __func__, e.what());
        mBrokerConnector->closeConnection(mConnection);
        ret = QPIDStatus::QPID_ERROR;
    }

    return ret;
}

QPIDStatus QPIDReceiver::receiveStop() {
    QPIDStatus ret = QPIDStatus::QPID_SUCCESS;

    try {
        if (mReceivingActive) {
            mReceivingActive = false;
            mBrokerConnector->closeConnection(mConnection);
            if (mMessageReceiverThreadFunction.joinable())
                mMessageReceiverThreadFunction.join();
        } else {
            ret = QPIDStatus::QPID_INVALID_PARAM;
        }
    } catch (const std::exception& e) {
        WARNING_LOG("[ %s ] Exception: %s!", __func__, e.what());
        ret = QPIDStatus::QPID_ERROR;
    }

    return ret;
}

}