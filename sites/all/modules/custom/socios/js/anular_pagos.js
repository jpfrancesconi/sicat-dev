(function ($) {
  Drupal.behaviors.socios = {
    attach: function (context, settings) {
		$(document).ready(function(event){
            $('#edit-anular').click(function() {    
                $('#panel-anular-pago-div').css("display", "block");                  
                return false;
                });
            $('#edit-cancelar-anulacion').click(function() {        
                $('#panel-anular-pago-div').css("display", "none");           
                return false;
                });
            });
            $('#listado-pagos-socio .form-type-checkbox input').click(function(e) {
                    var total = 0;
                    var check = 0;
             
                    $("#listado-pagos-socio table tbody tr").each(function (index) 
                    {                         
                        $(this).children("td").each(function (index2) 
                        {       
                       // console.log($(this).children('td .form-type-checkbox input'));                 
                            if(index2==0)
                            {      
                                if($(this).children('td .form-type-checkbox input').context.children["0"].children["0"].checked) {                                
                                    check = 1;                                
                                }
                            }
                            if(index2 == 5 && check == 1)
                            {
                                valor = $(this).children('td .form-type-checkbox input').context.childNodes["0"].nodeValue;
                                valor = parseInt(valor);

                                total = total + valor;
                                check= 0;
                            }
                        })                    
                    });
                    $('#edit-total').val(total);
                                                                                
            });
            
        }			
    } 
}(jQuery));

