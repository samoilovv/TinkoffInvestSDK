#include "investapiclient.h"
#include "sandboxservice.h" 
#include "marketdataservice.h"
#include "marketdatastreamservice.h"
#include "usersservice.h"
#include "instrumentsservice.h"
#include "operationsservice.h"
#include "ordersservice.h"
#include "ordersstreamservice.h"
#include "stopordersservice.h"
#include "commontypes.h"

#ifdef _WIN32
#include <Windows.h>
#include <wincrypt.h>

std::string utf8Encode(const std::wstring& wstr)
{
	if (wstr.empty())
		return std::string();

	int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
	std::string strTo(sizeNeeded, 0);
	WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], sizeNeeded, NULL, NULL);
	return strTo;
}

grpc::SslCredentialsOptions getSslOptions()
{
	// Fetch root certificate as required on Windows (s. issue 25533).
	grpc::SslCredentialsOptions result;

	// Open root certificate store.
	HANDLE hRootCertStore = CertOpenSystemStoreW(NULL, L"ROOT");
	if (!hRootCertStore)
		return result;

	// Get all root certificates.
	PCCERT_CONTEXT pCert = NULL;
	while ((pCert = CertEnumCertificatesInStore(hRootCertStore, pCert)) != NULL)
	{
		// Append this certificate in PEM formatted data.
		DWORD size = 0;
		CryptBinaryToStringW(pCert->pbCertEncoded, pCert->cbCertEncoded, CRYPT_STRING_BASE64HEADER, NULL, &size);
		std::vector<WCHAR> pem(size);
		CryptBinaryToStringW(pCert->pbCertEncoded, pCert->cbCertEncoded, CRYPT_STRING_BASE64HEADER, pem.data(), &size);

		result.pem_root_certs += utf8Encode(pem.data());
	}
	CertCloseStore(hRootCertStore, 0);

	return result;
}
#endif

InvestApiClient::InvestApiClient(const std::string &host, const std::string &pass)
{
    auto channel = grpc::CreateChannel(host, grpc::SslCredentials(
#ifdef _WIN32
       getSslOptions()
#else
       grpc::SslCredentialsOptions()
#endif
    ));

    m_services["sandbox"] = std::make_shared<Sandbox>(channel, pass);
    m_services["users"] = std::make_shared<Users>(channel, pass);
    m_services["marketdata"] = std::make_shared<MarketData>(channel, pass);    
    m_services["instruments"] = std::make_shared<Instruments>(channel, pass);
    m_services["operations"] = std::make_shared<Operations>(channel, pass);
    m_services["orders"] = std::make_shared<Orders>(channel, pass);
    m_services["stoporders"] = std::make_shared<StopOrders>(channel, pass);
    m_services["marketdatastream"] = std::make_shared<MarketDataStream>(channel, pass);
    m_services["ordersstream"] = std::make_shared<OrdersStream>(channel, pass);
}

InvestApiClient::~InvestApiClient()
{

}

std::shared_ptr<CustomService> InvestApiClient::service(const std::string &serviceName)
{
    return m_services.count(serviceName) ? m_services[serviceName] : nullptr;
}

