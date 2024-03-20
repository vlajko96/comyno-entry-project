#ifndef _QPID_MESSAGING_H_
#define _QPID_MESSAGING_H_

#include <QPIDCommon.h>
#include <QPIDReceiver.h>
#include <QPIDSender.h>
#include <QPIDBrokerConnector.h>

namespace qpidMessaging {

/**
 * @class QPIDMessaging
 * @brief Handles messaging operations using QPID.
 */
class QPIDMessaging {
 public:

    /**
    * @brief Constructor for QPIDMessaging.
    * 
    * Initializes messaging with the specified broker.
    * 
    * @param broker The address of the QPID broker. Defaults to "localhost:5672".
    */
    QPIDMessaging(std::string broker = "localhost:5672");

    /**
     * @brief Destructor for QPIDMessaging.
     * 
     * Cleans up resources used by the QPIDMessaging instance.
     */
    virtual ~QPIDMessaging();

    /**
     * @brief Sends a message to the specified exchange with an optional reply address.
     * 
     * @param message The message to be sent.
     * @param exchange The exchange to send the message to.
     * @param replyAddress An optional reply address for the message. Defaults to an empty string.
     * @return QPIDStatus indicating the result of the operation:
     *         - QPID_SUCCESS if the message is sent successfully.
     *         - QPID_ERROR on a failure to send the message.
     */
    QPIDStatus qpidSendMessage(std::string message, std::string exchange, std::string replyAddress = "");

    /**
    * @brief Sends an invalid message to the specified exchange. Primarily for testing error handling.
    * @param exchange The exchange to which the invalid message will be sent.
    * @return QPIDStatus indicating the result of the operation:
    *         - QPID_SUCCESS if the message is sent successfully.
    *         - QPID_ERROR on a failure during the operation.
    */
    QPIDStatus qpidSendInvalidMessage(std::string exchange);

    /**
    * @brief Starts receiving messages from the specified exchange.
    * @param exchange The exchange from which messages will be received.
    * @param callback The callback function to be called for each received message.
    * @return QPIDStatus indicating the result of the operation:
    *         - QPID_SUCCESS if the receiver is successfully started.
    *         - QPID_ALREADY if a receiver for the exchange is already running.
    *         - QPID_ERROR on a failure to start receiving messages.
    */
    QPIDStatus qpidReceiveMessages(std::string exchange, qpidReceiveCallback callback);

    /**
    * @brief Stops receiving messages from the specified exchange.
    * @param exchange The exchange for which message receiving will be stopped.
    * @return QPIDStatus indicating the result of the operation:
    *         - QPID_SUCCESS if the receiver is successfully stopped.
    *         - QPID_ERROR on a failure to stop receiving messages.
    *         - QPID_INVALID_PARAM if no receiver exists for the specified exchange.
    */
    QPIDStatus qpidReceiveStop(std::string exchange);


#ifdef GTEST
    void injectBrokerConnector(std::shared_ptr<IQPIDBrokerConnector> brokerConnector);
#endif

 private:
    std::string mBroker;
    std::unordered_map<std::string, std::shared_ptr<QPIDReceiver>> activeReceivers;
    std::shared_ptr<IQPIDBrokerConnector> mBrokerConnector;
};

}

#endif  /* _QPID_MESSAGING_H_ */