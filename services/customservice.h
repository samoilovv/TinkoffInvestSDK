#ifndef CUSTOMTINKOFFSERVICE_H
#define CUSTOMTINKOFFSERVICE_H

#include <grpcpp/grpcpp.h>
#include <thread>
#include "google/protobuf/message.h"
#include "commontypes.h"
#include "rpchandler.h"

using grpc::ClientContext;
using grpc::Status;
using grpc::CompletionQueue;

/*!
    \brief  Родительский класс для всех сервисов
*/
class CustomService
{

public:
    /** @brief  Конструктор класса
        @param token Токен авторизации в сервисе Тинькофф Инвестиции
    */
    CustomService(const std::string &token);
    virtual ~CustomService() = default;

protected:
    const std::string m_token;
    CompletionQueue m_cq;
    std::unique_ptr<std::thread> m_grpcThread;
    std::shared_ptr<ClientContext> makeContext();
    void StartThread();

};

#endif // CUSTOMTINKOFFSERVICE_H
