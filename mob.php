<!doctype html>
<html>
<head>
<meta http-equiv="content-type" content="text/html" charset="utf-8">

<style type="text/css">
#nav {

margin: 0px auto;
padding: 10px;
border:2px solid #999;

font-size:1.2em ;
font-style:bold;	
line-height:30px;
margin-bottom: 1px;
padding-left: 10px;

}

h1{color:#003;}
h2{color:#00F;
font-size:1.5em;}
fieldset{
	width:70%;
	height:100%:
	border-color:#999;
	font-size:0.7em;
	color:#666;
	background:#FEFEFE;
}
div{
	width:90%;
	height:80%;
	background:#CF3;
	border:thin solid #006 margin:10px 10px;
	font-size:2em;
	padding:10px;
	color:#003;
	
}
#a{
font-family:바탕체;
font-size:0.7em; 
color:#390;
}
label{width:200px;
      color:#006;
	  font-size:1.3em;
	  }
</style>


<?php
include 'mysql_selvia.php';

$sqlrec   = "select * from t_user where user_idx=1 ";

$info     = mysql_query($sqlrec) or die("쿼리실패 : ".mysql_error());


if($val=mysql_fetch_array($info))
{ 
	$user_key =$val['user_key'];
	$user_id  =$val['user_id'];
	$user_name=$val['user_name'];
	$win_count=$val['win_count'];
	$lose_count=$val['lose_count'];
	$create_date=$val['create_date'];
	$app_start_time=$val['app_start_time'];
	$app_end_time=$val['app_end_time'];
	$last_play_time=$val['last_play_time'];


}
?> 
</head>
<body>

 <fieldset>
  <legend> <h2>회원가입양식</h2></legend>
<div id="nav">
<table width="65%"border="0" cellspacing="1" cellpadding="20">

<?php
echo  "<tr>";
echo   " <td>이용자키</td>";
echo    "<td>$user_key</td>";
echo  "</tr>";
echo  "<tr>";
echo   " <td>이용자아이디</td>";
echo    "<td>$user_id</td>";
echo  "</tr>";
echo  "<tr>";
echo   " <td>이용자이름</td>";
echo    "<td>$user_name</td>";
echo  "</tr>";
echo  "<tr>";
echo   " <td>맞힌갯수</td>";
echo    "<td>$win_count</td>";
echo  "</tr>";
echo  "<tr>";
echo   " <td>틀린갯수</td>";
echo    "<td>$lose_count</td>";
echo  "</tr>";
echo  "<tr>";
echo   " <td>생성일자</td>";
echo    "<td>$create_date</td>";
echo  "</tr>";
echo  "<tr>";
echo   " <td>게임시작시간</td>";
echo    "<td>$app_start_time</td>";
echo  "</tr>";
echo  "<tr>";
echo   " <td>게임종료시간</td>";
echo    "<td>$app_end_time</td>";
echo  "</tr>";
echo  "<tr>";
echo   " <td>최종게임시간</td>";
echo    "<td>$last_play_time</td>";
echo  "</tr>";
	
?>
</table>
</div>

</fieldset>
</form>
</body>
</html>
