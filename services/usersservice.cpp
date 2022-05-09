#include "usersservice.h"

Users::Users(std::shared_ptr<grpc::Channel> channel, const std::string &token) :
    CustomService(token),
    m_usersService(UsersService::NewStub(channel))
{

}

Users::~Users()
{

}

ServiceReply Users::GetAccounts()
{
    GetAccountsRequest request;
    GetAccountsResponse reply;
    Status status = m_usersService->GetAccounts(makeContext().get(), request, &reply);
    return ServiceReply::prepareServiceAnswer<GetAccountsResponse>(status, reply);
}

ServiceReply Users::GetMarginAttributes(const std::string &accountId)
{
    GetMarginAttributesRequest request;
    request.set_account_id(accountId);
    GetMarginAttributesResponse reply;
    Status status = m_usersService->GetMarginAttributes(makeContext().get(), request, &reply);
    return ServiceReply::prepareServiceAnswer<GetMarginAttributesResponse>(status, reply);
}

ServiceReply Users::GetUserTariff()
{
    GetUserTariffRequest request;
    GetUserTariffResponse reply;
    Status status = m_usersService->GetUserTariff(makeContext().get(), request, &reply);
    return ServiceReply::prepareServiceAnswer<GetUserTariffResponse>(status, reply);
}

ServiceReply Users::GetInfo()
{
    GetInfoRequest request;
    GetInfoResponse reply;
    Status status = m_usersService->GetInfo(makeContext().get(), request, &reply);
    return ServiceReply::prepareServiceAnswer<GetInfoResponse>(status, reply);
}
