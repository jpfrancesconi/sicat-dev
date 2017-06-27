<?php
/**
* Permite Gennerar un plan de pago para un socio
*
* @author Giuici Francisco
* @version 1.0
* @package sicat
* @subpackage socios
*/


/**
* Formulario que busca la deuda de un socio para generar plan de pago
*/
function plan_pago_socio_form($form, &$form_state)
{
  //adjuntamos el css
  $form['#attached']['css'] = array(
		drupal_get_path('module', 'socios') . '/css/registrar_pagos.css',
	);

  $form['#attached']['js'] = array(
		drupal_get_path('module', 'generico') . '/js/imprimir.js',
    drupal_get_path('module', 'socios') . '/js/plan_pago_socio.js',
	);

  //seteamos el lugar al que se debe retrnar y los parametros
  if(strpos($_SERVER['HTTP_REFERER'], 'cobranza'))
	{
		$array_retorno = parse_url($_SERVER['HTTP_REFERER']);
		$_SESSION['retorno'] = 'socios/cobranza';
		$_SESSION['retorno_params'] =array('query' =>get_params_url($_SERVER['HTTP_REFERER']));
	}
  //recibimos como parametro el id del socio para el que se generará el plan de pago
  if(!empty($_GET['id_s']) && isset($_GET['id_s']))
  {
    $id_socio = $_GET['id_s'];

    //guardamos en un campo hidden el id del socio
    $form['id_socio'] = array(
      '#type'=>'hidden',
      '#value'=>$id_socio
    );

    //buscamos los datos del socio
    $socio = get_socio_por_id($id_socio);
    //buscamos la deuda del socio
    $deuda = consultar_deuda($id_socio, $socio->tipo_socio_id);

    //guardamos el array de la deuda para usarlo luego en el submit del formualario
    $_SESSION['deuda'] = $deuda;
    $_SESSION['socio'] = $socio;

    //generamos un item hidden con los datos de la deuda para imprimir
    $meses = array("Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio", "Julio", "Agosto", "Septiembre", "Octumbre", "Noviembre", "Diciembre");
    $texto_mes_anio="";
    foreach ($deuda as $item_deuda) {
      $texto_mes_anio=$texto_mes_anio.$meses[$item_deuda->mes-1].' de '.$item_deuda->anio.', ';
    }
    //el siguiente valor oculto se usa en la vista de impresion
    $form['texto_mes_anio'] = array(
      '#type'=>'hidden',
      '#value'=>$texto_mes_anio,
      '#attributes'=>array('id'=>array('texto-mes-anio'))
    );
    //el siguiente valor oculto se usa en la vista de impresion
    $form['tipo_socio'] = array(
      '#type'=>'hidden',
      '#value'=>$socio->tipo_socio_id,
      '#attributes'=>array('id'=>array('tipo-socio-id'))
    );


    //revisamos la categoria y tipo de socio
    if($socio->tipo_socio_id == 1){//si el socio es socio unico mostramos su categoria
      $categoria_tipo_grupo_familiar = '<div class="rp-socio" id="rp-categoria"><strong>CATEGORIA: </strong><span id="valor-categoria">'.$socio->categoria.'</span></div>';
    }
    else if($socio->tipo_socio_id == 3) {//si el socio es socio titular de familia mostramos el tipo de grupo familiar
      $grupo_familiar = get_grupo_familiar($socio->id_socio);
      $categoria_tipo_grupo_familiar = '<div class="rp-socio" id="rp-categoria"><strong>TIPO DE GRUPO FAMILIAR: </strong><span id="valor-categoria">'.$grupo_familiar->nombre_tipo_grupo_familiar.'</span></div>';
    }

    //creamos un fieldset que contendra al Formulario
    $form['plan_pago'] = array(
      '#type'=>'fieldset',
      '#title'=>'PLAN DE PAGO PARA EL SOCIO'
    );
    //mostramos los datos del socio
    $form['plan_pago']['datos_socio']['socio'] = array(
      '#markup' =>'
      <div id="rp-datos-socio">
      <div class="rp-socio" id="rp-nro-socio"><strong>NUMERO DE SOCIO: </strong><span id="valor-nro-socio">'.$socio->nro_socio.'</span>/'.$socio->secuencia.'</div>
      <div class="rp-socio" id="rp-apellidos"><strong>APELLIDOS: </strong><span id="valor-nro-apellidos">'.$socio->apellidos.'</span></div>
      <div class="rp-socio" id="rp-nombres"><strong>NOMBRES: </strong><span id="valor-nro-nombres">'.$socio->nombres.'</span></div>
      <div class="rp-socio" id="rp-tipo-socio"><strong>TIPO DE SOCIO: </strong>'.$socio->tipo_socio.'</div>
      <div class="rp-socio" id="rp-estado"><strong>ESTADO: </strong>'.$socio->estado.'</div>'.
      $categoria_tipo_grupo_familiar.'
      </div>',
    );

    //creamos un fieldset para mostrar la deuda detallada del socio
    $form['plan_pago']['deuda_socio'] = array(
      '#type'=>'fieldset',
      '#title'=>'DEUDA DEL SOCIO'
    );

    $form['plan_pago']['deuda_socio']['plan_de_pago_table'] = array(
      '#markup' => deuda_table_markup($deuda),
    );
  }
  else {
    //si no vino ningun id de socio entonces solo se puede volver atras y se muestra un mensaje
    drupal_set_message('NO SE HA SELECCIONADO NINGUN SOCIO');
  }

  $form['plan_pago']['cant_cuotas'] = array(
    '#type'=>'select',
    '#title'=>'CANTIDAD DE CUOTAS',
    '#options'=>array(1=>1,2=>2,3=>3,4=>4,5=>5,6=>6,7=>7,8=>8,9=>9,10=>10,11=>11,12=>12),
    '#required'=>true,
    '#attributes'=>array('id'=>array('cant-cuotas'), 'onchange'=>"myFunction()")
  );
  //calculamos el total de la dueda
  $total_deuda = calcular_total_deuda($deuda);

  $form['plan_pago']['total_deuda'] = array(
    '#type'=>'textfield',
    '#title'=>'TOTAL A REFINANCIAR',
    '#field_prefix'=>'$',
    '#disabled'=>TRUE,
    '#default_value'=>$total_deuda,
    '#attributes'=>array('id'=>array('total-deuda'))
  );

  $form['plan_pago']['valor_cuota'] = array(
    '#type'=>'textfield',
    '#title'=>'VALOR DE LA CUOTA',
    '#field_prefix'=>'$',
    '#disabled'=>TRUE,
    '#attributes'=>array('id'=>array('valor-cuota')),
  );

  $form['plan_pago']['registrar_plan_pago'] = array(
    '#type'=>'submit',
    '#value'=>'REGISTRAR PLAN DE PAGO',
    //'#executes_submit_callback'=>false,
    '#submit'=>array('registrar_plan_pago_form'),
    '#attributes'=>array('onclick'=>"PrintAcuerdoPlanPago('texto-mes-anio', 'cant-cuotas','total-deuda')")
  );

  $form['plan_pago']['volver'] = array(
    '#type'=>'submit',
    '#value'=>'VOLVER',
    '#limit_validation_errors'=>array(),
    '#submit'=>array('plan_pago_socio_volver_form'),
  );

  /*$form['plan_pago']['imprimir'] = array(
    '#markup'=>'<div id="imprimir"><a href="#" onclick="PrintElem(\'texto-mes-anio\', \'cant-cuotas\',\'total-deuda\')">IMPRIMIR</a></div>',
  );*/
  return $form;
}

