function AutoRefresh( t ) {
	setTimeout("location.reload(true);", t);
}
$(document).ready(function() {
	var socket = io();
	var x = document.getElementById("myAudio");
	x.pause();
	socket.emit("join-room");

	socket.on("server-send-list-rooms", function(rooms) {
		if($("#alertMark").val() == "Alert") {
			x.pause();
			x.play();
		}
		rooms.forEach(function(room) {
			var txtID = '#txtSend_'+room.RoomID;
			var btnSendID = '#btnSend_'+room.RoomID;
			var btnResetID = '#btnReset_'+room.RoomID;
			$( btnSendID ).click(function() {
				socket.emit("web-send-mess-to-esp", {RoomID: room.RoomID, Mess: $( txtID ).val()});
			});
			$( btnResetID ).click(function() {
				socket.emit("web-send-reset-room", {RoomID: room.RoomID});
			});
		});
	});

	socket.on("server-send-update-status-room", function(data) {
		if(data.room && data.messes) {
			var htmlStatus="";
			var MessIDs=[];
			data.room.forEach(function(roomInfo) {
				MessIDs.push(roomInfo.MessID);
			});
			MessIDs.forEach(function(MessID) {
				data.messes.forEach(function(mess) {
					if(MessID == mess.MessID) {
						if(MessID == 'M05') {
							htmlStatus+='<p style = "background-color:#ffa366; color:white; font-size: 20px;">'+mess.MessInFo+'</p>';
							x.pause();
							x.play();
						} else
							htmlStatus+='<p>'+mess.MessInFo+'</p>';
					}
				});
			});
			var changeStatusRoom = "#info_" + data.room[0].RoomID;
			$(changeStatusRoom).html(htmlStatus);
		} else if(!data.room && data.RoomID) {
			var changeStatusRoom = "#info_" + data.RoomID;
			$(changeStatusRoom).html("");
		}
	});
});