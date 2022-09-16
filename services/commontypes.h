#ifndef COMMONTYPES_H
#define COMMONTYPES_H

#include <grpcpp/grpcpp.h>
#include <functional>
#include "google/protobuf/message.h"
#include "tinkoffinvestsdk_export.h"

using grpc::Status;

static const std::string APP_NAME = "samoilovv.TinkoffInvestSDK";

/*!
    \brief Класс-обертка над proto-ответами сервисов

    Данный класс используется при передачи информации от сервисов клиентам
*/
class TINKOFFINVESTSDK_EXPORT ServiceReply
{

public:
    ServiceReply();
    ServiceReply(const std::shared_ptr<google::protobuf::Message> protoMsg, const Status& status, const std::string& messageIfError = "");
    const std::shared_ptr<google::protobuf::Message> ptr();
    const std::string accountID(const int i);
    const std::string accountName(const int i);
    int accountCount();
    const Status& GetStatus() const;
	const std::string& GetErrorMessage() const;

	template<class T>
    static const ServiceReply prepareServiceAnswer(const Status &status, const T &protoMsg, const std::string& messageIfError = "")
    {
        return (status.ok()) ? ServiceReply(std::make_shared<T>(protoMsg), status) : ServiceReply(nullptr, status, messageIfError);
    }

private:
    std::shared_ptr<google::protobuf::Message> m_replyPtr;
    Status m_status;
	std::string m_errorMessage;
};

using CallbackFunc = std::function<void (ServiceReply)>;

using Strings = std::vector<std::string>;

#endif // COMMONTYPES_H
