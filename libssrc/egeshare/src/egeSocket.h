/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   egeSocket.h
 * Author: giovanni maffioletti - mymailaddress is development at ege3d.it
 *
 * Created on 10 marzo 2018, 13.38
 */

#ifndef EGESOCKET_H
#define EGESOCKET_H

#include <ctime>
#include <iostream>
#include <string>
#include <cstdlib>
#include <queue>
#include <vector>

//#include <boost/bind.hpp>
//#include <boost/asio/io_context.hpp>
//#include <boost/asio.hpp>
//#include <boost/asio/ip/tcp.hpp>
//#include <boost/shared_ptr.hpp>
//#include <boost/enable_shared_from_this.hpp>
//#include <boost/asio.hpp>

#include <EGE3dBaseTypes.h>

#ifdef EGE_EMSCRIPTEN
    #include <emscripten.h>
#endif

// Elenco comandi da EGE2D GUI a server (generalmente impostazione valore variabili OPC)
// OUTPUT Commands
#define EGE2D_GUIOP_BROWSE_TAGS         400             // Richiede elenco tags e relativi UID
#define EGE2D_GUIOP_SET_HMI_PAGE        500             // Notifica server per cambio pagina     
#define EGE2D_GUIOP_SET_VARIABLE        1000            // Comanda impostazione di una variabile 
#define EGE2D_GUIOP_ALM_ACK_VARIABLE    2000            // Notifica riconoscimento singolo allarme
#define EGE2D_GUIOP_ALM_RST_VARIABLE    2001            // Notifica reset singolo allarme

// Comandi da Server a GUI
// INPUT Commands
#define EGE2D_SRVOP_CTRL_VISIBILITY     100             // Imposta visibilità di un controllo (Hide/Show)
#define EGE2D_SRVOP_CTRL_VALUE          200             // Imposta valore update (textboxCaption/imgIndex/bargraphValue)
#define EGE2D_SRVOP_CTRL_FORECOLOR      300             // Imposta colore primario di un controllo
#define EGE2D_SRVOP_CTRL_BACKCOLOR      310             // Imposta colore secondario di un controllo
#define EGE2D_SRVOP_CTRL_EFFECTCOLOR1   320             // Imposta colore effetto di un controllo
#define EGE2D_SRVOP_CTRL_EFFECTCOLOR2   321
#define EGE2D_SRVOP_CTRL_EFFECTCOLOR3   322
#define EGE2D_SRVOP_CTRL_EFFECTCOLOR4   323
#define EGE2D_SRVOP_CTRL_EFFECTSIZE1    330             // Imposta dimensione effetto di un controllo
#define EGE2D_SRVOP_CTRL_EFFECTSIZE2    331
#define EGE2D_SRVOP_CTRL_EFFECTSIZE3    332
#define EGE2D_SRVOP_CTRL_EFFECTSIZE4    333

#define EGE_GUI_MAX_MESSAGE_QUEUE_SIZE  256             // Numero massimo messaggi in lista di processo

#ifdef __cplusplus
extern "C" {
#endif
    // API librarywebsocket
    
    // Controllo comunicazione da Browser a server 
    extern void wsOpen(char[]);             // Attiva comunicazione verso server
    extern void wsClose();                  // Chiude comunicazione a server

    // Comandi a server da GUI
    extern void wsSendCommandInt(EGEshort opcode, EGEint varIndex, EGEint valueInt);        // ok ma non usato
    extern void wsSendString(char*);                                                        // invio stringa di comando
    
    // Eventi da browser a GUI
    void onComm(int status);              // evento di stato comm. (API in browser) : 0==Closed; 1==Opened; others==errors;
    void onMessageAsString(char* str);    // evento di nuovo dato in arrivo da server (API in browser)

#ifdef __cplusplus
}
#endif

// In/Out sono visti dalla prospettiva EGE2D GUI
// Concettualmente egeCommand può contenere qualunque tipo di messaggio.
// La formattazione byte by byte dipende dal messaggio.
struct egeCommand{
    union{
        struct{
            EGEushort opcode;       // 16 bit
            EGEushort tagIndex;     // Indice variabile in OPCServer (Se zero allora NULL)
            EGEint    value;        // 32 bit value    
        }out_value;                 // DATI VERSO SERVER (un singolo valore di tipo intero)
        struct{
            EGEushort opcode;       // 16 bit
            EGEushort controlIndex; // Indice variabile in OPCServer (Se zero allora NULL)
            EGEint    value;        // 32 bit value    
        }in_value;                  // DATI DA SERVER (un singolo valore di tipo intero)
        struct{
            char buffer[32];
        };
    };
};

