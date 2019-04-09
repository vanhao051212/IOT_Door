var db = require("./database/db");

var express = require('express');
var app = express();
app.use(express.static("./public"));
app.set("view engine","ejs");
app.set("views", "./views");
var port = 3000;
var server = require("http").Server(app);

var bodyParser = require('body-parser');
app.use(bodyParser.urlencoded({extended: false}));

//socket io and pass port
var io = require("socket.io")(server);

io.on("connection", function(socket) {
	console.log(socket.id + " connected");
    socket.on("disconnect", function() {
        console.log(socket.id + " disconnected");
    });
    //web client send join room master
    socket.on("join-room", function() {
    	socket.join("master");
    	listRooms();
		async function listRooms() {
		    try{
		        var roomInfo = await db.getRoomInfo();
		        if (roomInfo) {
		        	socket.emit("server-send-list-rooms", roomInfo);
		        }
		    }
		    catch(e){throw (e);}
		};
    });
    //esp send message
    // {"RoomID":"C103", "Mess":"M02"}
    socket.on("esp-send-mess", function(data) {
    	socket.emit("server-ack");
    	console.log("esp-send-mess");
    	console.log(data);
    	console.log("--------------------------");
    	if(data.RoomID && data.Mess) {
    		tryUpdateSend();
    		/*
    		- kiem tra mess co ton tai trong database hay chua
    		- neu chua ton tai va mess dung thi luu database
    		- gui data ve web (room master)
    		*/
    		async function tryUpdateSend() {
				try{
			        var checkMessExist = await db.checkMessExist(data.RoomID, data.Mess);
			        var checkMessCorrect = await db.checkMessCorrect(data.Mess);
			        var checkRoomExist = await db.checkRoomExist(data.RoomID);
			        if (!checkMessExist && checkMessCorrect && checkRoomExist) {
			        	var roomInfo = await db.getSpecRoomInfo(data.RoomID);
			        	if (roomInfo) {
			        		status = JSON.parse(roomInfo.Mess);
			        		status.push(data.Mess);
			        		await db.updateMess(data.RoomID, JSON.stringify(status));
			        		var roomInfo = await db.getSpecRoomInfo(data.RoomID);
	        				var messInfo = await db.getMessInfo();
	        				//send update status room to master
	        				io.sockets.in("master").emit("server-send-update-status-room", {room: roomInfo, messes: messInfo});
			        	}
			        }
			    }
			    catch(e){throw (e);}
			};
    	}
    });
    socket.on("esp-send-join-room", function(data) {
    	console.log("esp-send-join-room");
    	console.log(data);
    	console.log("--------------------------");
    	if(data.RoomID) {
    		tryJoin();
    		async function tryJoin() {
				try{
			        var checkRoomExist = await db.checkRoomExist(data.RoomID);
			        if (checkRoomExist) {
			        	socket.join(data.RoomID);
			        	console.log("join-success");
			        	io.sockets.in(data.RoomID).emit("join-success", "OK");
			        }
			    }
			    catch(e){throw (e);}
			};
    	}
    });

    socket.on("web-send-mess-to-esp", function(data) {
    	io.sockets.in(data.RoomID).emit("server-send-mess-to-esp", {Mess: data.Mess});
    });
    socket.on("web-send-reset-room", function(room) {
    	tryResetMess();
    	async function tryResetMess() {
    		await db.updateMess(room.RoomID, "[]");
    		var roomInfo = await db.getSpecRoomInfo(room.RoomID);
			var messInfo = await db.getMessInfo();
			//send update status room to master
			io.sockets.in("master").emit("server-send-update-status-room", {room: roomInfo, messes: messInfo});
    	}
    });
});

app.post('/checkRFID', function(req, res) {
  if (req.body && req.body.ID) {
  	checkRFID();
	async function checkRFID() {
	    try{
	        var result = await db.checkRFID(req.body.ID);
	        if (result) {
	        	res.send("ACCEPT");
	        } else {
	        	res.send("DECLINE");
	        }
	    }
	    catch(e){throw (e);}
	};
  } else {
  	res.sendStatus(405);
  }
});

app.get('/esp', function(req, res, next) {
    res.render('client_esp');
});

app.get('/control', function(req, res, next) {
    getRoomInfo();
	async function getRoomInfo() {
	    try{
	        var roomInfo = await db.getRoomInfo();
	        var messInfo = await db.getMessInfo();
	        if (roomInfo && messInfo) {
	        	res.render("control", {rooms:roomInfo, messes: messInfo});
	        } else {
	        	res.send("0 result");
	        }
	    }
	    catch(e){throw (e);}
	};
});

server.listen(process.env.PORT || port);