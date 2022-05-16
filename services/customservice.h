#ifndef CUSTOMTINKOFFSERVICE_H
#define CUSTOMTINKOFFSERVICE_H

#include <grpcpp/grpcpp.h>
#include "google/protobuf/message.h"
#include "commontypes.h"

using grpc::ClientContext;
using grpc::Status;

/*!
    \brief  Родительский класс для всех сервисов
*/
class CustomService
{

public:
    CustomService(const std::string &token);
    virtual ~CustomService() = default;

protected:
    const std::string m_token;
    std::shared_ptr<ClientContext> makeContext();


};

#endif // CUSTOMTINKOFFSERVICE_H