// Coda messaggi da server
extern std::queue<std::string> sharedEgeGuiFromSrvMsgQueue;
extern egeCommand sharedLastServerCommand;

// Coda messaggi a server
extern std::queue<std::string> sharedEgeGuiToSrvMsgQueue;

//using boost::asio::ip::tcp;
//using namespace boost;

//void write_to_socket(asio::ip::tcp::socket& sk, const std::string & msg ) {
//  std::size_t total_write {0};  // bytes successfully witten
//  std::size_t sz {msg.size()};
//  
//  // write_some returns the number of bytes sucessfully written
//  while (total_write != sz ) {
//    total_write += sk.write_some(asio::buffer(msg.c_str() + total_write, sz - total_write));
//  }
//}

// Implementa funzioni di I/O via socket TCP/UDP/ICMP
class egeSocketSrv {
public:
    egeSocketSrv();
    virtual ~egeSocketSrv();
    
//    void run(void){
//        boost::asio::io_service ioservice;
//        boost::asio::ip::tcp::resolver resolver{ioservice};  // resolver(my_io_context);
//        boost::asio::ip::tcp::resolver::query query("localhost", "10000");
//        boost::asio::ip::tcp::resolver::iterator iter = resolver.resolve(query);
//        boost::asio::ip::tcp::resolver::iterator end; // End marker.
//                
//        while (iter != end)
//        {
//          boost::asio::ip::tcp::endpoint endpoint = *iter++;
//          std::cout << endpoint << std::endl;
//        }
//        
//        // Attiva server in ascolto
//        boost::asio::ip::tcp::endpoint endpoint;
//        boost::asio::ip::address_v4 addr; addr=boost::asio::ip::address_v4();
//        endpoint.address(addr);
//        
//        endpoint.port(10000);
//        boost::asio::ip::tcp::acceptor acceptor(ioservice, endpoint);
//        boost::asio::ip::tcp::socket socket(ioservice);
//        acceptor.accept(socket);
//        
//        std::string message="TEST MESSAGE SERVER"; 
//        boost::system::error_code error;
////        write_to_socket(socket, "Hello World\n");
//        
//        int kk;
//        kk=0;
//    }
private:
};



#ifndef EGE_EMSCRIPTEN
//    #include <boost/beast/core.hpp>
//    #include <boost/beast/websocket.hpp>
//    #include <boost/asio/connect.hpp>
//    #include <boost/asio/ip/tcp.hpp>
//    #include <cstdlib>
//    #include <iostream>
//    #include <string>

#undef WS_BLOKS 
#ifdef WS_BLOKS

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>

using tcp = boost::asio::ip::tcp;               // from <boost/asio/ip/tcp.hpp>
namespace websocket = boost::beast::websocket;  // from <boost/beast/websocket.hpp>
    
    // Sends a WebSocket message and prints the response
class session : public std::enable_shared_from_this<session>
{
    tcp::resolver resolver_;
    websocket::stream<tcp::socket> ws_;
    boost::beast::multi_buffer buffer_;
    std::string host_;
    std::string text_;

public:
    // Resolver and socket require an io_context
    explicit
    session(boost::asio::io_context& ioc) : resolver_(ioc) , ws_(ioc){
    }

        // Report a failure
    void
    fail(boost::system::error_code ec, char const* what)
    {
        std::cerr << what << ": " << ec.message() << "\n";
    }    
        
    // Start the asynchronous operation
    void
    run(
        char const* host,
        char const* port,
        char const* text)
    {
        // Save these for later
        host_ = host;
        text_ = text;

        // Look up the domain name
        resolver_.async_resolve(
            host,
            port,
            std::bind(
                &session::on_resolve,
                shared_from_this(),
                std::placeholders::_1,
                std::placeholders::_2));
    }
    
    void close(void){
       // Close the WebSocket connection
        ws_.async_close(websocket::close_code::normal,
            std::bind(
                &session::on_close,
                shared_from_this(),
                std::placeholders::_1));
    }
    
