#include <QPIDSender.h>

namespace qpidMessaging {

QPIDSender::QPIDSender(std::string broker, std::shared_ptr<IQPIDBrokerConnector> brokerConnector) : mBrokerConnector{brokerConnector} {
    mConnection = mBrokerConnector->createConnection(broker);
}

QPIDStatus QPIDSender::sendMessage(std::string exchange, std::string message, std::string replyAddress) {
    QPIDStatus ret = QPIDStatus::QPID_SUCCESS;

    try {
        mBrokerConnector->openConnection(mConnection);
        mSession = mBrokerConnector->createSession(mConnection);
        mSender = mBrokerConnector->createSender(mSession, exchange);

        qpid::messaging::Message payload;
        payload.setContent(message);
        mReplyAddress = qpid::messaging::Address(replyAddress);
        payload.setReplyTo(mReplyAddress);
        DEBUG_LOG("[ %s ] Sending message: %s!", __func__, payload.getContent().c_str());
        mBrokerConnector->senderSend(mSender, payload);

        mBrokerConnector->closeConnection(mConnection);
    } catch (const std::exception& e) {
        WARNING_LOG("[ %s ] Exception: %s!", __func__, e.what());
        mBrokerConnector->closeConnection(mConnection);
        ret = QPIDStatus::QPID_ERROR;
    }

    return ret;
}

}