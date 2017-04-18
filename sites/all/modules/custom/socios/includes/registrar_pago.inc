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
		drupal_get_path('module', 'socios') . '/css/registrar_pagos.css',
	);

	$form['#attached']['js'] = array(
		drupal_get_path('module', 'socios') . '/js/anular_pagos.js',
	);
	
	//buscamos los roles del usuario logueado
  	$roles = get_roles_usuario_logueado();

	$disabled = false;

	$form['pago'] = array( 
		'#type'=>'fieldset',
		'#title'=>'REGISTRO DE PAGO',
	);

	$form['pago']['f_nro_socio'] = array( 
		'#type'=>'textfield',
		'#title'=>'NUMERO DE SOCIO',			
	);

	$form['pago']['f_tipo_doc'] = array( 
		'#type'=>'select',
		'#title'=>'TIPO DE DOCUMENTO',			
		'#options' => array('DNI'=>'DNI','LC'=>'LC','LE'=>'LE','DNI EXT.'=>'DNI EXT.'),
	);

	$form['pago']['f_nro_doc'] = array( 
		'#type'=>'textfield',
		'#title'=>'NUMERO DE DOCUMENTO',			
	);
	
	$form['pago']['buscar'] = array( 
		'#type'=>'submit',
		'#name'=>'buscar_pago',
		'#value'=>'BUSCAR',
		'#validate'=>array('buscar_pago_validate'),
		'#submit'=>array('buscar_pago_submit')
	);

	//$localidades = get_localidades_select();
	if(isset($_GET['ns']))
	{
		$form['nro_socio'] = array(
			'#type'=>'hidden',
			'#value'=>$_GET['ns']
		);
		$form['pago']['f_nro_socio']['#default_value'] = $_GET['ns'];
		//buscamos el socio
		$socio = get_socio_por_nro_socio($_GET['ns']);
	}
	else if(isset($_GET['td'])&&isset($_GET['nd']))
	{ 
		$form['tipo_doc'] = array(
			'#type'=>'hidden',
			'#value'=>$_GET['td']
		);

		$form['nro_doc'] = array(
			'#type'=>'hidden',
			'#value'=>$_GET['nd']
		);

		$form['pago']['f_nro_doc']['#default_value'] = $_GET['nd'];
		$form['pago']['f_tipo_doc']['#default_value'] = $_GET['td'];
		//buscamos el socio
		$socio = get_socio_por_tipo_nro_doc($_GET['td'], $_GET['nd']);
	}
	
	//si se encontro el socio buscado se mustran los datos y lo que tiene por pagar	
    if(isset($socio) && !empty($socio))
    {    	
    	//creamos un fieldset que contiene los datos del socio
    	$form['datos_socio'] = array(
    		'#type'=>'fieldset',
    		'#title'=>'DATOS DEL SOCIO'
    	);
    	//vemos si el socio es titular o socio unico
		//sino debemos buscar el titular y su cuenta corriente
		if($socio->tipo_socio_id == 2)
		{
			$grupo_familiar = get_grupo_familiar($socio->id_socio);
			$socio->nro_socio = $grupo_familiar->nro_socio_titular;
			//TODO: SI CAMBIA NRO_SOCIO_TITULAR POR ID_SOCIO_TITULAR CAMBIAR ACA
			$socio_titular_familia = get_socio_por_nro_socio($grupo_familiar->nro_socio_titular);
			//drupal_set_message('<pre>'.print_r($socio_titular_familia, true).'</pre>');
			//dependiendo de la forma de pago los datos que vamos a mostrar
			//se usa un string que se adiciono al markup  donde se muestran los datos del titualar
			//comienza siendo vacio
			$forma_pago = '';
			if($socio_titular_familia->forma_pago_id == 1)
			{//si la forma de pago es por cobrador, tenemos que buscar el cobrador que le corresponde
				$cobrador = get_personal_por_id($socio_titular_familia->personal_id);
				$forma_pago = $forma_pago.'<div class="rp-socio" id="rp-forma-pago"><strong>FORMA DE PAGO: </strong>'.$socio_titular_familia->forma_pago_nombre.'</div>
				<div class="rp-socio" id="rp-cobrador"><strong>COBRADOR: </strong>'.$cobrador->apellidos.' '.$cobrador->nombres.'</div>';
			}
			else
			{
				$forma_pago = $forma_pago.'<div class="rp-socio" id="rp-forma-pago"><strong>FORMA DE PAGO: </strong>'.$socio_titular_familia->forma_pago_nombre.'</div>';
			}

			//buscamos los datos del socio y armamamos un string con el html necesario
			$form['datos_socio']['grupo_familar_socio'] = array( 
    		'#markup' =>'<div id="rp-datos-socio-gf">    					
    						<div class="rp-gf" id="rp-tipo-gf"><h2>TIPO DE GRUPO FAMILIAR: </h2></div>
    						<div class="rp-gf" id="rp-nombre-tipo-gf">'.$grupo_familiar->nombre_tipo_grupo_familiar.'</div>    					
    					</div>',
    		);

			$form['datos_socio']['titular'] = array( 
    		'#markup' =>'<div id="rp-datos-socio-tit">
    						<div class="rp-socio" id="cabecera-socio-titular"><h2>DATOS DEL TITULAR</h2></div>
    						<div class="rp-socio" id="rp-nro-socio"><strong>NUMERO DE SOCIO: </strong>'.$socio_titular_familia->nro_socio.'/'.$socio_titular_familia->secuencia.'</div>
    						<div class="rp-socio" id="rp-apellidos"><strong>APELLIDOS: </strong>'.$socio_titular_familia->apellidos.'</div>
    						<div class="rp-socio" id="rp-nombres"><strong>NOMBRES: </strong>'.$socio_titular_familia->nombres.'</div>
    						<div class="rp-socio" id="rp-tipo-socio"><strong>TIPO DE SOCIO: </strong>'.$socio_titular_familia->tipo_socio.'</div>
    						<div class="rp-socio" id="rp-estado"><strong>ESTADO: </strong>'.$socio_titular_familia->estado.'</div>
    						'.$forma_pago.'
    						</div>',
    		);

			$form['datos_socio']['socio'] = array( 
    		'#markup' =>'<div id="rp-datos-socio-mf">
    						<div class="rp-socio" id="cabecera-socio"><h2>DATOS DEL SOCIO MIEMBRO DE FAMILIA</h2></div>
    						<div class="rp-socio" id="rp-nro-socio"><strong>NUMERO DE SOCIO: </strong>'.$socio_titular_familia->nro_socio.'/'.$socio->secuencia.'</div>
    						<div class="rp-socio" id="rp-apellidos"><strong>APELLIDOS: </strong>'.$socio->apellidos.'</div>
    						<div class="rp-socio" id="rp-nombres"><strong>NOMBRES: </strong>'.$socio->nombres.'</div>
    						<div class="rp-socio" id="rp-tipo-socio"><strong>TIPO DE SOCIO: </strong>'.$socio->tipo_socio.'</div>
    						<div class="rp-socio" id="rp-estado"><strong>ESTADO: </strong>'.$socio->estado.'</div>
    						</div>',
    		);

    		
			//se buscan los pagos del socio con los parametros:
			//id_socio = id del socio
			//mes = null
			//anio = null
			//pagado = 0
			//refinanciado = 0
			//anulado = 0
			$cta_cte = get_pago_por_id_socio($socio_titular_familia->id_socio, null, null, 0, 0, 0);	
			//guardamos el id del estado del socio y el tipo de grupo familiar 
			$estado_socio_id = $socio_titular_familia->estado_socio_id;	
			$tipo_grupo_familiar = $grupo_familiar->tipo_grupo_familiar_id;
		}
		else
		{
			//dependiendo de la forma de pago los datos que vamos a mostrar
			//se usa un string que se adiciono al markup  donde se muestran los datos del titualar
			//comienza siendo vacio
			$forma_pago = '';
			if($socio->forma_pago_id == 1)
			{//si la forma de pago es por cobrador, tenemos que buscar el cobrador que le corresponde
				$cobrador = get_personal_por_id($socio->personal_id);
				$forma_pago = $forma_pago.'<div class="rp-socio" id="rp-forma-pago"><strong>FORMA DE PAGO: </strong>'.$socio->forma_pago_nombre.'</div>
				<div class="rp-socio" id="rp-cobrador"><strong>COBRADOR: </strong>'.$cobrador->apellidos.' '.$cobrador->nombres.'</div>';
			}
			else
			{
				$forma_pago = $forma_pago.'<div class="rp-socio" id="rp-forma-pago"><strong>FORMA DE PAGO: </strong>'.$socio->forma_pago_nombre.'</div>';
			}

			$form['datos_socio']['socio'] = array( 
    		'#markup' =>'<div id="rp-datos-socio">
    						<div class="rp-socio" id="rp-nro-socio"><strong>NUMERO DE SOCIO: </strong>'.$socio->nro_socio.'/'.$socio->secuencia.'</div>
    						<div class="rp-socio" id="rp-apellidos"><strong>APELLIDOS: </strong>'.$socio->apellidos.'</div>
    						<div class="rp-socio" id="rp-nombres"><strong>NOMBRES: </strong>'.$socio->nombres.'</div>
    						<div class="rp-socio" id="rp-tipo-socio"><strong>TIPO DE SOCIO: </strong>'.$socio->tipo_socio.'</div>
    						<div class="rp-socio" id="rp-estado"><strong>ESTADO: </strong>'.$socio->estado.'</div>
    						<div class="rp-socio" id="rp-categoria"><strong>CATEGORIA: </strong>'.$socio->categoria.'</div>
    						'.$forma_pago.'
    						</div>',
    		);
			//se buscan los pagos del socio con los parametros:
			//id_socio = id del socio
			//mes = null
			//anio = null
			//pagado = 0
			//refinanciado = 0
			//anulado = 0
			$cta_cte = get_pago_por_id_socio($socio->id_socio, null, null, 0, 0, 0);

			//guardamos el id del socio y la categoria del socio 
			$estado_socio_id = $socio->estado_socio_id;
			$categoria = $socio->categoria_socio_id;
		}
		
		//buscamos el tipo de estado de socio para determinar si es baja o no
		$tipo_estado = get_tipo_estado($estado_socio_id);
	    //si estipo baja se debe actualizar el valor de los conceptos y cambiar el encabezado 

		//drupal_set_message('<pre>'.print_r($cta_cte, true).'</pre>');
		
		$form['datos_pago'] = array(
				'#title' => t('DATOS DE CUENTA CORRIENTE'),
				'#type' => 'fieldset',
				'#prefix' => '<div id="fieldset-pago">',
				'#suffix'=>'</div>',
		);

		$header = 
		array(    
        'fecha_emision' => t('FECHA DE EMISION'), 
        'mes' => array('data'=>t('MES')), 
        'anio' => array('data'=>t('AÑO')), 
        'concepto' => array('data'=>t('CONCEPTO')),        
        'valor' =>($tipo_estado == 1 ? array('data'=>t('VALOR ACTUALIZADO')) : array('data'=>t('VALOR'))),
        'pagado' => array('data'=>t('PAGADO')),
        'fecha_pago' => array('data'=>t('FECHA DE PAGO')),
        'anulado' => array('data'=>t('ANULADO')),
        );
  
		$opciones = array();
		foreach ($cta_cte as $item_cta_cte) {
			$id_cta_cte_item = $item_cta_cte->id_cta_cte_item;

			if($item_cta_cte->pagado == "SI")
		  		$color = "green";
		  	else
		  		$color = "red";      

		  	if($item_cta_cte->anulado == "SI")
		  		$color_anulado = "green";
		  	else
		  		$color_anulado = "red";

		  	//si el socio esta en estado de baja se debe actualizar el valor de los conceptos
		  	if($tipo_estado == 1)
		  	{
		  		//por ahora solo se busca el valor para el concepto cuota social
		  		if($item_cta_cte->id_concepto == 1)
		  		{
		  			//vemos si es un grupo familiar o un socio unico. 
		  			//si es grupo familiar el valor se calcula buscando por tipo de grupo familiar
		  			if(isset($tipo_grupo_familiar))
		  			{
		  				$valor = importe_cuota_gf($tipo_grupo_familiar);
		  			}//sino tenemos que actualizar el valor tomandolo desde la categoria del socio
		  			else if(isset($categoria))
		  			{
		  				$valor = importe_cuota_su($categoria);
		  			}
		  		}
		  	} 

			$opcion =  
			  array(
			    'fecha_emision'=> $item_cta_cte->fecha_emision,
			    'mes'=>$item_cta_cte->mes,
			    'anio'=>$item_cta_cte->anio,    
			    'concepto'=>$item_cta_cte->concepto,
			    'valor'=>($tipo_estado == 1 ? $valor : $item_cta_cte->valor),
			    'pagado'=>array('data'=>'<div style="color:'.$color.';"><strong>'.$item_cta_cte->pagado.'</strong></div>'),	
			    'fecha_pago'=>$item_cta_cte->fecha_pago,
			    'anulado'=>array('data'=>'<div style="color:'.$color_anulado.';"><strong>'.$item_cta_cte->anulado.'</strong></div>'),
			    'id_cta_cte_item'=> array(
				    'data'=> array( 
				      '#type' => 'hidden',               
				      '#name' => 'id_alumno',               
				      '#value'=> $id_cta_cte_item,
				      ), 
			    ),     
			); 
			$opciones[$id_cta_cte_item] = $opcion;
		}
		      
 		
 		//se dispone de un select de cobrador para seleccionar el cobrador que corresponde el pago
 		$cobradores = get_cobradores_club_select(true);
		$form['datos_pago']['personal'] = array(
			'#title' => t('COBRADOR'),
			'#type'=>'select',
			'#options'=>$cobradores,
			'#default_value'=> ((empty($_SESSION['registrar_pago']['personal_id'])||is_null($_SESSION['registrar_pago']['personal_id'])) ? null : $_SESSION['registrar_pago']['personal_id'])
		);

		$form['datos_pago']['listado_pago'] = array(
	        '#type' => 'tableselect',
	        '#header' => $header,
	        '#options' => $opciones,
	        '#empty' => 'No pagos pendientes para el socio.',
	        '#prefix'=>'<div id="listado-pagos-socio">',
	        '#suffix'=>'</div>',
	       
	      );

		$form['datos_pago']['aceptar'] = array(
			'#type'=>'submit',
			'#value'=>'PROCESAR PAGOS',
			'#validate'=>array('registrar_pagos_form_validate'),
			'#submit'=>array('registrar_pagos_form_submit')
		);

		if(es_usr_secretaria_control($roles))
		{
			$form['datos_pago']['anular'] = array(
				'#type'=>'submit',
				'#value'=>'ANULAR PAGOS',
				'#executes_submit_callback'=>false,
			);	
		}
		

		$form['datos_pago']['total'] = array(
			'#type'=>'textfield',
			'#title'=>'TOTAL A PAGAR',
			'#default_value'=>0,
			'#field_prefix'=>'$',
			'#disabled'=>true,
			'#size'=>5,
		);

		$form['datos_pago']['anulacion'] = array(   
			'#prefix' => '<div id=panel-anular-pago-div>',
			'#type'=>'fieldset',
			'#title'=>'ANULACION DE PAGO GENERADO',
			'#suffix' => '</div>',
		);

		$form['datos_pago']['anulacion']['motivo_anulacion'] = array(
			'#type'=>'select',
			'#title'=>'MOTIVO DE ANULACION',
			'#options'=>array( NULL=>'-Seleccionar-',
							  'DISPOSICIÓN DE COMISIÓN DIRECTIVA'=>'DISPOSICIÓN DE COMISIÓN DIRECTIVA',
							  'POR CAMBIO DE CATEGORÍA'=>'POR CAMBIO DE CATEGORÍA',
							  'ERROR ADMINISTRATIVO'=>'ERROR ADMINISTRATIVO'),
		);

		$form['datos_pago']['anulacion']['aceptar_anulacion'] = array(
			'#type'=>'submit',
			'#value'=>'ACEPTAR',
			'#validate'=>array('aceptar_anulacion_form_validate'),
			'#submit'=>array('aceptar_anulacion_form_submit'),
		);

		$form['datos_pago']['anulacion']['cancelar_anulacion'] = array(
			'#type'=>'submit',
			'#value'=>'CANCELAR',
			'#executes_submit_callback'=>false,
		);

    }
    //sino se mustra un mensaje 
    else if(isset($_GET['ns']) || isset($_GET['nd']))
    {
    	$form['socio_no_encontrado'] = array(
    		'#markup'=>'<div id="rp-socio-no-encontrado">
    						<h2>NO SE ENCUENTRÓ EL SOCIO BUSCADO</h2>
    					</div>'
    	);
    }
	
	return $form;
}