    void
    read(void)
    {        
        // Read a message into our buffer
        ws_.async_read(
            buffer_,
            std::bind(
                &session::on_read,
                shared_from_this(),
                std::placeholders::_1,
                std::placeholders::_2));
        
        int k=0;
        k++;
        
    }
    
    
    void sendSynch(std::string msg){
        ws_.write(  boost::asio::buffer(msg));
    }

    // Non ho capito come mai si pianta 
    void sendAsynch(std::string msg){
        ws_.async_write(
            //boost::asio::buffer(text_),
            boost::asio::buffer(msg),
            std::bind(
                &session::on_write,
                shared_from_this(),
                std::placeholders::_1,
                std::placeholders::_2));
    }

    void
    on_resolve(
        boost::system::error_code ec,
        tcp::resolver::results_type results)
    {
        if(ec)
            return fail(ec, "resolve");

        // Make the connection on the IP address we get from a lookup
        boost::asio::async_connect(
            ws_.next_layer(),
            results.begin(),
            results.end(),
            std::bind(
                &session::on_connect,
                shared_from_this(),
                std::placeholders::_1));
    }

    void
    on_connect(boost::system::error_code ec)
    {
        if(ec)
            return fail(ec, "connect");

        // Perform the websocket handshake
        ws_.async_handshake(host_, "/",
            std::bind(
                &session::on_handshake,
                shared_from_this(),
                std::placeholders::_1));
    }

    void
    on_handshake(boost::system::error_code ec)
    {
        if(ec)
            return fail(ec, "handshake");
        
        // Send the message
        ws_.async_write(
            boost::asio::buffer(text_),
            std::bind(
                &session::on_write,
                shared_from_this(),
                std::placeholders::_1,
                std::placeholders::_2));
    }
    
    void
    on_write(
        boost::system::error_code ec,
        std::size_t bytes_transferred)
    {
        boost::ignore_unused(bytes_transferred);

        if(ec)
            return fail(ec, "write");
        
        // Read a message into our buffer
        ws_.async_read(
            buffer_,
            std::bind(
                &session::on_read,
                shared_from_this(),
                std::placeholders::_1,
                std::placeholders::_2));
        // read();
        
    }

    void
    on_read(
        boost::system::error_code ec,
        std::size_t bytes_transferred)
    {
        boost::ignore_unused(bytes_transferred);

        if(ec)
            return fail(ec, "read");

        // Close the WebSocket connection
//        ws_.async_close(websocket::close_code::normal,
//            std::bind(
//                &session::on_close,
//                shared_from_this(),
//                std::placeholders::_1));
        
        std::cout << "READ BUFFER: "  << boost::beast::buffers(buffer_.data()) << std::endl;
        buffer_.consume(buffer_.size());
       
        
        std::cout << "READ BUFFER: "  << boost::beast::buffers(buffer_.data()) << std::endl;
        
        //sendAsynch("ZAZAAZA");
        read();
        
    }

    void
    on_close(boost::system::error_code ec)
    {
        if(ec)
            return fail(ec, "close");

        // If we get here then the connection is closed gracefully

        // The buffers() function helps print a ConstBufferSequence
        std::cout << boost::beast::buffers(buffer_.data()) << std::endl;
    }
};
  
#endif //  WS_BLOKS 

//
// chat_message.hpp
// ~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2018 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifdef CHAT_MESSAGE_HPP
#define CHAT_MESSAGE_HPP

#ifdef CHAT_MESSAGE_HPP

#include <cstdio>
#include <cstdlib>
#include <cstring>

class chat_message
{
public:
  enum { header_length = 4 };
  enum { max_body_length = 512 };

  chat_message()
    : body_length_(0)
  {
  }

  const char* data() const
  {
    return data_;
  }

  char* data()
  {
    return data_;
  }

  std::size_t length() const
  {
    return header_length + body_length_;
  }

  const char* body() const
  {
    return data_ + header_length;
  }

  char* body()
  {
    return data_ + header_length;
  }

  std::size_t body_length() const
  {
    return body_length_;
  }

  void body_length(std::size_t new_length)
  {
    body_length_ = new_length;
    if (body_length_ > max_body_length)
      body_length_ = max_body_length;
  }

