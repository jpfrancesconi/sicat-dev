<?php
/**
* Permite Gestionar las categorias de los socios
*
* @author Giuici Francisco
* @version 1.0
* @package sicat
* @subpackage socios
*/


/**
* Formulario que gestionar socios
*/
function categoria_form($form, &$form_state)
{
	if(strpos($_SERVER['HTTP_REFERER'], 'gestionar_categorias_socio'))
	{
		$array_retorno = parse_url($_SERVER['HTTP_REFERER']);
		$_SESSION['retorno'] = 'socios/gestionar_categorias_socio';    
		$_SESSION['retorno_params'] =array('query' =>get_params_url($_SERVER['HTTP_REFERER']));
	}

	$disabled = false;

	if(isset($_GET['id']))
	{
		$id_categoria_socio = $_GET['id'];

		$form['id_categoria_socio'] = array(
		  '#type' => 'hidden', 
		  '#value' => $id_categoria_socio,
		);

		$categoria = get_categoria_por_id($id_categoria_socio);	
		
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

		$texto_titulo = 'CATEGORIA: '.$categoria->nombre;
	}
	else
	{
		$texto_titulo = 'NUEVA CATEGORIA';
	}

	$form['categoria'] = array( 
		'#type'=>'fieldset',
		'#title'=>$texto_titulo,
	);

	$form['categoria']['nombre'] = array( 
		'#type'=>'textfield',
		'#title'=>'NOMBRE',
		'#disabled'=>$disabled,
		'#default_value'=>((empty($categoria->nombre)||is_null($categoria->nombre)) ? null : $categoria->nombre),
	);

	$form['categoria']['importe_cuota'] = array( 
		'#type'=>'textfield',
		'#title'=>'IMPORTE CUOTA',
		'#disabled'=>$disabled,
		'#default_value'=>((empty($categoria->importe_cuota)||is_null($categoria->importe_cuota)) ? null : number_format($categoria->importe_cuota, 2, ",", ".")),
		'#field_prefix'=>'$',
		'#size'=>6,
	);

	$form['categoria']['incluye_cuota_social'] = array( 
		'#type'=>'checkbox',
		'#title'=>'INCLUYE CUOTA SOCIAL',
		'#disabled'=>$disabled,
		'#default_value'=>((empty($categoria->incluye_cuota_social)||is_null($categoria->incluye_cuota_social)) ? null : $categoria->incluye_cuota_social),
		'#description'=>'La categoría ',
	);

	//TODO: agregar cuenta contable a categoria

	$form['categoria']['edad_desde'] = array( 
		'#type'=>'numberfield',
		'#title'=>'EDAD DESDE',
		'#disabled'=>$disabled,
		'#default_value'=>((empty($categoria->edad_desde)||is_null($categoria->edad_desde)) ? null : $categoria->edad_desde),
		'#description'=>'Edad de los socios desde la que la categoría es válida inclusive',
		'#field_suffix'=>'Años',
	);

	$form['categoria']['edad_hasta'] = array( 
		'#type'=>'numberfield',
		'#title'=>'EDAD HASTA',
		'#disabled'=>$disabled,
		'#default_value'=>((empty($categoria->edad_hasta)||is_null($categoria->edad_hasta)) ? null : $categoria->edad_hasta),
		'#description'=>'Edad de los socios hasta la que la categoría es válida inclusive',
		'#field_suffix'=>'Años',
	);

	$form['categoria']['derecho_a_voto'] = array( 
		'#type'=>'checkbox',
		'#title'=>'DERECHO A VOTO',
		'#disabled'=>$disabled,
		'#default_value'=>((empty($categoria->derecho_a_voto)||is_null($categoria->derecho_a_voto)) ? null : $categoria->derecho_a_voto),
		'#description'=>'La categoría da derecho a voto para el socio.',
	);

	$form['categoria']['observaciones'] = array( 
		'#type'=>'textarea',
		'#title'=>'OBSERVACIONES',
		'#disabled'=>$disabled,
		'#default_value'=>((empty($categoria->observaciones)||is_null($categoria->observaciones)) ? null : $categoria->observaciones),
		'#description'=>'Observaciones e información adicional de la categoría.',
	);	

	$form['categoria']['activo'] = array( 
		'#type'=>'checkbox',
		'#title'=>'ACTIVA',
		'#disabled'=>$disabled,
		'#default_value'=>((empty($categoria->activo)||is_null($categoria->activo)) ? null : $categoria->activo),
		'#description'=>'La categoría estará visible para ser utlizada.',
	);

	if((isset($editar) && $editar == 1) || !isset($editar))
	{
		$form['categoria']['guardar'] = array( 
			'#type'=>'submit',
			'#name'=>'guardar_categoria',
			'#value'=>'GUARDAR',
		);

		$form['categoria']['cancelar'] = array( 
			'#type'=>'submit',
			'#name'=>'cancelar_cat',
			'#value'=>'CANCELAR',
			'#limit_validation_errors'=> array(),
			'#submit'=> array('categoria_volver_submit'),
			'#attributes' => array('onclick' => 'if(!confirm("¿Está seguro que desea salir?")){return false;}'),
		);	
	}
	else
	{
		$form['categoria']['volver'] = array( 
			'#type'=>'submit',
			'#name'=>'volver_cat',
			'#value'=>'VOLVER',
			'#limit_validation_errors'=> array(),
			'#submit'=> array('categoria_volver_submit'),
		);		
	}
	
	
	return $form;
}