function buscar_pago_validate($form, &$form_state)
{
	if(!empty($form_state['values']['f_nro_doc']) && !is_numeric($form_state['values']['f_nro_doc'])){
      form_set_error('f_nro_doc', 'El campo NUMERO DE DOCUMENTO es incorrecto - Solo puede contener números');
    }
    if(!empty($form_state['values']['f_nro_socio']) && !is_numeric($form_state['values']['f_nro_socio'])){
      form_set_error('f_nro_socio', 'El campo NUMERO DE SOCIO es incorrecto - Solo puede contener números');
    }
    
}

function buscar_pago_submit($form, &$form_state)
{
	$tipo_doc = $form_state['values']['f_tipo_doc'];
	$nro_doc = $form_state['values']['f_nro_doc'];
	$nro_socio = $form_state['values']['f_nro_socio'];

	if(isset($nro_socio) && !empty($nro_socio))
	{
        $form_state['redirect'] = array('socios/registrar_pago',array('query' => array('ns'=>$nro_socio)));	

	}
	else if(isset($tipo_doc)&& !empty($tipo_doc) && isset($nro_doc) && !empty($nro_doc))
	{
        $form_state['redirect'] = array('socios/registrar_pago',array('query' => array('td'=>$tipo_doc, 'nd'=>$nro_doc)));
	}
}

