#include <QPIDBrokerConnector.h>

namespace qpidMessaging {

qpid::messaging::Connection QPIDBrokerConnector::createConnection(std::string broker) {
    return qpid::messaging::Connection(broker);
}

void QPIDBrokerConnector::openConnection(qpid::messaging::Connection& connection) {
    connection.open();
}

bool QPIDBrokerConnector::connectionIsOpen(qpid::messaging::Connection& connection) {
    return connection.isOpen();
}

void QPIDBrokerConnector::closeConnection(qpid::messaging::Connection& connection) {
    connection.close();
}

qpid::messaging::Session QPIDBrokerConnector::createSession(qpid::messaging::Connection& connection) {
    return connection.createSession();
}

void QPIDBrokerConnector::sessionAcknowledge(qpid::messaging::Session& session) {
    session.acknowledge();
}

qpid::messaging::Sender QPIDBrokerConnector::createSender(qpid::messaging::Session& session, std::string exchange) {
    return session.createSender(exchange);
}

void QPIDBrokerConnector::senderSend(qpid::messaging::Sender& sender, qpid::messaging::Message& message) {
    sender.send(message);
}

qpid::messaging::Receiver QPIDBrokerConnector::createReceiver(qpid::messaging::Session& session, std::string exchange) {
    return session.createReceiver(exchange);
}

qpid::messaging::Message QPIDBrokerConnector::receiverFetch(qpid::messaging::Receiver& receiver) {
    return receiver.fetch();
}

}
