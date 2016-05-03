
// writer.js
		
			
var socket_aw;
var writereditor;
			
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
				
	$("#btn-bold").on("click", function() {
	  document.execCommand('bold');
	});

	$("#btn-italic").on("click", function() {
		document.execCommand('italic');
	});
	
	$("#btn-underline").on("click", function() {
		document.execCommand('underline');
	});

	$("#btn-strikethrough").on("click", function() {
		document.execCommand('strikethrough');
	});

	$("#btn-subscript").on("click", function() {
		document.execCommand('subscript');
	});

	$("#btn-superscript").on("click", function() {
		document.execCommand('superscript');
	});
	
	$("#btn-paragraph").on("click", function() {
		document.execCommand('insertParagraph');
	});
	
	$("#btn-indent").on("click", function() {
		document.execCommand('indent');
	});
	
	$("#btn-outdent").on("click", function() {
		document.execCommand('outdent');
	});

	$("#btn-copy").on("click", function() {
		document.execCommand('copy');
	});
	
	$("#btn-cut").on("click", function() {
		document.execCommand('cut');
	});
	
	$("#btn-paste").on("click", function() {
		document.execCommand('paste');
	});

	$("#btn-undo").on("click", function() {
		document.execCommand('undo');
	});
	
	$("#btn-redo").on("click", function() {
		document.execCommand('redo');
	});

	
	writereditor = document.getElementById("editor");
	
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

