#ifndef _PG_CONNECTOR_H_
#define _PG_CONNECTOR_H_

#include <IPGConnector.h>

namespace pgClient {

class PGConnector : public IPGConnector {
 public:
    std::shared_ptr<pqxx::connection> openConnection(std::string database) override;
    std::shared_ptr<pqxx::work> createTransaction(pqxx::connection& connection) override;
    pqxx::result executeQuery(pqxx::work& transaction, std::string query) override;
    void prepareStatement(pqxx::connection& connection, std::string name, std::string query) override;
    void executePrepared(pqxx::work& transaction, std::string name) override;
    void commitTransaction(pqxx::work& transaction) override;
    std::string quoteString(pqxx::work& transaction, std::string str) override;
    bool resultIsEmpty(pqxx::result& result) override;
    int resultGetFirstInt(pqxx::result& result) override;
    std::vector<Message> resultGetAll(pqxx::result& result) override;
};

}

#endif  /* _PG_CONNECTOR_H_ */