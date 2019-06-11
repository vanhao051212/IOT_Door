var db = require("./database/db");

var axios = require('axios')
var qs = require('qs')

var express = require('express');
var app = express();
app.use(express.static("./public"));
app.set("view engine","ejs");
app.set("views", "./views");
var port = 8000;
var server = require("http").Server(app);

var bodyParser = require('body-parser');
app.use(bodyParser.urlencoded({extended: false}));

//socket io and pass port
var io = require("socket.io")(server);

io.on("connection", function(socket) {
	//console.log(socket.id + " connected");
    socket.on("disconnect", function() {
        //console.log(socket.id + " disconnected");
    });
    //web client send join room master
    socket.on("join-room", function(area) {
    	if(area)
    		socket.join(area);
    		listRooms();
		async function listRooms() {
		    try{
		        var roomsInfo = await db.getRoomInfoArea(area);
		        var roomsList = await db.getRoomArea(area);
		        var messesInfo = await db.getMessInfo();
		        if (roomsList) {
		        	socket.emit("server-send-list-rooms", {roomsInfo: roomsInfo, roomsList:roomsList, messesInfo: messesInfo});
		        }
		    }
		    catch(e){throw (e);}
		};
    });
    //esp send message
    // {"RoomID":"C103", "Mess":"M02"}
    socket.on("esp-send-mess", function(data) {
    	socket.emit("server-ack");
    	//console.log("esp-send-mess");
    	//console.log(data);
    	//console.log("--------------------------");
    	if(data.RoomID && data.MessID) {
    		tryUpdateSend();
    		/*
    		- kiem tra mess co ton tai trong database hay chua
    		- neu chua ton tai va mess dung thi luu database
    		- gui data ve web (room master)
    		*/
    		async function tryUpdateSend() {
				try{
			        var checkMessExist = await db.checkMessExist(data.RoomID, data.MessID);
			        //var checkMessCorrect = await db.checkMessCorrect(data.MessID);
			        //var checkRoomExist = await db.checkRoomExist(data.RoomID);
			        //if (!checkMessExist && checkMessCorrect && checkRoomExist) {
			        if (!checkMessExist) {
			        	if(data.CardID) {
			        		await db.insertMess(data.RoomID, data.MessID, data.CardID);
			        	} else {
			        		await db.insertMess(data.RoomID, data.MessID, "");
			        	}
			        	var roomInfo = await db.getSpecRoomInfo(data.RoomID);
	        			var messInfo = await db.getMessInfo();
	        			var area = await db.getAreaRoomID(data.RoomID);
	        			//console.log(area);
			        	if(area)
							io.sockets.in(area).emit("server-send-update-status-room", {room: roomInfo, messes: messInfo});
			        }
			    }
			    catch(e){throw (e);}
			};
    	}
    });
    socket.on("esp-send-join-room", function(data) {
    	if(data.RoomID) {
    		tryJoin();
    		async function tryJoin() {
				try{
			        var checkRoomExist = await db.checkRoomExist(data.RoomID);
			        if (checkRoomExist) {
			        	socket.join(data.RoomID);
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
    		var result = await db.deleteAllRoomStatus(room.RoomID);
    		if(result) {
    			var roomInfo = await db.getSpecRoomInfo(room.RoomID);
				var messInfo = await db.getMessInfo();
				//send update status room to master
				io.sockets.in(room.Area).emit("server-send-update-status-room", {room: roomInfo, messes: messInfo, RoomID: room.RoomID});
    		}
    	}
    });
});

//receivedCmd
app.post('/receivedCmd', function(req, res) {
  var diemDanhCmd = 'M06';
  var baoDongCmd = 'M05';
  //console.log(req.body);
  if (req.body) {
  	checkCMD();
	async function checkCMD() {
	    try{
	    	if (req.body.CMD == diemDanhCmd) {
	    		if(req.body.CardID && req.body.RoomID) {
	    			var t = await axios({
					  method: 'post',
					  url: 'http://192.168.20.135:8000/testPost',
					  data: qs.stringify({
					    name: 'thuc',
					  }),
					  headers: {
					    'content-type': 'application/x-www-form-urlencoded;charset=utf-8'
					  }
					});
					if(t.data && t.status == 200) {
						console.log(t.data);
					}
					return res.send("OK");
	    			/*
	    			var result = await db.insertDiemDanh(req.body.RoomID, req.body.CardID);
	    			if(result) {
	    				res.send("OK");
	    			} else {
	    				res.send("FAIL");
	    			}
	    			*/
	    		}
	    	} else if (req.body.CMD == baoDongCmd) {
	    		if(req.body.RoomID) {
		    		var checkMessExist = await db.checkMessExist(req.body.RoomID, baoDongCmd);
			        //var checkRoomExist = await db.checkRoomExist(req.body.RoomID);
			        //if (!checkMessExist && checkRoomExist) {
			        if (!checkMessExist) {
			        	await db.insertMess(req.body.RoomID, baoDongCmd, "");
	        			var roomInfo = await db.getSpecRoomInfo(req.body.RoomID);
	        			var messInfo = await db.getMessInfo();
	        			var area = await db.getAreaRoomID(req.body.RoomID);
			        	if(area)
							io.sockets.in(area).emit("server-send-update-status-room", {room: roomInfo, messes: messInfo, RoomID: req.body.RoomID});
			        }
			        return res.send("OK");
			    } else {
			    	return res.send("FAIL");
				}
	    	} else {
	    		return res.send("FAIL");
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
	        var listAreas = await db.getListArea();
	        if (listAreas) {
	        	res.render("control", {areas: listAreas});
	        } else {
	        	res.send("0 result");
	        }
	    }
	    catch(e){throw (e);}
	};
});

app.post('/testPost', function(req, res) {
  if (req.body && req.body.name) {
  	res.send("hello "+req.body.name);
  }
});

server.listen(process.env.PORT || port);