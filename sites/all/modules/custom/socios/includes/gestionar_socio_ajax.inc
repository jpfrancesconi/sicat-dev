<?php
/**
* Permite Gestionar los socios del club
*
* @author Giuici Francisco
* @version 1.0
* @package sicat
* @subpackage socios
*/


/**
* Formulario que gestionar socios
*/
function gestionar_socios_form($form, &$form_state)
{	
	$formulario = 'gestionar_socios';

	$form['formulario'] = array(
		'#type'=>'hidden',
		'#value'=>$formulario,
	);

	$form['#attached']['css'] = array(
		drupal_get_path('module', 'socios') . '/css/socios.css',
	);
  	$form['#attached']['js'] = array(
		drupal_get_path('module', 'socios') . '/js/socios.js',
	);	

	$form['gestion_socios']['filtrar'] = array(
		'#title' => t('FILTRAR SOCIOS'),
		'#type' => 'fieldset',
		'#collapsible'=>true, 
	);

	$form['gestion_socios']['filtrar']['filtro_nro_socio'] = array(
		'#title' => t('NUMERO DE SOCIO'),
	    '#type' => 'textfield', 
	    '#default_value'=>t(''),
	    '#disabled'=>false,	    
	);

	$form['gestion_socios']['filtrar']['filtro_nombres'] = array(
		'#title' => t('NOMBRE'),
	    '#type' => 'textfield', 
	    '#default_value'=>t(''),
	    '#disabled'=>false,	    
	);	

	$form['gestion_socios']['filtrar']['filtro_apellidos'] = array(
		'#title' => t('APELLIDO'),
	    '#type' => 'textfield', 
	    '#default_value'=>t(''),
	    '#disabled'=>false,	    
	);

	$form['gestion_socios']['filtrar']['filtro_estado'] = array(
		'#title' => t('ESTADO'),
	    '#type' => 'select', 
	    '#options'=>array(0=>'--TODOS--', 1=>'ACTIVO', 2=>'ACTIVO CON LICENCIA'),
	    '#disabled'=>false,	    
	);

	$form['gestion_socios']['filtrar']['buscar'] = array(
		'#value' => t('BUSCAR'),
	    '#type' => 'submit',    
	);	

	$header_socios = array(    
		'nro_socio'=>t('NRO. SOCIO'),
		'apellidos'=>t('APELLIDO'),
	    'nombres'=>t('NOMBRE'),
	    'tipo_doc'=>t('TIPO DOC.'), 
	    'nro_doc'=>t('NUMERO DOC.'), 
	    'estado'=>t('ESTADO')
    );


		
//drupal_set_message('<pre>'.print_r($socios->fetchAll(), true).'</pre>');
		
	//recuperamos los parametros
	if(isset($_SESSION[$formulario]['params']) && !empty($_SESSION[$formulario]['params']))
	{
		$filtro = false;
		$params = $_SESSION[$formulario]['params'];
		unset($_SESSION[$formulario]['params']);
		//buscamos los socios
		$socios = get_socios($params['apellidos'], $params['nombres'], $params['nro_socio'], $params['estado']);		
	}
	else
	{
		$socios = array();
	}

	if(empty($socios))
			$socios = array();
	//se genera un array filas_socios que tendra como elementos a cada fila generada por socio
	$filas_socios =array();

	$i = 0;
	//recorremos todos los socios recuperados y generamos el array de opciones
	//drupal_set_message('<pre>'.print_r(array_values($socios), true).'</pre>');
	if(!empty($socios))
		while ($socio = $socios->fetchObject()) 
		{
			//drupal_set_message('<pre>'.print_r($socio->id_socio, true).'</pre>');
			$id_socio = $socio->id_socio;
			$fila_socio =  
			  array(
			    'nro_socio'=> $socio->nro_socio.'/'.$socio->secuencia,
			    'apellidos'=> $socio->apellidos,
			    //'monto_acreditado'=> number_format($acreditacion['monto'], 2, ",", "."),
			    'nombres'=> $socio->nombres,
			    'tipo_doc'=> $socio->tipo_doc,
			    'nro_doc'=> $socio->nro_doc,
			    'estado'=> $socio->estado,
			    //l('Ver', 'microcreditos/ver_observacion_carpeta', array('query' => array('id' => $carpeta['id_carpeta_personal']))),
			    'id_socio'=> array(
			    	'data'=> array( 
			      		'#type' => 'hidden',               
		      			'#name' => '',               
			      		'#value'=> $id_socio,
			      	), 
		    	),     
			  ); 
			$filas_socios[$id_socio] = $fila_socio;
			$i++;
		}      
	
	$form['socios']['gestion_socios'] = array( 
		'#type'=>'fieldset',
		'#title'=>t('SOCIOS'),
		'#collapsible'=> TRUE,
	);

	$form['socios']['gestion_socios']['listado_socios'] = array(
		'#type' => 'tableselect',
		'#header' => $header_socios,
		'#options' => $filas_socios,
		'#default_value' => 109,
		'#multiple'=>FALSE,
		'#empty' => 'No existen socios.',
		'#prefix'=>'<div id="listado-socios">',
		'#suffix'=>'</div>',
		'#ajax'=> array(
	    	'callback'=>'ajax_radio_tabla_panel_socios_callback',
	    	'wrapper'=>'panel-socio-div',
	    	'method'=>'replace',
    	),
		'#description'=>t('Seleccione socio.')
	);
//drupal_set_message('<pre>'.print_r($form['socios']['gestion_socios']['listado_socios'], true).'</pre>');
	/*$form['socios']['gestion_socios']['paginador'] = array(
		'#markup'=>theme('pager'),
	);*/

/*	$form['socios']['gestion_socios']['ver_socio'] = array( 
		'#type'=>'submit',
		'#name'=>'ver_socio',
		'#value'=>'VER SOCIO',
		'#validate'=>array('gestionar_socios_form_validate'),
		'#submit'=>array('ver_socio_form_submit'),
	);*/

	$form['socios']['socio'] = array(   
		'#prefix' => '<div id=panel-socio-div>',
		'#suffix' => '</div>',
	);	
		
	if( isset($form_state['values']['listado_socios']) && !is_null($form_state['values']['listado_socios']))
		//isset($params['socio_seleccionado']) && !is_null($params['socio_seleccionado']))
	{
		//drupal_set_message('<pre>'.print_r($form_state['values']['listado_socios'],true).'</pre>');
		//$id_socio_seleccionado =$params['socio_seleccionado'];		
		$id_socio_seleccionado =$form_state['values']['listado_socios'];				
		/*drupal_set_message('<pre>'.print_r($filas_socios[$indice_socio_seleccionado],true).'</pre>');
		$id_socio_seleccionado = $filas_socios[$indice_socio_seleccionado]['id_socio']['data']['#value'];*/

		//buscamos el socio
		$socio_seleccionado = get_socio_por_id($id_socio_seleccionado);
		//drupal_set_message('<pre>'.print_r($socio_seleccionado,true).'</pre>');
		$form['socios']['socio']['fs_apellido_nombre']= array(
			'#type' => 'fieldset',			
		);
		
		$form['socios']['socio']['fs_apellido_nombre']['apellido_nombre']= array(
			'#prefix'=>'<div id="apellido-nombre-socio">',
	         '#markup'=>'<h2>'.$socio_seleccionado->apellidos./*' '.$socio_seleccionado->nombres.*/'- NUMERO DE SOCIO:'.$socio_seleccionado->nro_socio.'/'.$socio_seleccionado->secuencia.'</h2>',
	        '#suffix'=>'</div>',
  		);

  		$form['socios']['socio'] ['panel_socio'] = array(   
		'#type' => 'vertical_tabs', 
		);

		$form['socios']['socio']['panel_socio']['datos_personales']= array(    
			'#type'=>'fieldset',
			'#title'=>t('DATOS PERSONALES'),
  		);

		//BORRAR
		$uri_foto = 'https://cdn0.iconfinder.com/data/icons/handsome-man-avatars/283/stock_man_avatar-17-512.png';		

		$form['socios']['socio']['panel_socio']['datos_personales']['foto_socio']= array(
	        '#prefix'=>'<div id="foto-socio">',
	        '#markup'=>'<img src='.$uri_foto.'></img>',
	        '#suffix'=>'</div>',
      	);

  		$form['socios']['socio']['panel_socio']['datos_personales']['direccion']= array(
  			'#prefix'=>'<div id="datos-personales-socio"><div class="texto-dp" id="domicilio-socio">',
	        '#markup'=>'<span><strong>DOMICILIO: </strong>'.$socio_seleccionado->direccion.'</span>',
	        '#suffix'=>'</div>',
  		);

  		$form['socios']['socio']['panel_socio']['datos_personales']['fecha_nac']= array(
  			'#prefix'=>'<div class="io-texto texto-dp " id="fecha-nac-socio">',
	       	'#markup'=>'<span><strong>FECHA DE NACIMIENTO: </strong>'.$socio_seleccionado->fecha_nac.'</span>',
	        '#suffix'=>'</div>',
  		);

  		$form['socios']['socio']['panel_socio']['datos_personales']['edad']= array(
  			'#prefix'=>'<div class="io-texto texto-dp " id="edad-socio">',
	       	'#markup'=>'<span><strong>EDAD: </strong>'.$socio_seleccionado->edad.'</span>',
	        '#suffix'=>'</div>',
  		);

  		$form['socios']['socio']['panel_socio']['datos_personales']['telefono_fijo']= array(
  			'#prefix'=>'<div class="io-texto texto-dp" id="telefono-socio">',
	       	'#markup'=>'<span><strong>TELÉFONO FIJO: </strong>'.(empty($socio_seleccionado->telefono_fijo) ? '-' : $socio_seleccionado->telefono_fijo).'</span>',
	        '#suffix'=>'</div>',
  		);

  		$form['socios']['socio']['panel_socio']['datos_personales']['celular_1']= array(
  			'#prefix'=>'<div class="io-texto texto-dp" id="telefono-celular-1-socio">',
	       	'#markup'=>'<span><strong>TELÉFONO CELULAR: </strong>'.(empty($socio_seleccionado->celular_1) ? '-' : $socio_seleccionado->celular_1).'</span>',
	        '#suffix'=>'</div>',
  		);

  		$form['socios']['socio']['panel_socio']['datos_personales']['celular_2']= array(
  			'#prefix'=>'<div class="io-texto texto-dp" id="telefono-celular-2-socio">',
	       	'#markup'=>'<span><strong>TELÉFONO CELULAR ALTERNATIVO: </strong>'.(empty($socio_seleccionado->celular_2) ? '-' : $socio_seleccionado->celular_2).'</span>',
	        '#suffix'=>'</div>',
  		);

  		$form['socios']['socio']['panel_socio']['datos_personales']['estado_socio']= array(
  			'#prefix'=>'<div class="io-texto texto-dp" id="estado-socio">',
	       	'#markup'=>'<span><strong>ESTADO: </strong>'.$socio_seleccionado->estado.'</span>',
	        '#suffix'=>'</div>',
  		);

  		$form['socios']['socio']['panel_socio']['datos_personales']['categoria_socio']= array(
  			'#prefix'=>'<div class="io-texto texto-dp" id="categoria-socio">',
	       	'#markup'=>'<span><strong>CATEGORÍA: </strong>'.$socio_seleccionado->categoria.'</span>',
	        '#suffix'=>'</div>',
  		);

  		$form['socios']['socio']['panel_socio']['datos_personales']['tipo_socio']= array(
  			'#prefix'=>'<div class="io-texto texto-dp" id="tipo-socio">',
	       	'#markup'=>'<span><strong>TIPO DE SOCIO:</strong>'.$socio_seleccionado->tipo_socio.'</span>',
	        '#suffix'=>'</div></div>',
  		);


  		$form['socios']['socio']['panel_socio']['datos_personales']['cambiar_estado']= array(
  			'#prefix'=>'<div id="botones-dp"><div class="io-boton boton-dp" id="cambiar-estado-socio">',
	       	'#markup'=>l('CAMBIAR ESTADO', 'socios/estado_socio', array('query' => array('id' => $id_socio_seleccionado))),
	        '#suffix'=>'</div>',
  		);

  		$form['socios']['socio']['panel_socio']['datos_personales']['editar_socio']= array(
  			'#prefix'=>'<div class="io-boton boton-dp" id="editar-socio-socio">',
	       	'#markup'=>l('EDITAR DATOS', 'socios/socio', array('query' => array('id' => $id_socio_seleccionado))),
	        '#suffix'=>'</div></div>',
  		);
  		
  		//PESTAÑA GRUPO FAMILIAR
  		//esta pestaña solo se va mostrar si el soscio es de tipo 2 (MIEMBRO DE FAMILIA) o 3 (TITULAR DE FAMILIA) 
  		if(($socio_seleccionado->tipo_socio_id == 2) || ($socio_seleccionado->tipo_socio_id == 3))
  		{
  			//buscamos los socios que componen el grupo familiar
  			get_socios_grupo_familiar($id_socio_seleccionado);
  			$form['socios']['socio']['panel_socio']['grupo_familiar']= array(    
				'#type'=>'fieldset',
				'#title'=>t('GRUPO FAMILIAR'),
  			);

  			/*$form['socios']['socio']['panel_socio']['grupo_familiar']['tipo_gf']= array(    
				'#prefix'=>'<div class="io-texto texto-dp" id="tipo-socio">',
	       		'#markup'=>'<span><strong>TIPO DE SOCIO:</strong>'.$socio_seleccionado->tipo_socio.'</span>',
	        	'#suffix'=>'</div></div>',
  			);*/
  		}
  		

  		//PESTAÑA ACTIVIDADES
  		$form['socios']['socio']['panel_socio']['actividades']= array(    
			'#type'=>'fieldset',
			'#title'=>t('ACTIVIDADES'),
  		);

  		//PESTAÑA SERVICIOS
  		$form['socios']['socio']['panel_socio']['servicios']= array(    
			'#type'=>'fieldset',
			'#title'=>t('SERVICIOS'),
  		);

  		//PESTAÑA CUENTA CORRIENTE
  		$form['socios']['socio']['panel_socio']['cuenta_corriente']= array(    
			'#type'=>'fieldset',
			'#title'=>t('CUENTA CORRIENTE'),
  		);
	}

	$form['salir'] = array(
		'#type'=>'submit',
		'#value'=> t('SALIR'),
	);

	return $form;
}

