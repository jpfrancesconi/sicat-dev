<?php
/**
* Descripcion del Formulario
*
* @author Giudici Francisco
* @version 1.0
* @package sicat
* @subpackage socios
*/


/**
* Formulario
*/
function generar_liquidacion_form($form, &$form_state)
{
  $form['#attached']['css'] = array(
		drupal_get_path('module', 'socios') . '/css/cobranzas.css',
	);



	//buscamos los roles del usuario logueado
	$roles = get_roles_usuario_logueado();

  //generamos un fieldset que contendra los campos relacionados a la liquidacion
  $form['liquidacion'] = array(
    '#type'=>'fieldset',
    '#title'=>'PERIODO A LIQUIDAR'
  );
  //campo del form utilizado para seleccionar el mes del periodo a liquidar
  $form['liquidacion']['mes'] = array(
    '#type'=>'select',
    '#title'=>'MES',
    '#options'=>get_meses(),
    '#attributes'=>array('id'=>array('paramMes')),
  );
 //campo del form utilizado para seleccionar el mes del periodo a liquidar
  $form['liquidacion']['anio'] = array(
    '#type'=>'select',
    '#title'=>'AÑO',
    '#options'=>get_anios(),
    '#attributes'=>array('id'=>array('paramAnio')),
  );

  //agregamos un boton para generar
  $form['generar'] = array(
    '#prefix' => '<div class="btn-generar">',
    '#suffix' => '</div>',
    '#markup' => '<span>' . t('GENERAR') . '</span>',
  );

  $form['loading'] = array(
    '#prefix' => '<div id="wait">',
    '#suffix' => '</div>',
    '#markup' => '<div id="loading-wrapper"><div id="loading-sub-wrapper"><img id="loading-gif" src="../sites/all/modules/custom/socios/img/loading-bar.gif" ></img><span id="texto-gl">GENERANDO LIQUIDACION</span></div></div>',
  );

  //generamos un campo oculto donde se retornara el resultado de la generacion
  $form['mensaje_resultado'] = array(
    '#prefix' => '<div id="msj-resultado">',
    '#suffix' => '</div>',
    '#markup' => '<div id="texto-msj-resultado"><span></span></div>',
  );
  return $form;
}


function generar_liquidacion_validate($form, &$form_state)
{

}

function generar_liquidacion_submit($form, &$form_state)
{
  $uid = $GLOBALS['user']->uid;
  //$fecha_pago = date('Y').'-'.date('m').'-'.date('d');
  $mes = $form_state['values']['mes'];
  $anio = $form_state['values']['anio'];
  $mensaje='';
  //drupal_set_message('<pre>'.print_r($form_state['values'], true).'</pre>');
  $transaction = db_transaction();
  try
  {
    unset($transaction);
    if($form_state['values']['resultado_liquidacion'] == 0)
      $mensaje='SE GENERÓ LA LIQUIDACION PARA EL PERIODO</br><strong>MES: '.$mes.'</strong></br><strong>AÑO:'.$anio.'</br></strong>CORRECTAMENTE!.';
    else if($form_state['values']['resultado_liquidacion'] == 1)
      $mensaje='LA LIQUIDACION PARA EL PERIODO</br><strong>MES: '.$mes.'</strong></br><strong>AÑO:'.$anio.'</br></strong>YA EXISTE.';
    else
      $mensaje='OCURRIÓ UN ERROR LA LIQUIDACION PARA EL PERIODO</br><strong>MES: '.$mes.'</strong></br><strong>AÑO:'.$anio.'</br></strong>NO SE HA GENERADO.';

    drupal_set_message($mensaje);

  }
  catch (Exception $e) {
    $transaction->rollback();
    watchdog_exception('pago', $e);
    drupal_set_message('<pre>'. print_r("Error en la trasaccion - Comunicarse con el proveedor de sistemas.".$e, TRUE) .'</pre>');
  }
}

function generar_liquidacion_callback()
{
  //recuperamos los valores que la peticion ajax nos envia
  $mes = (isset($_GET['mes']) ? $_GET['mes'] : null);
  $anio = (isset($_GET['anio']) ? $_GET['anio'] : null);
  $uid = $GLOBALS['user']->uid;
  //vemos si alguno de los parametros es null no se puede  ejecutar el procedimiento
  if(is_null(mes) || is_null(anio))
  {
    //retornaremos un -1 en este caso como mensajes de error
    $resultado_liquidacion = '-1';
  }
  else {
    //ejecutamos el procedimiento si el resultado da 0, no hay error
    $resultado_liquidacion = generar_liquidacion_mensual($mes,$anio, $uid);
  }

  //el print es el valor que se retorna a la peticion ajax
  print $resultado_liquidacion;
  exit;
}
