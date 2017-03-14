(function ($) {
  Drupal.behaviors.socios = {
    attach: function (context, settings) {
		$(document).ready(function(event){
            if(!$('#edit-cod-barras').val())
            {
                $( "#edit-cod-barras" ).focus();
            }
            else if($('#edit-confirmar').val())
            {
                $("#edit-confirmar").focus();
            }
            else if($('#edit-aceptar').val())
            {
                $('#edit-aceptar').focus();
            }
            $('input').keydown(function(e) {
                //alert(e.keyCode);   
                if($( "#edit-cod-barras" ).is( ":focus")) {
                    if (e.keyCode == 13) {
                            e.preventDefault();
                            $("#edit-cod-barras").blur();
                    }                    
                }
                if($("#edit-confirmar").is( ":focus"))
                {           
                    /*e.preventDefault();    
                     if (e.keyCode == 67) {
                        $("form").submit();
                    }
                    if(e.keyCode==65)
                    {
                        location.reload();
                    }*/
                    
                    
                };
            });
        });			
    } 
  }   
}(jQuery));
