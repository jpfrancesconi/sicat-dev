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
