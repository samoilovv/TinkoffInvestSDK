#include "sandbox.grpc.pb.h"
#include "commontypes.h"

using namespace tinkoff::public_::invest::api::contract::v1;

ServiceReply::ServiceReply()
{

}

ServiceReply::ServiceReply(const std::shared_ptr<google::protobuf::Message>  protoMsg) : m_replyPtr(protoMsg)
{

}

const std::string ServiceReply::accountID(const int i)
{
    auto response = dynamic_cast<GetAccountsResponse *>(ptr().get());
    if (response && i < response->accounts_size())
    {
        return response->accounts(i).id();
    } else {
        return "";
    }
}

int ServiceReply::accountCount()
{
    auto response = dynamic_cast<GetAccountsResponse *>(ptr().get());
    if (response)
    {
        return response->accounts_size();
    } else {
        return 0;
    }
}

const std::shared_ptr<google::protobuf::Message> ServiceReply::ptr()
{
    return m_replyPtr;
}

