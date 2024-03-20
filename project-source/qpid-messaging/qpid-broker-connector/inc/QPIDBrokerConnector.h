#ifndef _QPID_BROKER_CONNECTOR_H_
#define _QPID_BROKER_CONNECTOR_H_

#include <IQPIDBrokerConnector.h>

namespace qpidMessaging {

class QPIDBrokerConnector : public IQPIDBrokerConnector {
 public:
    qpid::messaging::Connection createConnection(std::string broker) override;
    void openConnection(qpid::messaging::Connection& connection) override;
    bool connectionIsOpen(qpid::messaging::Connection& connection) override;
    void closeConnection(qpid::messaging::Connection& connection) override;
    qpid::messaging::Session createSession(qpid::messaging::Connection& connection) override;
    void sessionAcknowledge(qpid::messaging::Session& session) override;
    qpid::messaging::Sender createSender(qpid::messaging::Session& session, std::string exchange) override;
    void senderSend(qpid::messaging::Sender& sender, qpid::messaging::Message& message) override;
    qpid::messaging::Receiver createReceiver(qpid::messaging::Session& session, std::string exchange) override;
    qpid::messaging::Message receiverFetch(qpid::messaging::Receiver& receiver) override;
};

}

#endif  /* _QPID_BROKER_CONNECTOR_H_ */