function gestionar_socios_form_validate($form, &$form_state)
{
	/*if(empty($form_state['values']['listado_acreditaciones']))
				form_set_error('listado_acreditaciones', 'Debe seleccionar una acreditacion para realizar la accion que esta solicitando');*/
}

function gestionar_socios_form_submit($form, &$form_state)
{
	//drupal_set_message('<pre>'.print_r($form, true).'</pre>');
	
	$uid = $GLOBALS['user']->uid;
	$formulario = $form_state['values']['formulario'];
    try { 
    	//creamos un arrray dentro del SESSION para que sea recuperado dentro de params
    	$_SESSION[$formulario]['params']['apellidos'] = (!empty($form_state['values']['filtro_apellidos']) ? strtoupper($form_state['values']['filtro_apellidos']) : NULL);
    	$_SESSION[$formulario]['params']['nombres'] = (!empty($form_state['values']['filtro_nombres']) ? strtoupper($form_state['values']['filtro_nombres']) : NULL);
    	$_SESSION[$formulario]['params']['nro_socio'] = (!empty($form_state['values']['filtro_nro_socio']) ? $form_state['values']['filtro_nro_socio'] : NULL);
    	$_SESSION[$formulario]['params']['estado'] = (($form_state['values']['filtro_estado'] != 0) ? $form_state['values']['filtro_estado'] : NULL);
   //$form_state['redirect'] = array('socios/estado_socio',array('query' => array('id'=>50)));
  	}
 	catch (Exception $e) {  		
  		watchdog_exception('my_type', $e);
  		drupal_set_message('<pre>'. print_r("Error en la trasaccion - Comunicarse con el proveedor de sistemas.".$e, TRUE) .'</pre>');
	}	
}

