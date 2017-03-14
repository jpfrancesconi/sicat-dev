-- ACCESS = access content
SELECT cs.`nombre` AS CATEGORIA, 
	SUM(cci.`valor`) AS IMPORTE, 
    COUNT(soc.id_socio) AS CANTIDAD,
    cci.mes AS MES,
    cci.`fecha_emision` AS FECHA_EMISION
FROM io_cta_cte_item cci 
JOIN io_cta_cte cc ON cc.`id_cta_cte` = cci.`cta_cte_id`
JOIN io_socio soc ON soc.`id_socio` = cc.`socio_id`
JOIN `io_categoria_socio` cs ON cs.`id_categoria_socio` = soc.`categoria_socio_id`
WHERE cci.anulado = FALSE 
	AND cci.`cobrador_id` = :cobrador_id 
	AND cci.`mes` = :mes 
	AND cci.`anio` = :anio
	AND soc.tipo_socio_id = 1
GROUP BY cs.nombre, cci.mes, cci.`fecha_emision`;