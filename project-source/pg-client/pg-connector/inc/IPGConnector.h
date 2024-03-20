#ifndef _IPG_CONNECTOR_H_
#define _IPG_CONNECTOR_H_

#include <PGCommon.h>

namespace pgClient {

class IPGConnector {
 public:
    virtual ~IPGConnector() {};

    virtual std::shared_ptr<pqxx::connection> openConnection(std::string database) = 0;
    virtual std::shared_ptr<pqxx::work> createTransaction(pqxx::connection& connection) = 0;
    virtual pqxx::result executeQuery(pqxx::work& transaction, std::string query) = 0;
    virtual void prepareStatement(pqxx::connection& connection, std::string name, std::string query) = 0;
    virtual void executePrepared(pqxx::work& transaction, std::string name) = 0;
    virtual void commitTransaction(pqxx::work& transaction) = 0;
    virtual std::string quoteString(pqxx::work& transaction, std::string str) = 0;
    virtual bool resultIsEmpty(pqxx::result& result) = 0;
    virtual int resultGetFirstInt(pqxx::result& result) = 0;
    virtual std::vector<Message> resultGetAll(pqxx::result& result) = 0;
};

}

#endif  /* _IPG_CONNECTOR_H_ */