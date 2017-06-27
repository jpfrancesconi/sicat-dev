<?php
/**
* Este Formulario se utiliza para poder imprimir el resumen de cuenta del socio
*
* @author Giudici Francisco
* @version 1.0
* @package sicat
* @subpackage socios
*/


/**
* Formulario
*/
function imprimir_cuenta_socio_form($form, &$form_state)
{
  $form['#attached']['css'] = array(
		drupal_get_path('module', 'socios') . '/css/socios.css',
	);

  $form['#attached']['js'] = array(
		drupal_get_path('module', 'generico') . '/js/imprimir.js',
	);
	//seteamos el lugar al que se debe retornar y los parametros
  if(strpos($_SERVER['HTTP_REFERER'], 'registrar_pago'))
	{
		$array_retorno = parse_url($_SERVER['HTTP_REFERER']);
		$_SESSION['retorno'] = 'socios/registrar_pago';
		$_SESSION['retorno_params'] =array('query' =>get_params_url($_SERVER['HTTP_REFERER']));
	}

	//buscamos los roles del usuario logueado
	$roles = get_roles_usuario_logueado();

  //recuperamos de session los items pagados
  //$id_items_cta_cte = $_SESSION['item_pagado'];
  $id_recibo = $_SESSION['id_recibo'];
  //limpiamos los datos de session
  //unset($_SESSION['item_pagado']);
  unset($_SESSION['id_recibo']);

  //buscamos los items de cuenta corriente vinculados al recibo
  $items_pagados = get_items_cta_cte_por_recibo($id_recibo);
  //guardamos en este array los datos de pagos realizados pensando en lo necesario para imprimir
  $pagos = array();
  //sumaremos el total a pagado por el socio
  $total = 0;
  //Recuperamos los datos del socio y de cada item de ceunta corriente pagados
  $i=0;
  foreach ($items_pagados as $item_pagado) {
    $datos_socio_pago = get_datos_socio_pago_por_item_cc($item_pagado->id_cta_cte_item);
    //vamos sumando en la variable total el valor de cada concepto que se pagó
    $total+=$datos_socio_pago->valor;
    //le damos el formato correcto al valor de la cuota
    $datos_socio_pago->valor = number_format($datos_socio_pago->valor, 2, ".", "");

    $pagos[$i] = array('fecha_emision'=>$datos_socio_pago->fecha_emision, 'mes'=>$datos_socio_pago->mes, 'anio'=>$datos_socio_pago->anio, 'concepto'=>$datos_socio_pago->concepto, 'valor'=>$datos_socio_pago->valor);
    $i++;
  }
  //le damos el formato correcto al total
  $total = number_format($total, 2, ".", "");

  //convertirmos el array de pagos a un objeto json que se guarda como un valor en un campo hidden mas abajo
  $pagos = json_encode($pagos);
  //usaremos campos en el formulario de tipo hidden para poder guardar los valores que se consumen al momento de impirmir
  //se les asigna un id a cada campo para poder recuperarlo desde el archivo javaScript a través del mismo. Se hizo asi
  //porque resulto mas simple que pasar todo por parametros
  $form['resumen_pago']['nro_socio'] = array('#type'=>'hidden', '#value'=>$datos_socio_pago->nro_socio, '#attributes'=>array('id'=>'nro_socio'));
  $form['resumen_pago']['direccion'] = array('#type'=>'hidden', '#value'=>$datos_socio_pago->direccion, '#attributes'=>array('id'=>'direccion'));
  $form['resumen_pago']['apellidos'] = array('#type'=>'hidden', '#value'=>$datos_socio_pago->apellidos, '#attributes'=>array('id'=>'apellidos'));
  $form['resumen_pago']['nombres'] = array('#type'=>'hidden', '#value'=>$datos_socio_pago->nombres, '#attributes'=>array('id'=>'nombres'));
  $form['resumen_pago']['tipo_socio_id'] = array('#type'=>'hidden', '#value'=>$datos_socio_pago->tipo_socio_id, '#attributes'=>array('id'=>'tipo_socio_id'));
  $form['resumen_pago']['cate_tgf'] = array('#type'=>'hidden', '#value'=>$datos_socio_pago->cate_tgf, '#attributes'=>array('id'=>'cate_tgf'));
  $form['resumen_pago']['pagos'] = array('#type'=>'hidden', '#value'=>$pagos, '#attributes'=>array('id'=>'pagos'));
  $form['resumen_pago']['total'] = array('#type'=>'hidden', '#value'=>$total, '#attributes'=>array('id'=>'total'));
  //creamos un link que tiene forma de boton (la forma se la damos por css) que llama a la funcion que imprimie
  $form['resumen_pago']['imprimir'] = array(
    '#markup'=>'<a href="#" onclick="PrintResumenPago();"> IMPRIMIR RESUMEN DEL PAGO</a>'
  );

  $form['resumen_pago']['volver'] = array(
    '#type'=>'submit',
    '#value'=>'VOLVER',
    '#limit_validation_errors'=>array(),
    '#submit'=>array('imprimir_cuenta_socio_volver_form'),
  );

  return $form;
}


function imprimir_cuenta_socio_validate($form, &$form_state)
{
}

function imprimir_cuenta_socio_submit($form, &$form_state)
{

}

function imprimir_cuenta_socio_volver_form($form, &$form_state)
{
  $retorno = array($_SESSION['retorno']);
  unset($_SESSION['retorno']);
  if(isset($_SESSION['retorno_params']))
  {
    array_push($retorno, $_SESSION['retorno_params']);
    unset($_SESSION['retorno_params']);
  }
  //drupal_set_message('<pre>'.print_r($retorno, true).'</pre>');
  $form_state['redirect'] = $retorno;
}
