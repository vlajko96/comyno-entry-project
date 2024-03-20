#ifndef _QPID_SENDER_H_
#define _QPID_SENDER_H_

#include <QPIDCommon.h>

namespace qpidMessaging {

class QPIDSender{
    QPIDStatus sendMessage(std::string exchange, std::string message, std::string replyAddress = "");
    friend class QPIDMessaging;

    qpid::messaging::Connection mConnection;
    qpid::messaging::Session mSession;
    qpid::messaging::Sender mSender;
    qpid::messaging::Address mReplyAddress;

 public:
    QPIDSender(std::string broker);
    virtual ~QPIDSender() = default;
};

}

#endif  /* _QPID_SENDER_H_ */