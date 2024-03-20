#ifndef _IQPID_BROKER_CONNECTOR_H_
#define _IQPID_BROKER_CONNECTOR_H_

#include <QPIDCommon.h>

namespace qpidMessaging {

class IQPIDBrokerConnector {
 public:
    virtual ~IQPIDBrokerConnector() {};

    virtual qpid::messaging::Connection createConnection(std::string broker) = 0;
    virtual void openConnection(qpid::messaging::Connection& connection) = 0;
    virtual bool connectionIsOpen(qpid::messaging::Connection& connection) = 0;
    virtual void closeConnection(qpid::messaging::Connection& connection) = 0;
    virtual qpid::messaging::Session createSession(qpid::messaging::Connection& connection) = 0;
    virtual void sessionAcknowledge(qpid::messaging::Session& session) = 0;
    virtual qpid::messaging::Sender createSender(qpid::messaging::Session& session, std::string exchange) = 0;
    virtual void senderSend(qpid::messaging::Sender& sender, qpid::messaging::Message& message) = 0;
    virtual qpid::messaging::Receiver createReceiver(qpid::messaging::Session& session, std::string exchange) = 0;
    virtual qpid::messaging::Message receiverFetch(qpid::messaging::Receiver& receiver) = 0;
};

}

#endif  /* _IQPID_BROKER_CONNECTOR_H_ */