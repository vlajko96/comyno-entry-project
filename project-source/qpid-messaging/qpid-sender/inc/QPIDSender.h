#ifndef _QPID_SENDER_H_
#define _QPID_SENDER_H_

#include <QPIDCommon.h>
#include <IQPIDBrokerConnector.h>

namespace qpidMessaging {

class QPIDSender{
    QPIDStatus sendMessage(std::string exchange, std::string message, std::string replyAddress = "");
    friend class QPIDMessaging;

    qpid::messaging::Connection mConnection;
    qpid::messaging::Session mSession;
    qpid::messaging::Sender mSender;
    qpid::messaging::Address mReplyAddress;
    std::shared_ptr<IQPIDBrokerConnector> mBrokerConnector;

 public:
    QPIDSender(std::string broker, std::shared_ptr<IQPIDBrokerConnector> brokerConnector);
    virtual ~QPIDSender() = default;
};

}

#endif  /* _QPID_SENDER_H_ */