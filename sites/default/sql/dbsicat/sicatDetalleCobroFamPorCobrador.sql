-- ACCESS = access content
SELECT tgf.`nombre` AS GRUPO, 
	SUM(cci.`valor`) AS IMPORTE, 
    COUNT(soc.id_socio) AS CANTIDAD,
    cci.mes AS MES,
    cci.`fecha_emision` AS FECHA_EMISION

FROM io_cta_cte_item cci 

JOIN io_cta_cte cc ON cc.`id_cta_cte` = cci.`cta_cte_id`
JOIN io_socio soc ON soc.`id_socio` = cc.`socio_id`
JOIN io_grupo_familiar_socio gfs ON gfs.socio_id = soc.id_socio
JOIN io_grupo_familiar gf ON gf.id_grupo_familiar = gfs.grupo_familiar_id
JOIN io_tipo_grupo_familiar tgf ON tgf.id_tipo_grupo_familiar = gf.tipo_grupo_familiar_id

WHERE cci.anulado = FALSE 
	AND cci.`cobrador_id` = :cobrador_id 
	AND cci.`mes` = :mes 
	AND cci.`anio` = :anio
	AND (soc.tipo_socio_id = 3 OR soc.tipo_socio_id = 2)
GROUP BY tgf.nombre, cci.mes, cci.`fecha_emision`;