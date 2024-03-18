#ifndef _QPID_RECEIVER_H_
#define _QPID_RECEIVER_H_

#include <QPIDCommon.h>

namespace qpidMessaging {

class QPIDReceiver {
    void waitForMessages();
    QPIDStatus receiveMessages(std::string topic, qpidReceiveCallback callback);
    QPIDStatus receiveStop();
    friend class QPIDMessaging;

    qpid::messaging::Connection mConnection;
    qpid::messaging::Session mSession;
    qpid::messaging::Receiver mReceiver;
    qpid::messaging::Address mReplyAddress;
    std::string mTopic;
    std::thread mMessageReceiverThreadFunction;
    qpidReceiveCallback mReceiveCallback;
    bool mReceivingActive;
    static std::mutex mMutex;

 public:
    QPIDReceiver(std::string broker);
    virtual ~QPIDReceiver();
};

}

#endif  /* _QPID_RECEIVER_H_ */