  bool decode_header()
  {
    char header[header_length + 1] = "";
    std::strncat(header, data_, header_length);
    body_length_ = std::atoi(header);
    if (body_length_ > max_body_length)
    {
      body_length_ = 0;
      return false;
    }
    return true;
  }

  void encode_header()
  {
    char header[header_length + 1] = "";
    std::sprintf(header, "%4d", static_cast<int>(body_length_));
    std::memcpy(data_, header, header_length);
  }

private:
  char data_[header_length + max_body_length];
  std::size_t body_length_;
};

#endif // CHAT_MESSAGE_HPP

#endif 

#undef WS_CHATCLI
#ifdef WS_CHATCLI
#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>
//#include "chat_message.hpp"

using boost::asio::ip::tcp;

typedef std::deque<chat_message> chat_message_queue;

class chat_client
{
public:
  chat_client(boost::asio::io_context& io_context,
      const tcp::resolver::results_type& endpoints)
    : io_context_(io_context),
      socket_(io_context)
  {
    do_connect(endpoints);
  }

  void write(const chat_message& msg)
  {
    boost::asio::post(io_context_,
        [this, msg]()
        {
          bool write_in_progress = !write_msgs_.empty();
          write_msgs_.push_back(msg);
          if (!write_in_progress)
          {
            do_write();
          }
        });
  }

  void close()
  {
    boost::asio::post(io_context_, [this]() { socket_.close(); });
  }

private:
  void do_connect(const tcp::resolver::results_type& endpoints)
  {
    boost::asio::async_connect(socket_, endpoints,
        [this](boost::system::error_code ec, tcp::endpoint)
        {
          if (!ec)
          {
            do_read_header();
          }
        });
  }

  void do_read_header()
  {
    boost::asio::async_read(socket_,
        boost::asio::buffer(read_msg_.data(), chat_message::header_length),
        [this](boost::system::error_code ec, std::size_t /*length*/)
        {
          if (!ec && read_msg_.decode_header())
          {
            do_read_body();
          }
          else
          {
            socket_.close();
          }
        });
  }

  void do_read_body()
  {
    boost::asio::async_read(socket_,
        boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
        [this](boost::system::error_code ec, std::size_t /*length*/)
        {
          if (!ec)
          {
            std::cout.write(read_msg_.body(), read_msg_.body_length());
            std::cout << "\n";
            do_read_header();
          }
          else
          {
            socket_.close();
          }
        });
  }

  void do_write()
  {
    boost::asio::async_write(socket_,
        boost::asio::buffer(write_msgs_.front().data(),
          write_msgs_.front().length()),
        [this](boost::system::error_code ec, std::size_t /*length*/)
        {
          if (!ec)
          {
            write_msgs_.pop_front();
            if (!write_msgs_.empty())
            {
              do_write();
            }
          }
          else
          {
            socket_.close();
          }
        });
  }

private:
  boost::asio::io_context& io_context_;
  tcp::socket socket_;
  chat_message read_msg_;
  chat_message_queue write_msgs_;
};

#endif


#undef WS_WEBSOCKET_AYNCH_CLI
#ifdef WS_WEBSOCKET_AYNCH_CLI

//------------------------------------------------------------------------------
//
// Example: WebSocket client, asynchronous
//
//------------------------------------------------------------------------------

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>

using tcp = boost::asio::ip::tcp;               // from <boost/asio/ip/tcp.hpp>
namespace websocket = boost::beast::websocket;  // from <boost/beast/websocket.hpp>

//------------------------------------------------------------------------------

// Sends a WebSocket message and prints the response
class session : public std::enable_shared_from_this<session>
{
    tcp::resolver resolver_;
    websocket::stream<tcp::socket> ws_;
    boost::beast::multi_buffer buffer_;
    std::string host_;
    std::string text_;
    
    // boost::shared_ptr<int> bsp_;

public:
    // Resolver and socket require an io_context
    explicit
    session(boost::asio::io_context& ioc)
        : resolver_(ioc)
        , ws_(ioc)
    {
    }
    ~session()
    {
       std::cout << "~session()" << std::endl; 
    }    
        