function categoria_volver_submit($form, &$form_state)
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


function categoria_form_valdiate($form, &$form_state)
{
	if(!is_numeric($form_state['values']['importe_cuota'])){
      form_set_error('importe_cuota', 'El campo IMPORTE CUOTA es incorrecto - Solo puede contener números');
    }
}

function categoria_form_submit($form, &$form_state)
{
	$uid = $GLOBALS['user']->uid;	
	$nombre = $form_state['values']['nombre'];
	$importe_cuota = str_replace(',', '.', $form_state['values']['importe_cuota']);
	$incluye_cuota_social = $form_state['values']['incluye_cuota_social'];
	$edad_desde = $form_state['values']['edad_desde'];
	$edad_hasta = $form_state['values']['edad_hasta'];
	$derecho_a_voto = $form_state['values']['derecho_a_voto'];
	$observaciones = $form_state['values']['observaciones'];
	$activo = $form_state['values']['activo'];
	
	$transaction = db_transaction();
	try
	{
		if($form_state['values']['editar'] == 1 && isset($form_state['values']['editar']))
		{
			//si se entro a editar se debe hacer un update de la categoria
			$id_categoria_socio = $form_state['values']['id_categoria_socio'];
			db_update('io_categoria_socio')
				->fields(array( 'incluye_cuota_social'=>$incluye_cuota_social,
					'importe_cuota'=>$importe_cuota,					
					'nombre'=>strtoupper($nombre),
					'edad_desde'=>$edad_desde,
					'edad_hasta'=>$edad_hasta,				
					'derecho_a_voto'=>$derecho_a_voto,
					'observaciones'=>$observaciones,
					'activo'=>$activo,				
					'usuario_mod'=>$uid,
				))
				->condition('id_categoria_socio',$id_categoria_socio,'=')
				->execute();
				$mensaje = 'SE MODIFICÓ LA CATEGORÍA DE SOCIOS <strong>'.$nombre.'</strong> CORRECTAMENTE.';
		}
		else
		{
			db_insert('io_categoria_socio')
				->fields(array('incluye_cuota_social'=>$incluye_cuota_social,
					'importe_cuota'=>$importe_cuota,					
					'nombre'=>strtoupper($nombre),
					'edad_desde'=>$edad_desde,
					'edad_hasta'=>$edad_hasta,				
					'derecho_a_voto'=>$derecho_a_voto,
					'observaciones'=>$observaciones,	
					'activo'=>$activo,			
					'usuario_alta'=>$uid,
					'usuario_mod'=>$uid,
				))
				->execute();
			$mensaje = 'SE DIÓ DE ALTA LA CATEGORÍA DE SOCIOS <strong>'.$nombre.'</strong> CORRECTAMENTE.';
		}

		drupal_set_message($mensaje);
		$form_state['redirect'] = array('socios/gestionar_categorias_socio');	
	}
	catch (Exception $e) {
  		$transaction->rollback();
  		watchdog_exception('categoria', $e);
  		drupal_set_message('<pre>'. print_r("Error en la trasaccion - Comunicarse con el proveedor de sistemas.".$e, TRUE) .'</pre>');
	}
}
?>