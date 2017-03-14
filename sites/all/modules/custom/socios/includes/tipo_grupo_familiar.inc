<?php
/**
* Permite dar de alta, ver y modificar un tipo de grupo familiar
*
* @author Giuici Francisco
* @version 1.0
* @package sicat
* @subpackage socios
*/


/**
* Formulario que permite dar de alta, ver y modificar un tipo de grupo familiar
*/
function tipo_grupo_familiar_form($form, &$form_state)
{
	if(strpos($_SERVER['HTTP_REFERER'], 'gestionar_tipo_grupo_familiar'))
	{
		$array_retorno = parse_url($_SERVER['HTTP_REFERER']);
		$_SESSION['retorno'] = 'socios/gestionar_tipo_grupo_familiar';    
		$_SESSION['retorno_params'] =array('query' =>get_params_url($_SERVER['HTTP_REFERER']));
	}

	$disabled = false;

	if(isset($_GET['id']))
	{
		$id_tipo_grupo_familiar = $_GET['id'];

		$form['id_tipo_grupo_familiar'] = array(
		  '#type' => 'hidden', 
		  '#value' => $id_tipo_grupo_familiar,
		);

		$tipo_grupo_familiar = get_tipo_grupo_familiar_por_id($id_tipo_grupo_familiar);	
		
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

		$texto_titulo = 'TIPO DE GRUPO FAMILIAR: '.$tipo_grupo_familiar->nombre;
	}
	else
	{
		$texto_titulo = 'NUEVO TIPO DE GRUPO FAMILIAR';
	}

	$form['tipo_grupo_familiar'] = array( 
		'#type'=>'fieldset',
		'#title'=>$texto_titulo,
	);

	$form['tipo_grupo_familiar']['nombre'] = array( 
		'#type'=>'textfield',
		'#title'=>'NOMBRE',
		'#disabled'=>$disabled,
		'#default_value'=>((empty($tipo_grupo_familiar->nombre)||is_null($tipo_grupo_familiar->nombre)) ? null : $tipo_grupo_familiar->nombre),
	);

	$form['tipo_grupo_familiar']['cant_int_desde'] = array( 
		'#type'=>'numberfield',
		'#title'=>'CANTIDAD MINIMA DE INTEGRANTES',
		'#disabled'=>$disabled,
		'#default_value'=>((empty($tipo_grupo_familiar->cant_int_desde)||is_null($tipo_grupo_familiar->cant_int_desde)) ? 0 : $tipo_grupo_familiar->cant_int_desde),
		'#description'=>'Cantidad minima que debe tener el grupo familiar.',
	);


	$form['tipo_grupo_familiar']['cant_int_hasta'] = array( 
		'#type'=>'numberfield',
		'#title'=>'CANTIDAD MÁXIMA DE INTEGRANTES',
		'#disabled'=>$disabled,
		'#default_value'=>((empty($tipo_grupo_familiar->cant_int_hasta)||is_null($tipo_grupo_familiar->cant_int_hasta)) ? 0 : $tipo_grupo_familiar->cant_int_hasta),
		'#description'=>'Cantidad máxima que debe tener el grupo familiar.',
	);

	$form['tipo_grupo_familiar']['importe'] = array( 
		'#type'=>'textfield',
		'#title'=>'IMPORTE CUOTA',
		'#disabled'=>$disabled,
		'#default_value'=>((empty($tipo_grupo_familiar->importe)||is_null($tipo_grupo_familiar->importe)) ? null : number_format($tipo_grupo_familiar->importe, 2, ",", ".")),
		'#field_prefix'=>'$',
		'#size'=>6,
	);
	

	$form['tipo_grupo_familiar']['activo'] = array( 
		'#type'=>'checkbox',
		'#title'=>'ACTIVO',
		'#disabled'=>$disabled,
		'#default_value'=>((empty($tipo_grupo_familiar->activo)||is_null($tipo_grupo_familiar->activo)) ? null : $tipo_grupo_familiar->activo),
		'#description'=>'El tipo de grupo familiar estará visible para ser utlizado.',
	);

	if((isset($editar) && $editar == 1) || !isset($editar))
	{
		$form['tipo_grupo_familiar']['guardar'] = array( 
			'#type'=>'submit',
			'#name'=>'guardar_tgf',
			'#value'=>'GUARDAR',
		);

		$form['tipo_grupo_familiar']['cancelar'] = array( 
			'#type'=>'submit',
			'#name'=>'cancelar_tgf',
			'#value'=>'CANCELAR',
			'#limit_validation_errors'=> array(),
			'#submit'=> array('tipo_grupo_familiar_volver_submit'),
			'#attributes' => array('onclick' => 'if(!confirm("¿Está seguro que desea salir?")){return false;}'),
		);	
	}
	else
	{
		$form['tipo_grupo_familiar']['volver'] = array( 
			'#type'=>'submit',
			'#name'=>'volver_tgf',
			'#value'=>'VOLVER',
			'#limit_validation_errors'=> array(),
			'#submit'=> array('tipo_grupo_familiar_volver_submit'),
		);		
	}
	
	
	return $form;
}