function registrar_pagos_form_validate($form, &$form_state)
{
	//revisamos que se haya seleccionado al menos un pago para procesar
	//se suman los valores si la suma da 0 es que no se selecciono ningun pago
	$suma_seleccion = 0;
	foreach ($form_state['values']['listado_pago'] as $pago) {
		$suma_seleccion = $suma_seleccion+$pago;
	}
	
	if($suma_seleccion == 0)
    	form_set_error('listado_pago', 'No se ha seleccionado ningún item - Seleccione al menos uno para realizar la acción requerida.');

    if($form_state['values']['personal'] == 0)
    	form_set_error('personal', 'COBRADOR es obligatorio.');
    
}

function registrar_pagos_form_submit($form, &$form_state)
{
	$uid = $GLOBALS['user']->uid;		
	$fecha_pago = date('Y').'-'.date('m').'-'.date('d');
	//drupal_set_message('<pre>'.print_r($form_state['values'], true).'</pre>');
	$transaction = db_transaction();
	try
	{
		//buscamos el cobrador seleccionado 
		$cobro_id = $form_state['values']['personal']; 
		$_SESSION['registrar_pago']['personal_id'] = $cobro_id;
		//recorremos el listado mientras no sea 0
		$listado_pago = $form_state['values']['listado_pago'];
		foreach ($listado_pago as $item) {
			if($item == 0)
				break;

			db_update('io_cta_cte_item')
			->fields(array( 'pagado'=>1,
				'fecha_pago'=>$fecha_pago,			
				'cobro_id'=>$cobro_id,	
				'usuario_mod'=>$uid,
			))
			->condition('id_cta_cte_item',$item,'=')
			->execute();
			$mensaje = 'SE REGISTRO CORRECTAMENTE EL PAGO';		

			drupal_set_message($mensaje);
			$form_state['redirect'] = array('socios/registrar_pago');	

		}

		unset($transaction);
	}
	catch (Exception $e) {
  		$transaction->rollback();
  		watchdog_exception('pago', $e);
  		drupal_set_message('<pre>'. print_r("Error en la trasaccion - Comunicarse con el proveedor de sistemas.".$e, TRUE) .'</pre>');
	}
	
}

