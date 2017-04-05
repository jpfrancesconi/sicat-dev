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
        }			
    } 
}(jQuery));