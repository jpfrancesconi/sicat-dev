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
function datos_socio_form($form, &$form_state)
{
	$form['#attached']['css'] = array(
		drupal_get_path('module', 'socios') . '/css/socios.css',
	);

	if(strpos($_SERVER['HTTP_REFERER'], 'gestionar_socios'))
	{		
		$array_retorno = parse_url($_SERVER['HTTP_REFERER']);
		$_SESSION['retorno'] = 'socios/gestionar_socios';    
		$_SESSION['retorno_params'] =array('query' =>get_params_url($_SERVER['HTTP_REFERER']));
	}
	else if(strpos($_SERVER['HTTP_REFERER'], 'datos_socio') && isset($_GET['ret']))
	{		
		$_SESSION['retorno'] = 'socios/datos_socio_ret';    	
	}
	else if(strpos($_SERVER['HTTP_REFERER'], 'datos_socio') && !isset($_GET['ret']))
	{	
		$array_retorno = parse_url($_SERVER['HTTP_REFERER']);
		$_SESSION['retorno'] = 'socios/gestionar_socios';    
		$_SESSION['retorno_params'] =array('query' =>get_params_url($_SERVER['HTTP_REFERER']));
	}

	if(isset($_GET['id']) && !empty($_GET['id']))
	{
		$id_socio_seleccionado = $_GET['id'];

		$form['id_socio'] = array(
		'#type'=>'hidden',
		'#value'=>$id_socio_seleccionado,
		);

		//drupal_set_message('<pre>'.print_r($form_state['values']['listado_socios'],true).'</pre>');		
		/*drupal_set_message('<pre>'.print_r($filas_socios[$indice_socio_seleccionado],true).'</pre>');
		$id_socio_seleccionado = $filas_socios[$indice_socio_seleccionado]['id_socio']['data']['#value'];*/

		//buscamos el socio
		$socio_seleccionado = get_socio_por_id($id_socio_seleccionado);
		//drupal_set_message('<pre>'.print_r($socio_seleccionado,true).'</pre>');

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


  		$form['socios']['socio']['panel_socio']['datos_personales']['adjuntar_foto']= array(
  			'#prefix'=>'<div id="botones-dp"><div class="io-boton boton-dp" id="foto-socio">',
	       	'#markup'=>l('FOTO DEL SOCIO', 'socios/adjuntar_foto', array('query' => array('id_p' => $socio_seleccionado->id_persona))),
	        '#suffix'=>'</div>',
  		);

  		$form['socios']['socio']['panel_socio']['datos_personales']['editar_socio']= array(
  			'#prefix'=>'<div id="botones-dp"><div class="io-boton boton-dp" id="editar-socio-socio">',
	       	'#markup'=>l('EDITAR DATOS', 'socios/socio', array('query' => array('id' => $id_socio_seleccionado))),
	        '#suffix'=>'</div></div>',
  		);
  		
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
	}

	$form['volver'] = array(
		'#type'=>'submit',
		'#value'=> t('VOLVER'),
		'#submit'=> array('datos_socio_volver_submit'),
	);
	return $form;
}

function datos_socio_form_submit($form, &$form_state)
{
	
}

function datos_socio_volver_submit($form, &$form_state)
{
	$retorno = array($_SESSION['retorno']);  
	unset($_SESSION['retorno']);

	if($retorno[0] == 'socios/datos_socio_ret')
	{
		$form_state['redirect'] = array('socios/datos_socio',array('query' => array('id'=>$_GET['ret'])));
	}  	
  	else
  	{
  		if(isset($_SESSION['retorno_params']))
		{  	
		  array_push($retorno, $_SESSION['retorno_params']);
		  unset($_SESSION['retorno_params']);
		}
	    //drupal_set_message('<pre>'.print_r($retorno, true).'</pre>');
	    $form_state['redirect'] = $retorno;		
  	}
  
}

function get_listado_grupo_familiar_markup($grupo_familiar)
{
  $grupo_familiar_mostrar = array();
  
  for ($i=0;$i<count($grupo_familiar);$i++) {        
    $grupo_familiar_mostrar[$i]['nro_socio'] = $grupo_familiar[$i]->nro_socio_titular.'/'.$grupo_familiar[$i]->secuencia;
    $grupo_familiar_mostrar[$i]['apellidos'] = $grupo_familiar[$i]->apellidos;
 	$grupo_familiar_mostrar[$i]['nombres'] = $grupo_familiar[$i]->nombres;    
 	$grupo_familiar_mostrar[$i]['edad'] = $grupo_familiar[$i]->edad;
 	$grupo_familiar_mostrar[$i]['parentezco'] = $grupo_familiar[$i]->parentezco;
  }  
 
  $header = array('NUMERO DE SOCIO', 'APELLIDOS', 'NOMBRES', 'EDAD', 'PARENTEZCO');
            
 
 
  $output = theme('table', array('header' => $header, 
                   'rows' => $grupo_familiar_mostrar, 
                   'attributes' => array('class' => array('tabla-grupo-familiar-socio'))
                  )
           ); 

  return $output;  
}

function get_listado_cuenta_corriente_markup($cuenta_corriente)
{
  $cuenta_corriente_mostrar = array();
  
  for ($i=0;$i<count($cuenta_corriente);$i++) {  
  	if($cuenta_corriente[$i]->pagado == "SI")
  		$color = "green";
  	else
  		$color = "red";      

    $cuenta_corriente_mostrar[$i]['fecha_emision'] = $cuenta_corriente[$i]->fecha_emision;
    $cuenta_corriente_mostrar[$i]['mes'] = $cuenta_corriente[$i]->mes;
 	$cuenta_corriente_mostrar[$i]['anio'] = $cuenta_corriente[$i]->anio;
 	$cuenta_corriente_mostrar[$i]['concepto'] = $cuenta_corriente[$i]->concepto;
 	$cuenta_corriente_mostrar[$i]['valor'] = $cuenta_corriente[$i]->valor;
 	$cuenta_corriente_mostrar[$i]['pagado'] = array('data'=>'<span style="color:'.$color.';"><strong>'.$cuenta_corriente[$i]->pagado.'</strong></span>','width'=>'15%');	
 	$cuenta_corriente_mostrar[$i]['fecha_pago'] = $cuenta_corriente[$i]->fecha_pago; 	
 	$cuenta_corriente_mostrar[$i]['anulado'] = $cuenta_corriente[$i]->anulado; 	
  }  
 
  $header = array('FECHA DE EMISION', 'MES', 'AÑO', 'CONCEPTO', 'VALOR', 'PAGADO', 'FECHA DE PAGO', 'ANULADO');
            
 
 
  $output = theme('table', array('header' => $header, 
                   'rows' => $cuenta_corriente_mostrar, 
                   'attributes' => array('class' => array('tabla-cuenta-corriente-socio'))
                  )
           ); 

  return $output;  
}
?>