#include "customservice.h"
#include "commontypes.h"
//#include "rpchandler.h"

CustomService::CustomService(const std::string &token) : m_token(token)
{

}

std::shared_ptr<grpc::ClientContext> CustomService::makeContext()
{
    auto context = std::shared_ptr<grpc::ClientContext>(new grpc::ClientContext());
    std::string meta_value = "Bearer " + m_token;
    context->AddMetadata("authorization", meta_value);
    context->AddMetadata("x-app-name", APP_NAME);
    return context;
}

void CustomService::StartThread()
{
    if (!m_grpcThread)
        m_grpcThread = std::unique_ptr<std::thread>(new std::thread(&RpcHandler::handlingThread, &m_cq));
}




