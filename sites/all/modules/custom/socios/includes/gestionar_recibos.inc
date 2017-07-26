<?php
/**
* El formulario se utiliza para poder gestionar los recibos de un socio. Desde aqui se podra ver/reimprimir y anular los recibos
*
* @author Giudici Francisco
* @version 1.0
* @package sicat
* @subpackage socios
*/


function gestionar_recibos_form($form, &$form_state)
{
  $form['#attached']['css'] = array(
		drupal_get_path('module', 'socios') . '/css/socios.css',
	);

	//seteamos el lugar al que se debe retrnar y los parametros
  if(strpos($_SERVER['HTTP_REFERER'], 'gestionar_socios'))
	{
		$array_retorno = parse_url($_SERVER['HTTP_REFERER']);
		$_SESSION['retorno'] = 'socios/gestionar_socios';
		$_SESSION['retorno_params'] =array('query' =>get_params_url($_SERVER['HTTP_REFERER']));
	}

  //recibimos los parametros pasados, solo si existen los parametros y no son nulos mostramos el formulario sino
  //mostraremos un mensaje y el boton volver
  if(isset($_GET['id_s']) && !is_null($_GET['id_s']))
  {
    $id_socio = $_GET['id_s'];

    //creamos una variable $estado como default_value de estado
    $estado = 0;
    //vemos si tambien vino como parametro el estado del recibo est
    if(isset($_GET['est']) && !is_null($_GET['est']))
    {
      //guardamos el estado
      $estado = $_GET['est'];
    }

    //guardamos el id_socio en un campo tipo hidden
    $form['id_socio'] = array(
      '#type'=>'hidden',
      '#value'=>$id_socio,
    );
    //buscamos los roles del usuario logueado
  	$roles = get_roles_usuario_logueado();

    //vemos si el usuario tiene el rol control para permitir anular o no recibos
    $user_anula = false;
    if(es_usr_secretaria_control($roles))
    {
      $user_anula = true;
    }
    //TODO mostrar los datos del socio
    /*
    $socio = get_socio_por_id
    $form['datos_socio'] = array(
      '#markup'=>'<div id="datos_socio">
                    <div id=
                  </div>'
    );*/
    
    //fieldset para los filtros de busqueda
    $form['filtros_recibos'] = array(
      '#type'=>'fieldset',
      '#title'=>t('FILTRAR'),
      '#collapsible' => TRUE
    );

    //buscamos los posibles estados de los recibos
    $estados_recibos = get_estados_recibos_select("--TODOS--");
    $form['filtros_recibos']['estados'] = array(
      '#type'=>'select',
      '#title'=>'ESTADO',
      '#options'=>$estados_recibos,
      '#default_value'=>$estado,
    );

    $form['filtros_recibos']['buscar_estados'] = array(
      '#type'=>'submit',
      '#value'=>'BUSCAR',
      '#submit'=>array('buscar_recibos_submit'),
    );

    //fieldset para mostrar los recibos del socio
    $form['recibos_socio'] = array(
      '#type'=>'fieldset',
      '#title'=>t('RECIBOS')
    );

     $form['recibos_socio']['recibos'] = array(
      '#markup'=>get_listado_recibos_markup($id_socio, $estado, $user_anula),
    );
  }

  $form['plan_pago']['volver'] = array(
    '#type'=>'submit',
    '#value'=>'VOLVER',
    '#limit_validation_errors'=>array(),
    '#submit'=>array('gestionar_recibos_volver_form'),
  );

  return $form;
}

function buscar_recibos_submit($form, &$form_state)
{
  //recuperamos el id del socio
  $id_socio = $form_state['values']['id_socio'];
  //vemos si se selecciono algun estado que no sea 0, ya que 0 representa todos
  if(isset($form_state['values']['estados']) && ($form_state['values']['estados'] != 0))
  {
    $estado_seleccionado = $form_state['values']['estados'];
    //volvemos a llamar al formulario pasando como parametros el id socio y el estado seleccionado
    $form_state['redirect'] = array('socios/recibos',array('query' => array('id_s'=>$id_socio, 'est'=>$estado_seleccionado)));
  }
  else {//caso contrario esta seleccionado 0, o sea TODOS
    $form_state['redirect'] = array('socios/recibos',array('query' => array('id_s'=>$id_socio)));
  }
}

function gestionar_recibos_validate($form, &$form_state)
{
}

function gestionar_recibos_submit($form, &$form_state)
{
  $uid = $GLOBALS['user']->uid;
  //$fecha_pago = date('Y').'-'.date('m').'-'.date('d');
  //drupal_set_message('<pre>'.print_r($form_state['values'], true).'</pre>');
  $transaction = db_transaction();
  try
  {
    unset($transaction);
    drupal_set_message($mensaje);
    $form_state['redirect'] = array('socios/gestionar_socios');
  }
  catch (Exception $e) {
    $transaction->rollback();
    watchdog_exception('pago', $e);
    drupal_set_message('<pre>'. print_r("Error en la trasaccion - Comunicarse con el proveedor de sistemas.", TRUE) .'</pre>');
  }
}

function gestionar_recibos_volver_form($form, &$form_state)
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

