-- ACCESS = access content
SELECT cci.`anio` AS ANIO,
 	cci.`mes` AS MES,
    con.nombre AS CONCEPTO,
    cci.`valor` AS IMPORTE
 FROM `io_socio` soc 
 JOIN `io_cta_cte` cc ON cc.`socio_id` = soc.`id_socio`
 JOIN `io_cta_cte_item` cci ON cci.`cta_cte_id` = cc.`id_cta_cte`
 JOIN `io_concepto` con ON con.id_concepto = cci.concepto_id
 WHERE soc.`nro_socio` LIKE :NRO_SOCIO AND cci.`pagado` = 0;