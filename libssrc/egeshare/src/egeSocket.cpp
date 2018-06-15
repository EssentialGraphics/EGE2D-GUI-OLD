/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   egeSocket.cpp
 * Author: giovanni maffioletti - mymailaddress is development at ege3d.it
 * 
 * Created on 10 marzo 2018, 13.38
 */

#include <string.h>
#include <egeSocket.h>

std::queue<std::string> sharedEgeGuiFromSrvMsgQueue;     // coda messaggi shared da SRV
egeCommand sharedLastServerCommand;

std::queue<std::string> sharedEgeGuiToSrvMsgQueue;     // coda messaggi shared a SRV

using namespace std;

namespace ege{
namespace socket{
    // Metodo per IAction, aggiunge comando a server
    // append a message to the server message queue
    void send(string message){
        sharedEgeGuiToSrvMsgQueue.push(message); 
    }

    // prova a spedire tutti i messaggi contenuti in sharedEgeGuiToSrvMsgQueue
    void sendAllMessages(egeSocketCli *clientSocket){
        int itemcount=sharedEgeGuiToSrvMsgQueue.size();
        string msg;
        for(int i=0; i<itemcount; i++){
            msg=sharedEgeGuiToSrvMsgQueue.front();
            
            clientSocket->send(msg);
            
            sharedEgeGuiToSrvMsgQueue.pop();
        }
    }
    
    
};//socket
};//ege


vector<string> split(const string& str, const string& delim)
{
    vector<string> tokens;
    size_t prev = 0, pos = 0;
    do
    {
        pos = str.find(delim, prev);
        if (pos == string::npos) pos = str.length();
        string token = str.substr(prev, pos-prev);
        if (!token.empty()) tokens.push_back(token);
        prev = pos + delim.length();
    }
    while (pos < str.length() && prev < str.length());
    return tokens;
}


void splitMessage(void){
    // Il messaggio atteso dal server è di tipo stringa e nel formato
    // qualunquecosa...[xxx,xxx,xxx ... n.xxx]...qualunquecosa
    
    string s=sharedEgeGuiFromSrvMsgQueue.front();
    vector<string>vec=split(s,"[");                 // Rimuove dati prima di carattere [
    vector<string>vecEx=split(vec[0],"]");          // Rimuove dati dopo carattere ]
    vector<string>data=split(vecEx[0],",");         // Dati splittati
    
    // cout << "split[0]: " << data[0] << "\tsplit[1]: " << data[1] << "\tsplit[2]: " << data[2] << endl;
    
    sharedLastServerCommand.in_value.opcode=(EGEushort)stoi(data[0]);
    sharedLastServerCommand.in_value.controlIndex=(EGEushort)stoi(data[1]);
    sharedLastServerCommand.in_value.value=(EGEint)stoi(data[2]);
    
    sharedEgeGuiFromSrvMsgQueue.pop(); 
}

// Evento WebSocket da Browser
void onComm(int status){
    std:string lbl;
    switch(status){
        case 0: lbl="CLOSE"; 
            break;
        case 1: lbl="OPEN"; 
            break;
        default: lbl="unknown;";
    }
    
    std::cout << "Websock Communication Event received : " << lbl << std::endl;
};

// Evento NewData da Server - salva messaggio in coda di processo
void onMessageAsString(char* str){
    
    // Converte stringa da API browser
    std::string s=""; 
    for(int i=0; i<strlen(str); i++){ s+=str[i];  }
    
    // Salva la stringa e mantiene sotto controllo la dimensione della coda messaggi
    sharedEgeGuiFromSrvMsgQueue.push(s);
    if(sharedEgeGuiFromSrvMsgQueue.size() > EGE_GUI_MAX_MESSAGE_QUEUE_SIZE){ 
        sharedEgeGuiFromSrvMsgQueue.pop(); 
    };
    
    // std::cout << "onMessageAsString: " << s << std::endl;
    splitMessage();
}     

egeSocketSrv::egeSocketSrv() {
}
egeSocketSrv::~egeSocketSrv() {
}

////////////////////////////////////////////////////////////////////////////////
#ifdef WS_BLOKS
using tcp = boost::asio::ip::tcp;               // from <boost/asio/ip/tcp.hpp>
namespace websocket = boost::beast::websocket;  // from <boost/beast/websocket.hpp>
#endif //WS_BLOCK

egeSocketCli::egeSocketCli() {
}
egeSocketCli::~egeSocketCli() {
}

 // Comanda API Browser di iniziare comunicazione con server
void egeSocketCli::open(std::string host, std::string port){
    #ifdef EGE_EMSCRIPTEN
        // Esempio stringa funzionante - "ws://localhost:10000/" 
        std::string uri="ws://" + host + ":" + port +"/";
        wsOpen((char*)uri.c_str());
    #else

    #ifdef WS_BLOCK    
        std::string _host="localhost";
        std::string _port="10000";
        std::string _text="<WebSocket_Cpp_Client@localhost>";

        // The io_context is required for all I/O
        //boost::asio::io_context ioc;

        // Launch the asynchronous operation
        std::make_shared<session>(ioc)->run(_host.c_str(), _port.c_str(), _text.c_str());
        //_sock=std::make_shared<session>(ioc); 
        //_sock->run(_host.c_str(), _port.c_str(), _text.c_str());

        // Run the I/O service. The call will return when
        // the get operation is complete.
        ioc.run();

        //_sock->send("MIMMOMARINO");
    #endif // WS_BLOCK   

    #endif
}
    
    // Comanda API Browser di terminare comunicazione con server
void egeSocketCli::close(void){
    #ifdef EGE_EMSCRIPTEN        
        wsClose(); 
    #else    
/*
        // Close the WebSocket connection
        ws.close(websocket::close_code::normal);

        // If we get here then the connection is closed gracefully
        // The buffers() function helps print a ConstBufferSequence
        std::cout << " AFTER CLOSE: " << boost::beast::buffers(buffer.data()) << std::endl;
*/ 
    #endif        
}
    
    void egeSocketCli::synchRead(void){
        // Read a message into our buffer
//        ws.read(buffer);
//        std::cout << boost::beast::buffers(buffer.data()) << std::endl;
//        int x;
//        x++;
    }
    
    
// Comanda API Browser di spedire messaggio(comandi processo) a server
void egeSocketCli::send(egeCommand &cmd){
    #ifdef EGE_EMSCRIPTEN        
        // Comando verso WebSocket Server
        //wsSendCommandInt(12, 111, 32765);
        std::string buf;  
        buf=std::to_string(cmd.out_value.opcode);     buf+=",";
        buf+=std::to_string(cmd.out_value.tagIndex);  buf+=",";
        buf+=std::to_string(cmd.out_value.value);

        char *a=new char[buf.size()+1];
        a[buf.size()]=0;
        memcpy(a,buf.c_str(),buf.size());
        wsSendString(a);

        delete a;
    #endif        
}
    
/* Comanda API Browser di spedire messaggio(comandi processo) a server
 *
 * Invia messaggio direttamente come ricevuto da IAction in formato stringa
 */  
void egeSocketCli::send(std::string msg){
    #ifdef EGE_EMSCRIPTEN        
        wsSendString((char*)msg.c_str());
    #else     
        // Send the message
        //_sock->sendSynch(msg);
    #endif    
}