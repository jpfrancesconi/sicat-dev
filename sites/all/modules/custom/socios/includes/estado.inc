<?php
/**
* Permite Gestionar los estado del socio
*
* @author Giuici Francisco
* @version 1.0
* @package sicat
* @subpackage socios
*/


/**
* Formulario que gestionar estados del socio
*/
function estado_form($form, &$form_state)
{
	if(strpos($_SERVER['HTTP_REFERER'], 'gestionar_estados_socio'))
	{
		$array_retorno = parse_url($_SERVER['HTTP_REFERER']);
		$_SESSION['retorno'] = 'socios/gestionar_estados_socio';    
		$_SESSION['retorno_params'] =array('query' =>get_params_url($_SERVER['HTTP_REFERER']));
	}

	$disabled = false;

	if(isset($_GET['id']))
	{
		$id_estado_socio = $_GET['id'];

		$form['id_estado_socio'] = array(
		  '#type' => 'hidden', 
		  '#value' => $id_estado_socio,
		);

		$estado = get_estado_por_id($id_estado_socio);	
		
		//si se entro para editar se ingreso para editar o ver
		if(isset($_GET['edit']))
		{
			$editar = $_GET['edit'];
			$form['editar'] = array(
			  '#type' => 'hidden', 
			  '#value' => $editar,
			);
			//si editar es 0 se ingreso para ver caso contrario se ingreso para editar
			if($editar == 0)
			{
				//ponemos la bandera disabled en true para que todos los campos esten deshabilitados
				$disabled = true;
			}					
		}	

		$texto_titulo = 'ESTADO: '.$estado->nombre;
	}
	else
	{
		$texto_titulo = 'NUEVO ESTADO';
	}

	$form['estado'] = array( 
		'#type'=>'fieldset',
		'#title'=>$texto_titulo,
	);

	$form['estado']['nombre'] = array( 
		'#type'=>'textfield',
		'#title'=>'NOMBRE',
		'#disabled'=>$disabled,
		'#default_value'=>((empty($estado->nombre)||is_null($estado->nombre)) ? null : $estado->nombre),
	);

	$form['estado']['cant_cuota_adeuda'] = array( 
		'#type'=>'numberfield',
		'#title'=>'CANTIDAD DE CUOTAS QUE ADEUDA',
		'#disabled'=>$disabled,
		'#default_value'=>((empty($estado->cant_cuota_adeuda)||is_null($estado->cant_cuota_adeuda)) ? 0 : $estado->cant_cuota_adeuda),
		'#description'=>'Cantidad maxima de cuotas se pueden adeudar para mantener activo el socio.',
		'#field_suffix'=>'Cuotas',
	);

	$form['estado']['actualiza_estado'] = array( 
		'#type'=>'checkbox',
		'#title'=>'ACTUALIZA ESTADO',
		'#disabled'=>$disabled,
		'#default_value'=>((empty($estado->actualiza_estado)||is_null($estado->actualiza_estado)) ? null : $estado->actualiza_estado),
		'#description'=>'Actualiza el estado al estado posterior seleccionado automaticamente cuando se supera el valor de CANTIDAD DE CUOTAS QUE ADEUDA ',
	);

	$estados_pos = get_estados_socio_select('--NINGUNO--');
	$form['estado']['estado_posterior'] = array(
		'#title' => t('ESTADO POSTERIOR'),
	    '#type' => 'select', 
	    '#options' => $estados_pos,
	    '#default_value'=>((empty($estado->estado_posterior)||is_null($estado->estado_posterior)) ? '':$estado->estado_posterior),
	    '#disabled'=>$disabled,
	    '#states'=>array(
	    	'visible'=>array(':input[name="actualiza_estado"]' => array('checked' => TRUE))
	    	),
	    '#description'=>'Indica el estado al que pasará el usuario luego de actualizar ',
	);

	$estados_ant = get_estados_socio_select('ninguno');
	$form['estado']['estado_anterior'] = array(
		'#title' => t('ESTADO POSTERIOR'),
	    '#type' => 'select', 
	    '#options' => $estados_ant,
	    '#default_value'=>((empty($estado->estado_anterior)||is_null($estado->estado_anterior)) ? '':$estado->estado_anterior),
	    '#disabled'=>$disabled,
	    '#states'=>array(
	    	'visible'=>array(':input[name="actualiza_estado"]' => array('checked' => TRUE))
	    	),
	    '#description'=>'Indica el estado al que pasará el usuario luego de actualizar ',
	);

	$form['estado']['cuota'] = array( 
		'#type'=>'numberfield',
		'#title'=>'PORCETAJE DE CUOTA',
		'#disabled'=>$disabled,
		'#default_value'=>((empty($estado->cuota)||is_null($estado->cuota)) ? 100 : $estado->cuota),
		'#description'=>'Porcentaje de la cuota que abona.',
		'#field_suffix'=>'%',
	);

	$form['estado']['tipo'] = array( 
		'#type'=>'radios',
		'#title'=>'TIPO DE ESTADO DE SOCIO',
		'#disabled'=>$disabled,
		'#options' => array('ACTIVO', 'BAJA'),
		'#default_value'=>0,
		'#description'=>'Indica si el estado de socio representa socios activos o socios en baja',
	);

	$form['estado']['imprime_cupon'] = array( 
		'#type'=>'checkbox',
		'#title'=>'IMPRIME CUPÓN',
		'#disabled'=>$disabled,
		'#default_value'=>((empty($estado->imprime_cupon)||is_null($estado->imprime_cupon)) ? null : $estado->imprime_cupon),
		'#description'=>'Indica si el estado de socio imprime cupón de pago.',
	);

	$form['estado']['actualiza_fecha_tope'] = array( 
		'#type'=>'checkbox',
		'#title'=>'ACTUALIZA FECHA TOPE',
		'#disabled'=>$disabled,
		'#default_value'=>((empty($estado->actualiza_fecha_tope)||is_null($estado->actualiza_fecha_tope)) ? null : $estado->actualiza_fecha_tope),
		'#description'=>'Indica si el estado de socio actuliza fecha tope apra ingreso con carnet magnetico.',
	);

	$form['estado']['observacion'] = array( 
		'#type'=>'textarea',
		'#title'=>'OBSERVACIONES',
		'#disabled'=>$disabled,
		'#default_value'=>((empty($estado->observacion)||is_null($estado->observacion)) ? null : $estado->observacion),
		'#description'=>'Observaciones e información del estado de socio.',
	);	

	$form['estado']['activo'] = array( 
		'#type'=>'checkbox',
		'#title'=>'ACTIVO',
		'#disabled'=>$disabled,
		'#default_value'=>((empty($estado->activo)||is_null($estado->activo)) ? null : $estado->activo),
		'#description'=>'El estado de socio estará visible para ser utlizado.',
	);

	if((isset($editar) && $editar == 1) || !isset($editar))
	{
		$form['estado']['guardar'] = array( 
			'#type'=>'submit',
			'#name'=>'guardar_estado',
			'#value'=>'GUARDAR',
		);

		$form['estado']['cancelar'] = array( 
			'#type'=>'submit',
			'#name'=>'cancelar_estado',
			'#value'=>'CANCELAR',
			'#limit_validation_errors'=> array(),
			'#submit'=> array('estado_volver_submit'),
			'#attributes' => array('onclick' => 'if(!confirm("¿Está seguro que desea salir?")){return false;}'),
		);	
	}
	else
	{
		$form['estado']['volver'] = array( 
			'#type'=>'submit',
			'#name'=>'volver_estado',
			'#value'=>'VOLVER',
			'#limit_validation_errors'=> array(),
			'#submit'=> array('estado_volver_submit'),
		);		
	}
	
	
	return $form;
}

