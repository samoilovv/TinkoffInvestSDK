#ifndef SERVICEREPLY_H
#define SERVICEREPLY_H

#include <QObject>
#include <QMetaType>
#include "google/protobuf/message.h"
#include <grpcpp/grpcpp.h>
#include "tinkoffinvestsdk_export.h"

using grpc::Status;

class TINKOFFINVESTSDK_EXPORT ServiceReply
{

public:
    ServiceReply();
    ServiceReply(const std::shared_ptr<google::protobuf::Message> protoMsg);
    const std::shared_ptr<google::protobuf::Message> replyPtr();

    template<class T>
    static const ServiceReply prepareServiceAnswer(const Status &status, const T &protoMsg)
    {
        return (status.ok()) ? ServiceReply(std::make_shared<T>(protoMsg)) : ServiceReply(nullptr);
    }

private:
    std::shared_ptr<google::protobuf::Message> m_replyPtr;

};

Q_DECLARE_METATYPE(ServiceReply);

#endif // SERVICEREPLY_H