function aceptar_anulacion_form_validate($form, &$form_state)
{
	//revisamos que se haya seleccionado al menos un pago para procesar
	//se suman los valores si la suma da 0 es que no se selecciono ningun pago
	$suma_seleccion = 0;
	foreach ($form_state['values']['listado_pago'] as $pago) {
		$suma_seleccion = $suma_seleccion+$pago;
	}
	//drupal_set_message('<pre>'.print_r($form_state['values'],true).'</pre>');
	if($suma_seleccion == 0)
    	form_set_error('listado_pago', 'No se ha seleccionado ningún item - Seleccione al menos uno para realizar la acción requerida.');
    //drupal_set_message('<pre>'.print_r(empty($form_state['values']['motivo_anulacion']),true).'</pre>');
    if(empty($form_state['values']['motivo_anulacion']))
    {
    	form_set_error('motivo_anulacion', 'No se ha selecionado ningún motivo de aulación - Seleccione al menos uno para completar la anulación');
    }
    
}

function aceptar_anulacion_form_submit($form, &$form_state)
{
	$uid = $GLOBALS['user']->uid;		
	$fecha_anulado = date('Y').'-'.date('m').'-'.date('d');
	$motivo_anulacion = $form_state['values']['motivo_anulacion'];
	//drupal_set_message('<pre>'.print_r($form_state['values'], true).'</pre>');
	$transaction = db_transaction();
	try
	{
		//recorremos el listado mientras no sea 0
		$listado_pago = $form_state['values']['listado_pago'];
		foreach ($listado_pago as $item) {
			if($item == 0)
				break;

			db_update('io_cta_cte_item')
			->fields(array( 'anulado'=>1,
				'fecha_anulado'=>$fecha_anulado,
				'motivo_anulacion'=>$motivo_anulacion,				
				'usuario_mod'=>$uid,
			))
			->condition('id_cta_cte_item',$item,'=')
			->execute();
			$mensaje = 'SE REGISTRO CORRECTAMENTE LA ANULACION DEL PAGO';		

			drupal_set_message($mensaje);
			$form_state['redirect'] = array('socios/registrar_pago');	

		}

		unset($transaction);
	}
	catch (Exception $e) {
  		$transaction->rollback();
  		watchdog_exception('pago', $e);
  		drupal_set_message('<pre>'. print_r("Error en la trasaccion - Comunicarse con el proveedor de sistemas.".$e, TRUE) .'</pre>');
	}
	
}

function ajax_anular_callback($form, &$form_state)
{
	//drupal_set_message('<pre>'.print_r($form_state['values'], true).'</pre>');
	$_SESSION['anulando'] = 1;

	return $form['datos_pago'];
}

?>