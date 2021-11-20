var DBconnection = mysql.createConnection({
        host :'112.169.121.107',
        port :3306,
        user :'kduser',
        password:'kd123',
        database :'group1'
});

/////////////////////////////////////////
var socketio = require('socket.io');
var express = require('express');
var http = require('http');
var fs = require('fs');

var app = express();
app.set('port', 8000);
var server = http.createServer(app).listen(app.get('port'),function(){
	console.log("express erver listening on port " + app.get('port'));
});

//////////////////////////////라우팅
app.get('/',function(req,res){
	fs.readFile('./login3/login3.html', function(error,data){
		res.writeHead(200,{'Content-Type': 'text/html' });
		res.end(data);
	});
});
app.get('/imgs/bg.png',function(req,res){
	fs.readFile('/root/login3/imgs/bg.png',function(error,data){
	res.writeHead(200,{'Content-Type': 'text/html' });
		res.end(data);
	});
app.get('/imgs/btn_join1.png',function(req,res){
        fs.readFile('/root/login3/imgs/btn_join1.png',function(error,data){
                res.writeHead(200,{'Content-Type': 'text/html' });
                res.end(data);
        });
});

});
app.get('/imgs/btn_join2.png',function(req,res){
        fs.readFile('/root/login3/imgs/btn_join2.png',function(error,data){
                res.writeHead(200,{'Content-Type': 'text/html' });
                res.end(data);
        });
});
app.get('/imgs/x.png',function(req,res){
        fs.readFile('/root/login3/imgs/x.png',function(error,data){
                res.writeHead(200,{'Content-Type': 'text/html' });
                res.end(data);
        });
});

app.get('/css/css.css',function(req,res){
	fs.readFile('/root/login3/css/css.css',function(error,data){
		res.writeHead(200,{'Content-Type': 'text/html' });
		res.end(data);
	});
});
app.get('/css/bootstrap.min.css',function(req,res){
	fs.readFile('/root/login3/css/bootstrap.min.css',function(error,data){
		res.writeHead(200,{'Content-Type': 'text/html' });
		res.end(data);
	});
});
app.get('/js/bootstrap.min.js',function(req,res){
        fs.readFile('/root/login3/js/bootstrap.min.js',function(error,data){
                res.writeHead(200,{'Content-Type': 'text/html' });
                res.end(data);
        });
});
app.get('/fonts/SeoulNamsanL.woff',function(req,res){
        fs.readFile('/root/login3/fonts/SeoulNamsanL.woff',function(error,data){
                res.writeHead(200,{'Content-Type': 'text/html' });
                res.end(data);
        });
});
app.get('/fonts/glyphicons-halflings-regular.woff',function(req,res){
        fs.readFile('/root/login3/fonts/glyphicons-halflings-regular.woff',function(error,data){
                res.writeHead(200,{'Content-Type': 'text/html' });
                res.end(data);
        });
});

//app.use(express.logger());
//app.use(app.router);
//app.use(express.static(__dirname + '/public'));
//////////////////////////////////////////////////html 라우팅
app.get('/productView.html',function(req,res){
        fs.readFile('/root/login3/productView.html',function(error,data){
                res.writeHead(200,{'Content-Type': 'text/html' });
                res.end(data);
        });
});
app.get('/header2.html',function(req,res){
        fs.readFile('/root/login3/header2.html',function(error,data){
                res.writeHead(200,{'Content-Type': 'text/html' });
                res.end(data);
        });
});
app.get('/productView_tbl.html',function(req,res){
        fs.readFile('/root/login3/productView_tbl.html',function(error,data){
                res.writeHead(200,{'Content-Type': 'text/html' });
                res.end(data);
        });
});
app.get('/productReg_form.html',function(req,res){
        fs.readFile('/root/login3/productReg_form.html',function(error,data){
                res.writeHead(200,{'Content-Type': 'text/html' });
                res.end(data);
        });
});
app.get('/table.html',function(req,res){
        fs.readFile('/root/login3/table.html',function(error,data){
                res.writeHead(200,{'Content-Type': 'text/html' });
                res.end(data);
        });
});

////////////////////////////////////////////////리다이렉
app.get('/main',function(req,res){
        fs.readFile('./login3/main3.html', function(error,data){
                res.writeHead(200,{'Content-Type': 'text/html' });
                res.end(data);
        });
});

