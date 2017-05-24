function myFunction() {
    var cant_cuotas = document.getElementById("cant-cuotas").value;
    var total_deuda = document.getElementById("total-deuda").value;
    var valor_cuota = total_deuda / cant_cuotas;

    document.getElementById("valor-cuota").value = valor_cuota;
}
