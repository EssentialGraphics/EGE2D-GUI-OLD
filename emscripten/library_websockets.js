mergeInto(LibraryManager.library, {

// 2018.03.15 - C API : Websocket per comunicazione Browser <---> Server Apache (via Python WebSockets o altro WebSocket(c#/c++ etc..))
// 		Fornisce funzioni Open() Close() Send(toServer e toScada2D - bidirezionale)

    ws: {},				// websocket var persistente tra le funzioni definite nella presente API
    
    // Abilita / Disabilita output a console	
    wsConsoleEcho: function(enable){
		// console.log("wsConsoleEcho : " + enable);	
    },

    wsOpen: function (uri) {
	//console.log(">> Websock open command : <" + UTF8ToString(uri) + ">" );  // Valore valido di test "ws://localhost:10000/"

	_ws = new WebSocket(UTF8ToString(uri));
	
		_ws.onopen = function(evt) {
			console.log("WebSocket Connection opened!"); 
			var c_javascript_evt = Module.cwrap('onComm', null, ['int']); 
			c_javascript_evt(1);
		};

		_ws.onclose = function(evt) {
			console.log("WebSocket Connection closed!");
			var c_javascript_evt = Module.cwrap('onComm', null); 
			c_javascript_evt(0);
		};

		_ws.onmessage = function(evt) {
	 		// console.log("WebSocket incoming message event..." + evt.data);   
			var c_javascript_evt = Module.cwrap('onMessageAsString', null, ['string']); 
			c_javascript_evt(evt.data);
		};
	
     },

    wsClose: function(){
      	_ws.close();
    },

// API invio messaggi da browser a server 

    wsSend: function() {
        console.log(" wsSend : Non implementata !!!");
    },

    wsSendInt: function(data) {
        _ws.send("<int>[" + data + "]{DT:" + new Date().getTime() + "};");
    },

    wsSendString: function(buffer){
     	_ws.send("<string>[" + UTF8ToString(buffer) + "]{DT:" + new Date().getTime() + "};" );
    },

    wsSendCommandInt: function(opcode, varIndex, valueInt) {
        _ws.send("<cmdInt>[" + opcode + "," + varIndex + "," + valueInt + "]{DT:" + new Date().getTime() + "};" );
    } 
});