/*
app.get('/main.11',function(req,res){
	console.log('wwww');
	//res.send('<a href="/root/login3/main.html"></a>');
	//res.redirect('/main.html');
	       // fs.readFile('/root/login3/mian.html',function(error,data){
                //res.writeHead(200,{'Content-Type': 'text/html' });
                //res.end(data);


	console.log('qqqqq');
});
*/
//////////////////////////////////////////////소켓
var io = socketio.listen(server);
var login =
{ ID: 'jeffy',
  PW: '1234'
};

var headers = {
 'Content-Type':'application/json',
// 'Content-Length':userString.length
};

var loginString = JSON.stringify(login);

io.sockets.on('connection',function(socket)
{
        console.log('client connect');
        socket.emit('news',loginString);

        socket.on('RegistAdminAccount',function (data)
        {
		var info = JSON.parse(data);
                console.log("ID : " + info.id + "\nPW : " + info.pw + "\nName : " + info.name + "\nPH : " + info.phone);
	       // var password = Decrypt(info.pw);
	      //console.log(password);
       		console.log(info.pw);
		
		DBconnection.connect();
		console.log('start');
		var date = new Date();
		var sqlQuery = 'INSERT INTO UserMst SET ?';
		var post =  {User_Id:info.id,User_Nm:info.name,Login_Pwd:info.pw,Use_chk:1,In_User_Id:'root',In_DateTime:date,Mod_User_Id:'root',Mod_DateTime:date};
		var query = DBconnection.query(sqlQuery, post, function(err, res)
		{
       			if(err)
			{
				socket.emit('RegistResult','fail');
				throw err;
			}
			socket.emit('RegistResult','success');
       		 	console.log('test');
		}
		);
		console.log("end");
		DBconnection.end()

	 });
	socket.on('LoginBt',function (data)
        {
		console.log('login try');
		console.log(data);
		socket.emit('LoginRes','/main');
		socket.emit('LoginResult','success');
		socket.emit('LoginResult','id_fail');
		socket.emit('LoginResult','pw_fail');
	});
	socket.on('RequestItemsInfo',function (data)
        {
//		DBconnection.connect();
                console.log('itstart');
                console.log(data);
                var tmpData = data.substring(0,4);
		console.log(tmpData);
                var q = "SELECT * FROM ItemMst WHERE Category_Cd like '" + tmpData + "%'";
                DBconnection.query(q
                ,function(err, res, fields)
                {
                        if(err) {
                                console.log(err); }
                        else{
                                var itemTmp = {
                                        items: []
                                };
                                for(var i=0; i<res.length; i++){
                                        var tmp = {
					 code: res[i].Item_Cd,
					 name: res[i].Item_Nm,
					 safeStock: res[i].Safe_Stock,
					 fitStock: [Number]};
                                        itemTmp.items.push(tmp);
                                }
                                var Items = JSON.stringify(itemTmp);
                               	socket.emit('ResponseItemsInfo',Items);
                                console.log('test');
                        }
                }
                );
                console.log("itdend");
//                DBconnection.end()

        });
	socket.on('RequestBigCategory',function ()
        {
//		DBconnection.connect();
                console.log('bstart');
                DBconnection.query('SELECT * FROM CategoryMst WHERE Level = 1'
		,function(err, res, fields)
                {
                        if(err) {
				console.log(err); }
			else{
				var bigTmp = {
					category: []
				};
				for(var i=0; i<res.length; i++){
					var tmp = { code: res[i].Category_Cd,
						 name: res[i].Category_Nm };
					bigTmp.category.push(tmp);
				}
				var Big = JSON.stringify(bigTmp);
                        	socket.emit('ResponseBigCategory',Big);
                        	console.log('test');
			}
                }
                );
                console.log("bend");
//              DBconnection.end()
        });
	socket.on('RequestMiddleCategory',function (data)
        {
//		DBconnection.connect();
                console.log('mdstart');
		console.log(data);
		var tmpData = data.substring(0,2);
                var q = "SELECT * FROM CategoryMst WHERE Level = 2 and Category_Cd like '" + tmpData + "%'";
                DBconnection.query(q
                ,function(err, res, fields)
                {
                        if(err) {
                                console.log(err); }
                        else{
                                var middleTmp = {
                                        category: []
                                };
                                for(var i=0; i<res.length; i++){
                                        var tmp = { code: res[i].Category_Cd, name: res[i].Category_Nm };
                                        middleTmp.category.push(tmp);
                                }
                                var Middle = JSON.stringify(middleTmp);
                                socket.emit('ResponseMiddleCategory',Middle);
                                console.log('test');
                        }
                }
                );
                console.log("mdend");
//                DBconnection.end()
        });	
 });
/////////////////////////DB