function tipo_grupo_familiar_volver_submit($form, &$form_state)
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


function tipo_grupo_familiar_form_valdiate($form, &$form_state)
{
	if(!is_numeric($form_state['values']['importe'])){
      form_set_error('importe', 'El campo IMPORTE es incorrecto - Solo puede contener números');
    }
	
}

function tipo_grupo_familiar_form_submit($form, &$form_state)
{
	$uid = $GLOBALS['user']->uid;	
	$nombre = $form_state['values']['nombre'];
	$cant_int_desde = $form_state['values']['cant_int_desde'];
	$cant_int_hasta = $form_state['values']['cant_int_hasta'];
	$importe = str_replace(',', '.', $form_state['values']['importe']);	
	$activo = $form_state['values']['activo'];
	
	$transaction = db_transaction();
	try
	{
		if(isset($form_state['values']['editar']) && $form_state['values']['editar'] == 1)
		{
			//si se entro a editar se debe hacer un update de la categoria
			$id_tipo_grupo_familiar = $form_state['values']['id_tipo_grupo_familiar'];
			db_update('io_tipo_grupo_familiar')
				->fields(array( 'nombre'=>strtoupper($nombre),
					'cant_int_desde'=>$cant_int_desde,
					'cant_int_hasta'=>$cant_int_hasta,
					'importe'=>$importe,
					'activo'=>$activo,											
					'usuario_mod'=>$uid,
				))
				->condition('id_tipo_grupo_familiar',$id_tipo_grupo_familiar,'=')
				->execute();
				$mensaje = 'SE MODIFICÓ EL TIPO DE GRUPO FAMILIAR <strong>'.$nombre.'</strong> CORRECTAMENTE.';
		}
		else
		{
			db_insert('io_tipo_grupo_familiar')
				->fields(array('nombre'=>strtoupper($nombre),				
					'cant_int_desde'=>$cant_int_desde,
					'cant_int_hasta'=>$cant_int_hasta,
					'importe'=>$importe,
					'activo'=>$activo,			
					'usuario_alta'=>$uid,
					'usuario_mod'=>$uid,
				))
				->execute();
			$mensaje = 'SE DIÓ DE ALTA EL TIPO DE GRUPO FAMILIAR <strong>'.$nombre.'</strong> CORRECTAMENTE.';
		}

		drupal_set_message($mensaje);
		$form_state['redirect'] = array('socios/gestionar_tipo_grupo_familiar');	
	}
	catch (Exception $e) {
  		$transaction->rollback();
  		watchdog_exception('estado', $e);
  		drupal_set_message('<pre>'. print_r("Error en la trasaccion - Comunicarse con el proveedor de sistemas.".$e, TRUE) .'</pre>');
	}
}
?>