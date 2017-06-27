function PrintAcuerdoPlanPago(elem1, elem2, elem3)
{
    if(document.getElementById(elem2).value.length == 0)
    {
      alert('NO SE SELECCIONO NIONG UNA CANTIDAD DE CUOTAS');
      return false;
    }

    var fecha = new Date();
    var mywindow = window.open('', 'PRINT', 'height=400,width=1200');
    var meses = ["Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio", "Julio", "Agosto", "Septiembre", "Octumbre", "Noviembre", "Diciembre"]
    mywindow.document.write('<html><head><title>PLAN DE PAGO </title>');
    mywindow.document.write('<html><head><style>div#imp-contenedor {margin: 20px;} div#imp-encabezado {width: 100%;height: 95px;border-bottom: solid 1px;padding-bottom: 5px;} #imp-titulo {float:left;text-align: left;width: 20%;font-family: Arial, Helvetica, sans-serif;} #imp-logo {width: 60%;float: left;} #imp-logo img {width: 10%;margin-left: 45%;} #imp-nombre-club {font-family: Arial, Helvetica, sans-serif;text-align: right;float: left;width: 20%;} div#imp-lugar-fecha {width: 100%;text-align: right;} div#imp-contenendor-texto {font-family: Arial, Helvetica, sans-serif;} div#imp-cuerpo {line-height: 27px;} div#imp-firma-socio {margin-left: 70%;width: 20%;text-align: center;border-top: solid 1px;padding-top: 7px;margin-top: 40px;}</style>');
    mywindow.document.write('</head><body><div id="imp-contenedor">');
    mywindow.document.write('<div id="imp-encabezado"><div id="imp-titulo">PLAN DE PAGO DE DEUDA</div>');
    mywindow.document.write('<div id="imp-logo"><img src="../sites/default/files/logo_1.png"></div>');
    mywindow.document.write('<div id="imp-nombre-club">CLUB ATLÉTICO TALLERES</div></div>');
    mywindow.document.write('<div id="imp-lugar-fecha"><p>Paraná, ' + fecha.getDate() + ' de '+ meses[fecha.getMonth()] +' de '+ fecha.getFullYear()+'</p></div>');
    mywindow.document.write('<div id="imp-contenendor-texto">');
    mywindow.document.write('<div id="imp-cuerpo"><p>Conste por la presente que el socio <strong>'+document.getElementById('valor-nro-apellidos').innerHTML+' '+ document.getElementById('valor-nro-nombres').innerHTML +' Nº '+document.getElementById('valor-nro-socio').innerHTML+'</strong> se compromete a abonar la deuda de cuotas societarias correspondientes a los meses '+document.getElementById(elem1).value+' siendo un total de $'+document.getElementById(elem3).value+'; siendo abonado en '+document.getElementById(elem2).value+' cuotas.</p></div>');
    if(document.getElementById('tipo-socio-id'.value == 1))
      mywindow.document.write('<p>Las cuotas societarias corresponden a la categoría <strong>'+document.getElementById('valor-categoria').innerHTML +'</strong></p>');
    else
      mywindow.document.write('<p>Las cuotas societarias corresponden al tipo de grupo familiar <strong>'+document.getElementById('valor-categoria').innerHTML +'</strong></p>');

    mywindow.document.write('</br></br><div id="imp-firma-socio">Firma del socio</div></p>');
    mywindow.document.write('</div>');
    mywindow.document.write('</div></body></html>');

    mywindow.document.close(); // necessary for IE >= 10
    mywindow.focus(); // necessary for IE >= 10*/

    mywindow.print();
    mywindow.close();
    //(this).parents('form').submit();

    return true;
}


