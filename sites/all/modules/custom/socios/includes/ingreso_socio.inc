<?php
/**
* Permite ver los datos vinculados a un socio en particular
*
* @author Giuici Francisco
* @version 1.0
* @package sicat
* @subpackage socios
*/


/**
* Formulario que gestionar socios
*/
function ingreso_socio_form($form, &$form_state)
{
	$form['#attached']['css'] = array(
		drupal_get_path('module', 'socios') . '/css/socios.css',
	);

	$form['buscar_socio'] = array( 
		'#type'=>'fieldset',
		'#title'=>'SOCIO',
	);

	$form['buscar_socio']['nro_socio'] = array( 
		'#type'=>'textfield',
		'#title'=>'NUMERO SOCIO',
		'#ajax' => array(       
			'wrapper' => 'datos-socio',
			'callback' => 'datos_socio_ajax_callback',
			'method' => 'replace',
		),
	);



	$form['socios']['inicio']= array(    
		    	'#markup' => '<div id="datos-socio">',
	);


	if(isset($form_state['values']['nro_socio']) && !empty($form_state['values']['nro_socio']))
	{		
		$nro_socio = $form_state['values']['nro_socio'];				

		//drupal_set_message('<pre>'.print_r($form_state['values']['listado_socios'],true).'</pre>');		
		/*drupal_set_message('<pre>'.print_r($filas_socios[$indice_socio_seleccionado],true).'</pre>');
		$id_socio_seleccionado = $filas_socios[$indice_socio_seleccionado]['id_socio']['data']['#value'];*/

		//buscamos el socio
		$socio_seleccionado = get_socio_por_nro_socio($nro_socio);
		$id_socio_seleccionado = $socio_seleccionado->id_socio;
		//drupal_set_message('<pre>'.print_r($socio_seleccionado,true).'</pre>');

		//mostramos un mensaje con el estado del socio y su estado de deuda
		$form['socios']['socio']['estado']= array(
			'#prefix'=>'<div id="estado-deuda-socio" class="socio-sin-deuda">',
	        '#markup'=>'<strong>ACA VA EL ESTADO DE LA DEUDA</br>CASO SIN DEUDA</strong>',
	        '#suffix'=>'</div>',
  		);

		$form['socios']['socio']['estado_1']= array(
			'#prefix'=>'<div id="estado-deuda-socio" class="socio-con-deuda">',
	        '#markup'=>'<strong>ACA VA EL ESTADO DE LA DEUDA</br>CASO SIN DEUDA</strong>',
	        '#suffix'=>'</div>',
  		);
		//si es miembro de familia tiene que tener el numero de socio de su titular
		if($socio_seleccionado->tipo_socio_id == 2)
		{
			$grupo_familiar = get_grupo_familiar($socio_seleccionado->id_socio);
			$socio_seleccionado->nro_socio = $grupo_familiar->nro_socio_titular;
			//TODO: SI CAMBIA NRO_SOCIO_TITULAR POR ID_SOCIO_TITULAR CAMBIAR ACA
			$socio_titular_familia = get_socio_por_nro_socio($grupo_familiar->nro_socio_titular);
		}

		$form['socios']['socio']['fs_apellido_nombre']= array(
			'#type' => 'fieldset',		
			'#prefix' => '<div id="fieldset-socio">',	
		);
		
		$form['socios']['socio']['fs_apellido_nombre']['apellido_nombre']= array(
			'#prefix'=>'<div id="apellido-nombre-socio">',
	         '#markup'=>'<h2>'.$socio_seleccionado->apellidos.' '.$socio_seleccionado->nombres.'- NUMERO DE SOCIO:'.$socio_seleccionado->nro_socio.'/'.$socio_seleccionado->secuencia.'</h2>',
	        '#suffix'=>'</div>',
  		);

  		$form['socios']['socio'] ['panel_socio'] = array(   
		'#type' => 'vertical_tabs', 
		);

		$form['socios']['socio']['panel_socio']['datos_personales']= array(    
			'#type'=>'fieldset',
			'#title'=>t('DATOS PERSONALES'),
  		);

		if(isset($socio_seleccionado->uri_foto))
			$uri_foto = $socio_seleccionado->uri_foto;		
		else
			$uri_foto = 'https://cdn0.iconfinder.com/data/icons/handsome-man-avatars/283/stock_man_avatar-17-512.png';	

		if(validar_categoria($socio_seleccionado->edad, $socio_seleccionado->categoria_socio_id) || is_null($socio_seleccionado->categoria_socio_id))
		{
			$form['socios']['socio']['panel_socio']['datos_personales']['categoria_socio_invalida']= array(    
				'#prefix'=>'<div class="io-texto texto-dp" id="categoria-socio-invalida">',
				'#markup'=>'<span><strong>CATEGORÍA DE SOCIO INCORRECTA - REVISAR CATEGORÍA DEL SOCIO</strong></span>',
				'#suffix'=>'</div>',
			);
		}

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

  		//TODO: validar la categoria del socio
  		$form['socios']['socio']['panel_socio']['datos_personales']['categoria_socio']= array(
  			'#prefix'=>'<div class="io-texto texto-dp" id="categoria-socio">',
	       	'#markup'=>'<span><strong>CATEGORÍA: </strong>'.$socio_seleccionado->categoria.'</span>',
	        '#suffix'=>'</div>',
  		);

  		$form['socios']['socio']['panel_socio']['datos_personales']['tipo_socio']= array(
  			'#prefix'=>'<div class="io-texto texto-dp" id="tipo-socio">',
	       	'#markup'=>'<span><strong>TIPO DE SOCIO:</strong>'.$socio_seleccionado->tipo_socio.'</span>',
	        '#suffix'=>'</div>',
  		);

  		//si es miembro de familia se perimite ir al socio titular de familia
  		if($socio_seleccionado->tipo_socio_id == 2)
  		{
  			$form['socios']['socio']['panel_socio']['datos_personales']['socio_titular']= array(
  			'#prefix'=>'<div id="titular"><div>',
	       	'#markup'=>'<span><strong>TITULAR DE FAMILIA: </strong></span>'.l($socio_titular_familia->nombres.' '.$socio_titular_familia->apellidos, 'socios/datos_socio', array('query' => array('id' => $socio_titular_familia->id_socio, 'ret'=>$socio_seleccionado->id_socio))),
	        '#suffix'=>'</div></div>',
  			);
  		}
  		else
  		{
  			$form['socios']['socio']['panel_socio']['datos_personales']['socio_titular_vacio']= array(
	       	'#markup'=>'</div>'
  			);
  		}
  		
  		//PESTAÑA GRUPO FAMILIAR
  		//esta pestaña solo se va mostrar si el soscio es de tipo 2 (MIEMBRO DE FAMILIA) o 3 (TITULAR DE FAMILIA) 
  		if(($socio_seleccionado->tipo_socio_id == 2) || ($socio_seleccionado->tipo_socio_id == 3))
  		{
  			//buscamos los socios que componen el grupo familiar
  			$grupo_familiar = get_socios_grupo_familiar($id_socio_seleccionado);
  			//drupal_set_message('<pre>'.print_r($grupo_familiar, true).'</pre>');  			

  			$form['socios']['socio']['panel_socio']['grupo_familiar']= array(    
				'#type'=>'fieldset',
				'#title'=>t('GRUPO FAMILIAR'),
  			);		

  			if(validar_tipo_grupo_familiar($grupo_familiar))
  			{
  				$form['socios']['socio']['panel_socio']['grupo_familiar']['gf_invalido']= array(    
					'#prefix'=>'<div class="io-texto texto-gf" id="grupo-familiar-invalido">',
		       		'#markup'=>'<span><strong>GRUPO FAMILIAR INCORRECTO - REVISAR TIPO DE GRUPO FAMILIAR</strong></span>',
		        	'#suffix'=>'</div>',
  				);
  			}

  			$form['socios']['socio']['panel_socio']['grupo_familiar']['tipo_gf']= array(    
				'#prefix'=>'<div class="io-texto texto-gf" id="tipo-grupo-familiar">',
	       		'#markup'=>'<span><strong>TIPO DE GRUPO FAMILIAR: </strong>'.$grupo_familiar[0]->tipo_grupo_familiar.'</span>',
	        	'#suffix'=>'</div>',
  			);

		    $form['socios']['socio']['panel_socio']['grupo_familiar']['listado_gf']= array(    
		    	'#markup' => get_listado_grupo_familiar_markup($grupo_familiar),
		    );
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
  		if(($socio_seleccionado->tipo_socio_id != 2))
  		{
	  		$form['socios']['socio']['panel_socio']['cuenta_corriente']= array(    
				'#type'=>'fieldset',
				'#title'=>t('CUENTA CORRIENTE'),
	  		);

	  		$cuenta_corriente = get_cta_cte($socio_seleccionado->id_socio);

	  		$form['socios']['socio']['panel_socio']['cuenta_corriente']['listado_cta_cte']= array(    
		    	'#markup' => get_listado_cuenta_corriente_markup($cuenta_corriente),
			);
  		}

  		if(isset($grupo_familiar))
  		{  		
  			//drupal_set_message('<pre>'.print_r($familiar, true).'</pre>');			
  			foreach ($grupo_familiar as $familiar) {
  				$id_socio_familiar = $familiar->id_socio;
  				
  				if($familiar->nro_socio!=$socio_seleccionado->nro_socio)
					$form = obtener_formulario_datos($familiar,$socio_seleccionado->nro_socio ,$form);

  			}
  		}  		
	}

	$form['socios']['cierre']= array(    
		    	'#markup' => '</div>',
	);

	return $form;
}

function ingreso_socio_form_submit($form, &$form_state)
{
	
}

function datos_socio_ajax_callback($form, &$form_state) {
  return $form['socios'];
}

function obtener_formulario_datos($familiar, $nro_socio_titular ,$form)
{
	//creamos un indice del array que contemple el id del socio para que sea unico
	$indice = 'panel_socio_'.$familiar->id_socio;

	$socio_seleccionado = get_socio_por_id($familiar->id_socio);

	$form['socios']['socio']['fs_apellido_nombre'.$familiar->id_socio]= array(
		'#type' => 'fieldset',		
		'#prefix' => '<div id="fieldset-socio">',	
	);

	$form['socios']['socio']['fs_apellido_nombre'.$familiar->id_socio]['apellido_nombre']= array(
		'#prefix'=>'<div id="apellido-nombre-socio">',
	     '#markup'=>'<h2>'.$socio_seleccionado->apellidos.' '.$socio_seleccionado->nombres.'- NUMERO DE SOCIO:'.$nro_socio_titular.'/'.$socio_seleccionado->secuencia.'</h2>',
	    '#suffix'=>'</div>',
	);

	$form['socios']['socio'][$indice] = array(   
		'#type' => 'vertical_tabs', 
	);

	$form['socios']['socio'][$indice]['datos_personales']=array(    
		'#type'=>'fieldset',
		'#title'=>t('DATOS PERSONALES'),
	);

	if(isset($socio_seleccionado->uri_foto))
		$uri_foto = $socio_seleccionado->uri_foto;		
	else
		$uri_foto = 'https://cdn0.iconfinder.com/data/icons/handsome-man-avatars/283/stock_man_avatar-17-512.png';	

	if(validar_categoria($socio_seleccionado->edad, $socio_seleccionado->categoria_socio_id) || is_null($socio_seleccionado->categoria_socio_id))
	{
		$form['socios']['socio'][$indice]['datos_personales']['categoria_socio_invalida']= array(    
			'#prefix'=>'<div class="io-texto texto-dp" id="categoria-socio-invalida">',
			'#markup'=>'<span><strong>CATEGORÍA DE SOCIO INCORRECTA - REVISAR CATEGORÍA DEL SOCIO</strong></span>',
			'#suffix'=>'</div>',
		);
	}

	$form['socios']['socio'][$indice]['datos_personales']['foto_socio']= array(
        '#prefix'=>'<div id="foto-socio">',
        '#markup'=>'<img src='.$uri_foto.'></img>',
        '#suffix'=>'</div>',
  	);

	$form['socios']['socio'][$indice]['datos_personales']['direccion']= array(
		'#prefix'=>'<div id="datos-personales-socio"><div class="texto-dp" id="domicilio-socio">',
    '#markup'=>'<span><strong>DOMICILIO: </strong>'.$socio_seleccionado->direccion.'</span>',
    '#suffix'=>'</div>',
	);

	$form['socios']['socio'][$indice]['datos_personales']['fecha_nac']= array(
		'#prefix'=>'<div class="io-texto texto-dp " id="fecha-nac-socio">',
   	'#markup'=>'<span><strong>FECHA DE NACIMIENTO: </strong>'.$socio_seleccionado->fecha_nac.'</span>',
    '#suffix'=>'</div>',
	);

	$form['socios']['socio'][$indice]['datos_personales']['edad']= array(
		'#prefix'=>'<div class="io-texto texto-dp " id="edad-socio">',
   	'#markup'=>'<span><strong>EDAD: </strong>'.$socio_seleccionado->edad.'</span>',
    '#suffix'=>'</div>',
	);

	$form['socios']['socio'][$indice]['datos_personales']['telefono_fijo']= array(
		'#prefix'=>'<div class="io-texto texto-dp" id="telefono-socio">',
   	'#markup'=>'<span><strong>TELÉFONO FIJO: </strong>'.(empty($socio_seleccionado->telefono_fijo) ? '-' : $socio_seleccionado->telefono_fijo).'</span>',
    '#suffix'=>'</div>',
	);

	$form['socios']['socio'][$indice]['datos_personales']['celular_1']= array(
		'#prefix'=>'<div class="io-texto texto-dp" id="telefono-celular-1-socio">',
   	'#markup'=>'<span><strong>TELÉFONO CELULAR: </strong>'.(empty($socio_seleccionado->celular_1) ? '-' : $socio_seleccionado->celular_1).'</span>',
    '#suffix'=>'</div>',
	);

	$form['socios']['socio'][$indice]['datos_personales']['celular_2']= array(
		'#prefix'=>'<div class="io-texto texto-dp" id="telefono-celular-2-socio">',
   	'#markup'=>'<span><strong>TELÉFONO CELULAR ALTERNATIVO: </strong>'.(empty($socio_seleccionado->celular_2) ? '-' : $socio_seleccionado->celular_2).'</span>',
    '#suffix'=>'</div>',
	);

	$form['socios']['socio'][$indice]['datos_personales']['estado_socio']= array(
		'#prefix'=>'<div class="io-texto texto-dp" id="estado-socio">',
   	'#markup'=>'<span><strong>ESTADO: </strong>'.$socio_seleccionado->estado.'</span>',
    '#suffix'=>'</div>',
	);

	//TODO: validar la categoria del socio
	$form['socios']['socio'][$indice]['datos_personales']['categoria_socio']= array(
		'#prefix'=>'<div class="io-texto texto-dp" id="categoria-socio">',
   	'#markup'=>'<span><strong>CATEGORÍA: </strong>'.$socio_seleccionado->categoria.'</span>',
    '#suffix'=>'</div>',
	);

	$form['socios']['socio'][$indice]['datos_personales']['tipo_socio']= array(
		'#prefix'=>'<div class="io-texto texto-dp" id="tipo-socio">',
   	'#markup'=>'<span><strong>TIPO DE SOCIO:</strong>'.$socio_seleccionado->tipo_socio.'</span>',
    '#suffix'=>'</div>',
	);
	

	//PESTAÑA ACTIVIDADES
	$form['socios']['socio'][$indice]['actividades']= array(    
	'#type'=>'fieldset',
	'#title'=>t('ACTIVIDADES'),
	);

	//PESTAÑA SERVICIOS
	$form['socios']['socio'][$indice]['servicios']= array(    
	'#type'=>'fieldset',
	'#title'=>t('SERVICIOS'),
	);

	return $form;
}

?>