<?php
/**
* Permite Registrar pagos de cuota al sistema
*
* @author Giuici Francisco
* @version 1.0
* @package sicat
* @subpackage socios
*/


/**
* Formulario que registra pagos de cuotas
*/
function registrar_pagos_form($form, &$form_state)
{
	$form['#attached']['css'] = array(
		drupal_get_path('module', 'socios') . '/css/socios.css',
	);

	$form['#attached']['js'] = array(
		drupal_get_path('module', 'socios') . '/js/registrar_pagos.js',
	);

	$disabled = false;

	$form['pago'] = array( 
		'#type'=>'fieldset',
		'#title'=>'REGISTRO DE PAGO',
	);

	$form['pago']['cod_barras'] = array( 
		'#type'=>'textfield',
		'#title'=>'CÓDIGO DE BARRAS',
		'#ajax' => array(       
			'wrapper' => 'fieldset-pago',
			'callback' => 'pago_ajax_callback',
			'method' => 'replace',
		),
	);

	$form['datos_pago'] = array(
			'#title' => t('DATOS DE CUENTA CORRIENTE'),
			'#type' => 'fieldset',
			'#prefix' => '<div id="fieldset-pago">',
			'#suffix'=>'</div>',
	);

	//$localidades = get_localidades_select();
	if(isset($form_state['values']['cod_barras']))
	{   		
		$pago = get_pago_por_cod_barras($form_state['values']['cod_barras']);

		$form['datos_pago']['id_cta_cte_item']= array(
			'#type'=>'hidden',
			'#value'=>$pago->id_cta_cte_item,
		);

		$form['datos_pago']['listado_pago'] = array(
			'#markup' => get_listado_pago_markup($pago),
		);

		if($pago->pagado == "NO")
		{
			$form['datos_pago']['confirmar'] = array(
				'#type'=>'submit',
				'#value'=>'CONFIRMAR PAGO'
			);	
		}	
		else if(isset($pago) && $pago->pagado == "SI")
		{
			$form['datos_pago']['pago_realizado'] = array(
				'#markup'=>'<div id="pago-realizado">
							LA CUOTA REQUERIDA YA SE ENCUENTRA PAGADA.
							</div>'
			);	

			$form['datos_pago']['aceptar'] = array(
				'#type'=>'submit',
				'#value'=>'ACEPTAR'
			);
		}			
		
	}

	/*$form['pago']['buscar'] = array( 
		'#type'=>'submit',
		'#name'=>'buscar_pago',
		'#value'=>'BUSCAR',
	);*/
	
	
	return $form;
}



function registrar_pagos_form_valdiate($form, &$form_state)
{
	/*if(!is_numeric($form_state['values']['importe_cuota'])){
      form_set_error('importe_cuota', 'El campo IMPORTE CUOTA es incorrecto - Solo puede contener números');
    }*/
}

function registrar_pagos_form_submit($form, &$form_state)
{
	$uid = $GLOBALS['user']->uid;	
	$id_cta_cte_item = $form_state['values']['id_cta_cte_item'];	
	
	$fecha_pago = date('Y').'-'.date('m').'-'.date('d');
	if(isset($form_state['values']['confirmar']))
	{
		$transaction = db_transaction();
		try
		{		
			//si se entro a editar se debe hacer un update de la categoria			
			db_update('io_cta_cte_item')
				->fields(array( 'pagado'=>1,
					'fecha_pago'=>$fecha_pago,				
					'usuario_mod'=>$uid,
				))
				->condition('id_cta_cte_item',$id_cta_cte_item,'=')
				->execute();
				$mensaje = 'SE REGISTRO CORRECTAMENTE EL PAGO';		

			drupal_set_message($mensaje);
			$form_state['redirect'] = array('socios/registrar_pagos');	
		}
		catch (Exception $e) {
	  		$transaction->rollback();
	  		watchdog_exception('pago', $e);
	  		drupal_set_message('<pre>'. print_r("Error en la trasaccion - Comunicarse con el proveedor de sistemas.".$e, TRUE) .'</pre>');
		}
	}
	else
	{
		$form_state['redirect'] = array('socios/registrar_pagos');
	}
	
}

function pago_ajax_callback($form, &$form_state) {
  return $form['datos_pago'];
}

function get_listado_pago_markup($pago)
{
  $pago_mostrar = array();
  
  	if($pago->pagado == "SI")
  		$color = "green";
  	else
  		$color = "red";      

  	if($pago->anulado == "SI")
  		$color_anulado = "green";
  	else
  		$color_anulado = "red";

    $pago_mostrar[0]['fecha_emision'] = $pago->fecha_emision;
    $pago_mostrar[0]['mes'] = $pago->mes;
 	$pago_mostrar[0]['anio'] = $pago->anio;
 	$pago_mostrar[0]['concepto'] = $pago->concepto;
 	$pago_mostrar[0]['valor'] = $pago->valor;
 	$pago_mostrar[0]['pagado'] = array('data'=>'<span style="color:'.$color.';"><strong>'.$pago->pagado.'</strong></span>','width'=>'15%');	
 	$pago_mostrar[0]['fecha_pago'] = $pago->fecha_pago; 	
 	$pago_mostrar[0]['anulado'] = array('data'=>'<span style="color:'.$color_anulado.';"><strong>'.$pago->anulado.'</strong></span>','width'=>'15%');	
 
 
  $header = array('FECHA DE EMISION', 'MES', 'AÑO', 'CONCEPTO', 'VALOR', 'PAGADO', 'FECHA DE PAGO', 'ANULADO');
            
 
 
  $output = theme('table', array('header' => $header, 
                   'rows' => $pago_mostrar, 
                   'attributes' => array('class' => array('tabla-pago-socio'))
                  )
           ); 

  return $output;  
}
?>