function get_listado_recibos_markup($id_socio, $estado, $user_anula)
{
  $recibos = array();
  $recibos_mostrar = array();
  //si estado es 0 lo hacemos null porque vamos a recuperar todos los estados de los recibos
  if($estado == 0)
    $estado = null;
  //Recuperamos los integrantes del grupo o cooperativa
  $recibos = get_recibos_socio($id_socio, $estado);

  if(count($recibos) == 0)
  {
    $output = t('No se encontraron recibos para el socio');
  }
  else
  {
    //ahora recorremos todos los recibos y recuperamos los datos que usamos para mostrar que usaremos
    for ($i=0;$i<count($recibos);$i++) {
      //si el estado es anulado le damos la clases recibo-anulado sino recibo-no-anulado
      $clase_estado = ($recibos[$i]->id_estado_recibo == 2 ? 'recibo-anulado' : 'recibo-no-anulado');
      $recibos_mostrar[$i]['fecha'] = $recibos[$i]->fecha_emision;
      $recibos_mostrar[$i]['estado'] = '<span class="'.$clase_estado.'">'.$recibos[$i]->estado.'</span>';
      $recibos_mostrar[$i]['importe_total'] = $recibos[$i]->importe_total;
      $recibos_mostrar[$i]['ver'] = '<a href="">VER/IMPRIMIR</a>';
      //se permite anular solo si el recibo no esta previamente anulado y el usuario tiene el rol control
      if($recibos[$i]->id_estado_recibo != 2 && $user_anula)
      {
        $recibos_mostrar[$i]['anular'] = l('ANULAR', 'socios/anular_recibo', array('query' => array('id_r' => $recibos[$i]->id_recibo)));
      }
      else {
        $recibos_mostrar[$i]['anular'] = '-';
      }
    }
  }

  $header = array(t('FECHA'),t('ESTADO'), t('IMPORTE TOTAL ($)'), array('data'=>t('OPERACIONES'),'colspan'=>2));

  $output = theme('table', array('header' => $header, 'rows' => $recibos_mostrar));

  return $output;
}

function anular_recibo_form($form, &$form_state)
{
  $form['#attached']['css'] = array(
		drupal_get_path('module', 'socios') . '/css/socios.css',
	);

	//seteamos el lugar al que se debe retrnar y los parametros
  if(strpos($_SERVER['HTTP_REFERER'], 'recibos'))
	{
		$array_retorno = parse_url($_SERVER['HTTP_REFERER']);
		$_SESSION['retorno'] = 'socios/recibos';
		$_SESSION['retorno_params'] =array('query' =>get_params_url($_SERVER['HTTP_REFERER']));
	}

  //recibimos los parametros pasados, solo si existen los parametros y no son nulos mostramos el formulario sino
  //mostraremos un mensaje y el boton volver
  if(isset($_GET['id_r']) && !is_null($_GET['id_r']))
  {
    //guardamos el id_recibo
    $id_recibo = $_GET['id_r'];
    //y lo guardamos en un campo tipo hidden
    $form['id_recibo'] = array(
      '#type'=>'hidden',
      '#value'=>$id_recibo,
    );

    $form['anulacion_recibo'] = array(
      '#type'=>'fieldset',
      '#title'=>'DETALLE DEL RECIBO',
    );

    $form['anulacion_recibo']['recibo'] = array(
      '#markup'=>get_listado_items_recibo($id_recibo)
    );

    $form['motivo_anulacion'] = array(
      '#type'=>'textarea',
      '#title'=>'Motivo de anulación',
      '#required'=>true,
      '#resizable'=>false,
      '#rows'=>3,
      '#description'=>'Motivo por el cual se está anulando el recibo'
    );

    $form['anular_recibo'] = array(
      '#type'=>'submit',
      '#value'=>'ANULAR RECIBO',
      '#submit'=>array('anular_recibo_form_submit'),
      '#attributes' => array('onclick' => 'if(!confirm("Esta seguro que desea anular el recibo?")){return false;}'),
    );
  }

  $form['cancelar'] = array(
    '#type'=>'submit',
    '#value'=>'CANCELAR',
    '#limit_validation_errors'=>array(),
    '#submit'=>array('anular_recibos_volver_form'),
  );
  return $form;

}

function anular_recibo_form_submit($form, &$form_state)
{
  $uid = $GLOBALS['user']->uid;
  $id_recibo = $form_state['values']['id_recibo'];
  $motivo_anulacion = $form_state['values']['motivo_anulacion'];

  $transaction = db_transaction();
  try
  {
    //ejecutamos la fucion que anula el recibo
    anular_recibo($id_recibo, $motivo_anulacion);
    $mensaje = 'EL RECIBO FUE ANULADO CORRECTAMENTE';
    unset($transaction);
    drupal_set_message($mensaje);
    anular_recibos_volver_form($form, $form_state);
  }
  catch (Exception $e) {
    $transaction->rollback();
    watchdog_exception('pago', $e);
    drupal_set_message('<pre>'. print_r("Error en la trasaccion - Comunicarse con el proveedor de sistemas.", TRUE) .'</pre>');
  }

}

function get_listado_items_recibo($id_recibo)
{
  $items_recibo = array();
  $items_recibo_mostrar = array();

  //Recuperamos los integrantes del grupo o cooperativa
  $items_recibo = get_items_recibo($id_recibo);

  if(count($items_recibo) == 0)
  {
    $output = t('No se encontraron recibos para el socio');
  }
  else
  {
    //ahora recorremos todos los items del recibo y recuperamos los datos que usamos para mostrar que usaremos
    for ($i=0;$i<count($items_recibo);$i++) {
      $items_recibo_mostrar[$i]['fecha'] = $items_recibo[$i]->fecha;
      $items_recibo_mostrar[$i]['concepto'] = $items_recibo[$i]->concepto;
      $items_recibo_mostrar[$i]['importe'] = $items_recibo[$i]->importe;
    }
  }

  $header = array(t('FECHA'),t('CONCEPTO'), t('IMPORTE ($)'));

  $output = theme('table', array('header' => $header, 'rows' => $items_recibo_mostrar));

  return $output;
}

function anular_recibos_volver_form($form, &$form_state)
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
