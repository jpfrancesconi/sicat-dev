<?php
/**
* Set de funciones utiles para uso general del proyecto
*
* @author Giuici Francisco
* @version 1.0
* @package sicat
* @subpackage generico
*/


/**
* Formulario que gestionar socios
*/
function get_fecha_mostrar($fecha)
{
	$year=substr($fecha,0,4);
	$month=substr($fecha,5,2); 
	$day=substr($fecha,8,2);
	$fecha_mostrar=array('year'=>$year,'month'=>ltrim($month,'0'),'day'=>ltrim($day,'0'));

	return $fecha_mostrar;
}

function get_params_url($url)
{
	$params = array();
	$flag1 = false;
	$mas_parametros = true;

	$pos_params = stripos($url, '?');
	
	//si no tiene parametros retorna un array vacio
	if(empty($pos_params))
		return array();


	$url_params = substr($url, $pos_params+1, strlen($url)-1);

	
	while($mas_parametros)
	{
		//buscamos la posicion el siguiente paramtro y si existe & es porque hay un siguiente y es necesario saberlo para ir hasta esa posocion sino sera ya el ultimo parametro
		
		$pos_params = stripos($url_params,'=');
		
		$pos_sig = stripos($url_params,'&');
		
		$param = substr($url_params, 0, $pos_params);
		//drupal_set_message('url param:'.$url_params.'</br>pos param: '.$pos_params.'</br>pos sig: '.$pos_sig);

		if($pos_sig)
		{
			$pos = $pos_sig;
		}
		else
		{			
			$pos = strlen($url_params);
			$mas_parametros = false;
		}
		
		$valor = substr($url_params, $pos_params+1, ($pos-$pos_params-1));
		$params[$param]=$valor;

		$url_params = substr($url_params, $pos+1, strlen($url_params));
		//drupal_set_message('<pre>'.print_r($url_params, true).'</pre>');		
	}

	//drupal_set_message('<pre>'.print_r($params, true).'</pre>');
	return $params;
		
}


?>