<?php

	if($_SERVER['REQUEST_METHOD']==='POST'){
		
		$servername= "localhost";
		$username = "root";
		$password = "";
		$db = "iot_door";

		if(!empty($_POST['ID']))
		{
			$id = $_POST['ID'];

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
			
			$data=array();
			if($res->num_rows > 0){
				$data[] = array('Result' =>  1);
			}
			else{
				$data[] = array('Result' =>  0 );
			}
			

			$conn->close();
			echo json_encode($data);	
		}
	}







//115 87 212 2 242		192 36 231 23 20
 
 ?>