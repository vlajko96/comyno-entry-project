#include <QPIDSender.h>

namespace qpidMessaging {

QPIDSender::QPIDSender(std::string broker) {
    mConnection = qpid::messaging::Connection(broker);
}

QPIDStatus QPIDSender::sendMessage(std::string topic, std::string message, std::string replyAddress) {
    QPIDStatus ret = QPIDStatus::QPID_SUCCESS;

    try {
        mConnection.open();
        mSession = mConnection.createSession();
        mSender = mSession.createSender(topic);

        qpid::messaging::Message request;
        request.setContent(message);
        mReplyAddress = qpid::messaging::Address(replyAddress);
        request.setReplyTo(mReplyAddress);
        std::cout << "Sending message: " << request.getContent() << std::endl;
        mSender.send(request);

        mConnection.close();
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        mConnection.close();
        ret = QPIDStatus::QPID_ERROR;
    }

    return ret;
}

}