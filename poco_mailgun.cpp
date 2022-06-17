#include <iostream>

#include <Poco/Poco.h>
#include <Poco/Net/Net.h>
#include <Poco/SharedPtr.h>
#include <Poco/Net/InvalidCertificateHandler.h>
#include <Poco/Net/NetSSL.h>
#include <Poco/Net/AcceptCertificateHandler.h>
#include <Poco/Net/Context.h>
#include <Poco/Net/SSLManager.h>
#include <Poco/Net/SecureStreamSocket.h>
#include <Poco/Net/SecureSMTPClientSession.h>
#include <Poco/Net/ConsoleCertificateHandler.h>
#include <Poco/Net/MailMessage.h>
#include <Poco/Net/SMTPClientSession.h>
#include <Poco/Net/StringPartSource.h>
#include <Poco/Net/NetException.h>

int main()
{
    //Poco::SharedPtr<Poco::Net::InvalidCertificateHandler> pCert = new Poco::Net::ConsoleCertificateHandler(false); // ask the user via console
    //Poco::Net::Context::Ptr pContext = new Poco::Net::Context(Poco::Net::Context::CLIENT_USE, "", "", "", Poco::Net::Context::VERIFY_RELAXED, 9, true, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
    //Poco::Net::SSLManager::instance().initializeClient(0, pCert, pContext);

    //Poco::Net::MailMessage message;
    //message.setSender(sender);
    //message.addRecipient(MailRecipient(MailRecipient::PRIMARY_RECIPIENT, recipient));
    //message.setSubject("Hello from the POCO C++ Libraries");
    //std::string content;
    //content += "Hello ";
    //content += recipient;
    //content += ",\r\n\r\n";
    //content += "This is a greeting from the POCO C++ Libraries.\r\n\r\n";
    //std::string logo(reinterpret_cast<const char*>(PocoLogo), sizeof(PocoLogo));
    //message.addContent(new StringPartSource(content));
    //message.addAttachment("logo", new StringPartSource(logo, "image/gif"));

    //SecureSMTPClientSession session(mailhost);
    //session.login();
    //session.startTLS(pContext);
    //if (!username.empty())
    //{
    //	session.login(SMTPClientSession::AUTH_LOGIN, username, password);
    //}
    //session.sendMessage(message);
    //session.close();

    Poco::Net::MailMessage message;
    message.setSender("admin@mmocli.net");
    message.addRecipient(Poco::Net::MailRecipient(Poco::Net::MailRecipient::PRIMARY_RECIPIENT, "git.greenislet@gmail.com"));
    message.setSubject("Hello from the POCO C++ Libraries");
    std::string content;
    content += "Hello";

    message.addContent(new Poco::Net::StringPartSource(content));


    Poco::Net::SecureSMTPClientSession smtp("smtp.eu.mailgun.org", 587);

    smtp.login(Poco::Net::SMTPClientSession::AUTH_LOGIN, "postmaster@mail.mmocli.net", "");

    smtp.startTLS();

    smtp.login();

    try {
        smtp.sendMessage(message);
        smtp.close();
    }
    catch (Poco::Net::SMTPException& e) {
        std::cerr << e.displayText().c_str() << std::endl;
    }
    catch (Poco::Net::NetException& e) {
        std::cerr << e.displayText().c_str() << std::endl;
    }

}