    // Report a failure
void
fail(boost::system::error_code ec, char const* what)
{
    std::cerr << what << ": " << ec.message() << "\n";
}    
        
        
    // Start the asynchronous operation
    void
    run(
        char const* host,
        char const* port,
        char const* text)
    {
        // Save these for later
        host_ = host;
        text_ = text;
        
        // bsp_ = boost::make_shared<int>(1);
        
        
        // Look up the domain name
        resolver_.async_resolve(
            host,
            port,
            std::bind(
                &session::on_resolve,
                shared_from_this(),
                std::placeholders::_1,
                std::placeholders::_2));
    }

    void
    on_resolve(
        boost::system::error_code ec,
        tcp::resolver::results_type results)
    {
        if(ec)
            return fail(ec, "resolve");

        // Make the connection on the IP address we get from a lookup
        boost::asio::async_connect(
            ws_.next_layer(),
            results.begin(),
            results.end(),
            std::bind(
                &session::on_connect,
                shared_from_this(),
                std::placeholders::_1));
    }

    void
    on_connect(boost::system::error_code ec)
    {
        if(ec)
            return fail(ec, "connect");

        // Perform the websocket handshake
        ws_.async_handshake(host_, "/",
            std::bind(
                &session::on_handshake,
                shared_from_this(),
                std::placeholders::_1));
    }

    void
    on_handshake(boost::system::error_code ec)
    {
        if(ec)
            return fail(ec, "handshake");
                
        // Read a message into our buffer (aggiunta)
//        ws_.async_read(
//            buffer_,
//            std::bind(
//                &session::on_read,
//                shared_from_this(),
//                std::placeholders::_1,
//                std::placeholders::_2));
        
        
        // Send the message
        ws_.async_write(
            boost::asio::buffer(text_),
            std::bind(
                &session::on_write,
                shared_from_this(),
                std::placeholders::_1,
                std::placeholders::_2));
    }

    void
    on_write(
        boost::system::error_code ec,
        std::size_t bytes_transferred)
    {
        boost::ignore_unused(bytes_transferred);

        if(ec)
            return fail(ec, "write");
        
        // Read a message into our buffer
        ws_.async_read(
            buffer_,
            std::bind(
                &session::on_read,
                shared_from_this(),
                std::placeholders::_1,
                std::placeholders::_2));
        
    }

    void
    on_read(
        boost::system::error_code ec,
        std::size_t bytes_transferred)
    {
        boost::ignore_unused(bytes_transferred);

        if(ec)
            return fail(ec, "read");
       
        // Close the WebSocket connection
        ws_.async_close(websocket::close_code::normal,
            std::bind(
                &session::on_close,
                shared_from_this(),
                std::placeholders::_1));
        
       
    }

    void
    on_close(boost::system::error_code ec)
    {
        if(ec)
            return fail(ec, "close");

        // If we get here then the connection is closed gracefully

        // The buffers() function helps print a ConstBufferSequence
        std::cout << "CLOSING:  " <<boost::beast::buffers(buffer_.data()) << std::endl;
    }
};


#endif WS_WEBSOCKET_AYNCH_CLI


#endif //  EGE_EMSCRIPTEN


class egeSocketCli {
public:
    egeSocketCli();
    virtual ~egeSocketCli();
    
    // Comanda API Browser di iniziare comunicazione con server
    void open(std::string host, std::string port);
    
    // Comanda API Browser di terminare comunicazione con server
    void close(void);
    
    void synchRead(void);
    
    // Comanda API Browser di spedire messaggio(comandi processo) a server
    void send(egeCommand &cmd);
    
    /* Comanda API Browser di spedire messaggio(comandi processo) a server
     *
     * Invia messaggio direttamente come ricevuto da IAction in formato stringa
     */  
    void send(std::string msg);
    
private:
    #ifdef WS_BLOKS
        // The io_context is required for all I/O
        boost::asio::io_context ioc;

        // These objects perform our I/O
        tcp::resolver resolver{ioc};
        websocket::stream<tcp::socket> ws{ioc};
        
        // This buffer will hold the incoming message
        boost::beast::multi_buffer buffer;
        
        std::shared_ptr<session> _sock;
    #endif // WS_BLOKS    
};


////////////////////////////////////////////////////////////////////////////////


namespace ege{
namespace socket{
    // append a message to the server message queue
    void send(std::string message);
    
    // try to send the message queue to server until it's empty
    void sendAllMessages(egeSocketCli *clientSocket);
    
    
};  // socket
};  // ege

#endif /* EGESOCKET_H */