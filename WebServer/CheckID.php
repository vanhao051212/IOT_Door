<?php

	if($_SERVER['REQUEST_METHOD']==='POST'){
		$servername= "localhost";
		$username = "root";
		$password = "";
		$db = "iot_door";

		if(!empty($_POST['name']))
		{
			$id = $_POST['name'];

			$conn= new mysqli("localhost", $username, $password, $db );
			if ($conn->connect_error) {
				die("Connection failed: " . $conn->connect_error);
			}


			$sql = "SELECT ID from  gv where ID ='$id' ";
			 
			$res = $conn->query($sql);

			if(!$res){
				die("ERROR".$conn->connect_error);
				exit();
			}
			if($res->num_rows > 0){
				echo "ID right";
			}
			else{
				echo "ID wrong";
			}
			

			$conn->close();
			//echo json_encode($data);	
		}
		
	}







//115 87 212 2 242		192 36 231 23 20
 
 ?>