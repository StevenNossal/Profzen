
// writer.js
		
			
var socket_aw;
			
document.addEventListener('DOMContentLoaded', function() {
				
	try {
		socket_aw= new WebSocket(get_appropriate_ws_url(),"profzen-writer");
		socket_aw.onopen = function() {
				}	
		socket_aw.onmessage = function got_packet(msg) {
				}
		socket_aw.onclose = function() {
				}
		} catch(exception) {
			alert('<p>Error' + exception);
	}
				
	var writereditor = document.getElementById("editor");
	writereditor.onkeyup = function(){
				socket_aw.send(writereditor.innerHTML);	
	};

	function get_appropriate_ws_url(){
		var pcol;
		var u = document.URL;

		if (u.substring(0, 5) == "https") {
			pcol = "wss://";
			u = u.substr(8);
		} else {
			pcol = "ws://";
			if (u.substring(0,4) == "http")
			u = u.substr(7);
		}

		u = u.split('/');

		return pcol + u[0];
	}


}, false);