function plan_pago_socio_volver_form($form, &$form_state)
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

function registrar_plan_pago_form($form, &$form_state)
{
  $transaction = db_transaction();
  try{
    //recuperamos el id del socio
    $uid = $GLOBALS['user']->uid;
    //recuperamos la deuda del socio y limpiamos la variable de sesion
    $deuda = $_SESSION['deuda'];
    unset($_SESSION['deuda']);
    //limpiamos el socio tambien porque ya no lo usaremos
    unset($_SESSION['socio']);

    $socio_id = $form_state['values']['id_socio'];

    //calculamos el monto de las CUOTAS teniendo en cuenta que la ultima cuota sera un calculo diferente
    $cant_cuotas = $form_state['values']['cant_cuotas'];

    $monto_base = $form_state['values']['total_deuda'];
    $monto_base = str_replace(',', '', $monto_base);
    $monto_a_devolver = $form_state['values']['total_deuda'];
    $monto_a_devolver = str_replace(',', '', $monto_a_devolver);
    $interes = 0;

    //generamos el plan de pago en activo 1
    $id_plan_pago = db_insert('io_plan_pago')
    ->fields(array( 'monto_base'=>$monto_base,
    	'monto_a_devolver'=>$monto_a_devolver,
      'interes'=>$interes,
      'socio_id'=>$socio_id,
    	'usuario_mod'=>$uid,
    	'usuario_alta'=>$uid,
      'activo'=>1,
    ))
    ->execute();

    //ahora que ya tenemos el plan de pago creado calculamos el valor de cada cuota

    $valor_cuota = number_format(($monto_a_devolver / $cant_cuotas), 2);
    $valor_cuota = str_replace(',', '', $valor_cuota);
    //vamos sumando en un subtotal lo que acumula cada cuota
    $subtotal = 0;
    for($i=0;$i<$cant_cuotas-1;$i++)
    {
      //se pega en la base la cuota del plan de pago
      db_insert('io_cuota_plan_pago')
      ->fields(array( 'monto'=> $valor_cuota,
      	'plan_pago_id'=>$id_plan_pago,
      	'usuario_mod'=>$uid,
      	'usuario_alta'=>$uid,
      ))
      ->execute();
      //sumamos en el subtotal lo que acumula cada cuota
      $subtotal += $valor_cuota;
    }
    $subtotal = str_replace(',', '', $subtotal);
    //la ultima cuota es la resta del total_deuda - el subtotal
    //esto se hace por una cuestion de redondeo
    $ultima_cuota = $monto_a_devolver - $subtotal;
    $ultima_cuota = str_replace(',', '', $ultima_cuota);

    //se pega en la base la ultima cuota del plan de pago
    db_insert('io_cuota_plan_pago')
    ->fields(array( 'monto'=> $ultima_cuota,
      'plan_pago_id'=>$id_plan_pago,
      'usuario_mod'=>$uid,
      'usuario_alta'=>$uid,
    ))
    ->execute();

    //vincular todos los items de la cuenta corriente con con el plan de pago
    //actualizamos el item y le damos el valor 1 a refinanciada
    foreach($deuda as $item_deuda)
    {
      db_insert('io_item_plan_pago')
      ->fields(array('plan_pago_id'=>$id_plan_pago,
                     'cta_cte_item_id'=>$item_deuda->id_cta_cte_item))
      ->execute();

      db_update('io_cta_cte_item')
      ->fields(array('refinanciado'=>1))
      ->condition('id_cta_cte_item', $item_deuda->id_cta_cte_item, '=')
      ->execute();
    }
    drupal_set_message('El plan de pago para el socio se generó correctamente!');
    plan_pago_socio_volver_form($form, $form_state);
  }
  catch (Exception $e) {
  		$transaction->rollback();
  		watchdog_exception('plan_pago_socio', $e);
  		drupal_set_message('<pre>'. print_r("Error en la trasaccion - Comunicarse con el proveedor de sistemas.".$e, TRUE) .'</pre>');
	}

}

