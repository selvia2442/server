<<!doctype html>
<html>
<head>
<meta charset="utf-8">
<title>배달이로그인</title>

</head>

<body>

<?php

include 'mysql_selvia.php';


	$user_id 		=$_POST['user_id'];
	$user_name      =$_POST['user_name'];
	$user_img    	=$_POST['user_img'];
	$win_count      =$_POST['win_count'];
	$lose_count     =$_POST['lose_count'];
	$create_date    =$_POST['create_date'];
	$app_start_time =$_POST['app_start_time'];
	$app_end_time   =$_POST['app_end_time'];
	$last_play_time =$_POST['last_play_time'];

?> 

<?php

$sqlrec   = "insert into t_user set 
user_id='$user_id',user_name='$user_name',user_img='$user_img',win_count='$win_count',lose_count='$lose_count',create_date='$create_date',app_start_time='$app_start_time',app_end_time='$app_end_time',last_play_time='$last_play_time'";

$info=mysql_query($sqlrec,$connect);

	if(!$info)
	{
		echo "<script>alert('$user_id 님 실패.');history.back();</script>";exit;
	}
	


?>
 <a href='user_list.php'><h2>등록성공</h2></a></td>";

</body>
</html>