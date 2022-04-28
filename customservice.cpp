#include "customservice.h"
//#include "sandbox.grpc.pb.h"

CustomService::CustomService(const QString &token) : m_token(token)
{

}

//template<class T>
//const ServiceReply CustomService::prepareServiceAnswer(const Status &status, const google::protobuf::Message &protoMsg)
//{
//    if (status.ok())
//    {
//        auto res = std::make_shared<T>(protoMsg);
//        ServiceReply result(res);
//        return result;
//    } else {
//        ServiceReply result(nullptr);
//        return result;
//    }
//}

QSharedPointer<grpc::ClientContext> CustomService::makeContext()
{
    auto context = QSharedPointer<grpc::ClientContext>::create();
    QString meta_value = "Bearer " + m_token;
    context.get()->AddMetadata("authorization", meta_value.toStdString());
    context.get()->AddMetadata("x-app-name", "samoilovv.TinkoffInvestSDK");
    return context;
}