function ver_socio_form_valdiate($form, &$form_state)
{
	$socio_seleccionado = array_values(array_filter($form_state['values']['listado_socios']));
 	if(!isset($socio_seleccionado) || empty($socio_seleccionado))
    	form_set_error('listado_socios', 'Debe seleccionar un cheque para poder cambiar a listo');
}

function ver_socio_form_submit($form, &$form_state)
{
	drupal_set_message('<pre>'.print_r($form_state['values']['listado_socios'], true).'</pre>');
	
	$uid = $GLOBALS['user']->uid;
	$formulario = $form_state['values']['formulario'];
    try { 
    	//creamos un arrray dentro del SESSION para que sea recuperado dentro de params
    	$_SESSION[$formulario]['params']['apellidos'] = (!empty($form_state['values']['filtro_apellidos']) ? strtoupper($form_state['values']['filtro_apellidos']) : NULL);
    	$_SESSION[$formulario]['params']['nombres'] = (!empty($form_state['values']['filtro_nombres']) ? strtoupper($form_state['values']['filtro_nombres']) : NULL);
    	$_SESSION[$formulario]['params']['nro_socio'] = (!empty($form_state['values']['filtro_nro_socio']) ? $form_state['values']['filtro_nro_socio'] : NULL);
    	$_SESSION[$formulario]['params']['estado'] = (($form_state['values']['filtro_estado'] != 0) ? $form_state['values']['filtro_estado'] : NULL);
    	$_SESSION[$formulario]['params']['socio_seleccionado'] = (!empty($form_state['values']['listado_socios']) ? $form_state['values']['listado_socios'] : NULL);
   		
  	}
 	catch (Exception $e) {  		
  		watchdog_exception('my_type', $e);
  		drupal_set_message('<pre>'. print_r("Error en la trasaccion - Comunicarse con el proveedor de sistemas.".$e, TRUE) .'</pre>');
	}	
}


