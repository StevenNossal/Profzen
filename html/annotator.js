/* annotator.js */

var socket_aw;
var writerlist;


document.addEventListener('DOMContentLoaded', function() {
	
	show_thumbnail_view();

	try {
		socket_aw= new WebSocket(get_appropriate_ws_url(),"profzen-annotator");
		socket_aw.onopen = function() {
		  // request_writer_list();
		}	
		socket_aw.onmessage = function got_packet(msg) {
			console.log("onmessage:" + msg.data );
			var command = parseInt(msg.data.substr(0,2));
			var payload = msg.data.substring(2, msg.data.length);
			
			if ( 1 == command ) on_get_writer_list(payload);
			if ( 2 == command ) on_get_writer_inner_html(payload);

		}
		socket_aw.onclose = function() {
		  console.log("socket_aw.onclose");
		}

		

	} catch(exception) {
		alert('<p>Error' + exception);
	}
});
	

function get_appropriate_ws_url()
{
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

function request_writer_list()
{
  console.log("request_writer_list");
  socket_aw.send("01");	
}

function request_writer_inner_html( writerNumber )
{
  console.log("request_writer: writerNumber");
  console.dir( writerNumber );
  socket_aw.send("02" + writerNumber );
}

function on_get_writer_list(payload)
{
  console.log("on_get_writer_listi:payload:");
  console.dir(payload);

  writerlist = $.parseJSON( payload );
  console.log("writerlist:");
  console.dir(writerlist);
}

function on_get_writer_inner_html( payload )
{
  console.log("on_get_writer_inner_html: writerNumber");
  var writerNumber = payload.substring(0,2);
  console.dir( writerNumber );
  
  var targetDiv = get_or_create_thumbnail_div( writerNumber );
  targetDiv.html(payload.substring(2, payload.length));

  var singleWriterId = $("#single-writer").attr("data-writer-id");
  console.log( "singleWriterId");
  console.dir( singleWriterId );

  if ( writerNumber == singleWriterId )
  {
	$("#single-writer").html(payload.substring(2, payload.length));
  }
}

function get_or_create_thumbnail_div( writerNumber )
{
	// TO DO: Select by data-writer-id

   console.log( "get_or_create_thumbnail_div: writerNumber" );
   console.dir( writerNumber );

   if ( 0 == $("#writer" + writerNumber ).length )
   {
	   create_thumbnail_div( writerNumber );
   }

   target = $("#writer" + writerNumber );
	
   console.log( "get_or_create_thumbnail_div: return value ");
   console.dir( target );
	
   return target;

}


function create_thumbnail_div( writerNumber )
{
  console.log("create_thumbnail_div: writerNumber");
  console.dir( writerNumber );

  var emptySlot = get_free_thumbnail_slot();
  emptySlot.append("<h2>Writer " + writerNumber 
		  + "</h2><div id='writer" + writerNumber
		  + "' class='thumbnail-writer' data-writer-id='" + writerNumber + "'></div>");

  $(".thumbnail-writer").click(thumbnail_on_click);
}

function get_free_thumbnail_slot()
{
  if ( 0 == $(".free-thumbnail-slot").length )
  {
	  create_new_thumbnail_row();
  }

  var freeSlot = $(".free-thumbnail-slot").first();
  freeSlot.removeClass("free-thumbnail-slot");

  return freeSlot;
}

function create_new_thumbnail_row()
{
	var newRow = $("<div class='row'></div>");
	for ( i = 0; i < 3; i++ ) {
	  newRow.append("<div class='col-md-4 free-thumbnail-slot'></div>");
	}
	
	$("#thumbnail-view").append( newRow );
}

function thumbnail_on_click( )
{
	console.log("thumbnail on click");
	var writerNumber = $(this).attr("data-writer-id");
	console.log("writerNumber = " + writerNumber );
	show_single_writer_view( writerNumber );
}

function show_thumbnail_view()
{
  console.log("show_thumbnail_view");
  $("#thumbnail-view").show();
  $("#single-writer-buttons").hide();
  $("#single-writer-view").hide();
}

function show_single_writer_view( writerNumber )
{
  $("#single-writer").attr("data-writer-id", writerNumber );
  request_writer_inner_html( writerNumber );

  $("#single-writer-view").show();
  $("#single-writer-buttons").show();
  $("#thumbnail-view").hide();
  
  $(".btn-show-thumbnail-view").click(show_thumbnail_view);
}
