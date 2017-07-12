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
	/*$form['#attached']['js'] = array(
		drupal_get_path('module', 'socios') . '/js/ingreso_socio.js',
	);*/

	$form['buscar_socio'] = array(
		'#type'=>'fieldset',
		'#title'=>'BUSCAR POR NUMERO DE SOCIO',
	);

	$form['buscar_socio']['nro_socio'] = array(
		'#type'=>'textfield',
		'#title'=>'NUMERO SOCIO',
	);

	$form['buscar_socio']['tipo_doc'] = array(
		'#type'=>'select',
		'#title'=>'TIPO DE DOCUMENTO',
		'#options' => array('DNI'=>'DNI','LC'=>'LC','LE'=>'LE','DNI EXT.'=>'DNI EXT.'),
    '#default_value'=>(isset($tipo_doc) ? $tipo_doc : null)
	);

	$form['buscar_socio']['nro_doc'] = array(
		'#type'=>'textfield',
		'#title'=>'NUMERO DE DOCUMENTO',
    '#default_value'=>(isset($nro_doc) ? $nro_doc : null)
	);

	$form['buscar_socio']['nro_socio'] = array(
		'#type'=>'textfield',
		'#title'=>'NUMERO SOCIO',
	);

	$form['buscar_socio']['apellidos'] = array(
		'#type'=>'textfield',
		'#title'=>'APELLIDOS',
	);

	$form['buscar_socio']['nombres'] = array(
		'#type'=>'textfield',
		'#title'=>'NOMBRES',
	);

	$form['buscar_socio']['buscar'] = array(
		'#type'=>'submit',
		'#value'=>'BUSCAR',
	);

	$form['socios']['inicio']= array(
		    	'#markup' => '<div id="datos-socio">',
	);

	if((isset($_GET['ns']) && !is_null($_GET['ns'])))
	{
		$nro_socio = $_GET['ns'];
		$form['buscar_socio']['nro_socio']['#default_value'] = $nro_socio;
	}
	else
	{
		$nro_socio = NULL;
	}
	if((isset($_GET['nom']) && !is_null($_GET['nom'])))
	{
		$nombres = $_GET['nom'];
		$form['buscar_socio']['nombres']['#default_value'] = $nombres;
	}
	else
	{
		$nombres = NULL;
	}
	if((isset($_GET['ap']) && !is_null($_GET['ap'])))
	{
		$apellidos = $_GET['ap'];
		$form['buscar_socio']['apellidos']['#default_value'] = $apellidos;
	}
	else
	{
		$apellidos = NULL;
	}
	if((isset($_GET['td']) && !is_null($_GET['td'])))
	{
		$tipo_doc = $_GET['td'];
		$form['buscar_socio']['tipo_doc']['#default_value'] = $tipo_doc;
	}
	else
	{
		$tipo_doc = NULL;
	}
	if((isset($_GET['nd']) && !is_null($_GET['nd'])))
	{
		$nro_doc = $_GET['nd'];
		$form['buscar_socio']['nro_doc']['#default_value'] = $nro_doc;
	}
	else
	{
		$nro_doc = NULL;
	}

	if(!is_null($apellidos) || !is_null($nombres) || !is_null($nro_socio) || !is_null($tipo_doc) || !is_null($nro_doc))
	{
		//buscamos
		//drupal_set_message('<pre>'.print_r($apellidos,true).'</pre>');
		$socios = get_socios($apellidos, $nombres, $nro_socio,NULL, NULL, NULL, $tipo_doc, $nro_doc);

		//guardamos en socios_temp el resultado del fectchAll y vemo si hay solo un socio
		//drupal_set_message('<pre>'.print_r($socios,true).'</pre>');
		//vemos si solo hay un socio en el resultado
		if($socios->rowCount() == 1)
		{

			//en el caso de que solo haya un socio redireccionamos a ver los datos del socios
			//tomamos la url de base y le agregamos la ruta a donde redireccionamos
			//sacamos el socio
			$socio = $socios->fetchObject();
			header('Location: '.$GLOBALS['base_url'].'/socios/ingreso_socio/socio?id='.$socio->id_socio);
		}

		$header_socios = array(
		'nro_socio'=>t('NRO. SOCIO'),
		'apellidos'=>t('APELLIDO'),
	    'nombres'=>t('NOMBRE'),
	    'tipo_doc'=>t('TIPO DOC.'),
	    'nro_doc'=>t('NUMERO DOC.'),
	    'fecha_ingreso'=>t('FECHA DE INGRESO'),
	    'estado'=>t('ESTADO'),
	    'categoria_socio'=>t('CAT. O TIPO GRUPO FAMILIAR'),
	    'tipo_socio'=>t('TIPO DE SOCIO'),
		);


		if(empty($socios))
				$socios = array();
		//se genera un array filas_socios que tendra como elementos a cada fila generada por socio
		$filas_socios =array();

		$i = 0;
		//recorremos todos los socios recuperados y generamos el array de opciones

		if(!empty($socios))
		{
			//colapasamos el buscador
			$form['buscar_socio']['#collapsed'] = TRUE;
			while ($socio = $socios->fetchObject())
			{
				//drupal_set_message('<pre>'.print_r($socio, true).'</pre>');
				$id_socio = $socio->id_socio;
				//ver si el socio es miembro de familia y buscar el numero de socio del titular
				if($socio->id_tipo_socio == 2)
				{
					$grupo_familiar = get_grupo_familiar($socio->id_socio);
					$socio->nro_socio = $grupo_familiar->nro_socio_titular;
					if(isset($grupo_familiar->nombre_tipo_grupo_familiar))
						$socio->categoria_socio = $grupo_familiar->nombre_tipo_grupo_familiar;
				}
				else if($socio->id_tipo_socio == 3)
				{
					$grupo_familiar = get_grupo_familiar($socio->id_socio);
					if(isset($grupo_familiar->nombre_tipo_grupo_familiar))
						$socio->categoria_socio = $grupo_familiar->nombre_tipo_grupo_familiar;
				}

				//vemos si el socio esta en estado tipo baja para pintarlo
				//drupal_set_message('<pre>'.print_r($socio,true).'</pre>');
				if(get_tipo_estado($socio->estado_socio_id))
				{
					$color = 'red';
				}
				else {
					$color = 'green';
				}
				$fila_socio =
				  array(
				    'nro_socio'=> $socio->nro_socio.'/'.$socio->secuencia,
				    'apellidos'=> $socio->apellidos,
				    'nombres'=> $socio->nombres,
				    'tipo_doc'=> $socio->tipo_doc,
				    'nro_doc'=> $socio->nro_doc,
				    'fecha_ingreso'=> $socio->fecha_ingreso,
				    'estado'=> '<span style="color:'.$color.';">'.$socio->estado.'</span>',
				    'categoria_socio'=> $socio->categoria_socio,
				    'tipo_socio'=> $socio->tipo_socio,
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
		}


		$form['buscar_socio_ape_nom']['resultado_socios'] = array(
			'#type'=>'fieldset',
			'#title'=>t('SOCIOS'),
			'#collapsible'=> TRUE,
		);

		$form['buscar_socio_ape_nom']['resultado_socios']['listado_socios'] = array(
			'#type' => 'tableselect',
			'#header' => $header_socios,
			'#options' => $filas_socios,
			'#multiple'=>FALSE,
			'#empty' => 'No existen socios.',
			'#prefix'=>'<div id="listado-socios">',
			'#suffix'=>'</div>',
			'#description'=>t('Seleccione socio.')
		);
	}

	$form['buscar_socio_ape_nom']['resultado_socios']['ver_socio'] = array(
		'#type'=>'submit',
		'#name'=>'ver_socio',
		'#value'=>'VER SOCIO',
		'#validate'=>array('ver_socio_ingreso_from_valdiate'),
		'#submit'=>array('ver_socio_ingreso_from_submit'),
	);

	return $form;
}

function ingreso_socio_form_submit($form, &$form_state)
{
	try{
		//si la operación enviada en el submit es BUSCAR vamos a redireccionar nuevamente a la misma ventana de ingreso_socio pero con los filtros
		if($form_state['values']['op'] == 'BUSCAR')
		{
			$parametros = array();
			if(!empty($form_state['values']['nro_socio']))
				$parametros['ns'] = $form_state['values']['nro_socio'];

			if(!empty($form_state['values']['tipo_doc']) && !empty($form_state['values']['nro_doc']))
			{
				$parametros['td'] = $form_state['values']['tipo_doc'];
				$parametros['nd'] = $form_state['values']['nro_doc'];
			}

			if(!empty($form_state['values']['apellidos']))
				$parametros['ap'] = $form_state['values']['apellidos'];

			if(!empty($form_state['values']['nombres']))
				$parametros['nom'] = $form_state['values']['nombres'];

			$form_state['redirect'] = array('socios/ingreso_socio',array('query' => $parametros));
		}
		if($form_state['values']['op'] == 'VER SOCIO')
		{
			$id_socio_seleccionado = (!empty($form_state['values']['listado_socios']) ? $form_state['values']['listado_socios'] : NULL);

			$form_state['redirect'] = array('socios/ingreso_socio',array('query' => array('id'=>$id_socio_seleccionado)));
		}
	}
 	catch (Exception $e) {
  		watchdog_exception('ingreso_socio', $e);
  		drupal_set_message('<pre>'. print_r("Error en la trasaccion - Comunicarse con el proveedor de sistemas.", TRUE) .'</pre>');
	}
}

function ver_socio_ingreso_from_valdiate($form, &$form_state)
{
	if(empty($form_state['values']['listado_socios']))
	{
		form_set_error('listado_socios', 'Debe seleccionar un socio para realizar la accion sobre el mismo');
	}
}

function ver_socio_ingreso_from_submit($form, &$form_state)
{
	//drupal_set_message('<pre>'.print_r($form_state['values']['listado_socios'], true).'</pre>');

	$uid = $GLOBALS['user']->uid;
    try {
    	//creamos un arrray dentro del SESSION para que sea recuperado dentro de params
    	$id_socio_seleccionado = (!empty($form_state['values']['listado_socios']) ? $form_state['values']['listado_socios'] : NULL);

    	$form_state['redirect'] = array('socios/ingreso_socio/socio',array('query' => array('id'=>$id_socio_seleccionado)));
  	}
 	catch (Exception $e) {
  		watchdog_exception('my_type', $e);
  		drupal_set_message('<pre>'. print_r("Error en la trasaccion - Comunicarse con el proveedor de sistemas.".$e, TRUE) .'</pre>');
	}
}

function ingreso_socio_datos_socio_form($form, &$form_state)
{
	$form['#attached']['css'] = array(
		drupal_get_path('module', 'socios') . '/css/socios.css',
	);

	if(strpos($_SERVER['HTTP_REFERER'], 'ingreso_socio/socio') && isset($_GET['ret']))
	{
		$_SESSION['retorno'] = 'socios/ingreso_socio/socio_ret';
	}
	else if(strpos($_SERVER['HTTP_REFERER'], 'ingreso_socio/socio') && !isset($_GET['ret']))
	{
		$array_retorno = parse_url($_SERVER['HTTP_REFERER']);
		$_SESSION['retorno'] = 'socios/ingreso_socio';
		$_SESSION['retorno_params'] =array('query' =>get_params_url($_SERVER['HTTP_REFERER']));
	}
	else if(strpos($_SERVER['HTTP_REFERER'], 'ingreso_socio'))
	{
		$array_retorno = parse_url($_SERVER['HTTP_REFERER']);
		$_SESSION['retorno'] = 'socios/ingreso_socio';
		//drupal_set_message('<pre>'.print_r($_SESSION['params'], true).'</pre>');
		//$_SESSION['retorno_params'] =array('query' =>get_params_url($_SERVER['HTTP_REFERER']));
	}

	if((isset($_GET['id']) && !is_null($_GET['id'])))
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
			//bsucamos la deuda del socio titular de la familia con el id del socio, el tipo de socio y el id del concepto a buscar que en este caso
			//es solamente aquellas deudas con el id correspondiente a la cuota social ya que es para el ingreso
			$deuda_socio = consultar_deuda($socio_titular_familia->id_socio, $socio_titular_familia->tipo_socio_id, 1);
		}
		else {
			//bsucamos la deuda del socio con el id del socio, el tipo de socio y el id del concepto a buscar que en este caso
			//es solamente aquellas deudas con el id correspondiente a la cuota social ya que es para el ingreso
			$deuda_socio = consultar_deuda($id_socio_seleccionado, $socio_seleccionado->tipo_socio_id, 1);
		}



		//mostramos un mensaje con el estado del socio y su estado de deuda
		//preguntamos si el array de deuda tiene datos
		if(count($deuda_socio) > 0)
		{//si la cantidad de registros es mayor a 0 quiere decir que el socio tiene conceptos inpagos
			$deuda = '';
			for($i=0; $i < count($deuda_socio); $i++)
			{
				$deuda = $deuda.'<div class="fila-socio-con-deuda" id="fila-'.$i.'"><strong><span class="deuda-mes">MES: '.$deuda_socio[$i]->mes.'</span><span class = "deuda-anio">AÑO: '.$deuda_socio[$i]->anio.'</span></strong></div>';
			}
			$form['socios']['socio']['estado_con_deuda']= array(
				'#prefix'=>'<div id="estado-deuda-socio" class="socio-con-deuda">',
						'#markup'=>'<strong>EL SOCIO REGISTRA DEUDA</br>DETALLE DE LA DEUDA:<br></strong>'.$deuda,
						'#suffix'=>'</div>',
				);
		}
		else {
			$form['socios']['socio']['estado_sin_deuda']= array(
				'#prefix'=>'<div id="estado-deuda-socio" class="socio-sin-deuda">',
						'#markup'=>'<strong>EL SOCIO NO REGISTRA DEUDA</strong>',
						'#suffix'=>'</div>',
				);
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

		/*if(validar_categoria($socio_seleccionado->edad, $socio_seleccionado->categoria_socio_id) || is_null($socio_seleccionado->categoria_socio_id))
		{
			$form['socios']['socio']['panel_socio']['datos_personales']['categoria_socio_invalida']= array(
				'#prefix'=>'<div class="io-texto texto-dp" id="categoria-socio-invalida">',
				'#markup'=>'<span><strong>CATEGORÍA DE SOCIO INCORRECTA - REVISAR CATEGORÍA DEL SOCIO</strong></span>',
				'#suffix'=>'</div>',
			);
		}*/

		$form['socios']['socio']['panel_socio']['datos_personales']['foto_socio']= array(
	        '#prefix'=>'<div id="foto-socio">',
	        '#markup'=>'<img src='.$uri_foto.'></img>',
	        '#suffix'=>'</div>',
      	);

      	$form['socios']['socio']['panel_socio']['datos_personales']['fecha_ingreso']= array(
  			'#prefix'=>'<div id="datos-personales-socio"><div class="texto-dp" id="fecha-ingreso-socio">',
	        '#markup'=>'<span><strong>FECHA DE INGRESO: </strong>'.$socio_seleccionado->fecha_ingreso.'</span>',
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

			$cobrador = $socio_seleccionado->apellido_personal;

			$form['socios']['socio']['panel_socio']['datos_personales']['cobrador']= array(
  			'#prefix'=>'<div class="io-texto texto-dp" id="cobrador">',
	       	'#markup'=>'<span><strong>COBRADOR: </strong>'.$cobrador.'</span>',
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
	       	'#markup'=>'<span><strong>TITULAR DE FAMILIA: </strong></span>'.l($socio_titular_familia->nombres.' '.$socio_titular_familia->apellidos, 'socios/ingreso_socio/socio', array('query' => array('id' => $socio_titular_familia->id_socio, 'ret'=>$socio_seleccionado->id_socio))),
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

  			/*if(validar_tipo_grupo_familiar($grupo_familiar))
  			{
  				$form['socios']['socio']['panel_socio']['grupo_familiar']['gf_invalido']= array(
					'#prefix'=>'<div class="io-texto texto-gf" id="grupo-familiar-invalido">',
		       		'#markup'=>'<span><strong>GRUPO FAMILIAR INCORRECTO - REVISAR TIPO DE GRUPO FAMILIAR</strong></span>',
		        	'#suffix'=>'</div>',
  				);
  			}*/

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

	  		$cuenta_corriente = get_estado_cta_cte($socio_seleccionado->id_socio);
	  		$form['socios']['socio']['panel_socio']['cuenta_corriente']['listado_cta_cte']= array(
		    	'#markup' => get_listado_cuenta_corriente_markup($cuenta_corriente),
			);
  		}
	}

	$form['volver'] = array(
		'#type'=>'submit',
		'#value'=> t('VOLVER'),
		'#submit'=> array('datos_socio_ingreso_volver_submit'),
	);
	return $form;
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


function datos_socio_ingreso_volver_submit($form, &$form_state)
{
	$retorno = array($_SESSION['retorno']);
	unset($_SESSION['retorno']);

	if($retorno[0] == 'socios/ingreso_socio/socio_ret')
	{
		$form_state['redirect'] = array('socios/ingreso_socio/socio',array('query' => array('id'=>$_GET['ret'])));
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
?>
