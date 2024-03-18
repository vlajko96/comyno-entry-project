#include <QPIDReceiver.h>

namespace qpidMessaging {

std::mutex QPIDReceiver::mMutex;

QPIDReceiver::QPIDReceiver(std::string broker) {
    mConnection = qpid::messaging::Connection(broker);
    mReceivingActive = false;
}

QPIDReceiver::~QPIDReceiver() {
    if (mReceivingActive) {
        mReceivingActive = false;
        mConnection.close();
    }
    if (mMessageReceiverThreadFunction.joinable())
        mMessageReceiverThreadFunction.join();
}

void QPIDReceiver::waitForMessages() {
    try {
        while (mReceivingActive) { // Loop to keep listening for messages
            qpid::messaging::Message message = mReceiver.fetch();
            mMutex.lock();
            std::cout << "Received message: " << message.getContent() << std::endl;
            mReceiveCallback(message.getContent(), mTopic, message.getReplyTo().getName());
            mMutex.unlock();
            mSession.acknowledge();
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        if (mConnection.isOpen())
            mConnection.close();
    }
}


QPIDStatus QPIDReceiver::receiveMessages(std::string topic, qpidReceiveCallback callback) {
    QPIDStatus ret = QPIDStatus::QPID_SUCCESS;

    try {
        mConnection.open();
        mSession = mConnection.createSession();
        mTopic = topic;
        mReceiver = mSession.createReceiver(mTopic);

        mReceiveCallback = callback;
        mReceivingActive = true;
        mMessageReceiverThreadFunction = std::thread(&QPIDReceiver::waitForMessages, this);
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        mConnection.close();
        ret = QPIDStatus::QPID_ERROR;
    }

    return ret;
}

QPIDStatus QPIDReceiver::receiveStop() {
    QPIDStatus ret = QPIDStatus::QPID_SUCCESS;

    try {
        if (mReceivingActive) {
            mReceivingActive = false;
            mConnection.close();
            if (mMessageReceiverThreadFunction.joinable())
                mMessageReceiverThreadFunction.join();
        } else {
            ret = QPIDStatus::QPID_ALREADY;
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        ret = QPIDStatus::QPID_ERROR;
    }

    return ret;
}

}