//genera la tabla donde se muestra la deuda del socio
function deuda_table_markup($deuda_socio)
{
  $deuda_mostrar = array();
  $i=0;
  foreach ($deuda_socio as $deuda)
  {
    $deuda_mostrar[$i]['fecha_emision'] = $deuda->fecha_emision;
    $deuda_mostrar[$i]['mes'] = $deuda->mes;
    $deuda_mostrar[$i]['anio'] = $deuda->anio;
    $deuda_mostrar[$i]['concepto'] = $deuda->nombre;
    $deuda_mostrar[$i]['valor'] = '$ '.number_format($deuda->valor,2);
    $deuda_mostrar[$i]['valor_actualizado'] = '$ '.number_format($deuda->valor_actual,2);
    $i++;
  }

  //$cuota_plan_mostrar[$ultima_cuota]['monto'] = '$ '.number_format(($monto_devolver-$monto_subtotal),2);
  $header = array(t('FECHA DE EMISION'),t('MES'), t('AÑO'), t('CONCEPTO'), t('VALOR'), t('VALOR ACTUALIZADO'));

  $output = theme('table', array('header' => $header, 'rows' => $deuda_mostrar, 'class'=>'listado-deuda-socio'));

  return $output;
}

//calcula el tatal de la deuda a refinanciar
function calcular_total_deuda($deuda_socio)
{
  $total = 0;

  foreach ($deuda_socio as $deuda) {
    $total += $deuda->valor_actual;
  }

  $total = number_format($total,2);
  $total = str_replace(',', '', $total);
  return $total;
}
?>
