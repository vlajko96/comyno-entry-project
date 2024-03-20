#ifndef _QPID_RECEIVER_H_
#define _QPID_RECEIVER_H_

#include <QPIDCommon.h>
#include <IQPIDBrokerConnector.h>

namespace qpidMessaging {

class QPIDReceiver {
    void waitForMessages();
    QPIDStatus receiveMessages(std::string exchange, qpidReceiveCallback callback);
    QPIDStatus receiveStop();
    friend class QPIDMessaging;

    qpid::messaging::Connection mConnection;
    qpid::messaging::Session mSession;
    qpid::messaging::Receiver mReceiver;
    std::string mExchange;
    std::thread mMessageReceiverThreadFunction;
    qpidReceiveCallback mReceiveCallback;
    bool mReceivingActive;
    static std::mutex mMutex;
    std::shared_ptr<IQPIDBrokerConnector> mBrokerConnector;

 public:
    QPIDReceiver(std::string broker, std::shared_ptr<IQPIDBrokerConnector> brokerConnector);
    virtual ~QPIDReceiver();
};

}

#endif  /* _QPID_RECEIVER_H_ */