(function ($) {
  Drupal.behaviors.socios = {
    attach: function (context, settings) {
		$(document).ready(function(event){
            $('input').keydown(function(e) {                
                    if (e.keyCode === 13) {           
                            e.preventDefault();
                            $("#edit-nro-socio").blur();
                        }                                    
                });
            });
        }			
    } 
}(jQuery));