function estado_volver_submit($form, &$form_state)
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


function estado_form_valdiate($form, &$form_state)
{
	
}

function estado_form_submit($form, &$form_state)
{
	$uid = $GLOBALS['user']->uid;	
	$nombre = $form_state['values']['nombre'];
	$observacion = $form_state['values']['observacion'];
	$cant_cuota_adeuda = $form_state['values']['cant_cuota_adeuda'];
	$cuota = $form_state['values']['cuota'];
	$estado_posterior = ($form_state['values']['estado_posterior'] == 0 ? null : $form_state['values']['estado_posterior']);
	$estado_anterior = ($form_state['values']['estado_anterior'] == 0 ? null : $form_state['values']['estado_anterior']);
	$actualiza_estado = $form_state['values']['actualiza_estado'];
	$tipo = $form_state['values']['tipo'];
	if($tipo==0)
	{
		$socio_activo = 1;
		$tipo_baja = 0;
	}
	else
	{
		$socio_activo = 0;
		$tipo_baja = 1; 
	}	

	$imprime_cupon = $form_state['values']['imprime_cupon'];
	$actualiza_fecha_tope = $form_state['values']['actualiza_fecha_tope'];
	$activo = $form_state['values']['activo'];
	
	$transaction = db_transaction();
	try
	{
		if(isset($form_state['values']['editar']) && $form_state['values']['editar'] == 1)
		{
			//si se entro a editar se debe hacer un update de la categoria
			$id_estado_socio = $form_state['values']['id_estado_socio'];
			db_update('io_estado_socio')
				->fields(array( 'nombre'=>strtoupper($nombre),
					'observacion'=>$observacion,					
					'cant_cuota_adeuda'=>$cant_cuota_adeuda,
					'estado_posterior'=>$estado_posterior,
					'cuota'=>$cuota,
					'estado_anterior'=>$estado_anterior,				
					'actualiza_estado'=>$actualiza_estado,
					'socio_activo'=>$socio_activo,
					'tipo_baja'=>$tipo_baja,
					'imprime_cupon'=>$imprime_cupon,
					'actualiza_fecha_tope'=>$actualiza_fecha_tope,
					'activo'=>$activo,				
					'usuario_mod'=>$uid,
				))
				->condition('id_estado_socio',$id_estado_socio,'=')
				->execute();
				$mensaje = 'SE MODIFICÓ EL ESTADO DE SOCIOS <strong>'.$nombre.'</strong> CORRECTAMENTE.';
		}
		else
		{
			db_insert('io_estado_socio')
				->fields(array('nombre'=>strtoupper($nombre),
					'observacion'=>$observacion,					
					'cant_cuota_adeuda'=>$cant_cuota_adeuda,
					'cuota'=>$cuota,
					'estado_posterior'=>$estado_posterior,
					'estado_anterior'=>$estado_anterior,				
					'actualiza_estado'=>$actualiza_estado,
					'socio_activo'=>$socio_activo,
					'tipo_baja'=>$tipo_baja,
					'imprime_cupon'=>$imprime_cupon,
					'actualiza_fecha_tope'=>$actualiza_fecha_tope,
					'activo'=>$activo,				
					'usuario_alta'=>$uid,
					'usuario_mod'=>$uid,
				))
				->execute();
			$mensaje = 'SE DIÓ DE ALTA EL ESTADO DE SOCIOS <strong>'.$nombre.'</strong> CORRECTAMENTE.';
		}

		drupal_set_message($mensaje);
		$form_state['redirect'] = array('socios/gestionar_estados_socio');	
	}
	catch (Exception $e) {
  		$transaction->rollback();
  		watchdog_exception('estado', $e);
  		drupal_set_message('<pre>'. print_r("Error en la trasaccion - Comunicarse con el proveedor de sistemas.".$e, TRUE) .'</pre>');
	}
}
?>