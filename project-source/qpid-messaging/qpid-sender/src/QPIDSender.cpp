#include <QPIDSender.h>

namespace qpidMessaging {

QPIDSender::QPIDSender(std::string broker) {
    mConnection = qpid::messaging::Connection(broker);
}

QPIDStatus QPIDSender::sendMessage(std::string exchange, std::string message, std::string replyAddress) {
    QPIDStatus ret = QPIDStatus::QPID_SUCCESS;

    try {
        mConnection.open();
        mSession = mConnection.createSession();
        mSender = mSession.createSender(exchange);

        qpid::messaging::Message payload;
        payload.setContent(message);
        mReplyAddress = qpid::messaging::Address(replyAddress);
        payload.setReplyTo(mReplyAddress);
        DEBUG_LOG("[ %s ] Sending message: %s!", __func__, payload.getContent().c_str());
        mSender.send(payload);

        mConnection.close();
    } catch (const std::exception& e) {
        WARNING_LOG("[ %s ] Exception: %s!", __func__, e.what());
        mConnection.close();
        ret = QPIDStatus::QPID_ERROR;
    }

    return ret;
}

}