function ajax_radio_tabla_panel_socios_callback($form, $form_state)
{
	return $form['socios']['socio'];
}

function ajax_prueba_callback($form, $form_state)
{
	return $form['gestion_socios']['filtrar']['nombre_p'];
}

function get_listado_grupo_familiar_markup($id_socio)
{
  /*$historial_mostrar = array();
  //drupal_set_message('<pre>'.print_r($factura_estados, true).'</pre>');
  for ($i=0;$i<count($factura_estados);$i++) { 
   
    $color = get_color_estado_sol_pago($factura_estados[$i]['ESTADO_FACTURA_ID']);
    
    $historial_mostrar[$i]['estado'] = array('data'=>'<span class="'.$color.'">'.$factura_estados[$i]['ESTADO'].'</span>','width'=>'15%');
    $historial_mostrar[$i]['fecha_estado'] = array('data'=>$factura_estados[$i]['FECHA_ESTADO'],'width'=>'15%');
    $nombre=get_nombre_usuario_por_id($factura_estados[$i]['USUARIO_ALTA']);
    
    $historial_mostrar[$i]['usuario'] = array('data'=>$nombre[0]['name'],'width'=>'15%');
    $historial_mostrar[$i]['observacion'] = array('data'=>$factura_estados[$i]['OBSERVACION'],'width'=>'70%');
  }  
 
  $header = array(array('data'=>t('Estado'),'width'=>'15%'),
            array('data'=>t('Fecha'),'width'=>'15%'), 
            array('data'=>t('Usuario'),'width'=>'15%'),
            array('data'=>t('Observacion'),'width'=>'55%'));
            
 
 
  $output = theme('table', array('header' => $header, 
                   'rows' => $historial_mostrar, 
                   'attributes' => array('class' => array('tabla-historial-sol-pago'))
                  )
           ); 

  return $output;  */
}
?>