//function PrintResumenPago(nro_socio, domicilio, apellidos, nombres, tipo_socio, cate_tgf, pagos)
function PrintResumenPago()
{
  //se genera un originar y un duplicado por lo que necesitamos repetir los datos 2 veces
  //usamos esta variable copias como contador que arranca en 1
  copias = 1;
  //recuperamos los valores del formulario
  nro_socio = document.getElementById('nro_socio').value;
  direccion = document.getElementById('direccion').value;
  apellidos = document.getElementById('apellidos').value;
  nombres = document.getElementById('nombres').value;
  tipo_socio_id = document.getElementById('tipo_socio_id').value;
  cate_tgf = document.getElementById('cate_tgf').value;
  pagos = document.getElementById('pagos').value;
  pagos = JSON.parse(pagos);
  total = document.getElementById('total').value;


  console.log("SUS PAGOS: "+pagos[0]["fecha_emision"]);


  var fecha = new Date();
  var mywindow = window.open('', 'PRINT', 'height=400,width=1200');
  var meses = ["Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio", "Julio", "Agosto", "Septiembre", "Octumbre", "Noviembre", "Diciembre"]
  mywindow.document.write('<html><head><title>RESUMEN DE PAGO </title>');
  mywindow.document.write('<style>body{font-size:14px;} .imp-contenedor{margin: 20px;margin: 20px;height: 470px;border-bottom: black 2px dashed;} div#imp-contenedor-ORIGINAL{margin-top: 35px;border-bottom: none;}div#imp-encabezado {width: 100%;height: 95px;border-bottom: solid 1px;padding-bottom: 5px;} #imp-titulo {float:left;text-align: left;width: 20%;font-family: Arial, Helvetica, sans-serif;} #imp-logo {width: 381px;float: left;} #imp-logo img {width: 12%;margin-left: 45%;} #imp-nombre-club {font-family: Arial, Helvetica, sans-serif;text-align: right;float: left;width: 20%;} div#imp-lugar-fecha {width: 100%;text-align: right;} div#imp-contenendor-texto {font-family: Arial, Helvetica, sans-serif;} div#imp-cuerpo {line-height: 27px;} div#imp-firma-socio {margin-left: 70%;width: 20%;text-align: center;border-top: solid 1px;padding-top: 7px;margin-top: 40px;} div#imp-sub-encabezado {margin-top: 25px;} div#imp-nro-socio {width: 306px;float: left;} .imp-txt-se {margin-bottom: 7px;} div#imp-cate-tgf {width: 306px;float: left;}</style>');
  mywindow.document.write('<style>table.tb-pagos {font-size: 1em;width: 100%;margin-top: 22px;border-top: solid gray;border-bottom: solid gray;} th.tb-pagos-header {border-bottom: solid 1px;padding: 5px;text-align: left;} td.tb-pagos-dato {border-bottom: solid 1px #dadada; padding: 5px;} td.tb-pagos-ultima-fila.tb-pagos-dato-total-txt, td.tb-pagos-ultima-fila.tb-pagos-dato-total-valor {font-size: 1.2em;padding-top: 7px;padding-bottom: 7px; padding-left:5px;} td.tb-pagos-importe.tb-pagos-dato, td.tb-pagos-ultima-fila.tb-pagos-dato-total-valor{text-align: right;} div#imp-tipo-comp{font-size:1em;}</style>');
  mywindow.document.write('</head><body>');

  while(copias <= 2)
  {
    if(copias == 1)
      documento = "DUPLICADO";
    else
      documento = "ORIGINAL";
    mywindow.document.write('<div class="imp-contenedor" id="imp-contenedor-'+documento+'">');
    mywindow.document.write('<div id="imp-encabezado"><div id="imp-titulo"><strong>RESUMEN DE PAGO</strong><br><div id="imp-tipo-comp">'+documento+'</div></div>');
    mywindow.document.write('<div id="imp-logo"><img src="../../sites/default/files/logo_1.png"></div>');
    mywindow.document.write('<div id="imp-nombre-club"><strong>CLUB ATLÉTICO TALLERES</strong></div></div>');
    mywindow.document.write('<div id="imp-sub-encabezado">');
    mywindow.document.write('<div class="imp-txt-se" id="imp-nro-socio"><strong>NUMERO DE SOCIO:</strong> '+nro_socio+'</div>');
    mywindow.document.write('<div class="imp-txt-se" id="imp-apellidos-nombres"><strong>APELLIDOS y NOMBRES: </strong>'+apellidos+' '+nombres+'</div>');
    //si es un socio unico se pega el texto Categoria
    if(tipo_socio_id == 1)
    {
        mywindow.document.write('<div class="imp-txt-se" id="imp-cate-tgf"><strong>CATEGORÍA:</strong> '+cate_tgf+'</div>');
    }
    //sino se pega el texto TIPO DE GRUPO FAMILIAR
    else if(tipo_socio_id == 3)
    {
      mywindow.document.write('<div class="imp-txt-se" id="imp-cate-tgf"><strong>TIPO DE GRUPO FAMILIAR: </strong>'+cate_tgf+'</div>');
    }
    mywindow.document.write('<div class="imp-txt-se" id="imp-domicilio"><strong>DOMICILIO: </strong>'+direccion+'</div>');
    //creamos una tabla donde se van a mostrar los pagos
    mywindow.document.write('<table class="tb-pagos">');
    mywindow.document.write('<tr> <th class="tb-pagos-header">PERIODO</th> <th class="tb-pagos-header">TIPO DE CUOTA</th> <th class="tb-pagos-header">DESCRIPCIÓN</th> <th class="tb-pagos-header">IMPORTE ($)</th> </tr>');
    for(i = 0; i < pagos.length; i++)
    {
      mywindow.document.write('<tr> <td class="tb-pagos-dato">'+pagos[i]["mes"]+'-'+pagos[i]["anio"]+'</td> <td class="tb-pagos-dato">'+pagos[i]["concepto"]+'</td><td class="tb-pagos-dato">'+cate_tgf+'</td><td class="tb-pagos-importe tb-pagos-dato">'+pagos[i]["valor"]+'</td></tr>');
    }
    mywindow.document.write('<tr><td class="tb-pagos-ultima-fila tb-pagos-dato-total-vacio"></td><td class="tb-pagos-ultima-fila tb-pagos-dato-total-vacio"></td><td class="tb-pagos-ultima-fila tb-pagos-dato-total-txt"><strong>TOTAL<strong></td><td class="tb-pagos-ultima-fila tb-pagos-dato-total-valor"><strong>'+total+'</strong></td></tr>');
    mywindow.document.write('</table>');
    mywindow.document.write('</div>');
    mywindow.document.write('</div>');
    //incrementamos copias
    copias++;
  }
  mywindow.document.write('</body></html>');
  mywindow.document.close(); // necessary for IE >= 10
  mywindow.focus(); // necessary for IE >= 10*/

  mywindow.print();
  mywindow.close();
  //(this).parents('form').submit();

  return true;
}
